#ifndef ERRORS_STRUCT_h
#define ERRORS_STRUCT_h

typedef struct {
	char * filename;
	int line;
	int column;
	char * msg;
	char * lexBuff;
} Err_lex;

typedef struct {
	char * filename;
	int line;
	int column;
	char * msg;
	char * actualTokenName;
} Err_syntax;

typedef struct {
	char * filename;
	int line;
	int column;
	char * symbolName;
} Err_def;

typedef struct {
	char * filename;
	int line;
	int column;
	char * msg;
} Err_type;

typedef struct {
	char * filename;
	int line;
	int column;
	char * cndtTypeStr;
} Err_cond;


typedef struct {
	char * filename;
	int line;
	int column;
	char * msg;
} Err_sem;

#endif
