.include	"strncpy.asm"
.include	"user.asm"
.include	"autotests.asm"

.data
menu:	.asciz "\n\nMENU:\n1 - yours string\n2 - autotests\n0 - exit\n\nEnter: "

.text
.global main
main:
	# Output the menu.
	la		a0 menu
	li		a7 4
	ecall
	
	# Input the number of choice.
	li		a7 5
	ecall
	
	# 0 - end
	# 1 - user's input of string
	# 2 - autotests
	beqz	a0 end
	li		t0 1
	beq		a0 t0 choice_user
	li		t0 2
	beq		a0 t0 choice_tests

choice_user:
	user()
	j 		main

choice_tests:
	autotests()
	j		main
	
end:
	# Stop.
	li		a7 10
	ecall
