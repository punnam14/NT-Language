/* compile.c - tree evalution and creating assembly code */

#include "ntlang.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void compile_tree(struct parse_node_st *pt, int argc, char **argv){
    char output[10000];
    char* name = "codegen_main.s";
    
    preamble(name, output, argv[argc - 1]);
    
	strcat(output, argv[argc-1]);
	strcat(output, ":\n");

    compile(pt, output);

	strcat(output, "    lw a0, (sp)\n");
	strcat(output, "    addi sp, sp, 4\n");
	strcat(output, "    mv a1, a0\n");
	strcat(output, "    mv a2, a0\n");
	strcat(output, "    la a0, out_string\n");
	strcat(output, "    call printf\n");
	strcat(output, "    ld ra, 104(sp)\n");
	strcat(output, "    addi sp, sp, 112\n");
    strcat(output, "    ret\n");
	printf("%s", output);
}

void preamble(char *name, char* output, char* filename){
	int fd;
	char c; 
	int rv;
	fd = open(name, O_RDONLY);
	while(true){
		rv = read(fd, &c, 1);
		if(rv <= 0){
			break; 
		}
		char temp[2] = {c, '\0'};
		strcat(output, temp);
	}
	strcat(output, "\n\n");
}

void compile(struct parse_node_st *pt, char* output){
    char* source;
    if(pt->type == EX_REGVAL){
        source = "    addi sp, sp, -4\n";
        strcat(output, source);

        source = "    sw ";
        strcat(output, source);

        source = pt->regname.name;
        strcat(output, source);

        source = "    , (sp)\n";
        strcat(output, source);
    } else if(pt->type == EX_INTVAL){
        source = "    addi sp, sp, -4\n";
        strcat(output, source);
        
        source = "    li t0, ";
        strcat(output, source);
        char num[50];
        sprintf(num,"%d",pt->intval.value);
        strcat(output, num);
        strcat(output, "\n");

        source = "    sw t0, (sp)\n";
        strcat(output, source);
    } else if(pt->type == EX_OPER1) {
        source = "    lw t0, (sp)\n";
        strcat(output, source);
    	compile(pt->oper1.operand,output);
        if(pt->oper1.oper == OP_MINUS){
            source = "    sub t0, x0, t0\n";
        }
        else if(pt->oper1.oper == OP_NOT){
        	source = "    li t6, -1\n";
        	strcat(output,source);
            source = "    xor t0, t0, t6\n";
        }
        strcat(output, source);

        source = "    sw t0, (sp)\n";
        strcat(output, source);
    } else if(pt->type == EX_OPER2) {
        compile(pt->oper2.left,output);
        compile(pt->oper2.right,output);
        
        source = "    lw t1, (sp)\n";
        strcat(output, source);

        source = "    addi sp, sp, 4\n";
        strcat(output, source);

        source = "    lw t0, (sp)\n";
        strcat(output, source);
        
        if (pt->oper2.oper == OP_PLUS) {
            source = "    add t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_MINUS) {
            source = "    sub t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_MULT) {
            source = "    mul t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_DIV) {
            source = "    div t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_LSR) {
            source = "    srlw t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_LSL) {
            source = "    sllw t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_ASR) {
            source = "    sraw t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_AND) {
            source = "    and t0, t0, t1\n";
        }  else if (pt->oper2.oper == OP_OR) {
            source = "    or t0, t0, t1\n";
        } else if (pt->oper2.oper == OP_XOR) {
            source = "    xor t0, t0, t1\n";
        }
        strcat(output, source);
        source = "    sw t0, (sp)\n";
        strcat(output, source);
	}
}
