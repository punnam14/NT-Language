/* project01.c - initial parsing implemenation */

#include "ntlang.h"

int main(int argc, char **argv) {
    struct config_st config;
    struct scan_table_st scan_table;
    struct parse_table_st parse_table;
    struct parse_node_st *parse_tree;
    uint32_t value;

    bool compile = false;
    char compile_name[1024];

    int base = 10; 
    config.base = base;
    config.u = false;
    config.width = 32;
    
    char *expr = NULL;
    
    if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: project01 <expression>\n");
        printf("  -e <expression>  Specify the expression to evaluate.\n");
    	printf("  -b <base>        Specify the base for numeric output\n");
    	printf("  -w <width>       Specify the bit width for the output\n");
    	printf("  -u               Treat numbers as unsigned\n");
        printf("  Example: project01 \"1 + 2\"\n");
        exit(-1);
    }

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-e") == 0){
            strncpy(config.input, argv[i + 1], SCAN_INPUT_LEN - 1);
            i++;
        } else if (strstr(argv[i], "-a") != NULL){
        	char* reg = argv[i];
        	reg++; reg++;
        	int index = atoi(reg);
            config.reg[index] = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-c") == 0){
        	compile = true;
        	strcpy(compile_name, argv[i+1]);
        	i++;
        } else if (strcmp(argv[i], "-b") == 0){
            config.base = str_to_uint32(argv[i + 1], 10);
            i++;
        } else if (strcmp(argv[i], "-w") == 0){
            config.width = str_to_uint32(argv[i + 1], 10);
            i++;
        } else if (strcmp(argv[i], "-u") == 0){
            config.u = true;
        }
    }

    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, config.input);
    
    parse_table_init(&parse_table);
    parse_tree = parse_program(&parse_table, &scan_table);

	if(!compile){
    	value = eval(parse_tree, config.reg);
    	eval_print(&config, value);
    }else{
    	compile_tree(parse_tree, argc, argv);
    }
    return 0;
}
