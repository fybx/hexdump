#!/usr/bin/env bash
#
#       Ferit Yiğit BALABAN <f@fybx.dev>, 2022
#
echo "Compiling: \x1B[31mhexdump\x1B[0m"
gcc -Wall -O3 -pipe -fstack-protector -o hexdump hexdump.c
