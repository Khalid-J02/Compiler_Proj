
#include<stdio.h>
#include "File_Desc.h"
#include "scanner.h"

char *Res_Opt[] = {"lx_identifier", "lx_integer", "lx_string", "lx_float",
                   "kw_and", "kw_begin", "kw_boolean", "kw_by", "kw_constant",
                   "kw_do", "kw_else", "kw_end", "kw_false", "kw_fi", "kw_float",
                   "kw_for", "kw_from", "kw_function", "kw_if", "kw_integer",
                   "kw_not", "kw_od", "kw_or", "kw_procedure", "kw_program",
                   "kw_read", "kw_return", "kw_string", "kw_then", "kw_to",
                   "kw_true", "kw_var", "kw_while", "kw_write",
                   "lx_lparen", "lx_rparen", "lx_lbracket", "lx_rbracket", "lx_lbrace", "lx_rbrace",
                   "lx_dot", "lx_semicolon", "lx_comma", "lx_eq",
                   "lx_plus", "lx_minus", "lx_star", "lx_slash",
                   "lx_neq", "lx_lt", "lx_le", "lx_gt", "lx_ge", "lx_colon_eq", "lx_colon", "lx_eof"};

int main() {

    File_Desc *Fd;// = (File_Desc *) malloc(sizeof(File_Desc));
    Fd = fd_open("C:\\Summer2023\\Compiler_Proj\\data.txt");
    if (fd_is_open(Fd))
        printf("File Open\n");
    SCANNER *scan = new SCANNER(Fd);

//    fd_get_next_line(Fd);

//    printf("Token type \t \t String\n");
    while (1) {
        struct TOKEN *x;
        x = scan->Scan();
        printf("\ntoken : %d\n", x->type);

        if (x->type == 1)
            printf("\nvalue : %d\n", x->value);
        else if (x->type == 3)
            printf("\nfloat : %f\n", x->float_value);

        printf("%s \t \t %s", Res_Opt[x->type], x->str_ptr);
        if (x->type == lx_eof) {
            break;
        }
    }
    fd_close(Fd);
    return 0;

}