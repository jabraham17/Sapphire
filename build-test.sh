#!/usr/bin/env bash
/home/jabraham/Development/llvm/llvm-install/bin/llc -filetype=obj test.ll -o test.o
clang -g test.o -Lruntime -lruntime -static -o a.out
