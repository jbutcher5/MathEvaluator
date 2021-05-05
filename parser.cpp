#include <iostream>
#include <cmath>
#include <map>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <algorithm>

#define LOG(x) std::cout << x << std::endl

struct mp_SepValues{
  std::vector<std::string> values;
  std::string infix;
  std::string RPN;
};

class MathsParser{
public:
  MathsParser(){
    populateOperandsArray();
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

      for (auto const& operand : operands){
        if (item == operand){
          isOperand = true;
          break;
        }
      }

      if (!isOperand){
        store.push_back(item);
      }

      if (isOperand){
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
  

private:
  std::map<std::string, int> operandsMap = {{"+", 0}, {"-", 1}, {"*", 2}, {"/", 3}, {"(", 4}, {")", 4}};
  std::vector<std::string> operands;

  void populateOperandsArray(){
    for (auto const& element : operandsMap){
      operands.push_back(element.first);
    }
  }
};

int main(){
  MathsParser parser;
  mp_SepValues r = parser.seperate("sin(90)");
  for (auto i : r.values){
    LOG(i);
  }
  return 0;
}
