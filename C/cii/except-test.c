/* Last modified Time-stamp: <Ye Wenbin 2007-12-30 16:29:58>
 * @(#)except-test.c
 */
#include "except.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    int i = 0;

    Except_T e1 = {"e1 exception"};
    
    TRY
        i = 2;
        RAISE(e1);
    EXCEPT(e1)
        printf("hello world: e1 exception \n");
    FINALLY
        printf("finally\n");
    END_TRY;
        
    return 0;
}
