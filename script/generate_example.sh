$1
aarch64-linux-gnu-gcc-4.8 -S -O0 $1.c -o $1.s
cat $1.s
