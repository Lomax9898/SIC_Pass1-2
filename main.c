#include "headers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
int main( int argc, char* argv[])
{

    FILE *fp;
    char line[1024];
    char* newsym;
    char* nextoken;
    char* fintoken;
    int linenum = 0;
    int base;
    int locctr = 0x4000;
    int i = 0;
    int tcounter = 0;
    int mcounter = 0;
    int xemcounter = 0;
    int firstaddr;
    int started = 0;
    int temp = 0;
    int ephem = 0;
    char *op;
    int arrsize = 0;
    char hex[1024];
    strcpy(hex, "0x");
    char * separator = "";
    char * separator_m;
    char* newsym_m;
    char* nextoken_m;
    char* fintoken_m;
    char fullline[1024];
    int x;

    // This shows error if user inputs too many arguments
    if ( argc != 2 )
    {
        printf("ERROR: Usage: %s filename\n", argv[0]);
        return 0;
    }
    fp = fopen( argv[1], "r");
    //error if failed to open file
    if ( fp == NULL )
    {
        printf("ERROR: %s could not be opened for reading,\n", argv[1] );
        return 0;
    }
    SYMBOL* SymTab[1024];
    memset(SymTab, '\0', 1024*sizeof(SYMBOL*));// Holds the Symbol table
    LINEADD* LineTab[1024];
    memset(LineTab, '\0', 1024*sizeof(LINEADD*)); //Holds the line numbers and location counter for each line
    int IsAnInstruction( char *Test );
    int IsADirective( char *Test );
    char* InstructionOP( char *Test );
    char* opchanger(char *Test, int symboladdr,char* token, char *operand, int operandaddr, int baseaddr);
    int DupSym(char* Sname)  //This function looks for duplicate symbols and returns one if it finds any
    {
        int index = 0;
        int duprule = 0;
        int dupfound = 0;
        char* strptr;
        for(index =0; index < arrsize; index++)
        {
            strptr = SymTab[index]->Name;
            if((strcmp(strptr, Sname)==0))
            {
                dupfound++;
            }

        }
        if (dupfound > 1)
        {
            duprule = 1;
        }
        return duprule;
    }
    int stringcompare(char *a,char *b)
    {
        int flag=0;
        while(*a!='\0' && *b!='\0')  // while loop
        {
            if(*a!=*b)
            {
                flag=1;
            }
            a++;
            b++;
        }

        if(flag==0)
            return 0;
        else
            return 1;
    }
    int FindSymbol(char* Sname)  // This function loops through symbol table looking for match to return that symbols index
    {
        int index = 0;
        char str[8];
        char str2[8];
        char* newline = strchr(Sname, '\n'); // To get rid of extra tab spaces and newlines
        if (newline) *
            newline = '\0';
        char* tabline = strchr(Sname, '\t');
        if (tabline) *
            tabline = '\0';
        char delim[] = " ";
        char *ptr =strtok(Sname, delim);
        sprintf(str2,"%s",ptr);
        for(index= 0; index!=arrsize; index++) // Looping through the symbol table and comparing each element to string
        {
            sprintf(str,"%s",SymTab[index]->Name);
            if(stringcompare(str2,str)== 0)
            {
                return index;
                break;
            }
        }
        printf("Failed to find exact copy of %s in symbol table\n",Sname);
        return 0;
    }
    void AddSymbol(SYMBOL* Tab[], int addr, int src, char* Sname)  // This function add symbols with their name, source line, and the address to the symbol table
    {
        arrsize++;
        int index;
        index = 0;
        SYMBOL* newSym; // Pointer saves the memory addresses for later usage
        newSym = malloc(sizeof(SYMBOL));
        newSym->Address = addr;
        newSym->sourceline = src;
        strcpy(newSym->Name, Sname);
        while(Tab[index]!= NULL )
        {
            index++;
        }
        Tab[index] = newSym;
        if(DupSym(Sname) == 1) // Error checking
        {
            printf("ASSEMBLY ERROR:\n\n");
            printf("%s\n", fullline);
            printf("Line %d The symbol %s, is already in the Symbol Table.\n", linenum, Sname);
            fclose(fp);
            exit(0);
        }

        if( ( SymTab[index]->Address >= 0x100000 ) )
        {
            printf("ASSEMBLY ERROR:\n\n");
            printf("%s\n", fullline);
            printf("Line %d is not within SIC memory.\n", linenum);
            fclose(fp);
            exit(0);
        }
    }

    void string2hexString(char* input, char* output)  //Changes a string into a hex string
    {
        int loop;
        int i;
        i=0;
        loop=2;
        while(input[loop] != 39)
        {
            sprintf((char*)(output+i),"%02X", input[loop]);
            loop+=1;
            i+=2;
        }
        output[i++] = '\0';
    }
    void Addline(LINEADD* Tab[], int locaddr, int lsrc)  //Adds line number and location counter to the line table
    {
        int index;
        index = 0;
        LINEADD* newlin;
        newlin = malloc(sizeof(LINEADD));
        newlin->locaddress = locaddr;
        newlin->linesrc = lsrc;
        while(Tab[index]!= NULL )
        {
            index++;
        }
        Tab[index] = newlin;
    }
    newsym_m = newsym = malloc(  1024 * sizeof(char)             ); //Allocating space
    memset( newsym, '\0', 1024 * sizeof(char) );
    nextoken_m = nextoken = malloc(  1024 * sizeof(char)             );
    memset( nextoken, '\0', 1024 * sizeof(char) );
    fintoken_m = fintoken = malloc(  1024 * sizeof(char)             );
    memset( fintoken, '\0', 1024 * sizeof(char) );
    separator_m = separator = malloc(  1024 * sizeof(char)             );
    memset( separator, '\0', 1024 * sizeof(char) );
    while(  fgets( line, 1024, fp ) != NULL   ) // This is where the file gets scanned line by line
    {
        linenum++;
        strcpy( fullline, line );
        if ( line[0] == 35) // Skips any comments in the file
        {

            continue;
        }
        int len = strlen(line);
        if(len <= 2)  // Error Check for blank line
        {
            printf("ASSEMBLY ERROR:\n\n");
            printf("Line %d is a blank line, which violates specification.\n", linenum);
            fclose(fp);
            exit(0);
        }
        if (  !((line[0] >= 65 ) && ( line[0] <= 90 ))   ) // If line starts with A-Z character
        {
            if(((line[0] >= 33 ) && ( line[0] <= 64 )) || ((line[0] >= 91 ) && ( line[0] <= 126 ))   )
            {
                printf("ASSEMBLY ERROR:\n\n");
                printf("%s\n", fullline);
                printf("Line %d symbols must start with an alpha character.\n", linenum);
                fclose(fp);
                exit(0);
            }
        }
        if (  (line[0] >= 65 ) && ( line[0] <= 90 )   )
        {
            newsym = strtok( line, " \t\n");  // Turns line into token
            if ( IsAValidSymbol(newsym) == 0 )
            {
                printf("ASSEMBLY ERROR:\n\n");
                printf("%s\n", fullline);
                printf("Line %d has invalid symbol %s.\n", linenum, newsym);
                fclose(fp);
                exit(0);
            }
            nextoken = strtok( NULL, " \t\n"  ); // turns the next part of the line into a token
            fintoken = strtok( NULL, separator );// turns the last part of the line into a token
            if(IsADirective(nextoken) == 1) // First checking if the 2nd token is a directive
            {
                if(strcmp("START", nextoken)== 0)
                {
                    if(started == 1)
                    {
                        printf("ASSEMBLY ERROR:\n\n");
                        printf("%s\n", fullline);
                        printf("Line %d START can only appear once in the file.\n", linenum);
                        fclose(fp);
                        exit(0);
                    }
                    strcat(hex, fintoken); // convert the last token to hex
                    locctr = (int)strtol(hex, NULL, 16); // sets the location counter
                    AddSymbol(SymTab, locctr, linenum, newsym);
                    Addline(LineTab, locctr, linenum);
                    started = 1;
                    continue;
                }
                else if((started == 0) &&(strcmp("START", nextoken)!= 0 )) // Error catch for no start directive
                {
                    printf("ASSEMBLY ERROR:\n\n");
                    printf("%s\n", fullline);
                    printf("Line %d START directive not defined.\n", linenum);
                    fclose(fp);
                    exit(0);

                }
                else if(strcmp("END", nextoken)==0)
                {
                    AddSymbol(SymTab, locctr, linenum, newsym);
                    Addline(LineTab, locctr, linenum);
                    temp = FindSymbol(fintoken);
                    if(SymTab[temp])
                    {
                        firstaddr = SymTab[temp]->Address;
                    }
                    temp = 0;
                    continue;
                }
                else if(strcmp("RESB", nextoken)== 0)
                {
                    AddSymbol(SymTab, locctr, linenum, newsym);
                    Addline(LineTab, locctr, linenum);
                    temp = atoi(fintoken);
                    locctr = locctr + temp;
                    temp = 0;
                    continue;

                }
                else if(strcmp("BYTE", nextoken)== 0)
                {
                    if(fintoken[0] == 67) // This directive changes the location counter based on the last token
                    {
                        // First character determines what calculations to execute
                        int m;
                        int charcount;
                        charcount = 0;
                        for(m=2; fintoken[m]; m++)
                        {
                            if(fintoken[m] != 39)
                            {
                                charcount++;
                            }
                            else if(fintoken[m] == 39)
                            {
                                break;
                            }
                        }
                        temp = charcount;
                        AddSymbol(SymTab, locctr, linenum, newsym);
                        Addline(LineTab, locctr, linenum);
                        locctr = locctr + temp;
                        charcount = charcount * 2;
                        while(charcount >60)
                        {
                            charcount = charcount - 60;
                            tcounter++;
                        }
                        temp = 0;
                        tcounter ++;
                        continue;
                    }
                    else if(fintoken[0] == 88)
                    {
                        int n;
                        int charctr;
                        charctr = 0;
                        for(n=2; fintoken[n]; n++)
                        {
                            if(fintoken[n] != 39)
                            {
                                if(((fintoken[n] >= 33 ) && ( fintoken[n] <= 38 )) ||((fintoken[n] >= 40 ) && ( fintoken[n] <= 47 )) || ((fintoken[n] >= 58 ) && ( fintoken[n] <= 64 )) ||
                                        ((fintoken[n] >= 71 ) && ( fintoken[n] <= 96 )) || ((fintoken[n] >= 103 ) && ( fintoken[n] <= 126 ))  )
                                {
                                    printf("ASSEMBLY ERROR:\n\n");
                                    printf("%s\n", fullline);
                                    printf("Line %d has a invalid hex constant.\n", linenum);
                                    fclose(fp);
                                    exit(0);
                                }
                                else
                                {
                                    charctr++;
                                }
                            }
                            else if(fintoken[n] == 39)
                            {
                                break;
                            }
                        }
                        if(charctr%2 != 0)
                        {
                            printf("ASSEMBLY ERROR:\n\n");
                            printf("%s\n", fullline);
                            printf("Line %d has an odd number of hex characters.\n", linenum);
                            fclose(fp);
                            exit(0);
                        }
                        charctr = (charctr/2);
                        temp = charctr;
                        AddSymbol(SymTab, locctr, linenum, newsym);
                        Addline(LineTab, locctr, linenum);
                        locctr = locctr + temp;
                        temp = 0;
                        tcounter ++;
                        continue;
                    }
                }
                else if(strcmp("WORD", nextoken)== 0)
                {
                    temp = atoi(fintoken);
                    if(temp > 1048576 )
                    {
                        printf("ASSEMBLY ERROR:\n\n");
                        printf("%s\n", fullline);
                        printf("Line %d WORD constant exceeds 24 bit limitation.\n", linenum);
                        fclose(fp);
                        exit(0);
                    }
                    AddSymbol(SymTab, locctr, linenum, newsym);
                    Addline(LineTab, locctr, linenum);
                    locctr = locctr + 3;
                    tcounter ++;
                    continue;
                    temp = 0;
                }
                else if(strcmp("RESW", nextoken)== 0)
                {
                    AddSymbol(SymTab, locctr, linenum, newsym);
                    Addline(LineTab, locctr, linenum);
                    temp = atoi(fintoken);
                    temp = temp * 3;
                    locctr = locctr + temp;
                    temp = 0;
                    continue;
                }
            }
            if(strcmp( nextoken,"CLEAR")== 0 )
            {
                AddSymbol(SymTab, locctr, linenum, newsym);
                Addline(LineTab, locctr, linenum);
                locctr = locctr + 2;
                tcounter ++;
                continue;
                temp = 0;
            }

            if(IsAnInstruction(newsym) == 1)
            {
                printf("ASSEMBLY ERROR:\n\n");
                printf("%s\n", fullline);
                printf("Line %d has symbol name that is an assembler instruction %s.\n", linenum, newsym);
                fclose(fp);
                exit(0);
            }
            if(IsADirective(newsym) == 1)
            {
                printf("ASSEMBLY ERROR:\n\n");
                printf("%s\n", fullline);
                printf("Line %d has symbol name that is an assembler directive %s.\n", linenum, newsym);
                fclose(fp);
                exit(0);
            }
            AddSymbol(SymTab, locctr, linenum, newsym);
            Addline(LineTab, locctr, linenum);
            if(nextoken[0] == 43)
            {
                locctr = locctr + 4;
                char delim[] = " ";
                char delim2[] = "@";
                char delim3[] = "#";
                char *ptr = strtok(fintoken, delim);
                char *ptr2 = strtok(ptr, delim2);
                char *ptr3 = strtok(ptr2, delim3);
                xemcounter ++;
                if( (ptr3[0] >= 48) && (ptr3[0] <= 57 ))
                {
                    xemcounter --;
                }
            }
            else
            {
                locctr = locctr + 3;
            }
            tcounter ++;
            mcounter ++;
            i++;
            continue;
        }
        if (  !(line[0] >= 65 ) && ( line[0] <= 90 )   )//In case there is no symbol
        {
            // still process the rest of the line
            nextoken = strtok( line, " \t\n"  );
            fintoken = strtok( NULL, separator );
            if(nextoken[0] == 43)
            {
                char delim[] = " ";
                char delim2[] = "@";
                char delim3[] = "#";
                char *ptr = strtok(fintoken, delim);
                char *ptr2 = strtok(ptr, delim2);
                char *ptr3 = strtok(ptr2, delim3);
                xemcounter ++;
                if( (ptr3[0] >= 48) && (ptr3[0] <= 57 ))
                {
                    xemcounter --;
                }
                Addline(LineTab, locctr, linenum);
                tcounter ++;
                locctr = locctr + 4;
                continue;
            }
            if((strcmp( nextoken,"BASE") == 0) || (strcmp( nextoken,"END") == 0))
            {
                Addline(LineTab, locctr, linenum);
                temp = 0;
                continue;
            }
            else if((strcmp( nextoken,"CLEAR") == 0) || (strcmp( nextoken,"ADDR") == 0) || (strcmp( nextoken,"COMPR") == 0) || (strcmp( nextoken,"DIVR") == 0) || (strcmp( nextoken,"MULR") == 0) || (strcmp( nextoken,"SHIFTL") == 0) || (strcmp( nextoken,"SHIFTR") == 0) || (strcmp( nextoken,"SUBR") == 0) || (strcmp( nextoken,"SVC") == 0) || (strcmp( nextoken,"TIXR") == 0) )
            {
                Addline(LineTab, locctr, linenum);
                locctr = locctr + 2;
                tcounter ++;
                continue;
                temp = 0;
            }
            else if(len <= 6)
            {
                Addline(LineTab, locctr, linenum);
                tcounter ++;
                locctr = locctr + 3;
            }
            else
            {
                Addline(LineTab, locctr, linenum);
                tcounter ++;
                locctr = locctr + 3;
                mcounter ++;
            }
        }
    }
    int proglen = locctr - SymTab[0]->Address;
//Pass 2 Starts here
    char trecords[tcounter][70];
    memset( trecords, 0, tcounter*70*sizeof(char) );
    char mrecords[mcounter][16];
    memset( mrecords, 0, mcounter*16*sizeof(char) );
    xemcounter = 0;
    tcounter = 0;
    int areg = 0;
    int xreg = 1;
    int lreg = 2;
    int breg = 3;
    int sreg = 4;
    int treg = 5;
    int freg = 6;
    int recordlen = 0;
    char str[80];
    char str3[80];
    char str2[80];
    char hrec[20] = "H";
    char erec[9];
    rewind(fp); // Rewinding to the start of the file
    linenum = 0;// Pass 2 we are making T records and M records
    while(  fgets( line, 1024, fp ) != NULL   )// The source file tells us how to build each record
    {
        // In pass 1 we were collecting info and counting how many records would be needed.

        strcpy( fullline, line );
        if ( line[0] == 35)
        {

            continue;
        }
        int len = strlen(line);
        int numofspaces;
        linenum++;
        if (  (line[0] >= 65 ) && ( line[0] <= 90 )   )
        {
            newsym = strtok( line, " \t\n");
            nextoken = strtok( NULL, " \t\n" );
            fintoken = strtok( NULL, " \t\n"  );
            if(nextoken[0] == 43 )
            {
                if(!((fintoken[0] >= 48) && (fintoken[0] <= 57 )) && !((fintoken[1] >= 48) && (fintoken[1] <= 57 )))
                {
                    temp = (LineTab[linenum -1]->locaddress);
                    temp = temp + 1;
                    sprintf(str, "M%06X05+%s\n", temp,SymTab[0]->Name );
                    strcpy(mrecords[xemcounter], str);
                    xemcounter++;
                    temp = 0;
                }
            }
            char delimplus[] = "+";
            char *nextokenplus = strtok(nextoken, delimplus);
            char* tabline = strchr(fintoken, '\t');
            if (tabline) *
                tabline = '\0';
            if(IsADirective(nextoken) == 1)
            {
                if(strcmp("START", nextoken)== 0)
                {

                    strcat(hrec, newsym);// I make all the records with concatenating strings
                    temp = strlen(hrec);
                    numofspaces = 7 - temp;
                    while(numofspaces > 0)
                    {
                        strcat(hrec," ");
                        numofspaces = numofspaces - 1;
                    }
                    sprintf(str,"%06X",SymTab[1]->Address);
                    strcat(hrec, str);
                    sprintf(str,"%06X\n",proglen);
                    strcat(hrec, str);

                }
                else if(strcmp("END", nextoken)==0)
                {
                    sprintf(erec, "E%06X\n",firstaddr);
                    continue;
                }
                else if(strcmp("WORD", nextoken)==0)
                {

                    temp = FindSymbol(newsym);
                    sprintf(str,"T%06X",SymTab[temp]->Address);
                    temp = atoi(fintoken);
                    sprintf(str2,"%06X\n",temp);
                    recordlen = strlen(str2);
                    recordlen = recordlen/2;
                    sprintf(str3,"%02X",recordlen);
                    strcat(str, str3);
                    strcat(str, str2);
                    strcpy(trecords[tcounter], str);
                    tcounter++;

                    continue;

                }
                else if(strcmp("BYTE", nextoken)==0)
                {

                    if(fintoken[0] == 67)
                    {
                        int m = 2;
                        int charcount;
                        char buffer[200] = "init";// To hold big strings if needed
                        charcount = 0;
                        int offset = 0;
                        char newline[2] = "\n";
                        int thirtycount = 0;
                        int num = 60;
                        for(m=2; fintoken[m]; m++)
                        {
                            if(fintoken[m] != 39)
                            {
                                charcount++;
                            }
                            else if(fintoken[m] == 39)
                            {
                                break;
                            }
                        }
                        char hex_str[(charcount*2)+1]; //For this part of the directive I have to change the string to hex
                        string2hexString(fintoken, hex_str);
                        temp = FindSymbol(newsym);
                        recordlen = strlen(hex_str);// So that I can then put that into the record
                        charcount = recordlen;
                        if(recordlen > 60)
                        {
                            while(recordlen > 60)
                            {
                                strncpy(buffer, hex_str + offset, num);
                                buffer[num+1] = 0;
                                charcount = num;
                                charcount = charcount/2;
                                temp = SymTab[temp]->Address + (thirtycount * 30);
                                sprintf(str,"T%06X",temp);
                                sprintf(str2,"%02X",charcount);
                                strcat(str, str2);
                                strcat(str, buffer);
                                strcat(str,newline);
                                offset = offset + num;
                                strncpy(trecords[tcounter], str, 69);
                                thirtycount++;
                                tcounter++;
                                recordlen = recordlen - num;
                            }
                            temp = FindSymbol(newsym);
                            temp = SymTab[temp]->Address + (thirtycount * 30);
                            sprintf(str,"\nT%06X",temp);
                            num = recordlen;
                            charcount = num;
                            charcount = charcount/2;
                            strncpy(buffer, hex_str + offset,num);
                            buffer[num] = 0;
                            sprintf(str2,"%02X",charcount);
                            strcat(str, str2);
                            strcat(str, buffer);
                            strcat(str,newline);
                            charcount = strlen(str);
                            strncpy(trecords[tcounter], str,charcount);
                            tcounter++;

                            continue;

                        }
                        else
                        {

                            sprintf(str,"T%06X",SymTab[temp]->Address);
                            charcount = recordlen;
                            charcount = charcount/2;
                            sprintf(str2,"%02X",charcount);
                            sprintf(str3,"%s\n", hex_str);
                            strcat(str, str2);
                            strcat(str, str3);
                            strcpy(trecords[tcounter], str);
                            tcounter++;
                            temp = 0;
                            continue;
                        }
                    }
                    else if(fintoken[0] == 88) // different process if the directive starts with different character
                    {
                        int charcount;
                        int m;
                        charcount = 0;
                        for(m=2; fintoken[m]; m++)
                        {
                            if(fintoken[m] != 39)
                            {
                                charcount++;
                            }
                            else if(fintoken[m] == 39)
                            {
                                break;
                            }
                        }
                        char delim[] = "'";
                        char *ptr = strtok(fintoken, delim);
                        ptr = strtok(NULL, delim);
                        temp = FindSymbol(newsym);
                        sprintf(str,"T%06X",SymTab[temp]->Address);
                        sprintf(str3,"%s\n", ptr);
                        recordlen = strlen(str3);
                        recordlen = recordlen/2;
                        sprintf(str2,"%02X",recordlen);
                        strcat(str, str2);
                        strcat(str, str3);
                        strcpy(trecords[tcounter], str);
                        tcounter++;
                        temp = 0;
                        continue;
                    }


                }
            }
            else if(IsAnInstruction(nextokenplus) == 1)
            {
                if((strcmp( nextoken,"CLEAR") == 0) ||(strcmp( nextoken,"TIXR") == 0)  )// This is for assembler registers
                {
                    sprintf(str,"T%06X",(LineTab[linenum -1]->locaddress));
                    op = InstructionOP(nextokenplus);
                    sprintf(str2,"%s", op);
                    if((strcmp( fintoken,"A") == 0))
                    {
                        temp = areg;
                    }
                    else if((strcmp( fintoken,"X") == 0))
                    {
                        temp = xreg;
                    }
                    else if((strcmp( fintoken,"L") == 0))
                    {
                        temp = lreg;
                    }
                    else if((strcmp( fintoken,"B") == 0))
                    {
                        temp = breg;
                    }
                    else if((strcmp( fintoken,"S") == 0))
                    {
                        temp = sreg;
                    }
                    else if((strcmp( fintoken,"T") == 0))
                    {
                        temp = treg;
                    }
                    else if((strcmp( fintoken,"F") == 0))
                    {
                        temp = freg;
                    }
                    sprintf(str3,"%d0\n", temp);
                    strcat(str2, str3);
                    recordlen = strlen(str2);
                    recordlen = recordlen/2;
                    sprintf(str3,"%02X",recordlen);
                    strcat(str, str3);
                    strcat(str, str2);
                    strcpy(trecords[tcounter], str);
                    tcounter++;
                    continue;
                }
                op = InstructionOP(nextokenplus);
                temp = FindSymbol(newsym);// To clear space or symbols that could ruin the record
                char delim[] = " ";
                char delim2[] = "@";
                char delim3[] = "#";
                char *ptr = strtok(fintoken, delim);
                char *ptr2 = strtok(ptr, delim2);
                char *ptr3 = strtok(ptr2, delim3);
                ephem = FindSymbol(ptr3);
                sprintf(str,"T%06X",SymTab[temp]->Address);
                op = opchanger(op,LineTab[linenum]->locaddress,nextoken,fintoken,SymTab[ephem]->Address,base);
                recordlen = strlen(op);
                recordlen = recordlen/2;
                sprintf(str3,"%02X",recordlen);
                strcat(str, str3);
                strcat(str, op);
                strcpy(trecords[tcounter], str);
                tcounter++;
                continue;
            }

        }
        if (  !(line[0] >= 65 ) && ( line[0] <= 90 )   )
        {
            nextoken = strtok( line, " \t\n"  );
            fintoken = strtok(NULL, " \t\n");
            if(nextoken[0] == 43 )
            {
                if(!((fintoken[0] >= 48) && (fintoken[0] <= 57 )) && !((fintoken[1] >= 48) && (fintoken[1] <= 57 )))
                {
                    temp = (LineTab[linenum -1]->locaddress);
                    temp = temp + 1;
                    sprintf(str, "M%06X05+%s\n", temp,SymTab[0]->Name );
                    strcpy(mrecords[xemcounter], str);
                    xemcounter++;
                    temp = 0;
                }
            }
            char delimplus[] = "+";
            char *nextokenplus = strtok(nextoken, delimplus);
            if(strcmp( nextoken,"BASE") == 0)
            {
                temp = FindSymbol(fintoken);
                base = (SymTab[temp]->Address);
                continue;
            }
            if((strcmp( nextoken,"CLEAR") == 0) ||(strcmp( nextoken,"TIXR") == 0) || (strcmp( nextoken,"SHIFTL") == 0) || (strcmp( nextoken,"SHIFTR") == 0))
            {
                sprintf(str,"T%06X",(LineTab[linenum -1]->locaddress));
                op = InstructionOP(nextokenplus);
                sprintf(str2,"%s", op);
                if((strcmp( fintoken,"A") == 0))
                {
                    temp = areg;
                }
                else if((strcmp( fintoken,"X") == 0))
                {
                    temp = xreg;
                }
                else if((strcmp( fintoken,"L") == 0))
                {
                    temp = lreg;
                }
                else if((strcmp( fintoken,"B") == 0))
                {
                    temp = breg;
                }
                else if((strcmp( fintoken,"S") == 0))
                {
                    temp = sreg;
                }
                else if((strcmp( fintoken,"T") == 0))
                {
                    temp = treg;
                }
                else if((strcmp( fintoken,"F") == 0))
                {
                    temp = freg;
                }
                sprintf(str3,"%d0\n", temp);
                strcat(str2, str3);
                recordlen = strlen(str2);
                recordlen = recordlen/2;
                sprintf(str3,"%02X",recordlen);
                strcat(str, str3);
                strcat(str, str2);
                strcpy(trecords[tcounter], str);
                tcounter++;
                continue;
            }
            else if((strcmp( nextoken,"ADDR") == 0) ||(strcmp( nextoken,"COMPR") == 0) || (strcmp( nextoken,"DIVR") == 0) || (strcmp( nextoken,"MULR") == 0) ||  (strcmp( nextoken,"RMO") == 0) ||  (strcmp( nextoken,"SUBR") == 0))
            {
                // register instructions
                sprintf(str,"T%06X",(LineTab[linenum -1]->locaddress));
                op = InstructionOP(nextokenplus);
                sprintf(str2,"%s", op);
                if(fintoken[0] == 65)
                {
                    temp = areg;
                }
                else if(fintoken[0] == 88)
                {
                    temp = xreg;
                }
                else if(fintoken[0] == 76)
                {
                    temp = lreg;
                }
                else if(fintoken[0] == 66)
                {
                    temp = breg;
                }
                else if(fintoken[0] == 83)
                {
                    temp = sreg;
                }
                else if(fintoken[0] == 84)
                {
                    temp = treg;
                }
                else if(fintoken[0] == 70)
                {
                    temp = freg;
                }
                if(fintoken[2] == 65)
                {
                    ephem = areg;
                }
                else if(fintoken[2] == 88)
                {
                    ephem = xreg;
                }
                else if(fintoken[2] == 76)
                {
                    ephem = lreg;
                }
                else if(fintoken[2] == 66)
                {
                    ephem = breg;
                }
                else if(fintoken[2] == 83)
                {
                    ephem = sreg;
                }
                else if(fintoken[2] == 84)
                {
                    ephem = treg;
                }
                else if(fintoken[2] == 70)
                {
                    ephem = freg;
                }
                sprintf(str3,"%d%d\n", temp, ephem);
                strcat(str2, str3);
                recordlen = strlen(str2);
                recordlen = recordlen/2;
                sprintf(str3,"%02X",recordlen);
                strcat(str, str3);
                strcat(str, str2);
                strcpy(trecords[tcounter], str);
                tcounter++;
                continue;
            }
            else if(IsAnInstruction(nextokenplus) == 1)
            {
                op = InstructionOP(nextokenplus);
                if(fintoken!= NULL)
                {
                    char* newline = strchr(fintoken, '\n');
                    if (newline) *
                        newline = '\0';
                    char* tabline = strchr(fintoken, '\t');
                    if (tabline) *
                        tabline = '\0';
                    char delim[] = " ";
                    strtok(fintoken, delim);
                    len = strlen(fintoken);
                }
            }
            if((len == 0)||(fintoken == NULL))
            {
                int badp = 0;
                char *badcharp = NULL;
                sprintf(str,"T%06X",(LineTab[linenum -1]->locaddress));
                op = opchanger(op,LineTab[linenum]->locaddress,nextoken,badcharp,badp,base);
                temp = 0;
                recordlen = strlen(op);
                recordlen = recordlen/2;
                sprintf(str3,"%02X",recordlen);
                strcat(str, str3);
                strcat(str, op);
                strcpy(trecords[tcounter], str);
                tcounter++;
                continue;
            }
            else
            {
                char delim[] = ",";
                int length;
                length = strlen(fintoken);
                if((fintoken[length-2] == 44) && (fintoken[length-1] == 88))
                {
                    sprintf(str,"T%06X",(LineTab[linenum -1]->locaddress));
                    sprintf(str2,"%s", op);
                    char *holder;
                    strcpy(str3,fintoken);
                    char *ptr = strtok(fintoken, delim);
                    temp = FindSymbol(ptr);
                    holder = str3;
                    if((ptr[0] >= 48) && (ptr[0] <= 57 ))
                    {
                        temp = atoi(ptr);
                        op = opchanger(op,LineTab[linenum]->locaddress,nextoken,holder,temp,base);
                    }
                    else
                    {
                        ephem = FindSymbol(ptr);
                        op = opchanger(op,LineTab[linenum]->locaddress,nextoken,holder,SymTab[ephem]->Address,base);
                    }
                    recordlen = strlen(op);
                    recordlen = recordlen/2;
                    sprintf(str3,"%02X",recordlen);
                    strcat(str, str3);
                    strcat(str, op);
                    strcpy(trecords[tcounter], str);
                    tcounter++;
                    continue;
                }
                else
                {
                    if(strcmp("END", nextoken)==0)
                    {
                        sprintf(erec, "E%06X\n",firstaddr);
                        continue;
                    }
                    sprintf(str,"T%06X",(LineTab[linenum -1]->locaddress));
                    char delim[] = " ";
                    char delim2[] = "@";
                    char delim3[] = "#";
                    char *ptr = strtok(fintoken, delim);
                    char *ptr2 = strtok(ptr, delim2);
                    char *ptr3 = strtok(ptr2, delim3);
                    if((ptr3[0] >= 48) && (ptr3[0] <= 57 ))
                    {
                        temp = atoi(ptr3);
                        op = opchanger(op,LineTab[linenum]->locaddress,nextoken,fintoken,temp,base);
                    }
                    else
                    {
                        ephem = FindSymbol(ptr3);
                        op = opchanger(op,LineTab[linenum]->locaddress,nextoken,fintoken,SymTab[ephem]->Address,base);
                    }
                    recordlen = strlen(op);
                    recordlen = recordlen/2;
                    sprintf(str3,"%02X",recordlen);
                    strcat(str, str3);
                    strcat(str, op);
                    strcpy(trecords[tcounter], str);
                    tcounter++;
                    continue;

                }
            }
        }

    }// After this I free memory and write all the records to a file with the same name as input file
    char ext[5] = ".obj"; // but with the .obj extension
    strcat(argv[1],ext);
    free(nextoken_m);
    for(int i=0; SymTab[i]; i++)
    {
        free(SymTab[i]);
    }
    for(int i=0; LineTab[i]; i++)
    {
        free(LineTab[i]);
    }
    free(newsym_m);
    free(fintoken_m);
    free(separator_m);
    fclose( fp );

    fp = fopen (argv[1], "w");
    fputs(hrec, fp);
    for (x= 0; x<tcounter; x++)
    {
        fputs(trecords[x], fp);
    }
    for (x= 0; x<mcounter; x++)
    {
        fputs(mrecords[x], fp);
    }
    fputs(erec, fp);

    fclose( fp );


}
