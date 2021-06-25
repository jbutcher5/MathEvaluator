#pragma once

#include <cmath>
#include <map>
#include <string>

#include "list.hpp"
#include "stack.hpp"

struct Token{
  std::string value;
  std::string type;
};

struct me_SepValues{
  list<Token> infixValues;
  std::string infix;
};

struct me_RPN : public me_SepValues{
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
class me_List : public list<T>{
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

class MathEvaluator{
public:
  MathEvaluator(){
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

    Stack<double> resultStack;

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
          values[i] = resultStack.pop();
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
          double value = resultStack.pop();
          resultStack.push(functionsMap[token.value](value));
        }

        else if (functionParameters[token.value] == 2){
          std::array<double, 2> values;

          for (int i = 0; i < 2; i++){
            values[i] = resultStack.pop();
          }

          resultStack.push(multipleParameterFunction[token.value](values[1], values[0]));
        }
      }

    }

    if ((int)resultStack.size() > 1 || (int)resultStack.size() < 1) return 0.0;

    return round(resultStack.peak() * 10000) / 10000;
  }

  me_RPN getRPN(){
    return rpn;
  }

  std::map<std::string, double*> getExternalVariables(){
    return externalVariablesMap;
  }

private:

  me_SepValues seperate(std::string infix){

    list<std::string> store;
    list<std::string> values;

    std::string joiner;

    for (int i = 0; i < (int)infix.length(); i++){
      std::string item(1, infix[i]);

      // Deduce Type

      bool isOperator = operators.inList(item);
      bool isSymbol = symbols.inList(item);

      if (!isOperator && !isSymbol){
        store.append(item); // End head and tail of values are unsynced here
      }

      if (isOperator || isSymbol){

        bool lastNumOperator = false;

        if (values.size() > 0){
          if (operators.inList(values.getData(values.size()-1))) lastNumOperator = true;
        }

        if ((int)store.size() > 0 && lastNumOperator) lastNumOperator = false;

        if (item == "-" && ((i == 0) || (lastNumOperator))){
          store.append(item);
        }

        else{
          joiner = "";
          for (size_t index = 0; index < store.size(); index++){
            const auto j = store.getData(index);
            joiner += j;
          }
          store.freeAll();
          values.append(joiner);
          values.append(item);
        }
      }
    }

    // Dump Store

    if (store.size() > 0){
      joiner = "";
      for (size_t index = 0; index < store.size(); index++){
        const auto j = store.getData(index);
        joiner += j;
      }
      values.append(joiner);
    }

    me_SepValues result;
    result.infix = infix;

    // Deduce Type From Tokens

    list<Token> typedValues;

    for (size_t index = 0; index < values.size(); index++){
      const auto i = values.getData(index);
      std::string type;

      bool isOperator = operators.inList(i);
      bool isFunction = !isOperator && functions.inList(i);
      bool isSymbol = !isOperator && !isFunction && symbols.inList(i);
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

  me_RPN compile(const std::string infix){

    me_SepValues sep = seperate(infix);

    Stack<Token> stack;
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
          if(((stack.peak().type == "operator") && (operatorPrecedence[stack.peak().value] > operatorPrecedence[i.value]))
             || ((operatorPrecedence[stack.peak().value] == operatorPrecedence[i.value]) && (operatorAssociative[stack.peak().value] == 0) && (stack.peak().value != "("))){

            queue.append(stack.pop());
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
        while ((stack.peak().value != "(")){
          queue.append(stack.pop());
        }

        if (stack.peak().value == "("){
          stack.pop();
        }

        if (stack.size() > 0){
          if (functions.inList(stack.peak().value)){
            queue.append(stack.pop());
          }
        }
      }
    }

    while (stack.size() > 0){
      queue.append(stack.pop());
    }

    std::string joiner = "";
    list<Token> fixedQueue;

    for (size_t index = 0; index < queue.size(); index++){
      const auto i = queue.getData(index);
      joiner += i.value;
      if (i.value != "") fixedQueue.append(i);
    }

    me_RPN result;

    result.infix = sep.infix;
    result.infixValues = sep.infixValues;
    result.RPNValues = fixedQueue;
    result.RPN = joiner;

    return result;
  }

  me_RPN rpn;

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

  me_List<std::string> externalVariables;

  list<std::string> functions;
  list<std::string> operators;
  list<std::string> symbols;

  void populateArrays(){
    std::string symArr[3] = {"(", ")", ","};
    symbols.appendArr<3>(symArr);

    for (auto const &element : operatorMap) operators.append(element.first);
    for (auto const &element : functionsMap) functions.append(element.first);
  }
};

double evaluate(const std::string infix){
  MathEvaluator evaluator;
  return evaluator.eval(infix);
}
