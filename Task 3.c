#include <stdio.h>
#include <string.h>

// Structure to store employee information
struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};

// Function to display all employee records
void displayEmployees(struct Employee emp[], int n) {
    printf("\nAll Employee Records:\n");
    printf("ID\tName\t\tDesignation\tSalary\n");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n", emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
    }
}

// Function to find and display the employee with highest salary
void findHighestSalary(struct Employee emp[], int n) {
    int maxIndex = 0;
    for (int i = 1; i < n; i++) {
        if (emp[i].salary > emp[maxIndex].salary) {
            maxIndex = i;
        }
    }
    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
           emp[maxIndex].id, emp[maxIndex].name, emp[maxIndex].designation, emp[maxIndex].salary);
}

// Function to search employee by ID or Name
void searchEmployee(struct Employee emp[], int n) {
    int choice;
    printf("\nSearch Employee by:\n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int searchID;
        printf("Enter Employee ID: ");
        scanf("%d", &searchID);
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (emp[i].id == searchID) {
                printf("\nEmployee Found:\nID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Employee with ID %d not found.\n", searchID);
        }
    } else if (choice == 2) {
        char searchName[50];
        printf("Enter Employee Name: ");
        scanf("%s", searchName);
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(emp[i].name, searchName) == 0) {
                printf("\nEmployee Found:\nID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Employee with Name '%s' not found.\n", searchName);
        }
    } else {
        printf("Invalid choice.\n");
    }
}

// Function to give 10% bonus to employees below threshold
void giveBonus(struct Employee emp[], int n, float threshold) {
    for (int i = 0; i < n; i++) {
        if (emp[i].salary < threshold) {
            emp[i].salary *= 1.10; // increase by 10%
        }
    }
    printf("\nBonus applied to employees with salary below %.2f\n", threshold);
}

int main() {
    int n;
    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee emp[n];

    // Input employee details
    for (int i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d\n", i + 1);
        printf("ID: ");
        scanf("%d", &emp[i].id);
        printf("Name: ");
        scanf("%s", emp[i].name);
        printf("Designation: ");
        scanf("%s", emp[i].designation);
        printf("Salary: ");
        scanf("%f", &emp[i].salary);
    }

    displayEmployees(emp, n);
    findHighestSalary(emp, n);
    searchEmployee(emp, n);

    // Optional: apply bonus
    float threshold = 50000;
    giveBonus(emp, n, threshold);

    displayEmployees(emp, n);

    return 0;
}

