.global rstr_rec_s

# Reverse a string recursively
# a0 = char *dst
# a1 = char *src

# t0 = len
# t1 = i
# t2 = j

rstr_rec_s:
    addi sp, sp, -48
    sd ra, 0(sp)
	mv a2, a1
    j strlen_s

setup:
    mv t1, x0
    mv t2, x0
    addi t1, t0, -1   
    call rstr_rec_func_s

rstr_rec_func_s:
    add t3, a2, t1    # t3 = a2 + t1 (i)
    lb t4, (t3)       # t4 = src[t3]

    add t5, a0, t2    # t5 = a0 + t2 (j)

    beq t4, zero, base_case_done

    sb t4, (t5)

    addi t1, t1, -1
    addi t2, t2, 1
    call rstr_rec_func_s

base_case_done:
    sb zero, (t5)
    ld ra, 0(sp)
    addi sp, sp, 48
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
