#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

typedef struct {
    long long timestamp;   /* epoch ms – smaller = older */
    char memberId[20];
    char isbn[20];
} Reservation;

typedef struct {
    Reservation *heap;
    int size;
    int cap;
} PriorityQueue;

PriorityQueue* pq_create(void);
void pq_push(PriorityQueue *pq, long long ts, const char *mid, const char *isbn);
int  pq_pop(PriorityQueue *pq, Reservation *out);   /* returns 0 if empty */
void pq_destroy(PriorityQueue *pq);

#endif