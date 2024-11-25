.macro user()
.eqv			MAX_SIZE 1048576
.data
enter_str_msg:	.asciz "Enter the string, which need to copy: "	
enter_n_msg:	.asciz "Enter the copy size: "
error_n_msg:	.asciz "Sorry, the number is incorrect. Try again."
result_msg:		.asciz "Result: "

src:			.space MAX_SIZE
dest:			.space MAX_SIZE
.text
enter_str:
	# Output the message for enter the string.
	la		a0 enter_str_msg
	li		a7 4
	ecall
	
	# Input the string.
	la		a0 src
	li		a1 256
	li		a7 8
	ecall
	mv		a3 a0
	
enter_n:
	# Output the message for input n.
	la		a0 enter_n_msg
	li		a7 4
	ecall
	
	# Input n.
	li		a7 5
	ecall
	
	# Chech that n > 0.
	bltz	a0 error_n
	
	mv		a4 a0
	j		end_user
	
error_n:
	# Output the message about the error whit n.
	la		a0 error_n_msg
	li		a7 4
	ecall
	j		enter_n
	
end_user:
	# Output the result.
	la		a0 result_msg
	li		a7 4
	ecall
	
	la		a2 dest
	strncpy(a2, a3, a4)
	li		a7 4
	ecall
.end_macro