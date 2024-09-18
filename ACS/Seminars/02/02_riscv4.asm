li s2 10
li s1 1
loop:
li a7 1
mv a0 s1
ecall
addi s1 s1 1
blt s1 s2 loop
li a7 10
ecall