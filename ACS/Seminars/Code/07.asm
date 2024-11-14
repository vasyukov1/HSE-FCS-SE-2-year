.data
a:	.float	123.456
b:	.float	654.321
_2:	.float 	2

.text
	flw	ft0 a t0
	flw	ft1 b t0
	flw	ft2 _2 t0
	fadd.s	ft3 ft2 ft1
	fdiv.s	fa0 ft3 ft2
	li	a7 2
	ecall