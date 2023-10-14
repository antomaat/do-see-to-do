#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int contains_string(char input1[], char input2[]);

struct TodoField {
    char *message;
    int priority;
    int is_done;
    int is_deleted;
};

struct Command {
    char *command;
    int command_nr;
    char *message;
};

struct TodoField *todo_create(char *message, int priority) {
    struct TodoField *todo = malloc(sizeof(struct TodoField));
    assert(todo != NULL);

    todo->message = strdup(message);
    todo->priority = priority;
    todo->is_done = 0;
    return todo;
}

enum COMMANDS {
    NONE = 0,
    QUIT = 1,
    UP = 2,
    DOWN = 3,
    INTERACT = 4
};

struct termios term;


void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &term);
    atexit(disableRawMode);

    struct termios raw = term;
    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void todo_destroy(struct TodoField *todo) {
    if (todo != NULL) {
        free(todo->message);
        free(todo);
    }
}

void remove_from_array(struct TodoField *todoFields[], int removeIndex) {
    todo_destroy(todoFields[removeIndex]);
    int index = removeIndex + 1;
    while (todoFields[index] != 0 || todoFields[index] != NULL) {
        todoFields[index-1] = todoFields[index];
        index++;
    }
    todoFields[index-1] = NULL;
}


int is_todo_done(struct TodoField *todo) {
    return todo->is_done;
}

int is_todo_not_done(struct TodoField *todo) {
    return !todo->is_done;
}

typedef int (*is_done)(struct TodoField *todo);

void print_todo_fields(struct TodoField *todo_fields[], int selected) {
    int i = 0;
    while (todo_fields[i] != NULL) {
        if (!todo_fields[i]->is_deleted) {
            if (i == selected) {
                printf("\033[94m ");
            }
            if (todo_fields[i]->is_done) {
                printf("\x1b[9m");
            }
            printf("%s -> priority %d", todo_fields[i]->message, todo_fields[i]->priority);
            if (i == selected) {
                printf("\033[0m");
            }
            if (todo_fields[i]->is_done) {
                printf("\x1b[29m");
            }
            printf("\n");
        }
        i++;
    }
} 

int translate_command_type(char command[]) {
    if (contains_string(command, ":q")) {
        return QUIT;
    } 
    else if (contains_string(command, ":u")) {
        return UP;
    }
    else if (contains_string(command, ":d")) {
        return DOWN;
    }
    else if (contains_string(command, ":i")) {
        return INTERACT;
    }
    else {
        return NONE;
    }
}

struct Command process_input(char input[], int input_size) {
    struct Command command = {};
    if (contains_string(input, ":")) {
        int index = 0;
        char token[50]; 
        int token_index = 0;
        int token_part_index = 0;

        while (input[index] != '\0') {
            if (input[index] == ' ') {
                if (token_part_index == 0) {
                    command.command = token;
                    command.command_nr = translate_command_type(token);
                }
                if (token_part_index == 1) {
                    command.message = token;
                }
                memset(token, '\0', 50);
                token_part_index++;
                token_index = 0;
            } else {
                token[token_index] = input[index];
                token_index++;
            }
            index++;
        }
        if (token_index > 0) {
            if (token_part_index == 0) {
                command.command = token;
                command.command_nr = translate_command_type(token);
            }
            if (token_part_index == 1) {
                command.message = token;
            }
            memset(token, '\0', 50);
            token_part_index++;
            token_index = 0;
        }
    }
    return command;
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
    printf("\e[1;1H\e[2J");

    int selected = 0;
    print_todo_fields(todo_fields, selected);

    while(1) {
        char input[100] = "";
        fgets(input, 100-1, stdin);
        struct Command result = process_input(input, 100);
        if (result.command_nr == QUIT) {
            break;
        }
        else if (result.command_nr == UP) {
            selected--;
            if (selected < 0) {
                selected = 0;
            }
        }
        else if (result.command_nr == DOWN) {
            selected++;
        } 
        else if (result.command_nr == INTERACT) {
            printf("\e[1;1H\e[2J");
            printf("interactive mode\n");
            print_todo_fields(todo_fields, selected);
            enableRawMode();
            char in;
            while (read(STDIN_FILENO, &in, 1) == 1 && in != 'q') {
                if (in == 'k') {
                    printf("\e[1;1H\e[2J");
                    printf("interactive mode\n");
                    selected--;
                    if (selected < 0) {
                        selected = 0;
                    }
                    print_todo_fields(todo_fields, selected);
                }
                if (in == 'j') {
                    printf("\e[1;1H\e[2J");
                    printf("interactive mode\n");
                    selected++;
                    print_todo_fields(todo_fields, selected);
                }
                if (in == 'd') {
                    printf("\e[1;1H\e[2J");
                    printf("interactive mode\n");
                    todo_fields[selected]->is_done = !todo_fields[selected]->is_done; 
                    print_todo_fields(todo_fields, selected);
                }
                if (in == 'x') {
                    printf("\e[1;1H\e[2J");
                    printf("interactive mode\n");
                    remove_from_array(todo_fields, selected);
                    print_todo_fields(todo_fields, selected);
                }
            }
            disableRawMode();
        } 
        else {
            input[strcspn(input, "\n")] = 0;
            todo_fields[todo_index] = todo_create(input, todo_index);
            todo_index++;
        }
        printf("\e[1;1H\e[2J");
        print_todo_fields(todo_fields, selected);
    }

    // cleanup
    int i = 0;
    for (i = 0; i < sizeof(todo_fields) / sizeof(char *); i++) {
        todo_destroy(todo_fields[i]);
    };
    return 0;
}
