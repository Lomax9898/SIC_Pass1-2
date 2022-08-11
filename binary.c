#include "headers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>
// The opcode has to be changed based on the other variables in the line
// I implement a binary number converter, it's a bit rough
char * convert(int character);
int convertdecimal(long long);
long long first;
long long sec;
long long third;
long long last;
char* opchanger(char *opcode, int currentaddr,char* token, char *operand, int operandaddr, int baseaddr)
{
    char * b;
    char * c;
    int d;
    int e;
    int value;
    int result;
    char str[16];
    char endstr[16] = " ";
    char newop[4];
    char delim[] = " ";
    first = 0;
    last = 0;
    sec = 0;
    third = 0;
    char *ptr = strtok(operand, delim);
    char *tokenptr = strtok(token, delim);
    int length;
    if(ptr != NULL)
    {
        length = strlen(ptr);
    }
    char *temp = "00";
    long long binarynum;//Takes the opcode and split it into b and c
    b = convert(opcode[0]);// goes to the switch statement at the bottom
    c = convert(opcode[1]);
    if(c[3] == 49)  // If not one set variable to zero
    {
        first = 1;
    }
    else
    {
        first = 0;
    }
    if(c[2] == 49)
    {
        sec = 1;
    }
    else
    {
        sec = 0;
    }
    if(c[1] == 49)
    {
        third = 1;
    }
    else
    {
        third = 0;
    }
    if(c[0] == 49)
    {
        last = 1;
    }
    else
    {
        last = 0;
    }  // I combine all the variables into a string to simulate binary
    sprintf(str,"%I64d%I64d%I64d%I64d",last,third,sec,first);
    if(ptr == NULL)
    {
        first = 1;
        sec = 1;
        value= 0;
        sprintf(endstr,"%04X\n",value);
    }
    else if((tokenptr[0] == 43)) // adding values for # and @
    {
        if((ptr[0] == 35))
        {
            first = 1;
            sec = 0;
            value = operandaddr;
            sprintf(endstr,"10%04X\n",value);
        }
        else if((ptr[0] == 64))
        {
            first = 0;
            sec = 1;
            value = operandaddr;
            sprintf(endstr,"10%04X\n",value);
        }
        else  // This is for Byte directive in X mode
        {
            if((operand[length-2] == 44) && (operand[length-1] == 88))
            {
                first = 1;
                sec = 1;
                value = operandaddr;
                sprintf(endstr,"9%03X\n",value);
            }
            else
            {
                first = 1;
                sec = 1;
                value = operandaddr;
                sprintf(endstr,"10%04X\n",value);
            }
        }
    }
    else
    {
        if((ptr[0] == 35))
        {
            if((ptr[1] >= 48) && (ptr[1] <= 57 ))
            {
                first = 1;
                sec = 0;
                value = operandaddr;
                sprintf(endstr,"0%03X\n",value);
            }
            else
            {
                if(currentaddr > operandaddr)
                {
                    result = (currentaddr - operandaddr);
                    if(result > 4096)
                    {
                        value = operandaddr - baseaddr;
                        first = 1;
                        sec = 0;
                        sprintf(endstr,"4%03d\n",value);
                    }
                    else
                    {
                        value = (4096 - result);
                        first = 1;
                        sec = 0;
                        sprintf(endstr,"2%03X\n",value);
                    }
                }
                else
                {
                    first = 1;
                    sec = 0;
                    value = operandaddr - currentaddr;
                    sprintf(endstr,"2%03X\n",value);
                }
            }

        }
        else if((ptr[0] == 64))
        {
            if((ptr[1] >= 48) && (ptr[1] <= 57 ))
            {
                first = 0;
                sec = 1;
                value = operandaddr;
                sprintf(endstr,"0%03X\n",value);
            }
            else
            {
                if(currentaddr > operandaddr)
                {
                    result = (currentaddr - operandaddr);
                    if(result > 4096)
                    {
                        value = operandaddr - baseaddr;
                        first = 0;
                        sec = 1;
                        sprintf(endstr,"4%03d\n",value);
                    }
                    else
                    {
                        value = (4096 - result);
                        first = 0;
                        sec = 1;
                        sprintf(endstr,"2%03X\n",value);
                    }
                }
                else
                {
                    first = 0;
                    sec = 1;
                    value = operandaddr - currentaddr;
                    sprintf(endstr,"2%03X\n",value);
                }
            }
        }
        else
        {
            if((ptr[0] >= 48) && (ptr[0] <= 57 ))
            {
                if((ptr[length-2] == 44) && (ptr[length-1] == 88))
                {
                    first = 1;
                    sec = 1;
                    value = operandaddr;
                    sprintf(endstr,"8%03X\n",value);
                }
                else
                {

                    first = 1;
                    sec = 1;
                    value = operandaddr;
                    sprintf(endstr,"0%03X\n",value);
                }
            }
            else
            {
                first = 1;
                sec = 1;
                if(currentaddr > operandaddr)
                {
                    result = (currentaddr - operandaddr);
                    value = operandaddr - baseaddr;
                    if((ptr[length-2] == 44) && (ptr[length-1] == 88))
                    {
                        sprintf(endstr,"C%03X\n",value);

                    }
                    else if(result > 4096)
                    {
                        sprintf(endstr,"4%03X\n",value);
                    }
                    else
                    {
                        if((ptr[length-2] == 44) && (ptr[length-1] == 88))
                        {
                            sprintf(endstr,"A%03X\n",value);

                        }
                        else
                        {
                            value = (4096 - result);

                            sprintf(endstr,"2%03X\n",value);
                        }
                    }
                }
                else
                {
                    value = operandaddr - currentaddr;
                    sprintf(endstr,"2%03X\n",value);
                }
            }
        }
    }
    // This is where the initial opcode from opcodes.c is changed
    sprintf(str,"%I64d%I64d%I64d%I64d",last,third,sec,first);
    binarynum = atoi(str); // converting binary to hex
    d = convertdecimal(binarynum);
    binarynum = atoi(b);
    e = convertdecimal(binarynum);
    sprintf(newop,"%X%X",e,d);
    strcat(newop,endstr);
    temp = newop;
    return  strdup(temp);
}

int convertdecimal(long long n)
{
    int dec = 0, i = 0, rem;

    while (n!=0)
    {
        rem = n % 10;
        n /= 10;
        dec += rem * pow(2, i);
        ++i;
    }

    return dec;
}

char *convert(int character)  // Turns unicode of hex into binary
{
    char *emp;
    switch(character)
    {
    case 48 :
        emp = "0000";
        return emp;
        break;
    case 49 :
        emp = "0001";
        return emp;
        break;
    case 50 :
        emp = "0010";
        return emp;
        break;
    case 51 :
        emp = "0011";
        return emp;
        break;
    case 52 :
        emp = "0100";
        return emp;
        break;
    case 53 :
        emp = "0101";
        return emp;
        break;
    case 54 :
        emp ="0110";
        return emp;
        break;
    case 55 :
        emp = "0111";
        return emp;
        break;
    case 56 :
        emp = "1000";
        return emp;
        break;
    case 57 :
        emp = "1001";
        return emp;
        break;
    case 65 :
        emp = "1010";
        return emp;
        break;
    case 66 :
        emp = "1011";
        return emp;
        break;
    case 67 :
        emp = "1100";
        return emp;
        break;
    case 68 :
        emp = "1101";
        return emp;
        break;
    case 69 :
        emp = "1110";
        return emp;
        break;
    case 70 :
        emp = "1111";
        return emp;
        break;
    default :
        printf("ERROR IN SWITCH STATEMENT");
    }
    return 0;
}
