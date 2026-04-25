#include "postfix.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef long Item;
#include "Stack.h"

long evaluatePostfix(const char *expr) {
    if (expr == NULL)
        return LONG_MIN;

    // TODO: evalueaza expresia postfixata folosind o stiva (Stack.h).
    Stack *stack = createStack();
    //char *elem = strtok(expr, " ");
    //const char *expr nu se poate modifica, deci trebuie copiat in alta variabila

    char *copie = (char *)malloc(strlen(expr) * sizeof(char) + 1);
    if (copie == NULL) {
        destroyStack(stack);
        return LONG_MIN;
    }

    strcpy(copie, expr);
    char *elem = strtok(copie, " ");

    while(elem != NULL) {
        if (strstr("+-/*", elem) && stack->size < 2) {
            destroyStack(stack);
            free(copie);
            return LONG_MIN;
        }
        if (strcmp(elem, "+") == 0) {
            long a = top(stack);
            pop(stack);
            long b = top(stack);
            pop(stack);
            
            push(stack, a + b);
        } else if (strcmp(elem, "*") == 0) {
            long a = top(stack);
            pop(stack);
            long b = top(stack);
            pop(stack);
            
            push(stack, a * b);
        } else if (strcmp(elem, "-") == 0) {
            long b = top(stack);
            pop(stack);
            long a = top(stack);
            pop(stack);

            push(stack, a - b);
        } else if (strcmp(elem, "/") == 0){
            long b = top(stack);
            pop(stack);
            long a = top(stack);
            pop(stack);

            if (b == 0) {
                destroyStack(stack);
                free(copie);
                return LONG_MIN;
            }

            push(stack, a / b);
        } else {
            long nr = atol(elem);
            push(stack, nr);
        }
        elem = strtok(NULL, " ");
    }

    if (stack->size != 1) {
        destroyStack(stack);
        free(copie);
        return LONG_MIN;
    }

    free(copie);
    long result = top(stack);
    destroyStack(stack);
    return result;
}
