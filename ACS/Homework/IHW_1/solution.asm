.text
work:
	# Adding positive elements, zeros, negative elements
	add_positive(array_A, array_B, n, t6)
	add_zeros(array_A, t6, n, t6)
	add_negative(array_A, t6, n)
	# Print array
	print_array(array_B, n)
	# Print new line
	li	a7 4
	la	a0 newline
	ecall
	ret
