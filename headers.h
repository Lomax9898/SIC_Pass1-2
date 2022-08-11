#ifndef HEADERS_H_INCLUDED
#define HEADERS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct symbols
{
    int Address;
    int sourceline;
    char Name[7];

};

typedef struct symbols SYMBOL;

struct lineaddress
{
    int linesrc;
    int locaddress;

};

typedef struct lineaddress LINEADD;

int IsAValidSymbol( char *TestSymbol);
int IsADirective( char *Test );

#endif // HEADERS_H_INCLUDED
