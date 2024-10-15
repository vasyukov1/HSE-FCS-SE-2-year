.data
res_p1:	.asciz "RECURSIVE SOLUTION\nThe largest argument whose factorial fits in 32-bit machine word is "
res_p2:	.asciz ".\nFactorial: "

.text
main:	
	# The first part of message for answer
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

fact:	
	# Return: a0 - n (agrument of factorial), a1 - n! (value of factorial)
	# Reserve 3 numbers on stack for ra, argument and factorial
	addi sp sp -12 
	sw ra 8(sp)
	sw s0 4(sp)
	sw s1 (sp)
	
	li a2 1		# Setting the initial value of the argument
	li a3 1		# Setting the initial value of the factorial
	
	call recursive
	addi a0 a0 -1	# Adjusting the value of the argument
	
	# Restoring
	lw ra 8(sp)
	lw s0 4(sp)
	lw s1 (sp)
	addi sp sp 12
	
	ret

recursive:
	# Increasing the value of the argument
	addi s0 a2 1
	mv s1 a3
	
	# Saving
	addi sp sp -12
	sw ra 8(sp)
	sw s0 4(sp)
	sw s1 (sp)
	
	call check
	
	# Restoring
	lw ra 8(sp)
	lw s0 4(sp)
	lw s1 (sp)
	addi sp sp 12
	
	# If there is the overflow, go out
	bnez a0 end
	
	# Saving
	addi sp sp -12
	sw ra 8(sp)
	sw s0 4(sp)
	sw s1 (sp)
	
	# Increasing the factorial
	mv a2 s0
	mul a3 s1 s0
	
	call recursive
	
	# Restoring
	lw ra 8(sp)
	lw s0 4(sp)
	lw s1 (sp)
	addi sp sp 12
	
	ret
end:
	mv a0 s0
	mv a1 s1
	ret

check:	# Boolean function for checking overflow: 1 if overflow, else 0
	mul s0 a2 a3
	div s1 s0 a2
	
	bne s1 a3 overflow
	li a0 0
	ret
overflow:
	li a0 1
	ret