#include <stdio.h>

int main() {
    char *todo_items[] = {"pick up food", "go home", "make food", "eat"};
    int i = 0;
    printf("char size %ld \n", sizeof(char *));
    printf("todo items size in bytes %ld \n", sizeof(todo_items));
    printf("todo items size %ld \n", sizeof(todo_items) / sizeof(char *));
    for (i = 0; i < sizeof(todo_items) / sizeof(char *); i++) {
        printf("%s \n", todo_items[i]);
    };
    return 0;
}
