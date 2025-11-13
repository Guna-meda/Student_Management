#include <stdio.h>
#include "utils.h"
#include "book.h"
#include "member.h"
#include "datastructures/priorityqueue.h"

static void print_book(AVLNode *n) {
    Book *b = (Book*)n->data;
    printf("%s | %s | %s | %d copies\n", b->isbn, b->title, b->author, b->copies);
}

static void member_report(void *val) {
    MemberExt *m = val;
    printf("%s | %s | %s | borrowed:%d\n",
           m->info.memberId, m->info.name, m->info.email, m->borrowedCount);
}

void generateReport(Library *lib) {
    int type;
    printf("Report type (0=Inventory, 1=Members, 2=Reservations): ");
    scanf("%d", &type);
    switch (type) {
        case 0:
            printf("=== BOOK INVENTORY ===\n");
            avl_inorder(lib->bookRoot, print_book);
            break;
        case 1:
            printf("=== MEMBERS ===\n");
            hm_foreach(lib->memberMap, member_report);
            break;
        case 2:
            printf("=== RESERVATION QUEUE (size %d) ===\n", lib->reservationPQ->size);
            for (int i = 0; i < lib->reservationPQ->size; ++i) {
                Reservation *r = &lib->reservationPQ->heap[i];
                printf("%s wants %s (ts=%lld)\n", r->memberId, r->isbn, r->timestamp);
            }
            break;
        default:
            printf("Unknown report type.\n");
    }
}