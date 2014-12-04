#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>

#include "ifj_types.h"
#include "instruction.h"
#include "i_function.h"

//the entire header file file
void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2);
void SemAnalyzer_checkcond(tIFJ param);
void SemAnalyzer_typeconvert(InstrQueue * self, tIFJ param1, tIFJ param2);
#endif
