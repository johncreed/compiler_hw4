.text
_start_MAIN:
	stp x29, x30, [sp, -16]!
	add x29, sp, 0
	mov x0, 10
	bl _write_int
	ldp x29, x30, [sp], 16
	ret
