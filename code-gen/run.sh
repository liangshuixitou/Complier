#!/bin/bash
bison -d grammar.y
flex lexical.l

g++ -std=c++11 grammar.tab.c lex.yy.c codegen.cpp input.cpp astOutput.cpp main.cpp -o run.o $(llvm-config --cflags --ldflags) $(llvm-config --libs) $(llvm-config --system-libs)
clear
echo 'please input the c- source code'
./run.o

# llvm-as main.ll
# lli main.bc
