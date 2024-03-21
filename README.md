# NT-Language
number tool language and dynamic risc-v assembly code generation

$ ./ntlang -e "(a0 + a1) * a2" -a0 3 -a1 4 -a2 5 <br>
35

$ ./ntlang -e "(a0 + a1) * a2" -c foo > foo.s <br>
$ gcc -o foo foo.s<br>
$ ./foo 3 4 5<br>
35 (0x23)<br>
$ ./foo 1 2 3<br>
9 (0x9)

$ ./ntlang -e "(a0 + a1) * a2" -c foo > foo.s<br>

generated code - <br>
foo:<br>
    addi sp, sp, -4<br>
    sw a0, (sp)<br>
    addi sp, sp, -4<br>
    sw a1, (sp)<br>
    lw t1, (sp)<br>
    addi sp, sp, 4<br>
    lw t0, (sp)<br>
    add t0, t0, t1<br>
    sw t0, (sp)<br>
    addi sp, sp, -4<br>
    sw a2, (sp)<br>
    lw t1, (sp)<br>
    addi sp, sp, 4<br>
    lw t0, (sp)<br>
    mul t0, t0, t1<br>
    sw t0, (sp)<br>
    lw a0, (sp)<br>
    addi sp, sp, 4<br>
    ret<br>
