#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

	/* test */ 
/*	entry *head = (entry *) malloc(sizeof(entry));
	entry *t = head;
	//head = (entry *) malloc(sizeof(entry));
	//t = (entry *) malloc(sizeof(entry));
	t->pNext = NULL;

	//t = (entry *) malloc(sizeof(entry));
	strcpy(t->lastName, "t1");
	t->pDetail = (detail *) malloc(sizeof(detail));
	//t->pNext = NULL;
	//t = t->pNext;

	t->pNext = (entry *) malloc(sizeof(entry));
	t = t->pNext;
	strcpy(t->lastName, "t2");
	t->pDetail = (detail *) malloc(sizeof(detail));
	t->pNext = NULL;
	//t = t->pNext;

	printf("%s\n%s\n", head->lastName, head->pNext->lastName);
*/

    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;

	/* test */
	strcpy(pHead->lastName, "what?");

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif

	/* test */
	//strcpy(pHead->lastName, "haha");

    /* build the link list, and compute the execution time of function append() */
  /*  clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0') {
            i++;
        }
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);
*/
	bool firstRow = true;
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0') {
            i++;
        }
        line[i - 1] = '\0';
        i = 0;
		if(firstRow == true) {
			strcpy(e->lastName, line);	
			firstRow = false;
		}
		else {
			e = append(line, e);
		}
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

	/* test */
	printf("pHead:%s\npHead->pNext:%s\n", pHead->lastName, pHead->pNext->lastName);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    e = pHead;

    // The second  assert code is for checking if input is in the database DICT_FILE
    assert(findName(input, e) && "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif

    /* compute the execution time of function findName() */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output;
#if OPT == 1
    output = fopen("opt.txt", "a");
#else
    output = fopen("orig.txt", "a");
#endif
	fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
	fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    e = pHead;
    entry *tmp;
    while(e != NULL) {
        tmp = e;
        e = e->pNext;
        free(tmp);
#if OPT == 1
		free(tmp->pDetail);
#endif
    }
    pHead = NULL;
    e = NULL;
    tmp = NULL;

	//original version to free the link list
    //if (pHead->pNext) free(pHead->pNext);
    //free(pHead);

    return 0;
}
