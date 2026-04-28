#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Task {
    int id;
    char description[100];
    int isDone;
    struct Task* next;
};

struct Task* head = NULL;
int count = 0;

void addTask() {
    struct Task* newTask = (struct Task*)malloc(sizeof(struct Task));
    newTask->id = ++count;

    printf("Enter task description: ");
    getchar(); 
    fgets(newTask->description, 100, stdin);

    newTask->description[strcspn(newTask->description, "\n")] = 0;
    newTask->isDone = 0;
    newTask->next = NULL;

    if (head == NULL) {
        head = newTask;
    } else {
        struct Task* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newTask;
    }

    printf("Task added!\n");
}

void viewTasks() {
    struct Task* temp = head;

    if (temp == NULL) {
        printf("No tasks available.\n");
        return;
    }

    while (temp != NULL) {
        printf("ID: %d | %s | %s\n",temp->id,temp->description,temp->isDone?"Done":"Pending");         
        temp = temp->next;
    }
}

void markDone() {
    int id;
    printf("Enter task ID to mark as done: ");
    scanf("%d", &id);

    struct Task* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            temp->isDone = 1;
            printf("Task marked as done!\n");
            return;
        }
        temp = temp->next;
    }
    printf("Task not found.\n");
}

void deleteTask() {
    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);

    struct Task *temp = head, *prev = NULL;

    if (temp != NULL && temp->id == id) {
        head = temp->next;
        free(temp);
        printf("Task deleted!\n");
        return;
    }
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Task not found.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Task deleted!\n");
}

void saveToFile() {

    FILE* file = fopen("tasks.txt", "w");
    struct Task* temp = head;

    while (temp != NULL) {
        fprintf(file, "%d|%s|%d\n",
                temp->id,
                temp->description,
                temp->isDone);
        temp = temp->next;
    }

    fclose(file);
    printf("Tasks saved to file.\n");
}

void loadFromFile() {
    FILE* file = fopen("tasks.txt", "r");
    if (file == NULL) return;

    while (!feof(file)) {
        struct Task* newTask = (struct Task*)malloc(sizeof(struct Task));

        if (fscanf(file, "%d|%99[^|]|%d\n",
                   &newTask->id,
                   newTask->description,
                   &newTask->isDone) != 3) {
            free(newTask);
            break;
        }

        newTask->next = NULL;

        if (head == NULL) {
            head = newTask;
        } else {
            struct Task* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newTask;
        }

        if (newTask->id > count)
            count = newTask->id;
    }

    fclose(file);
}

int main() {
    int choice;

    loadFromFile();

    while (1) {
        printf("\n--- TO-DO LIST ---\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Done\n");
        printf("4. Delete Task\n");
        printf("5. Save & Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); 
            break;
            case 2: viewTasks(); 
            break;
            case 3: markDone(); 
            break;
            case 4: deleteTask(); 
            break;
            case 5:
                saveToFile();
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}



