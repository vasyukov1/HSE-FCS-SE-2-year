.macro pow_double_int(%x, %y, %res)
.text
	# Set default meanings
	fmv.d	fa3 %x
	li	t3 1
	fcvt.d.w fa4 t3
	mv	t2 %y
	
	# The loop for counting of time of power
loop:	beqz 	t2 end
	addi 	t2 t2 -1
	fmul.d	fa4 fa4 fa3
	j 	loop
	
end: 	fmv.d 	%res fa4
.end_macro


.macro print(%x)
.data
sep:	.asciz "\n"
.text
	# Read input meaning to registr fa0
	fcvt.d.w ft0 s0
	fadd.d	ft0 %x ft0
	fmv.d	fa0 ft0
	
	li	a7 3
	ecall
	# Output separation
	li	a7 4
	la	a0 sep
	ecall
.end_macro		

		
.macro f(%x, %res)
# f(x) = х^5 - х - 0.2
.text
	fmv.d	ft8 %x
	li	t5 5
	pow_double_int(ft8, t5, fa4) # x^5
	fsub.d	fa4 fa4 ft8 # x^5 - x
	
	li	t3 2
	li	t4 10
	fcvt.d.w ft8 t3
	fcvt.d.w ft9 t4
	fdiv.d	ft8 ft8 ft9 # 0.2
	
	fsub.d	fa4 fa4 ft8 # x^5 - x - 0.2
	fmv.d	%res fa4
.end_macro


.macro user()
.text
user_eps:
	# Getting epsilon
	get_epsilon(ft0)
	# Checking epsilon
	check_epsilon(ft0, t1)
	beqz  	t1 user_eps	
.end_macro


.macro get_epsilon(%x)
.data
prompt_eps:	.asciz "Input the epsilon from 0.1 to 0.00000001: "
.text
	# Message for input epsilon
	li  	a7 4
  	la  	a0 prompt_eps
  	ecall
  	# Read number
  	li  	a7 7
  	ecall
  	# Set number to register %x  	
  	fadd.d	%x fa0 ft0
.end_macro


.macro check_epsilon(%eps, %x)
.data
eps_min:	.double 0.00000001
eps_max:	.double 0.001
prompt_er_eps:	.asciz "\nYou need input the number from 0.1 to 0.00000001. Try again!\n"
.text
	fmv.d	fa0 %eps
	# Boundary values of epsilon
	fld 	ft0 eps_min t0
	fld	ft1 eps_max t0
	# Checking
	fge.d	t1 fa0 ft0
	beqz    t1 error_eps
	
	fle.d	t1 fa0 ft1
	beqz 	t1 error_eps
	
	# Set 1 (true), if epsilon is correct
	li	%x 1
	j	end_check
error_eps:	
	# Message that the array size is incorrect
	li 	a7 4
	la 	a0 prompt_er_eps
	ecall
	# Set 0 (false), if size is incorrect
	li	%x 0
end_check:
	# Save epsilon on the stack
	addi	sp sp -8
	fsd	fa0 (sp)
.end_macro

.macro print_eps(%eps)
.data
prompt_eps:	.asciz "Result for the test with epsilon = "
prompt_eps_2:	.asciz ": "
.text
	li	a7 4
	la 	a0 prompt_eps
	ecall
	
	# Output epsilon
	fmv.d	fa0 %eps
	li	a7 3
	ecall
	
	li	a7 4
	la 	a0 prompt_eps_2
	ecall
.end_macro