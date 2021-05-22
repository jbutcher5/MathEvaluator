# MathParser

MathParser is quite a basic C++ header file that evaluates infix math expressions. MathParser has a focus on simplicity, simply one header file that you include into your project and only a few commands to get a working evalutation.

## Examples

### Basic Eval
```c++
double result = evaluate("1+1");
```

### Basic Eval (With MathParser Class)
```c++
MathParser parser
mp_RPN rpn = parser.reversePolishNotation("1+1");

double result = parser.eval(rpn); // returns 2
```
### External Variable Eval
```c++
MathParser parser

double x = 20;

parser.appendVariable("x", x);

mp_RPN rpn = parser.reversePolishNotation("1+x");

double result = parser.eval(rpn); // returns 21
```
### Updating External Variable Eval
```c++
MathParser parser

double x = 20;

parser.appendVariable("x", x);

mp_RPN rpn = parser.reversePolishNotation("1+x");

double result1 = parser.eval(rpn); // returns 21

x = 10;

parser.appendVariable("x", x);

double result2 = parser.eval(rpn); // returns 11
```
### Functions Eval
```c++
MathParser parser
mp_RPN rpn = parser.reversePolishNotation("sin(1.5707963267948966)");

double result = parser.eval(rpn); // returns 1
```
