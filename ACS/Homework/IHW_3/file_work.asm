.include "macrolib.m"

.eqv    NAME_SIZE 256	# buffer size for filename
.eqv    TEXT_SIZE 512	# buffer size for text part
.eqv 	MAX_TEXT_SIZE 10240 # maximum length of text

.data
er_name_mes: .asciz "Incorrect file name\n"
er_read_mes: .asciz "Incorrect read operation\n"

file_name:	.space NAME_SIZE	# filename
strbuf:		.space TEXT_SIZE # buffer for text part

.text
# Read file name
# ------------------------------------------------------------------------------
read_name_of_file:
    addi 	sp sp -4
    sw 		ra (sp)
    
    li 		a3 0
input_name:
	# Prompt
    print_str("Input path to file for reading: ")
    # Get file name
    str_get(file_name, NAME_SIZE)
    newline
    
    beqz 	a3 read_name_end
    call 	read_string_from_file # if user input wrong name in last time
    
read_name_end:
	# End read
    lw 		ra (sp)
    addi 	sp sp 4
# ------------------------------------------------------------------------------
    

# Read text from file
# ------------------------------------------------------------------------------
read_string_from_file:
    addi 	sp sp -32
    sw 		s0 28(sp)
    sw 		s1 24(sp)
    sw 		s2 20(sp)
    sw 		s3 16(sp)
    sw 		s4 12(sp)
    sw 		s5 8(sp)
    sw 		s6 4(sp)
    sw 		ra (sp)
    
    open(file_name, READ_ONLY)
    # Error checking
    li		s1 -1
    beq		a0 s1 er_name
    # Move file descriptor
    mv   	s0 a0
   
   	# Get new memory
    allocate(TEXT_SIZE)
    # Address of the first symbol
    mv 		s3 a0
    # Address of the last symbol
    mv 		s5 a0
    li		s4 TEXT_SIZE	
    mv		s6 zero
    
read_loop:
	# Read a part of text
    read_addr_reg(s0, s5, TEXT_SIZE)
    # Error checking
    beq		a0 s1 er_read
    # Save string length
    mv   	s2 a0
    # Add text size
    add 	s6 s6 s2
    # Check text size
    li 		t0 MAX_TEXT_SIZE
    bge 	s6 t0 end_loop
    # Check, that have '\0'
    bne		s2 s4 end_loop
    # Get new memory
    allocate(TEXT_SIZE)
    add		s5 s5 s2		
    b 		read_loop
    			
end_loop:
	# Close file
    close(s0)

	# Address of the first and the last symbols
    mv		t0 s3
    add 	t0 t0 s6
    addi 	t0 t0 1	
    # Add '\0'
    sb		zero (t0)

    mv a0 s3 # adress of begining string
    mv a1 s6 # string length
    
    lw 		s0 28(sp)
    lw 		s1 24(sp)
    lw 		s2 20(sp)
    lw 		s3 16(sp)
    lw 		s4 12(sp)
    lw 		s5 8(sp)
    lw 		s6 4(sp)
    lw 		ra (sp)
    addi	sp sp 32
    ret
# ------------------------------------------------------------------------------
    
# Save result to file
# Parameters:
# a0 - string address
# a1 - string size
# ------------------------------------------------------------------------------
save_string_to_file:
    addi 	sp sp -32
    sw 		s0 28(sp)
    sw 		s1 24(sp)
    sw 		s2 20(sp)
    sw 		s3 16(sp)
    sw 		s4 12(sp)
    sw 		s5 8(sp)
    sw 		s6 4(sp)
    sw 		ra (sp)
    
    # String address
    mv 		s3 a0
    # String size
    mv 		s6 a1

	# Prompt
	newline
    print_str("Input path to file for writing: ")
    # Get file name
    str_get(file_name, NAME_SIZE)
    # Open file for writing
    open(file_name, WRITE_ONLY)
    # Error checking
    li		s1 -1			
    beq		a0 s1 er_name
    # Save file descriptor
    mv   	s0 a0

	# Write string
    li   	a7 64       		
    mv   	a0 s0
    mv   	a1 s3
    mv   	a2 s6
    ecall             		

    lw 		s0 28(sp)
    lw 		s1 24(sp)
    lw 		s2 20(sp)
    lw 		s3 16(sp)
    lw 		s4 12(sp)
    lw 		s5 8(sp)
    lw 		s6 4(sp)
    lw 		ra (sp)
    addi 	sp sp 32
   	ret
# ------------------------------------------------------------------------------

# Errors
# ------------------------------------------------------------------------------
er_name:
   	# File name error 
    li 		a3 1
    la		a0 er_name_mes
    li		a7 4
    j 		input_name

er_read:
    # Reading file error
    li 		a3 1
    la		a0 er_read_mes
    li		a7 4
    j 		input_name
# ------------------------------------------------------------------------------
   
# Output to console about print to file 
# Parameters:
# a0 - string address
# a1 - string size
# ------------------------------------------------------------------------------
.data
out_to_console:	.asciz "\nDo you want to print output into console? (Y/N)  ==> "
.text
output:
 	addi 	sp sp -12
 	sw 		s0 8(sp)
 	sw 		s1 4(sp)
 	sw 		ra (sp)
 	
 	# String address
 	mv 		s0 a0
 	# String size
 	mv 		s1 a1

	# Prompt
 	la 		a0 out_to_console
 	li 		a7 4
 	ecall 
 	
yes_no_loop:
	# Ask char
 	li 		a7 12
 	ecall
 	# 'Y'
 	li 		t0 89 
 	beq 	t0 a0 console_out
 	# 'N'
 	li 		t0 78 
 	beq 	t0 a0 file_out
 	j 		yes_no_loop

console_out:
 	mv 		a0 s0
 	li 		a7 4
 	ecall
 	j 		end_write
 	
file_out:
 	mv 		a0 s0
 	mv 		a1 s1
 	call 	save_string_to_file
 	
end_write:
	newline
	lw 		s0 8(sp)
 	lw 		s1 4(sp)
 	lw 		ra (sp)
 	addi 	sp sp 12
 	ret
# ------------------------------------------------------------------------------
