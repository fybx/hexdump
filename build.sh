#!/usr/bin/env bash
#
#       Ferit Yiğit BALABAN <f@fybx.dev>, 2022
#
echo -e "Compiling: \x1B[31mhexdump\x1B[0m"
gcc -Wall -O3 -pipe -fstack-protector -std=c99 -o hexdump hexdump.c
