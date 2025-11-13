#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "member.h"
#include "datastructures/hashmap.h"

/* each member also stores a tiny list of borrowed ISBNs (for fine calc) */
typedef struct {
    char isbn[20];
    int  daysOverdue;   /* set when check-in happens */
} BorrowRecord;


static MemberExt* create_member_ext(const char *id, const char *name, const char *email) {
    MemberExt *m = malloc(sizeof(MemberExt));
    if (!m) return NULL;
    strncpy(m->info.memberId, id, sizeof(m->info.memberId)-1);
    strncpy(m->info.name, name, sizeof(m->info.name)-1);
    strncpy(m->info.email, email, sizeof(m->info.email)-1);
    m->borrowed = NULL;
    m->borrowedCount = 0;
    m->borrowedCap = 0;
    return m;
}

/* helper – add a borrow record (used by checkOutBook) */
void member_add_borrow(MemberExt *m, const char *isbn) {
    if (m->borrowedCount == m->borrowedCap) {
        m->borrowedCap = m->borrowedCap ? m->borrowedCap*2 : 4;
        m->borrowed = realloc(m->borrowed, m->borrowedCap * sizeof(BorrowRecord));
    }
    strcpy(m->borrowed[m->borrowedCount].isbn, isbn);
    m->borrowed[m->borrowedCount].daysOverdue = 0;
    ++m->borrowedCount;
}

// helper – remove a borrow record (used by checkInBook) 
void member_remove_borrow(MemberExt *m, const char *isbn, int overdue) {
    for (int i = 0; i < m->borrowedCount; ++i) {
        if (strcmp(m->borrowed[i].isbn, isbn) == 0) {
            m->borrowed[i].daysOverdue = overdue;
            /* shift left */
            for (int j = i; j < m->borrowedCount-1; ++j)
                m->borrowed[j] = m->borrowed[j+1];
            --m->borrowedCount;
            return;
        }
    }
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

void free_member_ext(void *p) {
    MemberExt *m = (MemberExt*)p;
    if (m) {
        free(m->borrowed);
        free(m);
    }
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

    printf("Member %s fine: $%d\n", id, total);
}