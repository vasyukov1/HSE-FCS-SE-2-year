.data
n:			.word 0
array_A:		.space 40
array_B:		.space 40

input_size:		.asciz "Input the size of array from 1 to 10: "
input_item_p1:		.asciz "Input "
input_item_p2:		.asciz " element: "
prompt_error_size:	.asciz "\nBro, you need input a number from 1 to 10. Try again!\n"

.text
main:
	jal get_size
	
	li t0 1			# Counter of elements
	lw t1 n			# Array size
	jal fill_array_A
	
	li a7 10
	ecall


get_size:
	li a7 4
	la a0 input_size
	ecall
	
	li a7 5
	ecall
	mv t0 a0
	
	li t1 1
	blt t0 t1 error_size
	li t1 10
	bgt t0 t1 error_size
	
	la t1 n
	sw t0 (t1) 
	
	jr ra
	
fill_array_A:
	li 
	
	
	
error_size:
	li a7 4
	la a0 prompt_error_size
	ecall
	j get_size
