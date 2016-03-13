#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16

/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
// #define OPT 1
typedef struct __PHONE_BOOK_ENTRY_DETAIL {
    char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
    struct __PHONE_BOOK_ENTRY_DETAIL *pNext;
} detail;

/* The new simplified entry */
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *pNext;
    struct __PHONE_BOOK_ENTRY_DETAIL *pDetail;
} entry;

entry *findName(char lastname[], entry *pHead);
entry *append(char lastName[], entry *e);

#endif
