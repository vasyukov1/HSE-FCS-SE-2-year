.data
n: 		.word 0
array:		.space 40

input_size:	.asciz "Input the size of array from 1 to 10: "
input_item_p1:	.asciz "Input "
input_item_p2:	.asciz " element: "
result_prompt:	.asciz "\nSum: "
error_prompt:	.asciz "\nBro, you need input a number from 1 to 10. Try again!\n"
overflow_p1:	.asciz "\nOverflow!\nSum: "
overflow_p2:	.asciz "\nCount of elements: "


.text
size:	# Output prompt for input the size of array
	li a7 4
	la a0 input_size
	ecall
	
	# Input the size of array
	li a7 5
	ecall
	mv t0 a0
	
	# Check that the array's size is correct
	li t1 1
	blt t0 t1 error
	li t1 10
	bgt t0 t1 error
	
	# Load the array's size in memory
	la t1 n
	sw t0 (t1)
		
	# Load the element in memory
	la t0 array
	
	li t1 1
	lw t3 n
	
fill:	# Prompt for input the element of array
	li a7 4
	la a0 input_item_p1
	ecall
	
	li a7 1
	mv a0 t1
	ecall
	
	li a7 4
	la a0 input_item_p2
	ecall
	
	li a7 5
	ecall
	mv t2 a0
	
	# Save element and go to the next
	sw t2 (t0)
	addi t0 t0 4
	addi t1 t1 1
	ble t1 t3 fill

sum_preparation:	
	la t0 array	# Start of array
	li t1 0		# Sum of array
	li t2 0		# Counter of elements
	lw t3 n		# Size of array
	
add_el:	lw t4 (t0)	# Read element
	# Check overflow
	xor t5 t1 t4
	bgez t5 check_overflow	
no_overflow:
	# Save sum
	add t1 t1 t4		
	# Go to next element
	addi t0 t0 4
	addi t2 t2 1		
	blt t2 t3 add_el
	j result
	
check_overflow:
	# If sum and term have different signs, overflow occurred
	add t6 t1 t4
	xor t5 t1 t6
	bltz t5 error_overflow
	j no_overflow	
	
result:
	# Output result 
	li a7 4
	la a0 result_prompt
	ecall
	
	li a7 1
	mv a0 t1
	ecall
	j end
	
error_overflow:	
	# Message about overflow and result
	li a7 4
	la a0 overflow_p1
	ecall	
	
	li a7 1
	mv a0 t1
	ecall

	li a7 4
	la a0 overflow_p2
	ecall
	
	li a7 1
	mv a0 t2
	ecall
	
	j end

error:
	# Message that the array's size is incorrect and jump to new attempt
	li a7 4
	la a0 error_prompt
	ecall
	j size
	
end:	# Stop
	li a7 10
	ecall