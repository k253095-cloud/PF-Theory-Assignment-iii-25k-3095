#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentID;
    char fullName[100];
    char batch[20];              // FIXED: removed stray "this one too"
    char membershipType[10];
    char regDate[11];
    char dob[11];
    char interest[10];
} Student;

typedef struct {
    Student *arr;
    int count;
    int capacity;
} StudentDB;

int initDB(StudentDB *db, int initialCapacity) {
    db->arr = malloc(initialCapacity * sizeof(Student));
    if (!db->arr) return 0;
    db->count = 0;
    db->capacity = initialCapacity;
    return 1;
}

int ensureDBCapacity(StudentDB *db) {
    if (db->count < db->capacity) return 1;
    int newCap = db->capacity * 2;
    Student *tmp = realloc(db->arr, newCap * sizeof(Student));
    if (!tmp) return 0;
    db->arr = tmp;
    db->capacity = newCap;
    return 1;
}

int findStudentIndex(const StudentDB *db, int studentID) {
    for (int i = 0; i < db->count; i++) {
        if (db->arr[i].studentID == studentID)
            return i;
    }
    return -1;
}

int loadDatabase(StudentDB *db, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 1;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (!ensureDBCapacity(db)) {
            fclose(fp);
            return 0;
        }
        db->arr[db->count++] = s;
    }

    fclose(fp);
    return 1;
}

int saveDatabase(const StudentDB *db, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return 0;
    }
    fwrite(db->arr, sizeof(Student), db->count, fp);
    fclose(fp);
    return 1;
}

int addStudent(StudentDB *db, Student s) {
    if (findStudentIndex(db, s.studentID) != -1) {
        printf("Student ID already exists.\n");
        return 0;
    }
    if (!ensureDBCapacity(db)) {
        printf("Memory error.\n");
        return 0;
    }
    db->arr[db->count++] = s;
    return 1;
}

int updateStudent(StudentDB *db, int studentID) {
    int idx = findStudentIndex(db, studentID);
    if (idx == -1) {
        printf("Student not found.\n");
        return 0;
    }

    printf("Enter new batch: ");
    scanf("%19s", db->arr[idx].batch);

    printf("Enter new membership type (IEEE/ACM): ");
    scanf("%9s", db->arr[idx].membershipType);

    return 1;
}

int deleteStudent(StudentDB *db, int studentID) {
    int idx = findStudentIndex(db, studentID);
    if (idx == -1) {
        printf("Student not found.\n");
        return 0;
    }

    db->arr[idx] = db->arr[db->count - 1];
    db->count--;
    return 1;
}

void displayAll(const StudentDB *db) {
    printf("\n---- All Students ----\n");
    for (int i = 0; i < db->count; i++) {
        Student s = db->arr[i];
        printf("%d | %s | %s | %s | %s | %s | %s\n",
               s.studentID, s.fullName, s.batch,
               s.membershipType, s.regDate, s.dob, s.interest);
    }
}

void batchReport(const StudentDB *db, const char *batchFilter, const char *interestFilter) {
    printf("\n---- Batch Report ----\n");
    for (int i = 0; i < db->count; i++) {
        Student s = db->arr[i];
        if (strcmp(s.batch, batchFilter) == 0 &&
            strcmp(s.interest, interestFilter) == 0) {
            printf("%d | %s | %s | %s\n",
                   s.studentID, s.fullName, s.batch, s.interest);
        }
    }
}

int main(void) {
    StudentDB db;
    if (!initDB(&db, 10)) {
        printf("Initial allocation failed.\n");
        return 1;
    }

    const char *filename = "members.dat";
    loadDatabase(&db, filename);

    int choice;

    while (1) {
        printf("\n------ Student Membership System ------\n");
        printf("1. Register new student\n");
        printf("2. Update student\n");
        printf("3. Delete student\n");
        printf("4. View all\n");
        printf("5. Batch-wise report\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) break;

        if (choice == 1) {
            Student s;
            printf("Enter Student ID: ");
            scanf("%d", &s.studentID);
            getchar();

            printf("Enter Full Name: ");
            fgets(s.fullName, sizeof(s.fullName), stdin);
            s.fullName[strcspn(s.fullName, "\n")] = '\0';

            printf("Enter Batch (CS/SE/Cyber Security/AI): ");
            fgets(s.batch, sizeof(s.batch), stdin);
            s.batch[strcspn(s.batch, "\n")] = '\0';

            printf("Enter Membership Type (IEEE/ACM): ");
            fgets(s.membershipType, sizeof(s.membershipType), stdin);
            s.membershipType[strcspn(s.membershipType, "\n")] = '\0';

            printf("Enter Registration Date (YYYY-MM-DD): ");
            scanf("%10s", s.regDate);

            printf("Enter Date of Birth (YYYY-MM-DD): ");
            scanf("%10s", s.dob);

            printf("Enter Interest (IEEE/ACM/Both): ");
            scanf("%9s", s.interest);

            if (addStudent(&db, s)) {
                saveDatabase(&db, filename);
            }
        }

        else if (choice == 2) {
            int id;
            printf("Enter Student ID to update: ");
            scanf("%d", &id);
            if (updateStudent(&db, id)) {
                saveDatabase(&db, filename);
            }
        }

        else if (choice == 3) {
            int id;
            printf("Enter Student ID to delete: ");
            scanf("%d", &id);
            if (deleteStudent(&db, id)) {
                saveDatabase(&db, filename);
            }
        }

        else if (choice == 4) {
            displayAll(&db);
        }

        else if (choice == 5) {
            char batch[20], interest[10];
            printf("Enter batch: ");
            scanf("%19s", batch);
            printf("Enter interest (IEEE/ACM/Both): ");
            scanf("%9s", interest);
            batchReport(&db, batch, interest);
        }

        else if (choice == 6) {
            break;
        }
    }

    saveDatabase(&db, filename);
    free(db.arr);
    return 0;
}
