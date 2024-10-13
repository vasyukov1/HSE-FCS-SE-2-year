
.data
res_p1:	.asciz "Max argument of factorial in 32-bit letter is "
res_p2:	.asciz ".\nFactorial: "

.text
main:	# The first part of message for answer
	li a7 4
	la a0 res_p1
	ecall
	
	# Calculation
	jal fact
	
	# Result is already in a0
	# Output the argument of max factorial
	li a7 1
	ecall
	
	# The second part of message for answer
	li a7 4
	la a0 res_p2
	ecall
	
	# Output the value of max factorial
	li a7 1
	mv a0 a1
	ecall
	
	# Stop
	li a7 10
	ecall

fact:	# Return: a0 - n (agrument of factorial), a1 - n! (value of factorial)
	# Reserve 3 numbers on stack for ra, argument and factorial
	addi sp sp -12 
	sw ra 8(sp)
	sw s0 4(sp)
	sw s1 (sp)
	
	li s0 1		# Variable for the argument of factorial
	li s1 1 	# Variable for the value of factorial
	
loop:	addi s0 s0 1 	# Increase argument

	sw ra 8(sp)
	sw s0 4(sp)
	sw s1 (sp)
	
	mv a0 s0
	mv a1 s1
	
	jal check
	
	lw ra 8(sp)
	lw s0 4(sp)
	lw s1 (sp)
	
	# If there isn't overflow, go next
	beqz a0 end_loop 
	mul s1 s0 s1
	j loop
	
end_loop:
	# Return value of argument to correct after overflow
	addi a0 s0 -1 
	mv a1 s1
	
	# Restoring 
	lw ra 8(sp)
	lw s0 4(sp)
	lw s1 (sp)
	addi sp sp 12
	
	ret
	
check:	# Boolean function for checking overflow: 0 if overflow, else 1
	mul t0 a0 a1
	div t1 t0 a0
	
	bne t1 a1 overflow
	li a0 1
	ret

overflow:
	li a0 0
	ret
