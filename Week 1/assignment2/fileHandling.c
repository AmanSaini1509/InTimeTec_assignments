#include <stdio.h>
#include <stdlib.h>
struct employee { // structure for employee record
    int id;
    char name[15];
    int age;
};

int getId() { // function to get new auto incremented ID
    FILE *file;
    struct employee emp;
    int lastId = 0;
    file = fopen("users.txt", "r");
    if (file == NULL) return -1;
    while (fscanf(file, "%d %*s %*d", &emp.id) != EOF) { // ignore the rest of data other than ID
        lastId = emp.id;
    }
    fclose(file);
    return lastId + 1;
}

void create_user() { // function for creating new user
    FILE *file;
    struct employee emp;
    file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("File could not be opened\n");
        exit(1);
    }
    emp.id = getId();
    if (emp.id == -1) {
        printf("something went wrong\n");
        exit(2);
    }
    printf("Enter first name: ");
    scanf("%s", emp.name);
    printf("Enter age: ");
    scanf("%d", &emp.age);

    fprintf(file, "%d %s %d\n", emp.id, emp.name, emp.age);
    fclose(file);

    printf("User created successfully\n");
}

void read_data() { // function for read the record
    FILE *file;
    struct employee emp;
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("File doesn't EXIST\n");
        exit(1);
    }
    while (fscanf(file, "%d %s %d", &emp.id, emp.name, &emp.age) != EOF) {
        printf("ID: %d Name: %s Age: %d\n", emp.id, emp.name, emp.age);
    }
    fclose(file);
}

void update_user(int id) { // function for updating the user record by ID
    FILE *file;
    struct employee emp;
    FILE *temp;
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("File can not be opened or found\n");
        exit(1);
    }
    temp = fopen("temp.txt", "w"); // creating new temporary file for re-writing the update record
    if (temp == NULL) {
        printf("Some error occurred\n");
        exit(2);
    }
    while (fscanf(file, "%d %s %d", &emp.id, emp.name, &emp.age) != EOF) {
        if (emp.id == id) { // update the record in temporary file when found the ID
            printf("Enter new first name: ");
            scanf("%s", emp.name);
            printf("Enter new age: ");
            scanf("%d", &emp.age);
            fprintf(temp, "%d %s %d\n", emp.id, emp.name, emp.age);
        }
        else { // copying the rest of data except
            fprintf(temp, "%d %s %d\n", emp.id, emp.name, emp.age);
        }
    }
    fclose(file);
    fclose(temp);

    remove("users.txt"); // removing the previous file
    rename("temp.txt", "users.txt"); // renaming the temporary file to users.txt

    printf("User updated successfully\n");
}

void delete_user(int id) { // function for deleting the user by ID
    FILE *file;
    struct employee emp;
    FILE *temp;
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("File can not be opened or found\n");
        exit(1);
    }
    temp = fopen("temp.txt", "w"); //similar to update_user using temporary file
    if (temp == NULL) {
        printf("Some error occurred\n");
        exit(2);
    }
    while (fscanf(file, "%d %s %d", &emp.id, emp.name, &emp.age) != EOF) {
        if (emp.id != id) { // copy all data from users.txt to temp.txt except the record which is required to delete
            fprintf(temp, "%d %s %d\n", emp.id, emp.name, emp.age);
        }
    }
    fclose(file);
    fclose(temp);

    remove("users.txt"); // removing the previous record file
    rename("temp.txt", "users.txt"); // renaming the temp.txt to users.txt
    printf("User deleted successfully\n");
}

int main() {
    int choice, id;
    char ch = 'y';
    do {
        printf("1. Create new user.\n2. Read the all data.\n3. Update the user by ID.\n4. Delete the user by ID.\n5. Exit.\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                create_user();
                break;
            case 2:
                read_data();
                break;
            case 3:
                printf("Enter ID: ");
                scanf("%d", &id);
                update_user(id);
                break;
            case 4:
                printf("Enter ID: ");
                scanf("%d", &id);
                delete_user(id);
                break;
            case 5:
                ch = 'n';
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
    while (ch == 'y');
}