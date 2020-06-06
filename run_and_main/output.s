.data
_CONSTANT_0: .ascii "AA\n\000"
_CONSTANT_1: .ascii "B\n\000"
a .shift 4
_CONSTANT_2: .ascii "C\n\000"
.align 2
.text
_start_test:
_start_MAIN:
stp x29, x30, [sp, -16]!
add x29, sp, 0

adrp x0, _CONSTANT_0
bl _write_int
adrp x0, _CONSTANT_0
add x0, x0, :lo12:_CONSTANT_0
bl _write_str
mov x0, 0
add x0, x0, :lo12:_CONSTANT_0
bl _write_int

adrp x0, _CONSTANT_1
bl _write_int
adrp x0, _CONSTANT_1
add x0, x0, :lo12:_CONSTANT_1
bl _write_str
mov x0, 0
add x0, x0, :lo12:_CONSTANT_1
bl _write_int

adrp x0, _CONSTANT_2
bl _write_int
adrp x0, _CONSTANT_2
add x0, x0, :lo12:_CONSTANT_2
bl _write_str
mov x0, 0
add x0, x0, :lo12:_CONSTANT_2
bl _write_int
ldp x29, x30, [sp], 16
ret
.size _start_MAIN, .-_start_MAIN
