#!/bin/sh

find . -name "*.c" | xargs clang -g3 -std=c11 -lpthread -Wall -I. -Iinclude -o main && ./main
