.include "autotests.asm"

.global main
.text
main:
	# Choose autotests or user's file
	print_str("Enter 'Y' for start autotest, or another key to open your file  ==>  ")
	read_char(s0)
	newline
	
	# Char 'Y' in ASCII
	li 		t0 89
	# Check, that user enter 'Y'
	beq 	s0 t0 tests
	
	# Read file name
	call	read_name_of_file
	# Solve task
	call 	program
	exit
	
tests:
	# Autotests
	call 	run_tests 
	exit
