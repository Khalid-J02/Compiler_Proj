//
// Created by Administrator on 7/29/2023.
//

#ifndef COMPILER_PROJ_SYMBOL_H
#define COMPILER_PROJ_SYMBOL_H

typedef enum{
    type_integer,
    type_string,
    type_boolean,
    type_float,
    type_none
} j_type;

static char *type_names[] = {"integer", "string", "boolean", "float", "none"};

typedef enum {
    ste_var = 0, // a variable
    ste_const, //A constant
    ste_routine, //A routine
    ste_undefined // ste_entry
} ste_entry_type;
// You may change the following definition to a class
struct SymbolTableEntry{
    char *name;
    SymbolTableEntry *next;
    ste_entry_type entry_type;
// User-modifiable fields go here:
    union{
         // for a variable record its type
        struct{
            j_type type;
        } var;
// for a constant record its value
        struct{
            int value;
        } constant;
/* for a routine, record formal parameters and result type */
        struct{
// SteListCelll *formals;// will be defined later
            j_type result_type;
        } routine;
    } f;
}; //end of SymbolTableEntry definition

class SymbolTable{
/* Hash table */
private:
    SymbolTableEntry **slots; //Pointer to hash table array of entries
    int fold_case; // Non-zero => fold upper to lower case
// Statistics on hash table effectiveness
    int number_entries; // Number of entries in table
    int number_probes; // Number of probes into table
    int number_hits; // Number of probes that immediately found entry
    int max_search_dist; // Maximum entries searched
    SymbolTable *next; // To be used to create a stack of symbol table
// add your defined functions, e.g hash .
// Externally-visible functions
public:
    SymbolTable(); // fold_case will be set to zero
    SymbolTable(int flod_case_flag);
    void clear_symbol_table ( );
    SymbolTableEntry *Get_symbol ( char *);
    SymbolTableEntry *Put_symbol ( char *);
    void print_symbol_stats ( );
// will add more functions here in the future
};

#endif //COMPILER_PROJ_SYMBOL_H
