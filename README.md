# MathEvaluator

MathEvaluator is quite a basic C++ library that evaluates infix math expressions. MathEvaluator has a focus on simplicity, simply one header file that you include into your project and only a few commands to get a working evalutation.

## Examples

### Basic Eval
```c++
double result = evaluate("1+1"); // returns 2
```
### Basic Eval (With MathEvaluator Class)
```c++
MathEvaluator evalutor;

double result = evalutor.eval("1+1"); // returns 2
```
### External Variable Eval
```c++
MathEvaluator evalutor;

double x = 20;

evalutor.appendVariable("x", x);

double result = evalutor.eval("1+x"); // returns 21
```
### Updating External Variable Eval
```c++
MathEvaluator evalutor;

double x = 20;

evalutor.appendVariable("x", x);

double result1 = evalutor.eval("1+x"); // returns 21

x = 10;

double result2 = evalutor.eval("1+x"); // returns 11
```
### Functions Eval
```c++
MathEvaluator evalutor;
double result = evalutor.eval("sin(1.5707963267948966)"); // returns 1
```
