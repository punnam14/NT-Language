.global main
.global codegen_main
.extern atoi
.global printf

.data
out_string:
    .string "%d (0x%X)\n"
    
.text

main:
    addi sp, sp, -112    
    sd ra, 104(sp)
    li t6, 8
    li t0, 1
    mv t1, a0
    mv t2, a1

first_loop:
    sd zero, 0(sp)
    sd zero, 8(sp)
    sd zero, 16(sp)
    sd zero, 24(sp)
    sd zero, 32(sp)
    sd zero, 40(sp)
    sd zero, 48(sp)
    sd zero, 56(sp)

    j second_loop

second_loop:
    beq t0, t1, done_loop
    beq t0, t6, done_loop

    slli t3, t0, 3
    add t4, t2, t3
    ld a0, 0(t4)

	sd t1, 64(sp)
	sd ra, 72(sp)
    call atoi
    ld ra, 72(sp)
    ld t1, 64(sp)

    addi t5, t0, -1
    slli t5, t5, 3
    add t5, sp, t5
    sd a0, 0(t5)

    addi t0, t0, 1
    j second_loop

done_loop:
    j codegen_main_s

codegen_main_s:
	ld a0, 0(sp)
	ld a1, 8(sp)
	ld a2, 16(sp)
	ld a3, 24(sp)
	ld a4, 32(sp)
	ld a5, 40(sp)
	ld a6, 48(sp)
	ld a7, 56(sp)
