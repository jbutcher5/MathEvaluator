#include <iostream>
#include <string>
#include <vector>

#include "../include/parser.hpp"
#include "../include/list.hpp"

#define LOG(x) std::cout << x << std::endl

bool test(List<std::string>* input, List<std::string>* RPN, List<double>* result){
  if (input->size() != RPN->size() || input->size() != result->size() || RPN->size() != result->size()){
    LOG("Test Failed!");
    LOG("Invalid Lengths!");
    return true;
  }

  else {
    double x = 10;

    MathEvaluator parser;

    parser.appendVariable("x", x);

    int rpn_score = 0;
    int evaluation_score = 0;

    List<double> RPN_Values;

    LOG("Convertion");

    for (int x = 0; x < (int)input->size(); x++){

      double u = parser.eval(input->getData(x));

      if (parser.getRPN().RPN == RPN->getData(x)){
        LOG(input->getData(x) << " -> " << RPN->getData(x) << " | Success");
        rpn_score++;
      }

      else{
        LOG(input->getData(x) << " -> " << RPN->getData(x) << " (" << parser.getRPN().RPN << ") | Failed");
      }

      RPN_Values.append(u);
    }

    LOG("Evaluation");

    LOG(input->size());

    for (int x = 0; x < (int)input->size(); x++){
      double evaluation = RPN_Values.getData(x);

      if (evaluation == result->getData(x)){
        LOG(input->getData(x) << " -> " << result->getData(x) << " | Success");
        evaluation_score++;
      }

      else{
        LOG(input->getData(x) << " -> " << result->getData(x) << " (" << evaluation << ") | Failed");
      }
    }

    if (rpn_score == (int)input->size() && evaluation_score == (int)input->size()) return false;

  }

  return true;

}

int main(){
  std::string input_arr[] = {"1+1", "5%2", "4+8*3", "-5+20*30", "(4+8)*3", "6+9+(4*2+4^2)", "2*(1+(4*(2+1)+3))", "(5.9-5.3)*7.2+1.4^2", "2*20/2+(3+4)*3^2-6+15", "sin(1.5707963267948966)", "sqrt(4)", "x", "x+3", "10!", "pi"};
  std::string RPN_arr[] = {"11+", "52%", "483*+", "-52030*+", "48+3*", "69+42*42^++", "21421+*3++*", "5.95.3-7.2*1.42^+", "220*2/34+32^*+6-15+", "1.5707963267948966sin", "4sqrt", "x", "x3+", "10!", "pi"};
  double result_arr[] = {2, 1, 28, 595, 36, 39, 32, 6.28, 92, 1, 2, 10, 13, 3628800, 3.1416};

  List<std::string> input;
  input.append(input_arr, sizeof(input_arr)/sizeof(input_arr[0]));  
  List<std::string> RPN;
  RPN.append(RPN_arr, sizeof(RPN_arr)/sizeof(RPN_arr[0]));
  List<double> result;
  result.append(result_arr, sizeof(result_arr)/sizeof(result_arr[0]));

  return test(&input, &RPN, &result);
}
