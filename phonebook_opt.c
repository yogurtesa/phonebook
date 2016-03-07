#include <stdlib.h>
#include <string.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry_simple *findName(char lastName[], entry_simple *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry_simple *append(char lastName[], entry_simple *e)
{
    e->pNext = (entry_simple *) malloc(sizeof(entry_simple));
    e = e->pNext;
    e->pEntry = (entry *) malloc(sizeof(entry));
    strcpy(e->pEntry->lastName, lastName);
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}
