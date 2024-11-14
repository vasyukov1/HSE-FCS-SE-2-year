.macro secant()
.data
start_left:	.double 1.0
start_right:	.double 1.1

.text
	fld	ft0 start_left t0
	fld	ft1 start_right t0
	fld 	ft2 (sp)
	addi	sp sp 8
	fsub.d	ft3 ft0 ft0	# double zero
	
work:
	fsub.d	ft4 ft1 ft0	# ft4 = right - left
	fabs.d	ft5 ft4		# ft5 = |right - left|
	
	fgt.d	t1 ft5 ft2	# If ft5 > eps, t1 = 1
	beqz	t1 end_secant
	
	f(ft0, fa0)	# fa0 = f(left)
	f(ft1, fa1)	# fa1 = f(right)
	
	fmul.d	ft6 fa0 ft4
	fsub.d	ft7 fa1 fa0
	fdiv.d	ft6 ft6 ft7
	fsub.d	fa2 ft0 ft6	# c = left - ft6
	
	f(fa2, fa3)		# fa3 = f(c)
	
	fgt.d 	t1 fa3 ft3	# if fa0 > ft3 (f > 0), then t1 = 1, else t1 = 0
	beqz 	t1 change_left	# if t1 == 1, then change right
	
change_right:
	fmv.d	ft1 fa2
	j	next
change_left:
	fmv.d	ft0 fa2
	
next:
	fabs.d	fa4 fa3		# fa4 = |f(c)|
	flt.d 	t1 fa4 ft2	# if fa0 < ft4 (f < eps), then t1 = 1, else t1 = 0
	bnez	t1 end_secant	# if t1 != 0, the go out
	j work
	
end_secant:	
	print(fa2)
.end_macro
	
	
