#ifndef RESERVATION_H
#define RESERVATION_H

#include "utils.h"

void checkOutBook(Library *lib);
void checkInBook(Library *lib);
void reserveBook(Library *lib);
void processReservation(Library *lib);   /* internal – called by checkIn */

#endif