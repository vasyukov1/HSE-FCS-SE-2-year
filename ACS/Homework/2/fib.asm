#
# Example that calculates the Fibonacci sequence.
#
main:
	mv t0, zero		# pseudo: addi t0, zero, 0 (I-type)
	li t1, 1		# pseudo: addi t1, zero, 1 (I-type)
	
	li a7, 5		# pseudo: addi a7, zero, 5 (I-type)
	ecall			# I-type
	mv t3, a0		# pseudo: addi t3, a0, 0 (I-type)
fib:
	beqz t3, finish		# pseudo: beq t3. zero, finish (B-type)
	add t2, t1, t0		# R-type
	mv t0, t1		# pseudo: addi t0, t1, 0 (I-type)
	mv t1, t2		# pseudo: addi t1, t2, 0 (I-type)
	addi t3, t3, -1		# I-type
	j fib			# J-type
finish:
	li a7, 1		# pseudo: addi a7, zero, 1 (I-type)
	mv a0, t0		# pseudo: addi a0, t0, 0 (I-type)
	ecall			# I-type
