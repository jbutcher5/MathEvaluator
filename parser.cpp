#include <iostream>
#include <cmath>
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <stack>

#define LOG(x) std::cout << x << std::endl

struct mp_SepValues{
  std::vector<std::string> values;
  std::string infix;
};

struct mp_RPN : public mp_SepValues{
  std::string RPN;
};

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

      bool isOperand = false;
      bool isSymbol = false;

      bool isSplit;

      for (auto const &operand : operator) {
        if (item == operand){
          isOperand = true;
          break;
        }
      }

      for (auto const& symbol : symbols){
        if (item == symbol){
          isSymbol = true;
          break;
        }
      }

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
    result.values = values;

    return result;
  }

  mp_RPN shunting_yard(mp_SepValues sep){
    std::stack<std::string> stack;
    std::vector<std::string> queue;

    for (auto const& i : sep.values){
      bool isOperator = false;
      bool isFunction = false;
      bool isSymbol = false;

      bool hasPush = false;


      for (auto const& symbol : symbols){
        if (i == symbol){
          isSymbol = true;
          break;
        }
      }

      for (auto const &operand : operator) {
        if (i == operand){
          isOperand = true;
          break;
        }
      }

      for (auto const& function : functions){
        if (i == function){
          isFunction = true;
          break;
        }
      }

      if (!isOperator && !isFunction && !isSymbol) {
        queue.push_back(i);
      }

      else if (!isOperator && isFunction && !isSymbol) {
        stack.push(i);
      }

      else if (isOperator && !isFunction && !isSymbol) {


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
    result.values = sep.values;
    result.RPN = joiner;

    return result;

  }

private:
  std::map<std::string, double (*)(double)> functionsMap = {
      {"sin", sin},   {"cos", cos},   {"tan", tan},
      {"asin", asin}, {"acos", acos}, {"atan", atan}};

  std::map<std::string, int> operatorPrecedence = {
      {"^", 0}, {"/", 1}, {"*", 2}, {"+", 3}, {"-", 4}};

  std::map<std::string, float (*)(double, double)> operatorMap = {
      {"+", fadd}, {"-", fsub}, {"*", fmul}, {"/", fdiv}};

  std::map<std::string, int> operatorAssociative = {
      {"^", 1}, {"*", 0}, {"/", 0}, {"+", 0}, {"-", 0}};

  std::vector<std::string> functions;
  std::vector<std::string> op;
  std::vector<std::string> symbols = {"(", ")", ","};

  void populateArrays(){
    for (auto const &element : operatorMap) {
      op.push_back(element.first);
    }

    for (auto const &element : functionsMap) {
      functions.push_back(element.first);
    }
  }
};

int main(){
  MathsParser parser;
  mp_SepValues x = parser.seperate("67+456*654*634/3+2");
  mp_RPN r = parser.shunting_yard(x);

  LOG(r.RPN);
  return 0;
}
