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
#include "ifjTypes.h"

void memoryError(char * str);
void unimplementedError(char * str);

void lexError(char * str,char * lexBuff, int lineNum);

void syntaxError(char * str, int lineNum, char * actualTokenName);

void sem_definitionError( int lineNum, char * varName);
void sem_TypeError( char * nameOfType);
void sem_Error(char * str);

void rt_readlnNumError();
void rt_notInitError(char * varName);
void rt_zeroDivisionError();
void rt_error(char * str);

#endif
