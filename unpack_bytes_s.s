.global unpack_bytes_s

# a0 = val
# a1 = bytes[]
# t0 = b
# t1 = i


unpack_bytes_s:
    mv t1, x0    
    li t6, 4      
    li t5, 0xFF   
    li t4, 8      
    mv a2, a0      
	
for:
    beq t1, t6, done 
    and t0, a2, t5  

    li t2, 4
    mul t2, t1, t2
    add t2, a1, t2
    sb t0, 0(t2)   
    
    srlw a2, a2, t4   
    addi t1, t1, 1      
	j for
	
done:
    ret
