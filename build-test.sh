#!/usr/bin/env bash
llc -O0 -filetype=obj test.ll -o test.o
clang -O3 -c runtime/print.c -o runtime/print.o
clang -O3 -c runtime/main.c -o runtime/main.o
clang test.o runtime/print.o runtime/main.o -o a.out
