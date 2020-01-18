# Mathy
A toy programming language that supports basic integer arithmetic.

This project was mainly an excuse for me to learn about some testing libraries in the C++ ecosystem that I hadn't gotten the chance to try yet (Catch2 and Trompeloeil).
## Syntax
```
MATHY> var x = 1 + 3
NONE
MATHY> x
4
MATHY> 1 + 3
4
MATHY> (1 + 3) * 2
8
MATHY> (x + 8) * 2
24
MATHY> var y = 3 * 2; y
6
```
## Dependencies
* Catch2
* Trompeloeil
## Building
```
git submodule init
git submodule update
cmake . && make
```
## Usage
To run the REPL.
```
$ ./mathy/mathy
```
To run the unit test suite.
```
$ ./test/mathy_test
```