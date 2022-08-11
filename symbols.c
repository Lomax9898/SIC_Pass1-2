#include "headers.h"

//Checks if symbol is valid by checking length and characters of the symbol
int IsAValidSymbol( char *TestSymbol )
{

    int Result = 1;
    int index =0;
    int maxlength = 6;
    int symsize = 0;
    int done;
    done = 0 ;
    symsize = strlen(TestSymbol);
    for(index =0; index != symsize; index++)
    {
        if(((TestSymbol[index] >= 32 ) && ( TestSymbol[index] <= 47 )) || ((TestSymbol[index] >= 58 ) && ( TestSymbol[index] <= 64 )) || ((TestSymbol[index] >= 91 ) && ( TestSymbol[index] <= 96 ))
                || ((TestSymbol[index] >= 123 ) && ( TestSymbol[index] <= 126 ))   )
        {
            Result = 0;
            return Result;
        }
    }
    if(symsize > maxlength)
    {
        Result = 0;
        return Result;
    }

    index = 0;
    while( !done )
    {
        if ( TestSymbol[index] == '\0' )
        {
            break;
        }

        if ( index == maxlength )
        {
            done = 1;
            Result = 0;
        }
        index++;

    }


    return Result;
}
