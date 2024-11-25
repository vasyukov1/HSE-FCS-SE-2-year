.macro strncpy(%dest, %src, %n)
.text 
start_strncpy:
	mv		t0 a2	# dest
	mv		t1 a3	# src
	mv		t2 a4	# n
	
loop_strncpy:
	# If n <= 0, go to the end.
	blez	t2 end_strncpy
	
	# Read the char.
	lb		t3 (t1)
	
	# If char == '\0', go to the addition of nulls.
	beqz	t3 add_null_strncpy
	
	# Store the char to destination.
	sb		t3 (t0)
	
	# Update counters.
	addi	t0 t0 1
	addi	t1 t1 1
	addi	t2 t2 -1
	
	j		loop_strncpy
	
add_null_strncpy:
	# Store '\0' to the destionation.
	sb		zero (t0)
	
	# Update counters.
	addi	t0 t0 1
	addi	t2 t2 -1
	
	# If n > 0, add '\0' again.
	bgtz	t2 add_null_strncpy
	
end_strncpy:
	mv		a0 a2
.end_macro