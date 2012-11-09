#include "mem.h"
#include "except.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    char* p;

    TRY
    {
        p = ALLOC(20);
    }
    EXCEPT(Mem_Failed)
    {
        printf("Mem failed here\n");
        p = 0;
    }
    FINALLY
    {
        FREE(p);
        printf("Finaly\n");
    }
    END_TRY;
    
    return 0;
}
