# MathParser

MathParser is quite a basic C++ header file that evaluates infix math expressions. MathParser has a focus on simplicity, simply one header file that you include into your project and only a few commands to get a working evalutation.

## Examples

### Basic Eval
```c++
double result = evaluate("1+1"); // returns 2
```
### Basic Eval (With MathParser Class)
```c++
MathParser parser

double result = parser.eval("1+1"); // returns 2
```
### External Variable Eval
```c++
MathParser parser

double x = 20;

parser.appendVariable("x", x);

double result = parser.eval("1+x"); // returns 21
```
### Updating External Variable Eval
```c++
MathParser parser

double x = 20;

parser.appendVariable("x", x);

double result1 = parser.eval("1+x"); // returns 21

x = 10;

double result2 = parser.eval("1+x"); // returns 11
```
### Functions Eval
```c++
MathParser parser
double result = parser.eval("sin(1.5707963267948966)"); // returns 1
```
