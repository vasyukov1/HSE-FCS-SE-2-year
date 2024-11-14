.data
test_1:	.double 0.001
test_2:	.double 0.0001
test_3:	.double 0.00001
test_4:	.double 0.000001
test_5:	.double 0.0000001
test_6:	.double 0.00000001

.text
autotests:
	# Save return address
	addi 	sp sp -4
	sw	ra (sp)
	
	# Test 1
	fld	ft0 test_1 t0
	addi 	sp sp -8
 	fsd 	ft0 (sp)
 	print_eps(ft0)
 	secant()
 	
 	# Test 2
	fld	ft0 test_2 t0
	addi 	sp sp -8
 	fsd 	ft0 (sp)
 	print_eps(ft0)
 	secant()
 	
 	# Test 3
	fld	ft0 test_3 t0
	addi 	sp sp -8
 	fsd 	ft0 (sp)
 	print_eps(ft0)
 	secant()
 	
 	# Test 4
	fld	ft0 test_4 t0
	addi 	sp sp -8
 	fsd 	ft0 (sp)
 	print_eps(ft0)
 	secant()
 	
 	# Test 5
	fld	ft0 test_5 t0
	addi 	sp sp -8
 	fsd 	ft0 (sp)
 	print_eps(ft0)
 	secant()
 	
 	# Test 6
	fld	ft0 test_6 t0
	addi 	sp sp -8
 	fsd 	ft0 (sp)
 	print_eps(ft0)
 	secant()

	# Load return address
	lw	ra (sp)
	addi 	sp sp 4
	ret
