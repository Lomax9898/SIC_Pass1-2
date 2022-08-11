#include "headers.h"
// This holds all the opcodes and the initial hex values they would represent in the records
char* InstructionOP( char *Test )
{
    char *temp;
    if(strcmp( Test, "ADD" )  == 0)
    {
        temp = "18";
        return temp;
    }
    else if(strcmp( Test, "ADDF" )==  0)
    {
        temp  = "58";
        return temp;
    }
    else if(strcmp( Test, "ADDR" )==  0)
    {
        temp  = "90";
        return temp;
    }
    else if(strcmp( Test, "AND" )==  0)
    {
        temp  = "40";
        return temp;
    }
    else if(strcmp( Test, "CLEAR" )==  0)
    {
        temp  = "B4";
        return temp;
    }
    else if(strcmp( Test, "COMP" )==  0)
    {
        temp  = "28";
        return temp;
    }
    else if(strcmp( Test, "COMPF" )==  0)
    {
        temp  = "88";
        return temp;
    }
    else if(strcmp( Test, "COMPR" )==  0)
    {
        temp  = "A0";
        return temp;
    }
    else if(strcmp( Test, "DIV" )==  0)
    {
        temp  = "24";
        return temp;
    }
    else if(strcmp( Test, "DIVF" )==  0)
    {
        temp  = "64";
        return temp;
    }
    else if(strcmp( Test, "DIVR" )==  0)
    {
        temp  = "9C";
        return temp;
    }
    else if(strcmp( Test, "FIX" )==  0)
    {
        temp  = "C4";
        return temp;
    }
    else if(strcmp( Test, "FLOAT" )==  0)
    {
        temp  = "C0";
        return temp;
    }
    else if(strcmp( Test, "HIO" )==  0)
    {
        temp  = "F4";
        return temp;
    }
    else if(strcmp( Test, "J" )==  0)
    {
        temp  = "3C";
        return temp;;
    }
    else if(strcmp( Test, "JEQ" )==  0)
    {
        temp  = "30";
        return temp;
    }
    else if(strcmp( Test, "JGT" )==  0)
    {
        temp  = "34";
        return temp;
    }
    else if(strcmp( Test, "JLT" )==  0)
    {
        temp  = "38";
        return temp;
    }
    else if(strcmp( Test, "JSUB" )==  0)
    {
        temp  = "48";
        return temp;
    }
    else if(strcmp( Test, "LDA" )==  0)
    {
        temp  = "00";
        return temp;
    }
    else if(strcmp( Test, "LDB" )==  0)
    {
        temp  = "68";
        return temp;
    }
    else if(strcmp( Test, "LDCH" )==  0)
    {
        temp  = "50";
        return temp;
    }
    else if(strcmp( Test, "LDF" )==  0)
    {
        temp  = "70";
        return temp;
    }
    else if(strcmp( Test, "LDL" )==  0)
    {
        temp  = "08";
        return temp;
    }
    else if(strcmp( Test, "LDT" )==  0)
    {
        temp  = "74";
        return temp;
    }
    else if(strcmp( Test, "LDX" )==  0)
    {
        temp  = "04";
        return temp;
    }
    else if(strcmp( Test, "LPS" )==  0)
    {
        temp  = "D0";
        return temp;
    }
    else if(strcmp( Test, "MUL" )==  0)
    {
        temp  = "20";
        return temp;
    }
    else if(strcmp( Test, "MULF" )==  0)
    {
        temp  = "60";
        return temp;
    }
    else if(strcmp( Test, "MULR" )==  0)
    {
        temp  = "98";
        return temp;
    }
    else if(strcmp( Test, "NORM" )==  0)
    {
        temp  = "C8";
        return temp;
    }
    else if(strcmp( Test, "OR" )==  0)
    {
        temp  = "44";
        return temp;
    }
    else if(strcmp( Test, "RD" )==  0)
    {
        temp  = "D8";
        return temp;
    }
    else if(strcmp( Test, "RMO" )==  0)
    {
        temp  = "AC";
        return temp;
    }
    else if(strcmp( Test, "RSUB" )==  0)
    {
        temp  = "4C";
        return temp;
    }
    else if(strcmp( Test, "SHIFTL" )==  0)
    {
        temp  = "A4";
        return temp;
    }
    else if(strcmp( Test, "SHIFTR" )==  0)
    {
        temp  = "A8";
        return temp;
    }
    else if(strcmp( Test, "SIO" )==  0)
    {
        temp  = "FO";
        return temp;
    }
    else if(strcmp( Test, "SSK" )==  0)
    {
        temp  = "EC";
        return temp;
    }
    else if(strcmp( Test, "STA" )==  0)
    {
        temp  = "0C";
        return temp;
    }
    else if(strcmp( Test, "STB" )==  0)
    {
        temp  = "78";
        return temp;
    }
    else if(strcmp( Test, "STCH" )==  0)
    {
        temp  = "54";
        return temp;
    }
    else if(strcmp( Test, "STF" )==  0)
    {
        temp  = "80";
        return temp;
    }
    else if(strcmp( Test, "STI" )==  0)
    {
        temp  = "D4";
        return temp;
    }
    else if(strcmp( Test, "STL" )==  0)
    {
        temp  = "14";
        return temp;
    }
    else if(strcmp( Test, "STS" )==  0)
    {
        temp  = "7C";
        return temp;
    }
    else if(strcmp( Test, "STSW" )==  0)
    {
        temp  = "E8";
        return temp;
    }
    else if(strcmp( Test, "STT" )==  0)
    {
        temp  = "84";
        return temp;
    }
    else if(strcmp( Test, "STX" )==  0)
    {
        temp  = "10";
        return temp;
    }
    else if(strcmp( Test, "SUB" )==  0)
    {
        temp  = "1C";
        return temp;
    }
    else if(strcmp( Test, "SUBF" )==  0)
    {
        temp  = "5C";
        return temp;
    }
    else if(strcmp( Test, "SUBR" )==  0)
    {
        temp  = "94";
        return temp;
    }
    else if(strcmp( Test, "SVC" )==  0)
    {
        temp  = "B0";
        return temp;
    }
    else if(strcmp( Test, "TD" ) ==  0)
    {
        temp  = "E0";
        return temp;
    }
    else if(strcmp( Test, "TIO" )==  0)
    {
        temp  = "F8";
        return temp;
    }
    else if(strcmp( Test, "TIX" ) ==  0)
    {
        temp  = "2C";
        return temp;
    }
    else if(strcmp( Test, "TIXR" )==  0)
    {
        temp  = "B8";
        return temp;
    }
    else if(strcmp( Test, "WD" ) ==  0)
    {
        temp  = "DC";
        return temp;
    }
    else
    {
        printf("ERROR not found in opcode list");
    }

    return 0;
}
