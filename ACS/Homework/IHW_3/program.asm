.include "file_work.asm"
.include "worker.asm"

.text
program:
	addi	sp sp -4
	sw 		ra (sp)
	
	# Read file and get:
	# a0 - string address
	# a1 - string size
	call 	read_string_from_file
	
	#----------------------------------------------------------------
	# Solve task and return:
	# a0 - number of lowercase letters
	# a1 - number of uppercase letters
	call 	count

	# Create string for result. Return:
	# a0 - string address
	# a1 - string size
	call 	create_string

	# Write result to file or to console
	call 	output

	lw 		ra (sp)
	addi 	sp sp 4
	ret
