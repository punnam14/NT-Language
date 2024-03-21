/* eval.c - tree evaluation and value printing */

#include "ntlang.h"

void eval_error(char *err) {
    printf("eval_error: %s\n", err);
    exit(-1);
}

uint32_t eval(struct parse_node_st *pt, int* reg) {
    uint32_t v1, v2;

	if (pt->type == EX_REGVAL) {
		char* name = pt->regname.name;
		name++;
		int index = atoi(name);
		v1 = reg[index];
	}else if (pt->type == EX_INTVAL) {
        v1 = pt->intval.value;
    } else if (pt->type == EX_OPER1) {
        v1 = eval(pt->oper1.operand, reg);
        if (pt->oper1.oper == OP_MINUS) {
            v1 = -v1;
        } else if(pt->oper1.oper == OP_NOT) {
        	v1 = ~v1;
        } else {
            eval_error("Bad operator op1");
        }
    } else if (pt->type == EX_OPER2) {
        v1 = eval(pt->oper2.left,reg);
        v2 = eval(pt->oper2.right,reg);
        if (pt->oper2.oper == OP_PLUS) {
            v1 = v1 + v2;
        } else if (pt->oper2.oper == OP_MINUS) {
            v1 = v1 - v2;
        } else if (pt->oper2.oper == OP_MULT) {
            v1 = v1 * v2;
        } else if (pt->oper2.oper == OP_DIV) {
        	if (v2 == 0){
        		eval_error("Division by zero");
        	}
            v1 = v1 / v2;
        } else if (pt->oper2.oper == OP_LSR) {
            v1 = v1 >> v2;
        } else if (pt->oper2.oper == OP_LSL) {
            v1 = v1 << v2;
        } else if (pt->oper2.oper == OP_ASR) {
            v1 = ((int32_t) v1) >> v2;
        } else if (pt->oper2.oper == OP_AND) {
            v1 = v1 & v2;
        }  else if (pt->oper2.oper == OP_OR) {
            v1 = v1 | v2;
        } else if (pt->oper2.oper == OP_XOR) {
            v1 = v1 ^ v2;
        } else {
            eval_error("Bad operator op2");            
        }
    }

    return v1;
}

void eval_print(struct config_st *cp, uint32_t value) {
	char output[1024];
    printf("%d\n",value);
    
}

void reverse(char *out){
	int len = strlen(out);
	for(int i = 0; i < len / 2; i++){
		char temp = out[i];
		out[i] = out[len - i - 1];
		out[len - i - 1] = temp;
	}
}

void pad_leading_zeroes(char* str, int target, int base, char * out, int i) {
	int current = strlen(str);
	int difference = 0;
	if(base == 2) difference = 32 - current;
	if(base == 16) difference = 8 - current;
	if(difference <= 0){
		out = strcpy(out, str);
		return;
	}
	for(int j = 0; j < difference; j++){
		out[j] = '0';
	}
	for(int j = 0; j < i; j++){
		out[difference] = str[j];
		difference++;
	}
	out[difference] = '\0';
}

/* Adjusts length based on base and width requirements */
void fit_width(char* out, int base, int width){
	char res[1024];
	int remove = 32 - width;
	int i = 0;
	int count = 32 - remove;
	if(base == 16) {
		remove /= 4;
		count = 8 - remove;
	}
	for(i = 0; i < count; i++){
		res[i] = out[remove + i];
	}
	res[i] = '\0';
	out = strcpy(out, res);	
}

void uint32_to_string(uint32_t value, int base, int width, bool unsign, char* out){
	int i = 0; 
	char str[1024];
	if(width != 32){
		int32_t mask = (0b1 << width) - 1;
	    int32_t sign_mask = 1 << (width - 1);
		value = value & mask;
		if(base == 10){
			if (value & sign_mask && unsign == false) {
            	value = -((~value + 1) & mask);
			}
		}
	}
	if(value == 0){
		out[0] = '0';
		out[1] = '\0';
		return; 
	}
	if(base == 10){
		sprintf(out,"%d",value);
		return;
	}
	while(value > 0){
		int tmp = value % base;
		if(tmp < 10){
			str[i++] = '0' + tmp;
		}else{
			str[i++] = 'A' + tmp - 10;
		}
		value /= base;
	}
	str[i] = '\0';
	reverse(str);
	pad_leading_zeroes(str, width, base, out, i);
	fit_width(out, base, width);
}

void eval_print2(struct config_st *cp, uint32_t value) {
    /*
     * Handle -b -w -u
     *
     * Use your own conversion functions for uint32_t to string.
     */
	char output[1024];
    uint32_to_string(value, cp->base, cp->width,cp->u, output);
    if(cp->base == 2){
    	printf("0b%s\n",output);
    }
    if(cp->base == 16){
    	printf("0x%s\n",output);
    }
    if(cp->base == 10){
    	printf("%s\n",output);
    }  
}
