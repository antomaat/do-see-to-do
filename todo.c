#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct TodoField {
    char *message;
    int priority;
    int is_done;
};

struct TodoField *todo_create(char *message, int priority) {
    struct TodoField *todo = malloc(sizeof(struct TodoField));
    assert(todo != NULL);

    todo->message = strdup(message);
    todo->priority = priority;
    todo->is_done = 0;
    return todo;
}

void todo_destroy(struct TodoField *todo) {
    assert(todo != NULL);
    free(todo->message);
    free(todo);
}

int main() {
    struct TodoField *todo_fields[] = {
        todo_create("pick up food", 0),
        todo_create("go home", 1),
        todo_create("make food", 2),
        todo_create("eat", 3),
    };
    int i = 0;
    for (i = 0; i < sizeof(todo_fields) / sizeof(char *); i++) {
        printf("%s -> priority %d \n", todo_fields[i]->message, todo_fields[i]->priority);
    };
    for (i = 0; i < sizeof(todo_fields) / sizeof(char *); i++) {
        todo_destroy(todo_fields[i]);
    };
    return 0;
}
