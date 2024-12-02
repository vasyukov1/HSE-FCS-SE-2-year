# Print int from register %x
.macro print_int(%x)
	li 	a7 1
	mv 	a0 %x
	ecall
.end_macro

# Print int
.macro print_imm_int(%x)
	li 	a7 1
   	li 	a0 %x
   	ecall
.end_macro

# Print string
.macro print_str(%x)
.data
str:		 .asciz %x
.align 2
.text
   push(a0)
   li 	a7 4
   la	a0 str
   ecall
   pop(a0)
.end_macro

# Read char from consol
.macro read_char(%x)
	li 	a7 12
	ecall
	mv 	%x a0
.end_macro

# Print char
.macro print_char(%x)
   li 	a7 11
   li 	a0 %x
   ecall
.end_macro

# Print new line
.macro newline
   print_char('\n')
.end_macro

# Read int to register %x
.macro read_int(%x)
   push(a0)
   li 	a7 5
   ecall
   mv 	%x a0
   pop(a0)
.end_macro

# Input string to buffer
.macro str_get(%strbuf, %size)
	# %strbuf - address of buffer
	# %size - size of buffer
	push(s0)
    push(s1)
    push(s2)
    la		a0 %strbuf
    li  	a1 %size
    li  	a7 8
    ecall
    
    li		s0 '\n'
    la		s1	%strbuf
next:
    lb		s2 (s1)
    beq 	s0 s2 replace
    addi	s1 s1 1
    j	next
    
replace:
    sb		zero (s1)
    pop(s2)
    pop(s1)
    pop(s0)
.end_macro

# Open file
.eqv READ_ONLY	0	# Open file for reading
.eqv WRITE_ONLY	1	# Open file for writing
.eqv APPEND	    9	# Open file for appending
.macro open(%file_name, %opt)
    li   a7 1024
    la   a0 %file_name
    li   a1 %opt
    ecall
.end_macro

# Read file
.macro read(%file_descriptor, %strbuf, %size)
    li	a7 63
    mv  a0 %file_descriptor
    la	a1 %strbuf
    li  a2 %size
    ecall
.end_macro

# Read from file descriptor into buffer
.macro read_addr_reg(%file_descriptor, %reg, %size)
    li	a7 63
    mv  a0 %file_descriptor
    mv  a1 %reg
    li  a2 %size
    ecall
.end_macro

# Close file
.macro close(%file_descriptor)
    li	a7 57
    mv  a0 %file_descriptor
    ecall
.end_macro

# Get new dynamic memory
.macro allocate(%size)
    li	a7 9
    li 	a0 %size
    ecall
.end_macro

# Stop
.macro exit
    li 	a7 10
    ecall
.end_macro

# Save register on the stack
.macro push(%x)
	addi	sp sp -4
	sw		%x (sp)
.end_macro

# Retern register from the stack
.macro pop(%x)
	lw		%x (sp)
	addi	sp sp 4
.end_macro
