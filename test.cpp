#include <iostream>
#include <string>
#include <vector>
#include "parser.hpp"

#define LOG(x) std::cout << x << std::endl

bool eval(std::vector<std::string> input, std::vector<std::string> RPN, std::vector<double> result){
  if (input.size() != RPN.size()){
    LOG("Test Failed!");
    LOG("Invalid Lengths!");
    return true;
  }

  else {
    MathsParser parser;

    int rpn_score = 0;
    int evaluation_score = 0;

    std::vector<mp_RPN> RPN_Values;

    for (int x = 0; x < (int)input.size(); x++){
      mp_RPN u = parser.reversePolishNotation(input[x]);
      if (u.RPN == RPN[x]){
        std::cout << input[x] << " -> " << RPN[x] << " | " << "Success" << std::endl;
        rpn_score++;
      }

      else{
        std::cout << input[x] << " -> " << RPN[x] << " (" << u.RPN << ")" << " | " << "Failed" << std::endl;
      }

      RPN_Values.push_back(u);
    }

    std::cout << (rpn_score/(int)input.size())*100 << "%" << std::endl;

    for (int x = 0; x < (int)input.size(); x++){
      double evaluation = parser.eval(RPN_Values[x]);

      if (evaluation == result[x]){
        std::cout << input[x] << " -> " << result[x] << " | " << "Success" << std::endl;
        evaluation_score++;
      }

      else{
        std::cout << input[x] << " -> " << result[x] << " (" << evaluation << ")" << " | " << "Failed" << std::endl;
      }
    }

    std::cout << (evaluation_score/(int)input.size())*100 << "%" << std::endl;

    if (rpn_score == (int)input.size() && evaluation_score == (int)input.size()) return false;

  }

  return true;

}

int main(){
  std::vector<std::string> input = {"1+1", "4+8*3", "-5+20*30", "(4+8)*3", "6+9+(4*2+4^2)", "2*(1+(4*(2+1)+3))", "(5.9-5.3)*7.2+1.4^2", "2*20/2+(3+4)*3^2-6+15", "sin(1.5707963267948966)"};
  std::vector<std::string> RPN = {"11+", "483*+", "5-2030*+", "48+3*", "69+42*42^++", "21421+*3++*", "5.95.3-7.2*1.42^+", "220*2/34+32^*+6-15+", "1.5707963267948966sin"};
  std::vector<double> result = {2, 28, 595, 36, 39, 32, 6.28, 92, 1};

  return eval(input, RPN, result);
}
