.data 
n:		.word 	0
array_A:	.space 	40
array_B:	.space 	40

prompt_start:	.asciz "\nMENU\nEnter '0': input your array.\nEnter other symbol: launch autotests.\n\nYour choice: "
prompt_next:	.asciz "If you want to finish program, enter '0', otherwise press any other number to restart: "
sep:		.asciz " "
newline:	.asciz "\n"

.include "macros.asm"
.include "solution.asm"
.include "user.asm"
.include "tests.asm"

.text
.global main
main:
	# Output message for choice own array or autotests
	li	a7 4
	la	a0 prompt_start
	ecall
	# Read number of choice
	li	a7 5
	ecall
	beqz	a0 your_array
	j	to_tests
your_array:
	# Creating own array A
	call 	user_array
	# Creating array B
	call	work
	
	# Message with an offer to continue
	li	a7 4
	la	a0 prompt_next
	ecall
	# Read number of choice
	li	a7 5
	ecall
	beqz	a0 end
	j	your_array
to_tests:
	# Launch autotests
	call 	autotests
end:
	# Stop
	li	a7 10
	ecall
