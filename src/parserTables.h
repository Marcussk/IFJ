#ifndef PAreduceshiftequalreduce_TABLequalshift_h
#define PAreduceshiftequalreduce_TABLequalshift_h

typedef enum {
	shift, reduce, error, equal
} precedenceRule;


//keep synergyc with tokenMap.h/Token
extern precedenceRule prTable[16][16];

#endif
