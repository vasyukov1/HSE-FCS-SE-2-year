.macro autotests()
.eqv			MAX_SIZE 1048576
.data
text_msg:	.asciz "\n\nString: "
result_msg:	.asciz "Result: "

text1:		.asciz "I want to get excellent for this homework! :)\n"
text2:		.asciz "I don't understand assembly(((\n"
text3:		.asciz "Why is so cold? I want to go to Spain...\n"

dest:		.space MAX_SIZE

.text
test1:	# Test 1. n < string.size().
	la		a0 text_msg
	li		a7 4
	ecall
	
	la		a0 text1
	li		a7 4
	ecall
	
	la		a0 result_msg
	li		a7 4
	ecall
	
	la		a2 dest
	la		a3 text1
	li		a4 13
	strncpy(a2, a3, a4)
	li		a7 4
	ecall

test2:	# Test 2. n == string.size().
	la		a0 text_msg
	li		a7 4
	ecall
	
	la		a0 text2
	li		a7 4
	ecall
	
	la		a0 result_msg
	li		a7 4
	ecall
	
	la		a2 dest
	la		a3 text2
	li		a4 30
	strncpy(a2, a3, a4)
	li		a7 4
	ecall

test3:	# Test 3. n > string.size().
	la		a0 text_msg
	li		a7 4
	ecall
	
	la		a0 text3
	li		a7 4
	ecall
	
	la		a0 result_msg
	li		a7 4
	ecall
	
	la		a2 dest
	la		a3 text3
	li		a4 1024
	strncpy(a2, a3, a4)
	li		a7 4
	ecall
.end_macro