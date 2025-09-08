#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patient {
    char name[50];
    int age;
    char type[20];
    int time_needed;
    struct Patient *next;
} Patient;

typedef struct Queue {
    Patient *front, *rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

Patient* newPatient(char *name, int age, char *type) {
    Patient *p = (Patient*)malloc(sizeof(Patient));
    strcpy(p->name, name);
    p->age = age;
    strcpy(p->type, type);
    p->time_needed = strcmp(type, "emergency") == 0 ? 30 : 15;
    p->next = NULL;
    return p;
}

void addPatient(Queue *q, Patient *p) {
    if (q->rear == NULL) {
        q->front = q->rear = p;
    } else {
        q->rear->next = p;
        q->rear = p;
    }
    printf("Added %s to %s queue.\n", p->name, p->type);
}

Patient* removePatient(Queue *q) {
    if (q->front == NULL) return NULL;
    Patient *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    return temp;
}

void showQueues(Queue *emergency, Queue *regular) {
    printf("\n--- Queue Status ---\n");
    int wait = 0;
    Patient *temp;

    temp = emergency->front;
    while (temp) {
        printf("%s (Emergency, %d yrs) - wait: %d mins\n", temp->name, temp->age, wait);
        wait += temp->time_needed;
        temp = temp->next;
    }

    temp = regular->front;
    while (temp) {
        printf("%s (Regular, %d yrs) - wait: %d mins\n", temp->name, temp->age, wait);
        wait += temp->time_needed;
        temp = temp->next;
    }

    if (!emergency->front && !regular->front)
        printf("No patients in queue.\n");
}

void serveNext(Queue *emergency, Queue *regular) {
    Patient *p = NULL;
    if (emergency->front)
        p = removePatient(emergency);
    else if (regular->front)
        p = removePatient(regular);

    if (p) {
        printf("Now serving: %s (%s)\n", p->name, p->type);
        free(p);
    } else {
        printf("No patients to serve.\n");
    }
}

int main() {
    Queue emergency, regular;
    initQueue(&emergency);
    initQueue(&regular);

    int choice, age;
    char name[50], type[20];

    while (1) {
        printf("\nHospital Queue System\n");
        printf("1. Add Patient\n");
        printf("2. Serve Patient\n");
        printf("3. Show Queue\n");
        printf("4. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Name: "); scanf("%s", name);
            printf("Age: "); scanf("%d", &age);
            printf("Type (emergency/regular): "); scanf("%s", type);

            if (strcmp(type, "emergency") == 0)
                addPatient(&emergency, newPatient(name, age, type));
            else
                addPatient(&regular, newPatient(name, age, type));

        } else if (choice == 2) {
            serveNext(&emergency, &regular);

        } else if (choice == 3) {
            showQueues(&emergency, &regular);

        } else if (choice == 4) {
            printf("Goodbye!\n");
            break;

        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
