.text
        la a0, string       # buffer
        li a7, 4            # syscall write (4)
.data
    string:  .asciz "Hello! It works!!!\n"
.text
        ecall
        li a0, 0            # exit code
        li a7, 10           # syscall exit
        ecall
