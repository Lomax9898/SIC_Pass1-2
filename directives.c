#include "headers.h"
// // String comparison check for directives

int IsADirective( char *Test )
{

    return		( 	! (
                        strcmp( Test, "START" ) &&
                        strcmp( Test, "END" )   &&
                        strcmp( Test, "RESB" )  &&
                        strcmp( Test, "RESW" )  &&
                        strcmp( Test, "BYTE" )  &&
                        strcmp( Test, "WORD" )

                    ) ) ;
}

