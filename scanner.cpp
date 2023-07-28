//
// Created by Jenan on 7/21/2023.
//

#include "scanner.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <cctype>


using namespace std;

int keys = 32; /* number of keywords */
string keyword[] = {
        "and", "begin", "boolean", "by", "constant",
        "do", "else", "end", "false", "fi", "float", "for", "from",
        "function", "if", "integer", "not", "od", "or", "procedure",
        "program", "read", "return", "string", "then", "to", "true",
        "var", "while", "write"
};

char optr[] = {//length 18
        '(', ')', '[', ']', '{', '}',
        '.', ';', ',', '=', '+', '-',
        '*', '/', '<', '>', ':', '!'
};

LEXEME_TYPE key_type[] = {
        kw_and, kw_begin, kw_boolean, kw_by, kw_constant,
        kw_do, kw_else, kw_end, kw_false, kw_fi, kw_float,
        kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not,
        kw_od, kw_or, kw_procedure, kw_program, kw_read, kw_return,
        kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write
};

SCANNER::SCANNER() {
    Fd = nullptr;
}

SCANNER::SCANNER(File_Desc *fd) {
    Fd = fd;
}

bool SCANNER::is_number(char c) {

    if ((c >= '0' && c <= '9') || c == '-')
        return true;
    return false;

}

bool SCANNER::is_char(char c) {

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        return true;
    }
    return false;
}

int SCANNER::is_rsvd_wrd(string s) {

    for (int i = 0; i < 32; i++) {
        if (s == keyword[i]) {
            return i;
        }
    }
    return -1;

}

int SCANNER::is_optr(char c) {

    for (int i = 0; i < 18; i++) {
        if (c == optr[i]) {
            return i;
        }
    }
    return -1;

}

bool SCANNER::is_dlmtr(char c) {

    if ((is_optr(c) > -1) || c == '"' || c == '#') {
        fd_unget_char(Fd, c);
        return true;
    } else if (c == ' ' || c == '\n' || c == '\t' || c == '\f' || c == '\0') {
        return true;
    }
    return false;

}

TOKEN *SCANNER::get_int(char c) {
    struct TOKEN *token = (struct TOKEN *) malloc(sizeof(struct TOKEN));
    string tkn_str = "";
    tkn_str += c;
    if (c == '-') {
        c = fd_get_char(Fd);
        if (isdigit(c)) {
            tkn_str += c;
        } else {
            //error report
            fd_report_error(Fd, "expected int");

        }
    }
    while (1) {
        c = fd_get_char(Fd);
        if (isdigit(c)) {
            tkn_str += c;
        } else if (c == '.') {
            c = fd_get_char(Fd);
            if (isdigit(c)) {
                tkn_str += c;
                break;
            } else {
                //error report
                fd_report_error(Fd, "expected float, digit is missing");
            }
        } else if (is_dlmtr(c)) {
            //find value
            token->type = lx_integer;
            token->value = stoi(tkn_str);
            return token;
        } else {
            //report error
            printf("\n %d %c\n", c, c);
            fd_report_error(Fd, "expected int");
        }
    }
    while (1) {
        c = fd_get_char(Fd);
        if (isdigit(c)) {
            tkn_str += c;
        } else if (is_dlmtr(c)) {
            //find value
            token->type = lx_float;
            token->float_value = stof(tkn_str);
            return token;
        } else {
            //report error
            fd_report_error(Fd, "expected float");
        }
    }
}

TOKEN *SCANNER::get_id(char c) {
    struct TOKEN *token = (struct TOKEN *) malloc(sizeof(struct TOKEN));
    token->str_ptr  = new char[1000];
    token->str_ptr[0] = c;
    int i = 1;
    while (1) {
        c = fd_get_char(Fd);
        if (isdigit(c) || isalpha(c) || c == '_') {
            token->str_ptr[i] = c;
            i++;
        } else if (is_dlmtr(c)) {
            //find value
            int wrd_num = is_rsvd_wrd(token->str_ptr);

            if (wrd_num > -1) {
                token->type = key_type[wrd_num];
            } else {
                token->type = lx_identifier;
            }
            token->str_ptr[i] = '\0';
//            strcpy(token->str_ptr, tkn_str);
            return token;
        } else {
            //report error
            fd_report_error(Fd, "illegal char");
        }
    }
}

TOKEN *SCANNER::get_string(char c) {
    struct TOKEN *token = (struct TOKEN *) malloc(sizeof(struct TOKEN));
//    char *tkn_str = new char[1000];
    token->str_ptr  = new char[1000];
    int i =0;
    while (1) {
        c = fd_get_char(Fd);
        if (c == '\n' || c == '\0') {
            //report error-->expecting "
            fd_report_error(Fd, "unterminated string, expected \"");
        } else if (c == '"') {
            token->type = lx_string;
            token->str_ptr[i] = '\0';
//            strcpy(token->str_ptr, tkn_str);
            return token;
        } else {
            token->str_ptr[i] = c;
            i++;
        }
    }
}

void SCANNER::skip_comments() {
    char c;
    while (1) {
        c = fd_get_char(Fd);
        while (c != '#' && c != '\n' && c != '\0') {
            c = fd_get_char(Fd);
        }
        if (c == '#') {
            c = fd_get_char(Fd);
            if (c == '#') {
                return;
            }
        } else if (c == '\n') {
            return;
        } else if (c == '\0') {
            fd_unget_char(Fd, c);
            return;
        }
    }

}

TOKEN *SCANNER::get_optr(char c) {
    struct TOKEN *token = (struct TOKEN *) malloc(sizeof(struct TOKEN));
    string tkn_str = "";
    tkn_str += c;
    int op_val = is_optr(c);
    if (op_val < 14) {
        token->type = (LEXEME_TYPE) (34 + op_val);
        return token;
    }
    switch (c) {
        case '>':
            c = fd_get_char(Fd);
            if (c == '=') {
                token->type = lx_ge;
            } else if (is_dlmtr(c)) {
                token->type = lx_gt;
                fd_unget_char(Fd, c);
            }
            break;
        case '<':
            c = fd_get_char(Fd);
            if (c == '=') {
                token->type = lx_le;
            } else if (is_dlmtr(c)) {
                token->type = lx_lt;
                fd_unget_char(Fd, c);
            }
            break;
        case ':':
            c = fd_get_char(Fd);
            if (c == '=') {
                token->type = lx_colon_eq;
            } else if (is_dlmtr(c)) {
                token->type = lx_colon;
                fd_unget_char(Fd, c);
            }
            break;
        case '!':
            c = fd_get_char(Fd);
            if (c == '=') {
                token->type = lx_neq;
            } else {
                //report error
                fd_report_error(Fd, "illegal token, expected \"=\" operator");
            }
            break;
    }
    return token;
}

TOKEN *SCANNER::Scan() {

    char c = fd_get_char(Fd);

    if (is_number(c)) {//the incoming token is either an int or a float
        if (c == '-') {
            char tmp = fd_get_char(Fd);
            fd_unget_char(Fd, tmp);
            if (isdigit(tmp)) {
                return get_int(c);
            }
            return get_optr(c);
        } else {
            return get_int(c);
        }
    } else if (is_char(c)) {//if incoming token is an identifier-could also be a reserved word-
        return get_id(c);
    } else if (c == '"') {//if the incoming token is a string
        return get_string(c);
    } else if (c == '#') {//if the incoming token is a comment
        //check for another #
        c = fd_get_char(Fd);
        if (c == '#') {
            skip_comments();
            return Scan();
        } else {
            fd_report_error(Fd, "illegal token ");
        }
    } else if (c == ' ' || c == '\n' || c == '\t') {//if incoming token is a whitespace
        return Scan();
    } else if (is_optr(c) > -1) {//if operator or illegal token
        return get_optr(c);
    } else if (c == '\0') {
        struct TOKEN *token = (struct TOKEN *) malloc(sizeof(struct TOKEN));
        token->type = lx_eof;
    } else {
        printf("\n%d\n", c);
        fd_report_error(Fd, "illegal token ");
        return nullptr;
    }
}


