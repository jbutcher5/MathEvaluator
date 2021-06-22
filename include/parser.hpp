#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>

#include "list.hpp"

struct Token{
  std::string value;
  std::string type;
};

struct mp_SepValues{
  list<Token> infixValues;
  std::string infix;
};

struct mp_RPN : public mp_SepValues{
  std::string RPN;
  list<Token> RPNValues;
};

inline float _pow(double x, double y) { return (float)pow(x, y); }
inline float _mod(double x, double y) { return (float)((int)x % (int)y); }
inline float _add(double x, double y) { return (float)(x+y); }
inline float _mul(double x, double y) { return (float)(x*y); }
inline float _div(double x, double y) { return (float)(x/y); }
inline float _sub(double x, double y) { return (float)(x-y); }

template <typename T>
class mp_List : public list<T>{
public:
  size_t getIndex(const T data){
    for (int i = 0; i < this->size(); i++){
      if (this->getData(i) == data){
        return i;
      }
    }

    return 0;
  }
};

class MathParser{
public:
  MathParser(){
    populateArrays();
  }

  void appendVariable(const std::string name, double &value){
    externalVariablesMap[name] = &value;

    if (!externalVariables.inList(name)) externalVariables.append(name);
  }

  void deleteVariable(const std::string name){
    externalVariablesMap.erase(name);

    externalVariables.remove(externalVariables.getIndex(name));
  }

  double eval(const std::string expr){

    rpn = compile(expr);

    std::stack<double> resultStack;

    for (size_t index = 0; index < rpn.RPNValues.size(); index++){
      const Token token = rpn.RPNValues.getData(index);

      bool isOperator = token.type == "operator";
      bool isFunction = token.type == "function";
      bool isVariable = externalVariables.inList(token.value);
      bool isOperand = !isOperator && !isFunction && !isVariable;

      if (isOperator){
        std::string functionName = operatorTranslationTable[token.value];
        int operands = functionParameters[functionName];

        if (operands > (int)resultStack.size()) return 0.0;

        std::array<double, 2> values;

        for (int i = 0; i < operands; i++){
          values[i] = resultStack.top();
          resultStack.pop();
        }

        resultStack.push((double)operatorMap[token.value](values[1], values[0]));
      }
      
      else if (isOperand){
        resultStack.push(round(std::stod(token.value) * 10000) / 10000);
      }

      else if (isVariable){
        resultStack.push((double)*externalVariablesMap[token.value]);
      }

      if (isFunction){
        if (functionParameters[token.value] == 1){
          double value = resultStack.top();
          resultStack.pop();
          resultStack.push(functionsMap[token.value](value));
        }

        else if (functionParameters[token.value] == 2){
          std::array<double, 2> values;

          for (int i = 0; i < 2; i++){
            values[i] = resultStack.top();
            resultStack.pop();
          }

          resultStack.push(multipleParameterFunction[token.value](values[1], values[0]));
        }
      }

    }

    if ((int)resultStack.size() > 1 || (int)resultStack.size() < 1) return 0.0;

    return round(resultStack.top() * 10000) / 10000;
  }

  mp_RPN getRPN(){
    return rpn;
  }

  std::map<std::string, double*> getExternalVariables(){
    return externalVariablesMap;
  }

private:

  mp_SepValues seperate(std::string infix){

    list<std::string> store;
    list<std::string> values;

    std::string joiner;

    for (int i = 0; i < (int)infix.length(); i++){
      std::string item(1, infix[i]);

      // Deduce Type

      bool isOperator = inVector(item, operators);
      bool isSymbol = inVector(item, symbols);

      if (!isOperator && !isSymbol){
        store.append(item); // End head and tail of values are unsynced here
      }

      if (isOperator || isSymbol){

        bool lastNumOperator = false;

        if (values.size() > 0){
          if (inVector(values.getData(values.size()-1), operators)) lastNumOperator = true;
        }

        if ((int)store.size() > 0 && lastNumOperator) lastNumOperator = false;

        if (item == "-" && ((i == 0) || (lastNumOperator))){
          store.append(item);
        }

        else{
          joiner = "";
          for (auto const j : store.exposeVec()) joiner += j;
          store.freeAll();
          values.append(joiner);
          values.append(item);
        }
      }



    }

    // Dump Store

    if (store.size() > 0){
      joiner = "";
      for (auto const j : store.exposeVec()) joiner += j;
      values.append(joiner);
    }

    mp_SepValues result;
    result.infix = infix;

    // Deduce Type From Tokens

    list<Token> typedValues;

    for (auto const i : values.exposeVec()){
      std::string type;

      bool isOperator = inVector(i, operators);
      bool isFunction = !isOperator && inVector(i, functions);
      bool isSymbol = !isOperator && !isFunction && inVector(i, symbols);
      bool isOperand = !isOperator && !isFunction && !isSymbol;

      if (isOperator) type = "operator";
      if (isFunction) type = "function";
      if (isSymbol) type = "symbol";
      if (isOperand) type = "operand";

      Token item = {i, type};

      typedValues.append(item);
    }

    result.infixValues = typedValues;

    return result;
  }

  mp_RPN compile(const std::string infix){

    mp_SepValues sep = seperate(infix);

    std::stack<Token> stack;
    list<Token> queue;

    for (size_t index = 0; index < sep.infixValues.size(); index++){
      const auto i = sep.infixValues.getData(index);
      bool isOperator = i.type == "operator";
      bool isFunction = i.type == "function";
      bool isSymbol = i.type == "symbol";

      if (!isOperator && !isFunction && !isSymbol) {
        queue.append(i);
      }

      else if (!isOperator && isFunction && !isSymbol) {
        stack.push(i);
      }

      else if (isOperator && !isFunction && !isSymbol) {
        while(stack.size() > 0){
          if(((stack.top().type == "operator") && (operatorPrecedence[stack.top().value] > operatorPrecedence[i.value]))
             || ((operatorPrecedence[stack.top().value] == operatorPrecedence[i.value]) && (operatorAssociative[stack.top().value] == 0) && (stack.top().value != "("))){

            queue.append(stack.top());
            stack.pop();
          }

          else{
            break;
          }
        }

      stack.push(i);

      }

      else if (i.value == "("){
        stack.push(i);
      }

      else if (i.value == ")"){
        while ((stack.top().value != "(")){
          queue.append(stack.top());
          stack.pop();
        }

        if (stack.top().value == "("){
          stack.pop();
        }

        if (stack.size() > 0){
          if (inVector(stack.top().value, functions)){
            queue.append(stack.top());
            stack.pop();
          }
        }
      }
    }

    while (stack.size() > 0){
      queue.append(stack.top());
      stack.pop();
    }

    std::string joiner = "";
    list<Token> fixedQueue;

    for (size_t index = 0; index < queue.size(); index++){
      const auto i = queue.getData(index);
      joiner += i.value;
      if (i.value != "") fixedQueue.append(i);
    }

    mp_RPN result;

    result.infix = sep.infix;
    result.infixValues = sep.infixValues;
    result.RPNValues = fixedQueue;
    result.RPN = joiner;

    return result;
  }

  mp_RPN rpn;

  std::map<std::string, double (*)(double, double)> multipleParameterFunction;

  std::map<std::string, double*> externalVariablesMap;

  std::map<std::string, double (*)(double)> functionsMap = {
      {"sin", sin},   {"cos", cos},   {"tan", tan},
      {"asin", asin}, {"acos", acos}, {"atan", atan},
      {"sqrt", sqrt}};

  std::map<std::string, std::string> operatorTranslationTable = {
      {"^", "pow"}, {"*", "mul"}, {"/", "div"}, {"+", "add"}, {"-", "sub"}, {"%", "mod"}};

  std::map<std::string, int> functionParameters = {
      {"sin", 1},   {"cos", 1},   {"tan", 1},
      {"asin", 1}, {"acos", 1}, {"atan", 1},
      {"pow", 2}, {"mul", 2}, {"div", 2},
      {"add", 2}, {"sub", 2}, {"mod", 2},
      {"sqrt", 1}};

  std::map<std::string, int> operatorPrecedence = {
      {"^", 4}, {"*", 3}, {"/", 3}, {"%", 3}, {"+", 2}, {"-", 2}};

  std::map<std::string, float (*)(double, double)> operatorMap = {
      {"^", _pow}, {"+", _add}, {"-", _sub}, {"*", _mul}, {"/", _div}, {"%", _mod}};

  std::map<std::string, int> operatorAssociative = {
      {"^", 1}, {"*", 0}, {"/", 0}, {"+", 0}, {"-", 0}, {"%", 0}};

  mp_List<std::string> externalVariables;

  std::vector<std::string> functions;
  std::vector<std::string> operators;
  std::vector<std::string> symbols = {"(", ")", ","};

  void populateArrays(){
    for (auto const &element : operatorMap) operators.push_back(element.first);
    for (auto const &element : functionsMap) functions.push_back(element.first);
  }

  bool inVector(const std::string item, const std::vector<std::string> v){

    return std::find(v.begin(), v.end(), item) != std::end(v);

  }

  void removeItemInVector(const std::string item, std::vector<std::string> vector){
    vector.erase(std::find(vector.begin(), vector.end(), item));
  }
};

double evaluate(const std::string infix){
  MathParser parser;
  return parser.eval(infix);
}
