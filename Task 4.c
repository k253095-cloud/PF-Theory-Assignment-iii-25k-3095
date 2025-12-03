#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    int id;
    int popularity;
    int lastAccessTime;
    int isUsed;
} Book;

// Find a book by ID
int findBookIndex(Book shelf[], int capacity, int id) {
    for (int i = 0; i < capacity; i++) {
        if (shelf[i].isUsed && shelf[i].id == id)
            return i;
    }
    return -1;
}

// Find first empty slot
int findEmptySlot(Book shelf[], int capacity) {
    for (int i = 0; i < capacity; i++) {
        if (!shelf[i].isUsed)
            return i;
    }
    return -1;
}

// Find least recently used (LRU) book
int findLeastRecentlyUsedIndex(Book shelf[], int capacity) {
    int idx = -1;
    int minTime = INT_MAX;

    for (int i = 0; i < capacity; i++) {
        if (shelf[i].isUsed && shelf[i].lastAccessTime < minTime) {
            minTime = shelf[i].lastAccessTime;
            idx = i;
        }
    }
    return idx;
}

// Handle ADD operation
void handleAdd(Book shelf[], int capacity, int *currentTime, int id, int popularity) {
    (*currentTime)++;

    int idx = findBookIndex(shelf, capacity, id);
    if (idx != -1) {
        shelf[idx].popularity = popularity;
        shelf[idx].lastAccessTime = *currentTime;
        return;
    }

    int empty = findEmptySlot(shelf, capacity);
    if (empty == -1) {
        empty = findLeastRecentlyUsedIndex(shelf, capacity);
    }

    shelf[empty].id = id;
    shelf[empty].popularity = popularity;
    shelf[empty].lastAccessTime = *currentTime;
    shelf[empty].isUsed = 1;
}

// Handle ACCESS operation
void handleAccess(Book shelf[], int capacity, int *currentTime, int id) {
    (*currentTime)++;

    int idx = findBookIndex(shelf, capacity, id);
    if (idx == -1) {
        printf("-1\n");
    } else {
        shelf[idx].lastAccessTime = *currentTime;
        printf("Popularity: %d\n", shelf[idx].popularity);
    }
}

int main(void) {
    int capacity, Q;

    // PROMPT FOR INITIAL INPUT
    printf("Enter shelf capacity and number of operations: ");
    if (scanf("%d %d", &capacity, &Q) != 2) {
        printf("Invalid input.\n");
        return 1;
    }

    Book *shelf = malloc(capacity * sizeof(Book));
    if (!shelf) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < capacity; i++) {
        shelf[i].isUsed = 0;
    }

    int currentTime = 0;
    char op[10];
    int x, y;

    printf("\n--- Enter operations now (ADD or ACCESS) ---\n");
    printf("Examples:\n");
    printf("  ADD 10 5\n");
    printf("  ACCESS 10\n");
    printf("-------------------------------------------\n\n");

    for (int i = 0; i < Q; i++) {

        printf("Operation %d: ", i + 1);

        if (scanf("%9s", op) != 1)
            break;

        if (strcmp(op, "ADD") == 0) {
            printf("Enter Book ID and Popularity: ");
            if (scanf("%d %d", &x, &y) != 2)
                break;
            handleAdd(shelf, capacity, &currentTime, x, y);
        } 
        else if (strcmp(op, "ACCESS") == 0) {
            printf("Enter Book ID: ");
            if (scanf("%d", &x) != 1)
                break;
            handleAccess(shelf, capacity, &currentTime, x);
        } 
        else {
            printf("Invalid operation.\n");
        }
    }

    free(shelf);
    return 0;
}
