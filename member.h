// member.h
#ifndef MEMBER_H
#define MEMBER_H

#include "utils.h"

typedef struct {
    char memberId[20];
    char name[50];
    char email[50];
} Member;

/* Borrow record type */
typedef struct {
    char isbn[20];
    int  daysOverdue;
    long long checkoutTime;
    long long dueTime;
} BorrowRecord;

/* FULL DEFINITION — visible to all modules */
typedef struct {
    Member info;
    BorrowRecord *borrowed;
    int borrowedCount;
    int borrowedCap;

    // ← NEW: HISTORY FIELDS
    BorrowRecord *history;
    int historyCount;
    int historyCap;
} MemberExt;

/* Public API */
void registerMember(Library *lib);
void calculateFine(Library *lib);
void getOverdueBooks(Library *lib, const char *memberId);
void getMemberHistory(Library *lib, const char *memberId);  // ← already there

/* Helpers */
void member_add_borrow(MemberExt *m, const char *isbn);
void member_remove_borrow(MemberExt *m, const char *isbn, int overdue);
void free_member_ext(void *p);

/* Map init/destroy */
void init_member_map(Library *lib);
void destroy_member_map(Library *lib);

#endif