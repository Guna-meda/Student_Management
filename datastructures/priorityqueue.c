#include "priorityqueue.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

PriorityQueue* pq_create(void) {
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    pq->size = 0;
    pq->cap = 0;
    pq->heap = NULL;
    return pq;
}

static void swap(Reservation *a, Reservation *b) {
    Reservation t = *a; *a = *b; *b = t;
}

static void sift_up(PriorityQueue *pq, int i) {
    while (i > 0) {
        int p = (i-1)/2;
        if (pq->heap[p].timestamp <= pq->heap[i].timestamp) break;
        swap(&pq->heap[p], &pq->heap[i]);
        i = p;
    }
}

static void sift_down(PriorityQueue *pq, int i) {
    int sz = pq->size;
    while (1) {
        int l = 2*i + 1, r = 2*i + 2, smallest = i;
        if (l < sz && pq->heap[l].timestamp < pq->heap[smallest].timestamp) smallest = l;
        if (r < sz && pq->heap[r].timestamp < pq->heap[smallest].timestamp) smallest = r;
        if (smallest == i) break;
        swap(&pq->heap[i], &pq->heap[smallest]);
        i = smallest;
    }
}

void pq_push(PriorityQueue *pq, long long ts, const char *mid, const char *isbn) {
    if (pq->size == pq->cap) {
        pq->cap = pq->cap ? pq->cap*2 : 8;
        pq->heap = realloc(pq->heap, pq->cap * sizeof(Reservation));
    }
    Reservation *r = &pq->heap[pq->size];
    r->timestamp = ts;
    strncpy(r->memberId, mid, sizeof(r->memberId)-1);
    strncpy(r->isbn, isbn, sizeof(r->isbn)-1);
    ++pq->size;
    sift_up(pq, pq->size-1);
}

int pq_pop(PriorityQueue *pq, Reservation *out) {
    if (pq->size == 0) return 0;
    *out = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];
    sift_down(pq, 0);
    return 1;
}

void pq_destroy(PriorityQueue *pq) {
    free(pq->heap);
    free(pq);
}