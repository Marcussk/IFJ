#ifndef ERROR_HANDLER_h
#define ERROR_HANDLER_h
/*
 *
 • 1 - chyba v programu v ramci lexikalni analyzy (chybna struktura aktualniho lexe-
 mu).
 • 2 - chyba v programu v ramci syntakticke analyzy (chybna syntaxe struktury pro-
 gramu).
 • 3 - semanticka chyba v programu – nedefinovana funkce/promenna, pokus o redefi-
 nici funkce/promenne, atd.
 • 4 - semanticka chyba typove kompatibility v aritmetickych, retezcových a relacnich
 vyrazech, prip. spatny pocet ci typ parametruu u volani funkce.
 • 5 - ostatni semanticke chyby.
 • 6 - behová chyba pri nacitani ciselne hodnoty ze vstupu.
 • 7 - behová chyba pri praci s neinicializovanou promennou.
 • 8 - behová chyba deleni nulou.
 • 9 - ostatní beehové chyby.
 • 99 - interni chyba interpretu tj. neovlivnená vstupnim programem (napr. chyba a-
 lokace pameti, chyba p

 */
#include <stdio.h>
#include <stdlib.h>
#include "errors_structs.h"

void Error_memory(char * str);
void Error_unimplemented(char * str);

void Error_lex(Err_lex err);

void Error_syntax(Err_syntax err);

void Error_sem_definition( Err_def err);
void Error_sem_Type(Err_type err) ;
void Error_sem_Cond(Err_cond err);
void Error_sem(Err_sem err);

void Error_rt_readln();
void Error_rt_notInit( int stackAddr);
void Error_rt_zeroDivision();
void Error_rt(char * str);

#endif
