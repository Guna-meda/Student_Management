#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "book.h"
#include "member.h"
#include "datastructures/priorityqueue.h"
#include "datastructures/graph.h"
#include "datastructures/hashmap.h"

void checkOutBook(Library *lib) {
    char mid[64], isbn[64];
    printf("Member ID: "); scanf("%s", mid);
    printf("ISBN: ");      scanf("%s", isbn);

    MemberExt *m = hm_get(lib->memberMap, mid);
    Book *b = avl_search(lib->bookRoot, isbn);
    if (!m) { printf("Member not found.\n"); return; }
    if (!b) { printf("Book not found.\n"); return; }
    if (b->copies <= 0) { printf("No copies available.\n"); return; }

    /* decrement copies */
    --b->copies;

    /* ← NEW: Set checkout time and due date (14 days) */
    long long now = (long long)time(NULL);
    long long due = now + (14LL * 24 * 3600);  // 14 days in seconds

    member_add_borrow(m, isbn);
    m->borrowed[m->borrowedCount - 1].checkoutTime = now;
    m->borrowed[m->borrowedCount - 1].dueTime = due;

    /* update recommendation graph */
    for (int i = 0; i < m->borrowedCount; ++i) {
        if (strcmp(m->borrowed[i].isbn, isbn) != 0)
            graph_add_edge(lib->recommendGraph, isbn, m->borrowed[i].isbn);
    }

    printf("Checked out %s to %s (copies left: %d)\n", isbn, mid, b->copies);
}

/* called automatically after every check-in */
void processReservation(Library *lib) {
    Reservation r;
    while (pq_pop(lib->reservationPQ, &r)) {
        Book *b = avl_search(lib->bookRoot, r.isbn);
        if (b && b->copies > 0) {
            /* give it to the member */
            MemberExt *m = hm_get(lib->memberMap, r.memberId);
            if (m) {
                --b->copies;
                member_add_borrow(m, r.isbn);
                printf("Reservation fulfilled: %s gets %s\n", r.memberId, r.isbn);
            }
            break;   /* only one copy became free */
        }
    }
}

void checkInBook(Library *lib) {
    char isbn[64];
    printf("ISBN to check-in: "); scanf("%s", isbn);
    Book *b = avl_search(lib->bookRoot, isbn);
    if (!b) { printf("Book not found.\n"); return; }

    /* find who has it (simple linear scan – ok for demo) */
   MemberExt *owner = NULL;
HashMap *map = lib->memberMap;
for (unsigned b = 0; b < map->size && !owner; ++b) {
    HashEntry *e = map->buckets[b];
    for (; e; e = e->next) {
        MemberExt *m = (MemberExt*)e->value;
        for (int i = 0; i < m->borrowedCount; ++i) {
            if (strcmp(m->borrowed[i].isbn, isbn) == 0) {
                owner = m;
                break;
            }
        }
        if (owner) break;
    }
}
    if (!owner) { printf("Book not checked out.\n"); return; }

    ++b->copies;
    /* assume 14-day loan, calculate overdue */
    int overdue = 0;   /* in real system use checkout timestamp */
    member_remove_borrow(owner, isbn, overdue);
    printf("Checked in %s (copies now: %d)\n", isbn, b->copies);

    /* after return → try to satisfy a reservation */
    processReservation(lib);
}

void reserveBook(Library *lib) {
    char mid[64], isbn[64];
    printf("Member ID: "); scanf("%s", mid);
    printf("ISBN: ");      scanf("%s", isbn);

    MemberExt *m = hm_get(lib->memberMap, mid);
    Book *b = avl_search(lib->bookRoot, isbn);
    if (!m) { printf("Member not found.\n"); return; }
    if (!b) { printf("Book not found.\n"); return; }
    if (b->copies > 0) { printf("Book available – just check out.\n"); return; }

    long long ts = (long long)time(NULL) * 1000LL;
    pq_push(lib->reservationPQ, ts, mid, isbn);
    printf("Reservation queued for %s (ISBN %s)\n", mid, isbn);
}

