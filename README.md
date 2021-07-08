# MathEvaluator

MathEvaluator is quite a basic C++ library that evaluates infix math expressions. MathEvaluator has a focus on simplicity, simply one header file and implementation file that you include and compile into your project and only a few commands to get a working evalutation.

## Compiling

### Standard C++
To compile `./test/test.cpp` you must compile `./include/parser.cpp` alongside it. This can be done with `g++ ./test/test.cpp ./include/parser.cpp`. 

### Make
You can compile the test cases with make `cd build` `make`.

### Webassembly
Install [emscripten](https://emscripten.org/docs/getting_started/downloads.html#platform-notes-installation-instructions-sdk) and compile `./test/test.cpp` with `em++ ./test/test.cpp ./include/parser.cpp`.

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
