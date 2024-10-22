.text
autotests:
	# Save return address
	addi 	sp sp -4
	sw	ra (sp)
	
	# Call tests and print result
	call 	store_test_1
	print_array(array_A, n)
	call 	work
	
	call 	store_test_2
	print_array(array_A, n)
	call 	work
	
	call 	store_test_3
	print_array(array_A, n)
	call 	work
	
	call 	store_test_4
	print_array(array_A, n)
	call 	work
	
	call 	store_test_5
	print_array(array_A, n)
	call 	work
	
	call 	store_test_6
	print_array(array_A, n)
	call 	work
	
	call 	store_test_7
	print_array(array_A, n)
	call 	work
	
	# Load return address
	lw	ra (sp)
	addi 	sp sp 4
	ret
	
# test 1
store_test_1:
	la	t0 array_A
	li	t1 1
	sw 	t1 (t0)
	li 	t1 2
	sw 	t1 4(t0)
	li 	t1 3
	sw 	t1 8(t0)
	li 	t1 4
	sw 	t1 12(t0)
	li 	t1 5
	sw 	t1 16(t0)
	la 	t0 n
	li 	t1 5
	sw 	t1 (t0)
	ret
	
# test 2
store_test_2:
	la	t0 array_A
	li	t1 -1
	sw 	t1 (t0)
	li 	t1 -2
	sw 	t1 4(t0)
	li 	t1 -3
	sw 	t1 8(t0)
	li 	t1 -4
	sw 	t1 12(t0)
	la 	t0 n
	li 	t1 4
	sw 	t1 (t0)
	ret
	
# test 3
store_test_3:
	la	t0 array_A
	li	t1 0
	sw 	t1 (t0)
	li 	t1 0
	sw 	t1 4(t0)
	li 	t1 0
	sw 	t1 8(t0)
	la 	t0 n
	li 	t1 3
	sw 	t1 (t0)
	ret
	
# test 4
store_test_4:
	la	t0 array_A
	li	t1 -4
	sw 	t1 (t0)
	li 	t1 5
	sw 	t1 4(t0)
	li 	t1 0
	sw 	t1 8(t0)
	li	t1 -5
	sw	t1 12(t0)
	li	t1 52
	sw	t1 16(t0)
	la 	t0 n
	li 	t1 5
	sw 	t1 (t0)
	ret
	
# test 5
store_test_5:
	la	t0 array_A
	li	t1 0
	sw 	t1 (t0)
	li 	t1 1
	sw 	t1 4(t0)
	li 	t1 234
	sw 	t1 8(t0)
	li 	t1 0
	sw 	t1 12(t0)
	la 	t0 n
	li 	t1 4
	sw 	t1 (t0)
	ret
	
# test 6
store_test_6:
	la	t0 array_A
	li	t1 0
	sw 	t1 (t0)
	li 	t1 -1
	sw 	t1 4(t0)
	li 	t1 -234
	sw 	t1 8(t0)
	li 	t1 0
	sw 	t1 12(t0)
	la 	t0 n
	li 	t1 4
	sw 	t1 (t0)
	ret
	
# test 7
store_test_7:
	la	t0 array_A
	li	t1 -123
	sw 	t1 (t0)
	li 	t1 1
	sw 	t1 4(t0)
	li 	t1 234
	sw 	t1 8(t0)
	li 	t1 -5
	sw 	t1 12(t0)
	la 	t0 n
	li 	t1 4
	sw 	t1 (t0)
	ret