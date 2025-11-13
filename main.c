#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "book.h"
#include "member.h"
#include "reservation.h"
#include "search.h"     // ← For autoComplete & recommendBooks
#include "report.h"

int main() {
    Library lib;
    initLibrary(&lib);

    int choice;
    while (1) {
        printf("\n=== Library Management System ===\n");
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. Search Book\n");
        printf("4. Register Member\n");
        printf("5. Check Out Book\n");
        printf("6. Check In Book\n");
        printf("7. Reserve Book\n");
        printf("8. Calculate Fine\n");
        printf("9. Generate Report\n");
        printf("10. Recommend Books\n");
        printf("11. Auto Complete Title\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);  // Clear invalid input
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:  addBook(&lib); break;
            case 2:  removeBook(&lib); break;
            case 3:  searchBook(&lib); break;
            case 4:  registerMember(&lib); break;
            case 5:  checkOutBook(&lib); break;
            case 6:  checkInBook(&lib); break;
            case 7:  reserveBook(&lib); break;
            case 8:  calculateFine(&lib); break;
            case 9:  generateReport(&lib); break;
            case 10: recommendBooks(&lib); break;
            case 11: autoComplete(&lib); break;
            case 0:
                printf("Thank you for using the Library System. Goodbye!\n");
                // Optional: destroyLibrary(&lib);  // Uncomment if you add cleanup
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}