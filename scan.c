/* scan.c - an initial project01 scanner */

#include "ntlang.h"

char *scan_token_strings[] = SCAN_TOKEN_STRINGS;

void scan_table_init(struct scan_table_st *st) {
    st->len = 0;
    st->cur = 0;
}

void scan_token_print(struct scan_token_st *tp) {
    printf("%s(\"%s\")\n", scan_token_strings[tp->id], tp->value);
}

void scan_table_print(struct scan_table_st *st) {
    int i;

    for (i = 0; i < st->len; i++) {
        scan_token_print(&st->table[i]);
    }
}

struct scan_token_st * scan_table_new_token(struct scan_table_st *st) {
    /* Allocate a new token from the token table */
    /* We just return a pointer to the next avaiable token in the array. */
    struct scan_token_st *tp;

    tp = &(st->table[st->len]);
    st->len += 1;

    return tp;
}

bool scan_is_whitespace(char ch) {
    return (ch == ' ') || (ch == '\t');
}

char *scan_whitespace(char *p, char *end) {
    while(scan_is_whitespace(*p) && (p < end)) {
        p += 1;
    }
    return p;
}

bool scan_is_hexdigit(char ch) {
    return (ch >= '0' && ch <= '9') || 
           (ch >= 'a' && ch <= 'f') || 
           (ch >= 'A' && ch <= 'F');
}

bool scan_is_bindigit(char ch){
	return (ch == '0' || ch == '1');
}

bool scan_is_digit(char ch) {
    return (ch >= '0' && ch <= '9');
}

char * scan_hexdigit(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;

    if(!scan_is_hexdigit(*p)) {
    	printf("error: scan_hexdigit expecting hexadecimal digit\n");
    	exit(-1);
    }

    while (scan_is_hexdigit(*p) && (p < end)) {
    	 if (*p >= 'a' && *p <= 'f') {
            tp->value[i] = *p - 32;
        } else {
            tp->value[i] = *p;
        }
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_HEXLIT;

    return p;
}

char * scan_bindigit(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;

    if(!scan_is_bindigit(*p)) {
    	printf("error: scan_bindigit expecting 0 or 1\n");
    	exit(-1);
    }

    while (scan_is_bindigit(*p) && (p < end)) {
        tp->value[i] = *p;
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_BINLIT;

    return p;
}

char * scan_intlit(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;

    while (scan_is_digit(*p) && (p < end)) {
        tp->value[i] = *p;
        p += 1;
        i += 1;
    }
    tp->value[i] = '\0';
    tp->id = TK_INTLIT;

    return p;
}

char * scan_register(char *p, char*end, struct scan_token_st *tp) {
	int i = 0;

	tp->value[i] = *p;
	p++; 
	i++;
	
	tp->value[i] = *p;
	p++;
	i++;
	
	tp->value[i] = '\0';
	tp->id = TK_REGNAME;
	
	return p;
}

char * scan_token_helper(struct scan_token_st *tp, char *p, int len,
                       enum scan_token_enum id) {
    /* Read a token starting a p for len characters.
       Update the given token with the token string and token id. */
    int i;

    tp->id = id;
    for (i = 0; i < len; i++) {
        tp->value[i] = *p;
        p += 1;
    }
    tp->value[i] = '\0';
    return p;
}

char * scan_token(char *p, char *end, struct scan_token_st *tp) {
    /* Scan the next token at the current location in the input string */
    /* p points to the current location in the input string */
    /* end is a pointer to the end of the string */
    /* The return value is the updated location in the input string. */
    /* The token pointed to by tp is updated with the scanned token. */
	
    if (p == end) {
        /* Check if we are at the end of the input string */
        tp->value[0] = '\0';
        tp->id = TK_EOT;
    } else if (scan_is_whitespace(*p)) {
        /* Ingore whitespace. Notice the recursive call. */
        p = scan_whitespace(p, end);
        p = scan_token(p, end, tp);
    } else if (*p == 'a'){
    	p = scan_register(p, end, tp);
    } else if (*p == '0' && *(p + 1) == 'b'){
    	p = scan_bindigit(p + 2, end, tp);
    } else if (*p == '0' && *(p + 1) == 'x'){
        p = scan_hexdigit(p + 2, end, tp);
    } else if (scan_is_digit(*p)) {
        p = scan_intlit(p, end, tp);        
    } else if (*p == '+') {
        p = scan_token_helper(tp, p, 1, TK_PLUS);
    } else if (*p == '-') {
        p = scan_token_helper(tp, p, 1, TK_MINUS);
    } else if (*p == '*') {
        p = scan_token_helper(tp, p, 1, TK_MULT);
    } else if (*p == '/') {
        p = scan_token_helper(tp, p, 1, TK_DIV); 
    } else if (*p == '>' && *(p + 1) == '>') {
        p = scan_token_helper(tp, p, 2, TK_LSR);
    } else if (*p == '>' && *(p + 1) == '-') {
        p = scan_token_helper(tp, p, 2, TK_ASR);
    } else if (*p == '<' && *(p + 1) == '<') {
        p = scan_token_helper(tp, p, 2, TK_LSL);
    } else if (*p == '~') {
        p = scan_token_helper(tp, p, 1, TK_NOT);
    } else if (*p == '&') {
        p = scan_token_helper(tp, p, 1, TK_AND);
    } else if (*p == '|') {
        p = scan_token_helper(tp, p, 1, TK_OR);
    } else if (*p == '^') {
        p = scan_token_helper(tp, p, 1, TK_XOR);
    } else if (*p == '(') {
        p = scan_token_helper(tp, p, 1, TK_LPAREN);
    } else if (*p == ')') {
        p = scan_token_helper(tp, p, 1, TK_RPAREN);
    } else {
        /* Instead of returning an error code, we will usually
           exit on failure. */
        printf("scan error: invalid char: %c\n", *p);
        exit(-1);
    }
    return p;
}

/* Scan valid tokens in the given input string. */
void scan_table_scan(struct scan_table_st *st, char *input) {
    struct scan_token_st *tp;
    char *p = input;
    char *end;
    int len;

    /* Get a pointer to the end of the input string. */
    /* Technically this pointer points to the null termination character. */
    len = strnlen(input, SCAN_INPUT_LEN);
    end = p + len;

    do {
        /* Allocate a token */
        tp = scan_table_new_token(st);
        /* Scan one token from input string */
        p = scan_token(p, end, tp);
        /* Are we done? */
        if (tp->id == TK_EOT) {
            break;
       }
    } while(true);
}

/* Get the token at the current (cur) position + i in the token table. */
struct scan_token_st * scan_table_get(struct scan_table_st *st, int i) {
    return &(st->table[st->cur + i]);
}

/* Accept the current token if it matches tk_expected.
 * If it matches, then increment the cur position by 1 and return true.
 * Otherwise, return false */
bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected) {
    struct scan_token_st *tp;

    if (tk_expected == TK_ANY) {
        st->cur += 1;
        return true;
    }

    tp = scan_table_get(st, 0);

    if (tp->id == tk_expected) {
        st->cur += 1;
        return true;
    }

    return false;
}
