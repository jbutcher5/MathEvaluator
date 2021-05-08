#include <iostream>
#include <string>
#include <vector>
#include "parser.hpp"

bool eval(std::vector<std::string> input, std::vector<std::string> output){
  if (input.size() != output.size()){
    LOG("Test Failed!");
    LOG("Invalid Lengths!");
    return true;
  }

  else {
    MathsParser parser;

    int score = 0;

    for (int x = 0; x < (int)input.size(); x++){
      if (parser.shunting_yard(parser.seperate(input[x])).RPN == output[x]){
        std::cout << input[x] << " -> " << output[x] << " | " << "Success" << std::endl;
        score++;
      }

      else{
        std::cout << input[x] << " -> " << output[x] << " | " << "Failed" << std::endl;
      }
    }

    std::cout << (score/(int)input.size())*100 << "%" << std::endl;

    if (score == (int)input.size()) return false;

  }

  return true;

}

int main(){
  std::vector<std::string> input = {"1+1", "4+8*3", "(4+8)*3", "6+9+(4*2+4^2)", "2*(1+(4*(2+1)+3))", "(5.9-5.3)*7.2+1.4^2", "2*20/2+(3+4)*3^2-6+15"};
  std::vector<std::string> output = {"11+", "483*+", "48+3*", "69+42*42^++", "21421+*3++*", "5.95.3-7.2*1.42^+", "220*2/34+32^*+6-15+"};

  return eval(input, output);
}
