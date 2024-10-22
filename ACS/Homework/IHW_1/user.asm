.data
prompt_size:	.asciz "Input the size of the array from 1 to 10: "
prompt_er_size:	.asciz "\nYou need input a number from 1 to 10. Try again!\n"
prompt_element:	.asciz "Input element: "

.text
user_array:
size:	
	# Getting array size
	get_size(t0)
	la	t1 n
	sw   	t0 (t1)
	mv	a0 t0
	# Checking size
	check_size(t1)
	beqz  	t1 size
make_array_A:
	# Creating array A
	la	t0 array_A
	li	t2 0
	lw	t3 n
add_el:
	# Adding element in array A
	beq	t2 t3 stop
	add_element(t1)
	sw 	t1 (t0)
	addi	t0 t0 4
	addi	t2 t2 1
	j	add_el
stop:
	ret
	