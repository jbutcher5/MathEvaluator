#include "parser.hpp"

#include <cmath>
#include <map>
#include <string>

#include "list.hpp"
#include "stack.hpp"

MathEvaluator::MathEvaluator(){
  populateArrays();
}

void MathEvaluator::appendVariable(const std::string name, double &value){
  externalVariablesMap[name] = &value;

  if (!externalVariables.inList(name)) externalVariables.append(name);
}

void MathEvaluator::deleteVariable(const std::string name){
  externalVariablesMap.erase(name);

  externalVariables.remove(externalVariables.getIndex(name));
}

double MathEvaluator::eval(const std::string expr){

  rpn = compile(expr);

  Stack<double> resultStack;

  for (size_t index = 0; index < rpn.RPNValues.size(); index++){
    const Token token = rpn.RPNValues.getData(index);

    bool isOperator = token.type == OPERATOR;
    bool isFunction = token.type == FUNCTION;
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

me_RPN MathEvaluator::getRPN(){
  return rpn;
}

std::map<std::string, double*> MathEvaluator::getExternalVariables(){
  return externalVariablesMap;
}

me_SepValues MathEvaluator::seperate(std::string infix){

  List<std::string> store;
  List<std::string> values;

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

  List<Token> typedValues;

  for (size_t index = 0; index < values.size(); index++){
    const auto i = values.getData(index);
    token_type type;

    bool isOperator = operators.inList(i);
    bool isFunction = !isOperator && functions.inList(i);
    bool isSymbol = !isOperator && !isFunction && symbols.inList(i);
    bool isOperand = !isOperator && !isFunction && !isSymbol;

    if (isOperator) type = OPERATOR;
    if (isFunction) type = FUNCTION;
    if (isSymbol) type = SYMBOL;
    if (isOperand) type = OPERAND;

    Token item = {i, type};

    typedValues.append(item);
  }

  result.infixValues = typedValues;

  return result;
}

me_RPN MathEvaluator::compile(const std::string infix){

  me_SepValues sep = seperate(infix);

  Stack<Token> stack;
  List<Token> queue;

  for (size_t index = 0; index < sep.infixValues.size(); index++){
    const auto i = sep.infixValues.getData(index);
    bool isOperator = i.type == OPERATOR;
    bool isFunction = i.type == FUNCTION;
    bool isSymbol = i.type == SYMBOL;

    if (!isOperator && !isFunction && !isSymbol) {
      queue.append(i);
    }

    else if (!isOperator && isFunction && !isSymbol) {
      stack.push(i);
    }

    else if (isOperator && !isFunction && !isSymbol) {
      while(stack.size() > 0){
        if(((stack.peak().type == OPERATOR) && (operatorPrecedence[stack.peak().value] > operatorPrecedence[i.value]))
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
  List<Token> fixedQueue;

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

void MathEvaluator::populateArrays(){
  std::string symArr[3] = {"(", ")", ","};
  for (int i = 0; i < 3; i++) symbols.append(symArr[i]);

  for (auto const &element : operatorMap) operators.append(element.first);
  for (auto const &element : functionsMap) functions.append(element.first);
}

double evaluate(const std::string infix){
  MathEvaluator evaluator;
  return evaluator.eval(infix);
}