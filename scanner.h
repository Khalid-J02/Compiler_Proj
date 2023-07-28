
#ifndef COMPILER_PROJ_SCANNER_H
#define COMPILER_PROJ_SCANNER_H


#include "File_Desc.h"
#include <string.h>
#include <string>

using namespace std;

//Token types enumeration
typedef enum {
    /* Literals */
    lx_identifier = 0, lx_integer, lx_string, lx_float,
    /* Keywords */
    kw_and, kw_begin, kw_boolean, kw_by, kw_constant,
    kw_do, kw_else, kw_end, kw_false, kw_fi, kw_float,
    kw_for, kw_from, kw_function, kw_if, kw_integer,
    kw_not, kw_od, kw_or, kw_procedure, kw_program,
    kw_read, kw_return, kw_string, kw_then, kw_to,
    kw_true, kw_var, kw_while, kw_write,
    /* Operators */
    lx_lparen, lx_rparen, lx_lbracket, lx_rbracket, lx_lbrace, lx_rbrace,
    lx_dot, lx_semicolon, lx_comma, lx_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_neq, lx_lt, lx_le, lx_gt, lx_ge, lx_colon_eq, lx_colon, lx_eof


}
        LEXEME_TYPE;

// Definition of TOKEN, you can use a class if you  want
struct TOKEN {
    LEXEME_TYPE type;
    int value;  // can be used instead of the str_ptr for IDs and strings
    float float_value;

    char* str_ptr; // points to strings or Identifiers, can use value
    //instead but with type casting
};

class SCANNER {
private:
    File_Desc *Fd;

    // define your functions ..
    //I would define the following functions..
    void skip_comments();

//     bool check_keyword(...);
    TOKEN *get_id(char c);

    TOKEN *get_string(char c);

    TOKEN *get_int(char c); // gets integers and floats
    TOKEN *get_optr(char c);

    bool is_number(char c);

    bool is_char(char c);

    bool is_cmnt(char c);//might be deleted
    int is_rsvd_wrd(string c);//if reserved word or identifier
    bool is_dlmtr(char c);//if a char is a delimiter
    int is_optr(char c);

public:
    SCANNER();

    SCANNER(File_Desc *fd);//{ Fd = fd; /* write other code if needed*/};
    TOKEN *Scan();
    // You may define it as TOKEN *Scan(FileDescriptor *fd);

};

// It is helpful to define the following arrays in your .cpp file
// not in this header file

/* The keyword list is and must be kept in alphabetical order. To
add a keyword, insert it at the appropriate place in keyword[],
insert its type in the appropriate place in key type[], and
increment the var keys. Order is needed for binary search
*/



#endif //COMPILER_PROJ_SCANNER_H
