#pragma once

#include <array>
#include <iostream>
#include <cmath>
#include <iterator>
#include <limits>
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>
#include <tuple>

struct mp_SepValues{
  std::vector<std::string> infixValues;
  std::string infix;
};

struct mp_RPN : public mp_SepValues{
  std::string RPN;
  std::vector<std::string> RPNValues;
};

float fpow(double x, double y) { return (float)pow(x, y); }

inline double round(double val)
{
    if( val < 0 ) return ceil(val - 0.5);
    return floor(val + 0.5);
}

class MathsParser{
public:
  MathsParser(){
    populateArrays();
  }

  void addVariable(std::string name, double &value){
    externalVariablesMap.insert(std::pair<std::string, double>(name, value));
    externalVariables.push_back(name);
  }


  mp_RPN reversePolishNotation(std::string infix){
    return shunting_yard(seperate(infix));
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
        resultStack.push((double)externalVariablesMap[token]);
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

    for (int i = 0; i < infix.length(); i++){
      std::stringstream ss;
      ss << infix[i];
      std::string item = ss.str();

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

  std::map<std::string, double (*)(double, double)> multipleParameterFunction = {};

  std::map<std::string, double> externalVariablesMap = {};

  std::map<std::string, double (*)(double)> functionsMap = {
      {"sin", sin},   {"cos", cos},   {"tan", tan},
      {"asin", asin}, {"acos", acos}, {"atan", atan}};

  std::map<std::string, std::string> operatorTranslationTable = {
      {"^", "pow"}, {"*", "mul"}, {"/", "div"}, {"+", "add"}, {"-", "sub"}};

  std::map<std::string, int> functionParameters = {
      {"sin", 1},   {"cos", 1},   {"tan", 1},
      {"asin", 1}, {"acos", 1}, {"atan", 1},
      {"pow", 2}, {"mul", 2}, {"div", 2},
      {"add", 2}, {"sub", 2}};

  std::map<std::string, int> operatorPrecedence = {
      {"^", 4}, {"*", 3}, {"/", 3}, {"+", 2}, {"-", 2}};

  std::map<std::string, float (*)(double, double)> operatorMap = {
      {"^", fpow}, {"+", fadd}, {"-", fsub}, {"*", fmul}, {"/", fdiv}};

  std::map<std::string, int> operatorAssociative = {
      {"^", 1}, {"*", 0}, {"/", 0}, {"+", 0}, {"-", 0}};

  std::vector<std::string> externalVariables;
  std::vector<std::string> functions;
  std::vector<std::string> operators;
  std::vector<std::string> symbols = {"(", ")", ","};

  void populateArrays(){
    for (auto const &element : operatorMap) operators.push_back(element.first);
    for (auto const &element : functionsMap) functions.push_back(element.first);
  }

  bool inVector(std::string item, std::vector<std::string> vector){
    for (auto const& i : vector) if (item == i) return true;

    return false;

  }
};
