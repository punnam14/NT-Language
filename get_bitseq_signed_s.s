.global get_bitseq_signed_s
.global get_bitseq_s

# a0 - int32_t num 
# a1 - int start
# a2 - int end

# t3 - unint32_t val
# t4 - val_signed
# t6 - len
# a4 - shift_amt

get_bitseq_signed_s:
	addi sp, sp, -16
	sd ra, (sp)
    sub t6, a2, a1
    addi t6, t6, 1
    li a4, 32
    sub a4, a4, t6

    call get_bitseq_s

    sllw t3, a0, a4
    sraw t4, t3, a4

    mv a0, t4
    ld ra, (sp)
    addi sp, sp, 16
    ret
