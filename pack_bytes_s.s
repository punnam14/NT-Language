.global pack_bytes_s

# a0 = b3
# a1 = b2
# a2 = b1
# a3 = b0

pack_bytes_s:
	mv t0, x0
	add t0, t0, a0
	li t1, 8

	sllw t0, t0, t1
	or t0, t0, a1

	sllw t0, t0, t1
	or t0, t0, a2

	sllw t0, t0, t1
	or t0, t0, a3

	mv a0, t0
	
    ret
