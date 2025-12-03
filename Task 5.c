#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **lines;
    int count;
    int capacity;
} TextBuffer;

// Initialize dynamic buffer
int initBuffer(TextBuffer *buf, int initialCapacity) {
    buf->lines = malloc(initialCapacity * sizeof(char *));
    if (!buf->lines) return 0;
    buf->count = 0;
    buf->capacity = initialCapacity;
    return 1;
}

// Expand capacity when needed
int ensureCapacity(TextBuffer *buf) {
    if (buf->count < buf->capacity) return 1;

    int newCap = buf->capacity * 2;
    char **tmp = realloc(buf->lines, newCap * sizeof(char *));
    if (!tmp) return 0;

    buf->lines = tmp;
    buf->capacity = newCap;
    return 1;
}

// Insert a line at any index
int insertLine(TextBuffer *buf, int index, const char *text) {
    if (index < 0 || index > buf->count) {
        printf("Error: Invalid index.\n");
        return 0;
    }

    if (!ensureCapacity(buf)) {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }

    char *line = malloc(strlen(text) + 1);
    if (!line) {
        printf("Error: Memory allocation for line failed.\n");
        return 0;
    }
    strcpy(line, text);

    memmove(&buf->lines[index + 1], &buf->lines[index],
            (buf->count - index) * sizeof(char *));

    buf->lines[index] = line;
    buf->count++;
    return 1;
}

// Delete a line
int deleteLine(TextBuffer *buf, int index) {
    if (index < 0 || index >= buf->count) {
        printf("Error: Invalid index.\n");
        return 0;
    }

    free(buf->lines[index]);

    memmove(&buf->lines[index], &buf->lines[index + 1],
            (buf->count - index - 1) * sizeof(char *));

    buf->count--;
    return 1;
}

// Print all lines
void printAllLines(const TextBuffer *buf) {
    if (buf->count == 0) {
        printf("[Empty Document]\n");
        return;
    }

    printf("\n--- Current Lines ---\n");
    for (int i = 0; i < buf->count; i++) {
        printf("%d: %s\n", i, buf->lines[i]);
    }
    printf("----------------------\n");
}

// Shrink allocation to current size
void shrinkToFit(TextBuffer *buf) {
    char **tmp = realloc(buf->lines, buf->count * sizeof(char *));
    if (!tmp) {
        printf("Warning: Shrink failed. Keeping original size.\n");
        return;
    }

    buf->lines = tmp;
    buf->capacity = buf->count;
    printf("Buffer shrunk to fit exactly %d lines.\n", buf->count);
}

// Free everything
void freeAll(TextBuffer *buf) {
    for (int i = 0; i < buf->count; i++) {
        free(buf->lines[i]);
    }
    free(buf->lines);
    buf->lines = NULL;
    buf->count = buf->capacity = 0;
}

// Save to file
int saveToFile(const TextBuffer *buf, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file for writing.\n");
        return 0;
    }

    for (int i = 0; i < buf->count; i++) {
        fprintf(fp, "%s\n", buf->lines[i]);
    }

    fclose(fp);
    printf("Saved to file: %s\n", filename);
    return 1;
}

// Load from file
int loadFromFile(TextBuffer *buf, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file for reading.\n");
        return 0;
    }

    // clear old content
    for (int i = 0; i < buf->count; i++)
        free(buf->lines[i]);
    buf->count = 0;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        insertLine(buf, buf->count, buffer);
    }

    fclose(fp);
    printf("Loaded file: %s\n", filename);
    return 1;
}

// Read a line safely
void readLineStdin(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int main(void) {
    TextBuffer buf;
    if (!initBuffer(&buf, 4)) {
        printf("Error: Initial allocation failed.\n");
        return 1;
    }

    int choice;
    char temp[1024];

    while (1) {
        printf("\n========= TEXT EDITOR MENU =========\n");
        printf("1. Insert line at end\n");
        printf("2. Insert line at specific index\n");
        printf("3. Delete line\n");
        printf("4. Print all lines\n");
        printf("5. Save to file\n");
        printf("6. Load from file\n");
        printf("7. Shrink memory\n");
        printf("8. Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) break;
        getchar(); // remove newline

        if (choice == 1) {
            printf("Enter text: ");
            readLineStdin(temp, sizeof(temp));
            insertLine(&buf, buf.count, temp);
        }
        else if (choice == 2) {
            int index;
            printf("Enter index: ");
            scanf("%d", &index);
            getchar();
            printf("Enter text: ");
            readLineStdin(temp, sizeof(temp));
            insertLine(&buf, index, temp);
        }
        else if (choice == 3) {
            int index;
            printf("Enter index to delete: ");
            scanf("%d", &index);
            getchar();
            deleteLine(&buf, index);
        }
        else if (choice == 4) {
            printAllLines(&buf);
        }
        else if (choice == 5) {
            printf("Enter filename to save: ");
            readLineStdin(temp, sizeof(temp));
            saveToFile(&buf, temp);
        }
        else if (choice == 6) {
            printf("Enter filename to load: ");
            readLineStdin(temp, sizeof(temp));
            loadFromFile(&buf, temp);
        }
        else if (choice == 7) {
            shrinkToFit(&buf);
        }
        else if (choice == 8) {
            printf("Exiting program...\n");
            break;
        }
        else {
            printf("Invalid choice. Try again.\n");
        }
    }

    freeAll(&buf);
    return 0;
}
