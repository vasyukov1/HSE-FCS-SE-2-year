.data 
prompt_start:	.asciz "\nMENU\nEnter '0': input your epsilon.\nEnter other symbol: launch autotests.\n\nYour choice: "
prompt_next:	.asciz "If you want to finish program, enter '0', otherwise press any other number to restart: "
sep:		.asciz " "
newline:	.asciz "\n"

.include "macro.asm"
.include "testing.asm"
.include "tests.asm"

.text
.global main
main:
	li	s0 0
	fcvt.d.w ft0 s0
	
	# Output message for choice own array or autotests
	li	a7 4
	la	a0 prompt_start
	ecall
	# Read number of choice
	li	a7 5
	ecall
	beqz	a0 your_eps
	j	to_tests
your_eps:
	# User iunputs the epsilon
	user()
	
	# Solution
	secant()

what_is_next:
	# Message with an offer to continue
	li	a7 4
	la	a0 prompt_next
	ecall
	# Read number of choice
	li	a7 5
	ecall
	beqz	a0 end
	j	main
to_tests:
	# Launch autotests
	call 	autotests
	j 	what_is_next
end:
	# Stop
	li	a7 10
	ecall
