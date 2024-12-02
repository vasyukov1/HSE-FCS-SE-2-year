.text
# Parameter: a0 - letter
# Return: 
# 1 - lowercase
# 2 - uppercase
# 0 - other
# ------------------------------------------------------------------------------
.text
letter_type:	
	li		t0 97	# a			
	li		t1 122	# z
	
	li		t2 65	# A
	li		t3 90	# Z
	
	# Need t2 <= a0 <= t3
	blt		a0 t2 skip
	ble		a0 t3 is_up
	
	# Need t0 <= a0 <= t1
	blt		a0 t0 skip
	ble		a0 t1 is_low

skip:
	li a0 0
	
	ret
is_low:
	li a0 1
	ret
is_up:
	li a0 2
	ret
# ------------------------------------------------------------------------------

# FUNCTION - count lowercase and uppercse letters
# Parameters:
# 	a0 - string address
# 	a1 - string size
# Return:
# 	a0 - number of lowercase letters
#	a1 - number of uppercase letters
# ------------------------------------------------------------------------------
count:
	addi 	sp sp -24
	sw 		s0 20(sp)
	sw 		s1 16(sp)
	sw 		s2 12(sp)
	sw 		s3 8(sp)
	sw 		s4 4(sp)
	sw 		ra(sp)

	# Save string address
	mv 		s0 a0
	# Save string size
	mv 		s1 a1
	# Index in string
	li 		s2 0
	# Number of lowercase letters
	li 		s3 0
	# Number of uppercase letters
	li 		s4 0

string_loop:
	# Get letter
	lb 		a0 (s0)
	# Get letter type
	call 	letter_type
	beqz 	a0 ass_embler
	# Change: 
	# lowercase -> 0
	# uppercase -> 1
	addi 	a0 a0 -1
	
	# Add lowercase
	beqz 	a0 add_low
	
	# Add uppercase
	addi 	s4 s4 1
	j 		ass_embler

add_low:
	addi 	s3 s3 1

ass_embler:
	# Go to the next letter
	addi 	s0 s0 1
	addi 	s2 s2 1
	blt 	s2 s1 string_loop

	# Save lowercase and uppercase letters
	mv 		a0 s3
	mv 		a1 s4

	lw 		s0 20(sp)
	lw 		s1 16(sp)
	lw 		s2 12(sp)
	lw 		s3 8(sp)
	lw 		s4 4(sp)
	lw 		ra(sp)
	addi 	sp sp 24

	ret
# ------------------------------------------------------------------------------
	
# Create result string
# Parameters:
# a0 - number of lowercase letters
# a1 - number of uppercase letters
# Return:
# a0 - string address
# a1 - string size
# ------------------------------------------------------------------------------
.data
low: 	.asciz "\nnumber of lowercase letters: "
up: 	.asciz "\nnumber of uppercase letters: "
.text
create_string:
	addi 	sp sp -32
	sw 		s0 28(sp)
	sw 		s1 24(sp)
	sw 		s2 20(sp)
	sw 		s3 16(sp)
	sw 		s4 12(sp)
	sw 		s5 8(sp)
	sw 		s6 4(sp)
	sw 		s7 (sp)
	
	# Save numbers
	mv 		s0 a0
	mv 		s1 a1
	
	# memory allocation
	li 		a7 9
	li 		a0 100
	ecall
	# Address of the first and the last sumbols
	mv 		s5 a0
	mv 		s7 a0
	# Size of result string
	mv 		s6 zero
	
	la 		s2 low
	li 		s3 0
	# String size
	li 		s4 30
	
low_loop:
	lb 		t0 (s2)
	sb 		t0 (s7)
	addi 	s7 s7 1
	addi 	s3 s3 1
	addi 	s2 s2 1
	blt 	s3 s4 low_loop
	
	add 	s6 s6 s4
	
	# Get 1 digit
	li 		t0 1000 
	div 	t1 s0 t0
	# Add '0' for ASCII
	addi	t1 t1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	# Get 2 digit
	li 		t0 1000 
	rem 	s0 s0 t0
	li 		t0 100
	div 	t1 s0 t0
	# Add '0' for ASCII
	addi 	t1 t1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	# Get 3 digit
	li 		t0 100
	rem 	s0 s0 t0
	li 		t0 10 
	div 	t1 s0 t0
	# Add '0' for ASCII
	addi 	t1 t1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	# Get 4 digit
	li 		t0 10
	rem 	s0 s0 t0
	# Add '0' for ASCII
	addi 	t1 s0 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	addi 	s6 s6 4
	
	la 		s2 up
	li 		s3 0
	# String size
	li 		s4 30
	
up_loop:
	lb 		t0 (s2)
	sb 		t0 (s7)
	addi 	s7 s7 1
	addi 	s3 s3 1
	addi 	s2 s2 1
	blt 	s3 s4 up_loop
	
	add 	s6 s6 s4
	
	# Get 1 digit
	li 		t0 1000
	div 	t1 s1 t0
	# Add '0' for ASCII
	addi 	t1 t1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	# Get 2 digit
	li 		t0 1000
	rem 	s1 s1 t0
	li 		t0 100
	div 	t1 s1 t0
	# Add '0' for ASCII
	addi 	t1 t1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	# Get 3 digit
	li 		t0 100
	rem 	s1 s1 t0
	li 		t0 10 
	div 	t1 s1 t0
	# Add '0' for ASCII
	addi 	t1 t1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	# Get 4 digit
	li 		t0 10
	rem 	s1 s1 t0
	# Add '0' for ASCII
	addi 	t1 s1 48
	sb 		t1 (s7)
	addi 	s7 s7 1
	
	addi 	s6 s6 4

	addi 	s6 s6 1
	sb 		zero (s7)
	
	# String address
	mv 		a0 s5
	# String size
	mv 		a1 s6
	
	lw 		s0 28(sp)
	lw 		s1 24(sp)
	lw 		s2 20(sp)
	lw 		s3 16(sp)
	lw 		s4 12(sp)
	lw 		s5 8(sp)
	lw 		s6 4(sp)
	lw 		s7 (sp)
	addi 	sp sp 32
	
	ret
# ------------------------------------------------------------------------------