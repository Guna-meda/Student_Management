#ifndef BOOK_H
#define BOOK_H

typedef struct {
    char isbn[20];
    char title[100];
    char author[50];
    int copies;
} Book;

void addBook();
void removeBook();
void searchBook();
void updateInventory();
void loadBooks();
void saveBooks();

#endif
