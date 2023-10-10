#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int contains_string(char input1[], char input2[]);

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
    if (todo != NULL) {
        free(todo->message);
        free(todo);
    }
}

int is_todo_done(struct TodoField *todo) {
    return todo->is_done;
}

int is_todo_not_done(struct TodoField *todo) {
    return !todo->is_done;
}

typedef int (*is_done)(struct TodoField *todo);

void print_todo_fields(struct TodoField *todo_fields[]) {
    int i = 0;
    while (todo_fields[i] != NULL) {
        if (!todo_fields[i]->is_done) {
            printf("%s -> priority %d \n", todo_fields[i]->message, todo_fields[i]->priority);
        }
        i++;
    }
} 

int process_input(char input[], int input_size) {
    if (contains_string(input, "q")) {
        return 1;
    }    
    return 0;
}

int contains_string(char *input1, char *input2) {
    int index = 0;
    int contains = 0;
    while(input1[index] != '\0' && input2[index] != '\0') {
        if (input1[index] == input2[index]) {
            contains = 1;
        } else {
            contains = 0;
            break;
        }
        index++;
    }
    return contains;
}

int main() {
    int todo_index = 0;
    struct TodoField *todo_fields[50] = {
        todo_create("pick up food", 0),
        todo_create("go home", 1),
        todo_create("make food", 2),
        todo_create("eat", 3),
        NULL
    };
    todo_index = 4;

    print_todo_fields(todo_fields);

    while(1) {
        char input[100] = "";
        fgets(input, 100-1, stdin);
        int result = process_input(input, 100);
        if (result == 1) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        todo_fields[todo_index] = todo_create(input, todo_index);
        print_todo_fields(todo_fields);
        todo_index++;
    }

    // cleanup
    int i = 0;
    for (i = 0; i < sizeof(todo_fields) / sizeof(char *); i++) {
        todo_destroy(todo_fields[i]);
    };
    return 0;
}
