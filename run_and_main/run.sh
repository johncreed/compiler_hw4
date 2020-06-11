#! /bin/bash

# rm a.out
rm a.out

# Generate output.s
./parser $1 

# Compiler to bin and run on aarch64 emulator
aarch64-linux-gnu-gcc-4.8 --static -O0 main.S
qemu-aarch64 a.out
