#pragma once

#include <array>
#include <cmath>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>

struct mp_SepValues{
  std::vector<std::string> infixValues;
  std::string infix;
};

struct mp_RPN : public mp_SepValues{
  std::string RPN;
  std::vector<std::string> RPNValues;
};

inline float _pow(double x, double y) { return (float)pow(x, y); }
inline float _mod(double x, double y) { return (float)((int)x % (int)y); }
inline float _add(double x, double y) { return (float)(x+y); }
inline float _mul(double x, double y) { return (float)(x*y); }
inline float _div(double x, double y) { return (float)(x/y); }
inline float _sub(double x, double y) { return (float)(x-y); }

class MathParser{
public:
  MathParser(){
    populateArrays();
  }

  void appendVariable(std::string name, double &value){
    externalVariablesMap[name] = &value;

    if (!inVector(name, externalVariables)) externalVariables.push_back(name);
  }

  void deleteVariable(std::string name){
    externalVariablesMap.erase(name);
    removeItemInVector(name, externalVariables);
  }

  mp_RPN reversePolishNotation(std::string infix, bool doCache = true){
    if (doCache){
      if (inVector(infix, cachedVariables)){
        return cachedRPN[infix];
      }

      if (!inVector(infix, cachedVariables)){
        cachedVariables.push_back(infix);
        cachedRPN[infix] = shunting_yard(seperate(infix));

        return cachedRPN[infix];
      }
    }

    return shunting_yard(seperate(infix));
  }

  double eval(){
    if (!(cachedVariables.size() > 0)) return 0.0;

    return eval(cachedRPN[cachedVariables.at(-1)]);
  }

  double eval(mp_RPN RPN){

    std::stack<double> resultStack;

    for (std::string const& token : RPN.RPNValues){

      bool isOperator = inVector(token, operators);
      bool isFunction = inVector(token, functions);
      bool isVariable = inVector(token, externalVariables);
      bool isOperand = !isOperator && !isFunction && !isVariable;

      if (isOperator){
        std::string functionName = operatorTranslationTable[token];
        int operands = functionParameters[functionName];

        if (operands > (int)resultStack.size()) return 0.0;

        std::array<double, 2> values;

        for (int i = 0; i < operands; i++){
          values[i] = resultStack.top();
          resultStack.pop();
        }

        resultStack.push((double)operatorMap[token](values[1], values[0]));
      }
      
      else if (isOperand){
        resultStack.push(round(std::stod(token) * 10000) / 10000);
      }

      else if (isVariable){
        resultStack.push((double)*externalVariablesMap[token]);
      }

      if (isFunction){
        if (functionParameters[token] == 1){
          double value = resultStack.top();
          resultStack.pop();
          resultStack.push(functionsMap[token](value));
        }

        else if (functionParameters[token] == 2){
          std::array<double, 2> values;

          for (int i = 0; i < 2; i++){
            values[i] = resultStack.top();
            resultStack.pop();
          }

          resultStack.push(multipleParameterFunction[token](values[1], values[0]));
        }
      }

    }

    if ((int)resultStack.size() > 1 || (int)resultStack.size() < 1) return 0.0;

    return round(resultStack.top() * 10000) / 10000;
  }

private:
  mp_SepValues seperate(std::string infix){
    remove(infix.begin(), infix.end(), ' ');

    std::vector<std::string> store;
    std::vector<std::string> values;

    std::string joiner;

    for (int i = 0; i < (int)infix.length(); i++){
      std::string item(1, infix[i]);

      bool isOperator = inVector(item, operators);
      bool isSymbol = inVector(item, symbols);

      if (!isOperator && !isSymbol){
        store.push_back(item);
      }

      if (isOperator || isSymbol){

        bool lastNumOperator = false;

        if (values.size() > 0){
          if (inVector(values.back(), operators)) lastNumOperator = true;
        }

        if ((int)store.size() > 0 && lastNumOperator) lastNumOperator = false;

        if (item == "-" && ((i == 0) || (lastNumOperator))){
          store.push_back(item);
        }

        else{
          joiner = "";
          for (auto const& j : store) joiner += j;
          store.clear();
          values.push_back(joiner);
          values.push_back(item);
        }
      }
    }

    // Dump Store

    if (store.size() > 0){
      joiner = "";
      for (auto const& j : store) joiner += j;
      values.push_back(joiner);
    }

    mp_SepValues result;
    result.infix = infix;
    result.infixValues = values;

    return result;
  }

  mp_RPN shunting_yard(mp_SepValues sep){
    std::stack<std::string> stack;
    std::vector<std::string> queue;

    for (auto const& i : sep.infixValues){
      bool isOperator = inVector(i, operators);
      bool isFunction = inVector(i, functions);
      bool isSymbol = inVector(i, symbols);

      if (!isOperator && !isFunction && !isSymbol) {
        queue.push_back(i);
      }

      else if (!isOperator && isFunction && !isSymbol) {
        stack.push(i);
      }

      else if (isOperator && !isFunction && !isSymbol) {
        while(stack.size() > 0){
          if(((inVector(stack.top(), operators)) && (operatorPrecedence[stack.top()] > operatorPrecedence[i]))
             || ((operatorPrecedence[stack.top()] == operatorPrecedence[i]) && (operatorAssociative[stack.top()] == 0) && (stack.top() != "("))){

            queue.push_back(stack.top());
            stack.pop();
          }

          else{
            break;
          }
        }

      stack.push(i);

      }

      else if (i == "("){
        stack.push(i);
      }

      else if (i == ")"){
        while ((stack.top() != "(")){
          queue.push_back(stack.top());
          stack.pop();
        }

        if (stack.top() == "("){
          stack.pop();
        }

        if (stack.size() > 0){
          if (inVector(stack.top(), functions)){
            queue.push_back(stack.top());
            stack.pop();
          }
        }
      }
    }

    while (stack.size() > 0){
      queue.push_back(stack.top());
      stack.pop();
    }

    std::string joiner = "";
    std::vector<std::string> fixedQueue;

    for (auto const& i : queue){
      joiner += i;
      if (i != "") fixedQueue.push_back(i);
    }

    mp_RPN result;
    result.infix = sep.infix;
    result.infixValues = sep.infixValues;
    result.RPNValues = fixedQueue;
    result.RPN = joiner;

    return result;

  }

  std::map<std::string, mp_RPN> cachedRPN;

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

  std::vector<std::string> cachedVariables;
  std::vector<std::string> externalVariables;
  std::vector<std::string> functions;
  std::vector<std::string> operators;
  std::vector<std::string> symbols = {"(", ")", ","};

  void populateArrays(){
    for (auto const &element : operatorMap) operators.push_back(element.first);
    for (auto const &element : functionsMap) functions.push_back(element.first);
  }

  bool inVector(std::string item, std::vector<std::string> v){

    auto result = std::find(v.begin(), v.end(), item);

    if (result != std::end(v)) return true;

    return false;

  }

  void removeItemInVector(std::string item, std::vector<std::string> vector){
    vector.erase(std::find(vector.begin(), vector.end(), item));
  }
};

double evaluate(std::string infix){
  MathParser parser;
  return parser.eval(parser.reversePolishNotation(infix));
}
