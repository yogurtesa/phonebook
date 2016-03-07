#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

#if !defined(OPT)
    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(OPT)
    /* build the simplified entry */
    entry_simple *pHead_simple, *e_simple;
    pHead_simple = (entry_simple *) malloc(sizeof(entry_simple));
    printf("size of entry_simple : %lu bytes\n", sizeof(entry_simple));
    e_simple = pHead_simple;
    e_simple->pNext = NULL;
#endif

#if defined(__GNUC__)
#if !defined(OPT)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#if defined(OPT)
    __builtin___clear_cache((char *) pHead_simple, (char *) pHead_simple + sizeof(entry_simple));
#endif
#endif

    /* build the link list, and compute the execution time of function append()i */
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0') {
            i++;
        }
        line[i - 1] = '\0';
        i = 0;
#if !defined(OPT)
        e = append(line, e);
#endif
#if defined(OPT)
        e_simple = append(line, e_simple);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
#if !defined(OPT)
    e = pHead;
#endif
#if defined(OPT)
    e_simple = pHead_simple;
#endif

    // I guess the second  assert code is for checking if input is in the database DICT_FILE
    // I add the #ifdef and #ifndef to put the link list we want to search into the cache
#if !defined(OPT)
    assert(findName(input, e) && "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif
#if defined(OPT)
    assert(findName(input, e_simple) && "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e_simple)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
#if !defined(OPT)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#if defined(OPT)
    __builtin___clear_cache((char *) pHead_simple, (char *) pHead_simple + sizeof(entry_simple));
#endif
#endif

    /* test the link list */ /*
    #if !defined(OPT)
    printf("pHead->pNext->lastName: %s\n", pHead->pNext->lastName);
    printf("pHead->lastName: %s\n", pHead->lastName);
    #endif */

    /* compute the execution time of function findName() */
    clock_gettime(CLOCK_REALTIME, &start);
#if !defined(OPT)
    findName(input, e);
#endif
#if defined(OPT)
    findName(input, e_simple);
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output;
#if defined(OPT)
    output = fopen("opt.txt", "a");
#else
    output = fopen("orig.txt", "a");
#endif
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

#if !defined(OPT)
    e = pHead;
    entry *tmp;
    while(e->pNext != NULL) {
        tmp = e;
        e = e->pNext;
        free(tmp);
    }
    pHead = NULL;
    e = NULL;
    tmp = NULL;
#endif
    //if (pHead->pNext) free(pHead->pNext);
    //free(pHead);
#if defined(OPT)
    e_simple = pHead_simple;
    entry_simple *tmp;
    while(e_simple->pNext != NULL) {
        tmp = e_simple;
        e_simple = e_simple->pNext;
        free(tmp->pEntry);
        free(tmp);
    }
    pHead_simple = NULL;
    e_simple = NULL;
    tmp = NULL;
#endif

    return 0;
}
