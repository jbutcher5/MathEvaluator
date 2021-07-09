#pragma once

#include <cmath>
#include <map>
#include <string>

#include "list.hpp"
#include "stack.hpp"

enum token_type{ OPERATOR, OPERAND, SYMBOL, FUNCTION };

struct Token{
  std::string value;
  token_type type;
};

struct me_SepValues{
  List<Token> infixValues;
  std::string infix;
};

struct me_RPN : public me_SepValues{
  std::string RPN;
  List<Token> RPNValues;
};

inline float _pow(double x, double y) { return (float)pow(x, y); }
inline float _mod(double x, double y) { return (float)((int)x % (int)y); }
inline float _add(double x, double y) { return (float)(x+y); }
inline float _mul(double x, double y) { return (float)(x*y); }
inline float _div(double x, double y) { return (float)(x/y); }
inline float _sub(double x, double y) { return (float)(x-y); }

template <typename T>
class me_List : public List<T>{
public:
  size_t getIndex(const T);
};

template <class T>
size_t me_List<T>::getIndex(const T data){
  for (int i = 0; i < this->size(); i++){
    if (this->getData(i) == data){
      return i;
    }
  }

  return 0;
}

class MathEvaluator{
public:
  MathEvaluator() {populateArrays();};
  void appendVariable(const std::string, double&);
  void deleteVariable(const std::string);
  double eval(const std::string);
  me_RPN getRPN() {return rpn;};
  std::map<std::string, double*> getExternalVariables() {return externalVariablesMap;};

private:

  me_SepValues seperate(std::string);
  me_RPN compile(const std::string);
  void populateArrays();

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
  List<std::string> functions;
  List<std::string> operators;
  List<std::string> symbols;
};

double evaluate(const std::string);
