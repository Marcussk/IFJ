#ifndef INSTRUCTION_h
#define INSTRUCTION_h

#define T_IS_INT 0				// type used via instruction
#define T_IS_DOUBLE 1
#define T_IS_STRING 2
#define T_IS_BOOL 3


typedef enum
{
    STOP = 0,
    NOOP,		// does nothing
    JMP,  //skok pri pravdive podmince
    JMPZ, // skok pri nepravdive podmince
    CALL,
    RETURN,
    WRITE,
    READ,
    ASSIGN,
    ADD,
    SUB,
    MUL,
    DIV,
    CONCAT, //only string!!
    EQUAL,
    NOT_EQUAL,
    LESS,
    MORE,
    LESS_or_EQUAL,
    MORE_or_EQUAL,

    //funkce ze zadani
    FIND,
    COPY,
    LENGTH,
    SORT

} Instructions;


typedef struct
{
    Instructions code;  //klucove slovo instrukcie
    int tIFJ;           //datovy typ
    void *a1;           //operand1
    void *a2;           //operand2
    void *a3;           //medzivysledok
} Instruction;


#endif
