# NT-Language
number tool language and dynamic risc-v assembly code generation

$ ./ntlang -e "(a0 + a1) * a2" -a0 3 -a1 4 -a2 5 <br>
35

$ ./ntlang -e "(a0 + a1) * a2" -c foo > foo.s
$ gcc -o foo foo.s
$ ./foo 3 4 5
35 (0x23)
$ ./foo 1 2 3
9 (0x9)

$ ./ntlang -e "(a0 + a1) * a2" -c foo > foo.s

generated code - 
foo:
    addi sp, sp, -4
    sw a0, (sp)
    addi sp, sp, -4
    sw a1, (sp)
    lw t1, (sp)
    addi sp, sp, 4
    lw t0, (sp)
    add t0, t0, t1
    sw t0, (sp)
    addi sp, sp, -4
    sw a2, (sp)
    lw t1, (sp)
    addi sp, sp, 4
    lw t0, (sp)
    mul t0, t0, t1
    sw t0, (sp)
    lw a0, (sp)
    addi sp, sp, 4
    ret
