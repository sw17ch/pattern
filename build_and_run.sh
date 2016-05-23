#!/bin/sh

find . -name "*.c" | xargs clang -lpthread -Wall -I. -Iinclude -o main && ./main
