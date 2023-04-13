#!/usr/bin/env bash

for f in `ls runtime/*.c`; do
    clang -g -c $f -o $f.o
done
ar rcs runtime/libruntime.a `ls runtime/*.c.o`
