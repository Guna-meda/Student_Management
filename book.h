#ifndef BOOK_H
#define BOOK_H

#include "utils.h"

typedef struct {
    char isbn[20];
    char title[100];
    char author[50];
    int copies;
} Book;


void addBook(Library *lib);
void removeBook(Library *lib);
void searchBook(Library *lib);
void updateInventory(Library *lib);

#endif
