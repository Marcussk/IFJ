#ifndef ERROR_HANDLER_h
#define ERROR_HANDLER_h
/*
 • 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexé-
 mu).
 • 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe struktury pro-
 gramu).
 • 3 - sémantická chyba v programu – nedefinovaná funkce/promˇenná, pokus o redefi-
 nici funkce/promˇenné, atd.
 • 4 - sémantická chyba typové kompatibility v aritmetických, ˇretˇezcových a relaˇcních
 výrazech, pˇríp. špatný poˇcet c ˇ i typ parametr ̊u u volání funkce.
 • 5 - ostatní sémantické chyby.
 • 6 - bˇehová chyba pˇri naˇcítání c ˇ íselné hodnoty ze vstupu.
 • 7 - bˇehová chyba pˇri práci s neinicializovanou promˇennou.
 • 8 - bˇehová chyba dˇelení nulou.
 • 9 - ostatní bˇehové chyby.
 • 99 - interní chyba interpretu tj. neovlivnˇená vstupním programem (napˇr. chyba a-
 lokace pamˇeti, chyba p
 */
#include <stdio.h>
#include <stdlib.h>

void memoryError(char * str);

#endif
