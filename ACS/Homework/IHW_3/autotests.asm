.include "program.asm"

.macro load_test(%s)
.data 
test:	.asciz %s

.text
	la 		t0 test
	la 		t1 file_name
test_loop:
	lb 		t2 (t0)
	sb 		t2 (t1)
	addi 	t0 t0 1
	addi 	t1 t1 1
	bnez 	t2 test_loop
.end_macro


.text
# void non parametric func, runs tests by existing files
run_tests:
	addi 	sp sp -4
	sw 		ra (sp)
	
	load_test("texts/1984.txt")
	call program
	
	load_test("texts/Never Gonna Give You Up.txt")
	call program
	
	load_test("texts/Holy Bible.txt")
	call program

	lw 		ra (sp)
	addi 	sp sp 4
	
	ret
