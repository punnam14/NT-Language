.global get_bitseq_s

get_bitseq_s:
    sub t1, a2, a1
    addi t1, t1, 1
    srlw t0, a0, a1

    li t3, 32
    bne t1, t3, else
    li t3, 0xFFFFFFFF
    j done
else:
    li t3, 1
    sllw t3, t3, t1
    addi t3, t3, -1
done:
    and a0, t0, t3
    ret
