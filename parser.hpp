#pragma once

#include <cmath>
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <stack>

struct mp_SepValues{
  std::vector<std::string> infixValues;
  std::string infix;
};

struct mp_RPN : public mp_SepValues{
  std::string RPN;
  std::vector<std::string> RPNValues;
};

float fpow(double x, double y){
  return (float)pow(x, y);
}

class MathsParser{
public:
  MathsParser(){
    populateArrays();
  }

  mp_SepValues seperate(std::string infix){
    remove(infix.begin(), infix.end(), ' ');

    std::vector<std::string> store;
    std::vector<std::string> values;

    std::string joiner;

    for (char const& i : infix){
      std::stringstream ss;
      ss << i;
      std::string item = ss.str();

      bool isOperand = inVector(item, operators);
      bool isSymbol = inVector(item, symbols);

      if (!isOperand && !isSymbol){
        store.push_back(item);
      }

      if (isOperand || isSymbol){
        joiner = "";
        for (auto const& j : store) joiner += j;
        store.clear();
        values.push_back(joiner);
        values.push_back(item);
      }
    }

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
          if(((inVector(stack.top(), operators)) && (operatorPrecedence[stack.top()] > operatorPrecedence[i])) || ((operatorPrecedence[stack.top()] == operatorPrecedence[i]) && (operatorAssociative[stack.top()] == 0) && (stack.top() != "("))){
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

    for (auto const& i : queue){
      joiner += i;
    }

    mp_RPN result;
    result.infix = sep.infix;
    result.infixValues = sep.infixValues;
    result.RPNValues = queue;
    result.RPN = joiner;

    return result;

  }

private:
  std::map<std::string, double (*)(double)> functionsMap = {
      {"sin", sin},   {"cos", cos},   {"tan", tan},
      {"asin", asin}, {"acos", acos}, {"atan", atan}};

  std::map<std::string, int> operatorPrecedence = {
      {"^", 4}, {"*", 3}, {"/", 3}, {"+", 2}, {"-", 2}};

  std::map<std::string, float (*)(double, double)> operatorMap = {
      {"^", fpow}, {"+", fadd}, {"-", fsub}, {"*", fmul}, {"/", fdiv}};

  std::map<std::string, int> operatorAssociative = {
      {"^", 1}, {"*", 0}, {"/", 0}, {"+", 0}, {"-", 0}};

  std::vector<std::string> functions;
  std::vector<std::string> operators;
  std::vector<std::string> symbols = {"(", ")", ","};

  void populateArrays(){
    for (auto const &element : operatorMap) operators.push_back(element.first);
    for (auto const &element : functionsMap) functions.push_back(element.first);
  }

  bool inVector(std::string item, std::vector<std::string> vector){
    for (auto const& i : vector){

      if (item == i) return true;

    }

    return false;

  }
};
