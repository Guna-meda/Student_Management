#ifndef MEMBER_H
#define MEMBER_H

typedef struct {
    char memberId[20];
    char name[50];
    int borrowedCount;
} Member;

void registerMember();
void calculateFine();
void displayMemberDetails();

#endif
