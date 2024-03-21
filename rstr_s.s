.global rstr_s

# Reverse a string iteratively
# a0 = char *dst
# a1 = char *src

# len = t0
# i = t2
# j = t3

rstr_s:
    mv a2, a1
	j strlen_s

setup:
    li t2, 0
    mul t3, t3, zero
    add t3, t3, t0
    addi t3, t3, -1
    j for

for:
    bge t2, t0, return

    add t4, a2, t3       # t2 = a1 + t3 (j)
    lb t5, (t4)          # t5 = src[t4]

    add t6, a0, t2       # t6 = a0 + i
    sb t5, (t6)
    
    addi t2, t2, 1
    addi t3, t3, -1
    j for

return:
    add t6, a0, t2
    sb zero, (t6)
    ret

strlen_s:
    li t0, 0             # t0 (len) = 0
    j while

while:
    lb t1, (a1)          # t1 = *a0
    beq t1, zero, done   # is t1 == '\0'?
    addi t0, t0, 1       # t1 (len) = t1 (len) + 1
    addi a1, a1, 1       # a0 = a0 + 1 
    j while

done:
    j setup
