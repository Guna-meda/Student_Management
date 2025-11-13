#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "datastructures/avl.h"

/* helper to create a Book* */
static Book* create_book(const char* isbn, const char* title, const char* author, int copies) {
    Book* b = (Book*)malloc(sizeof(Book));
    if (!b) return NULL;
    strncpy(b->isbn, isbn, sizeof(b->isbn)-1); b->isbn[sizeof(b->isbn)-1] = '\0';
    strncpy(b->title, title, sizeof(b->title)-1); b->title[sizeof(b->title)-1] = '\0';
    strncpy(b->author, author, sizeof(b->author)-1); b->author[sizeof(b->author)-1] = '\0';
    b->copies = copies;
    return b;
}

void addBook(Library *lib) {
    char isbn[64], title[128], author[64];
    int copies;
    printf("Enter ISBN: "); scanf("%s", isbn);
    printf("Enter Title (no spaces please or use underscores): "); scanf("%s", title);
    printf("Enter Author (no spaces please or use underscores): "); scanf("%s", author);
    printf("Enter copies: "); scanf("%d", &copies);

    /* check if exists */
    Book *existing = (Book*)avl_search(lib->bookRoot, isbn);
    if (existing) {
        printf("Book with ISBN %s already exists. Updating copies.\n", isbn);
        existing->copies += copies;
        return;
    }

    Book *b = create_book(isbn, title, author, copies);
    if (!b) {
        printf("Memory error\n");
        return;
    }

    lib->bookRoot = avl_insert(lib->bookRoot, isbn, (void*)b);
    printf("Book added: %s (%s) copies=%d\n", title, isbn, copies);
}

void removeBook(Library *lib) {
    char isbn[64];
    printf("Enter ISBN to remove: "); scanf("%s", isbn);
    Book *b = (Book*)avl_search(lib->bookRoot, isbn);
    if (!b) {
        printf("No book with ISBN %s found.\n", isbn);
        return;
    }
    /* Optionally free Book* payload here (avl_delete currently frees node but not payload) */
    lib->bookRoot = avl_delete(lib->bookRoot, isbn);
    free(b);
    printf("Book %s removed.\n", isbn);
}

void searchBook(Library *lib) {
    char isbn[64];
    printf("Enter ISBN to search: "); scanf("%s", isbn);
    Book *b = (Book*)avl_search(lib->bookRoot, isbn);
    if (!b) {
        printf("Book not found.\n");
    } else {
        printf("Found: %s | Author: %s | Copies: %d\n", b->title, b->author, b->copies);
    }
}

void updateInventory(Library *lib) {
    char isbn[64];
    int newCopies;
    printf("Enter ISBN to update: "); scanf("%s", isbn);
    printf("Enter new copies count: "); scanf("%d", &newCopies);
    Book *b = (Book*)avl_search(lib->bookRoot, isbn);
    if (!b) {
        printf("Book not found.\n");
    } else {
        b->copies = newCopies;
        printf("Updated copies for %s to %d\n", isbn, newCopies);
    }
}
