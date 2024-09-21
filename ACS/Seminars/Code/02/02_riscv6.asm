.data
	.word 0x1223344
var: 	.word 0xdeadbeef
addr:	.word var
.text
	lw t1 var
	lw t2 addr
	lw t3 (t2)
	lw t4 4(t2)
	lw t5 -4(t2)