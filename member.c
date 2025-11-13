#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // ← NEW
#include "utils.h"
#include "member.h"
#include "book.h"
#include "datastructures/avl.h"
#include "datastructures/hashmap.h"

/* each member also stores a tiny list of borrowed ISBNs (for fine calc) */
/* BorrowRecord is declared in member.h */

static MemberExt* create_member_ext(const char *id, const char *name, const char *email) {
    MemberExt *m = malloc(sizeof(MemberExt));
    if (!m) return NULL;
    strncpy(m->info.memberId, id, sizeof(m->info.memberId)-1);
    strncpy(m->info.name, name, sizeof(m->info.name)-1);
    strncpy(m->info.email, email, sizeof(m->info.email)-1);
    m->borrowed = NULL;
    m->borrowedCount = 0;
    m->borrowedCap = 0;
    m->history = NULL;        // ← NEW
    m->historyCount = 0;      // ← NEW
    m->historyCap = 0;        // ← NEW
    return m;
}

void member_remove_borrow(MemberExt *m, const char *isbn, int overdue) {
    for (int i = 0; i < m->borrowedCount; ++i) {
        if (strcmp(m->borrowed[i].isbn, isbn) == 0) {
            m->borrowed[i].daysOverdue = overdue;

            // ← NEW: Add to history
            if (m->historyCount == m->historyCap) {
                m->historyCap = m->historyCap ? m->historyCap * 2 : 4;
                m->history = realloc(m->history, m->historyCap * sizeof(BorrowRecord));
            }
            m->history[m->historyCount] = m->borrowed[i];  // copy
            ++m->historyCount;

            // shift left
            for (int j = i; j < m->borrowedCount - 1; ++j)
                m->borrowed[j] = m->borrowed[j + 1];
            --m->borrowedCount;
            return;
        }
    }
}

void free_member_ext(void *p) {
    MemberExt *m = (MemberExt*)p;
    if (m) {
        free(m->borrowed);
        free(m->history);  // ← FREE HISTORY
        free(m);
    }
}

/* helper – add a borrow record (used by checkOutBook) */
void member_add_borrow(MemberExt *m, const char *isbn) {
    if (m->borrowedCount == m->borrowedCap) {
        m->borrowedCap = m->borrowedCap ? m->borrowedCap*2 : 4;
        m->borrowed = realloc(m->borrowed, m->borrowedCap * sizeof(BorrowRecord));
    }
    strcpy(m->borrowed[m->borrowedCount].isbn, isbn);
    m->borrowed[m->borrowedCount].daysOverdue = 0;
    m->borrowed[m->borrowedCount].checkoutTime = 0;
    m->borrowed[m->borrowedCount].dueTime = 0;
    ++m->borrowedCount;
}



void registerMember(Library *lib) {
    char id[64], name[128], email[128];
    printf("Enter Member ID: ");   scanf("%s", id);
    printf("Enter Name: ");       scanf("%s", name);
    printf("Enter Email: ");      scanf("%s", email);

    if (hm_get(lib->memberMap, id)) {
        printf("Member %s already exists!\n", id);
        return;
    }
    MemberExt *m = create_member_ext(id, name, email);
    if (!m) { printf("Memory error\n"); return; }
    hm_put(lib->memberMap, id, m);
    printf("Member %s registered.\n", id);
}


/* called from initLibrary() – you must add the memberMap there */
void init_member_map(Library *lib) {
    lib->memberMap = hm_create(64);
}

/* called from program exit – you may add a cleanup function */
void destroy_member_map(Library *lib) {
    hm_destroy(lib->memberMap, free_member_ext);
    lib->memberMap = NULL;
}

// Fine = 1$ per day overdue (simple) 
void calculateFine(Library *lib) {
    char id[64];
    printf("Enter Member ID: "); scanf("%s", id);
    MemberExt *m = hm_get(lib->memberMap, id);
    if (!m) { printf("Member not found.\n"); return; }

    int total = 0;
    for (int i = 0; i < m->borrowedCount; ++i)
        total += m->borrowed[i].daysOverdue;
    for (int i = 0; i < m->historyCount; ++i)  // ← INCLUDE HISTORY
        total += m->history[i].daysOverdue;

    printf("Member %s fine: $%d\n", id, total);
}

// ← NEW: getMemberHistory
void getMemberHistory(Library *lib, const char *memberId) {
    MemberExt *m = hm_get(lib->memberMap, memberId);
    if (!m) {
        printf("Member %s not found.\n", memberId);
        return;
    }

    long long now = 1762992000LL;  // November 13, 2025 (your current date)

    printf("\n=== MEMBER HISTORY for %s ===\n", memberId);

    // CURRENT CHECKOUTS
    printf("Current Checkouts:\n");
    if (m->borrowedCount == 0) {
        printf("  None\n");
    } else {
        for (int i = 0; i < m->borrowedCount; ++i) {
            BorrowRecord *br = &m->borrowed[i];
            Book *b = avl_search(lib->bookRoot, br->isbn);
            if (!b) continue;

            int daysLeft = (int)((br->dueTime - now) / (24 * 3600));
            if (daysLeft < 0) {
                printf("  [OVERDUE] %s | %s | %d days late\n", b->title, br->isbn, -daysLeft);
            } else {
                printf("  [CURRENT] %s | %s | Due in %d days\n", b->title, br->isbn, daysLeft);
            }
        }
    }

    // PAST CHECK-INS (HISTORY)
    printf("\nPast Check-ins:\n");
    if (m->historyCount == 0) {
        printf("  None\n");
    } else {
        for (int i = 0; i < m->historyCount; ++i) {
            BorrowRecord *hr = &m->history[i];
            Book *b = avl_search(lib->bookRoot, hr->isbn);
            if (!b) continue;

            int daysLate = hr->daysOverdue;
            if (daysLate > 0) {
                printf("  [RETURNED] %s | %s | %d days overdue\n", b->title, hr->isbn, daysLate);
            } else {
                printf("  [RETURNED] %s | %s | on time\n", b->title, hr->isbn);
            }
        }
    }
}

// ← NEW FUNCTION: getOverdueBooks
void getOverdueBooks(Library *lib, const char *memberId) {
    MemberExt *m = hm_get(lib->memberMap, memberId);
    if (!m) {
        printf("Member %s not found.\n", memberId);
        return;
    }

    long long now = (long long)time(NULL);
    int found_overdue = 0;

    printf("\n=== OVERDUE & CURRENT CHECKOUTS for %s ===\n", memberId);

    if (m->borrowedCount == 0) {
        printf("No books checked out.\n");
        return;
    }

    for (int i = 0; i < m->borrowedCount; ++i) {
        BorrowRecord *br = &m->borrowed[i];
        Book *b = avl_search(lib->bookRoot, br->isbn);
        if (!b) continue;

        long long due = br->dueTime;
        int daysLate = (int)((now - due) / (24 * 3600));

        if (daysLate > 0) {
            printf("[OVERDUE] %s | %s | %d days late\n",
                   b->title, br->isbn, daysLate);
            found_overdue = 1;
        } else {
            printf("[CURRENT] %s | %s | Due in %d days\n",
                   b->title, br->isbn, -daysLate);
        }
    }

    if (!found_overdue) {
        printf("No overdue books.\n");
    }
}