.data
	dividend:	.asciz "Input dividend: "
	divisor:	.asciz "Input divider: "
	error_input:	.asciz "It isn't a number. Try again."
	error_zero:	.asciz "Division by zero."
	ceil:		.asciz "Ceil: "
	remainder:	.asciz "Remainder: "
	new_line:	.asciz "\n"

.text

main:
	# Input dividend
	li a7 4
	la a0 dividend
	ecall
	li a7 5
	ecall
	mv t0 a0

	# Input divivsor
	li a7 4
	la a0 divisor
	ecall
	li a7 5
	ecall
	mv t1 a0
	
	# Check by zero
	beqz t1 division_by_zero
	
	# Xor for dividend and divisor. If they have different signs, t2 < 0, else t2 >= 0
	xor t2 t1 t0
	
	# t2 = (t2 < 0) ? 1 : 0. If signs of dividend and divisor are different t2 = 1, else t2 = 0.
	slti t2 t2 0
	# t5 = (t0 < 0) ? 1 : 0. If the sign of dividend is negative, the sign of the remainder will be negative too.
	slti t5 t0 0
	
	# if t0 < 0 goto negative_dividend
	bltz t0 negative_dividend
	j continue_dividend
	
# Change sign of dividend to positive
negative_dividend:
 	neg t0 t0

continue_dividend:
	# if t1 < 0 goto negative_divisor
	bltz t1 negative_divisor
	j continue_divisor
	
# Change sign of divisor to positive
negative_divisor:
	neg t1 t1
	
# Set the ceiling part and the remainder
continue_divisor:
	# ceil
	li t3 0 
	# remainder	
	mv t4 t0	
	
# Substraction cycle
division:
	blt t4 t1 check_ceil_sign
	sub t4 t4 t1
	addi t3 t3 1
	j division

# Change the sign if the ceiling part is negative
check_ceil_sign:
	beqz t2 check_remainder_sign
	neg t3 t3

check_remainder_sign:
	beqz t5 unput_result
	neg t4 t4
	
# Input result
unput_result:
	# Input ceiling part
	li a7 4
	la a0 ceil
	ecall
	mv a0 t3
	li a7 1
	ecall
	
	# New line for aesthetics
	li a7 4
	la a0 new_line
	ecall
	
	# Input remainder
	li a7 4
	la a0 remainder
	ecall
	mv a0 t4
	li a7 1
	ecall
	
	j exit	

# Input error if divisor equals zero
division_by_zero:
	li a7 4
	la a0 error_zero
	ecall

# Stop
exit:
	li a7 10
	ecall
	
