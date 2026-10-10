#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 50

struct expr {
    char op[4];
    char op1[16];
    char op2[16];
    char res[16];
};

struct expr arr[MAX];

int n;

/* Check whether a string is an integer constant */
int is_const(const char *s)
{
    int i = 0;

    if (s[0] == '-' && s[1] != '\0')
        i = 1;

    if (s[i] == '\0')
        return 0;

    for (; s[i] != '\0'; i++) {
        if (!isdigit((unsigned char)s[i]))
            return 0;
    }

    return 1;
}

/* Read input */
void input(void)
{
    int i;

    printf("\nEnter the number of expressions: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number (1 to %d allowed)\n", MAX);
        exit(1);
    }

    printf("\nEnter the input (op op1 op2 result), "
           "use '.' as op2 for '=':\n");

    for (i = 0; i < n; i++) {
        if (scanf("%3s %15s %15s %15s",
                  arr[i].op,
                  arr[i].op1,
                  arr[i].op2,
                  arr[i].res) != 4) {

            printf("Bad input\n");
            exit(1);
        }
    }
}

/*
 * Propagate a constant value to later instructions.
 *
 * Example:
 * = 5 . a
 * + a 10 b
 *
 * becomes:
 * = 5 . a
 * + 5 10 b
 */
void propagate(int pos, const char *value)
{
    int i;

    for (i = pos + 1; i < n; i++) {

        /* Stop if the variable is redefined */
        if (strcmp(arr[pos].res, arr[i].res) == 0)
            break;

        if (strcmp(arr[pos].res, arr[i].op1) == 0)
            strcpy(arr[i].op1, value);

        if (strcmp(arr[i].op, "=") != 0 &&
            strcmp(arr[pos].res, arr[i].op2) == 0)
            strcpy(arr[i].op2, value);
    }
}

/* Perform constant folding and propagation */
void constant_fold(void)
{
    int i;
    int a, b, result;
    char value[16];

    for (i = 0; i < n; i++) {

        /* Assignment of a constant */
        if (strcmp(arr[i].op, "=") == 0) {

            if (is_const(arr[i].op1)) {

                /*
                 * Propagate the constant to later instructions.
                 */
                strcpy(value, arr[i].op1);

                propagate(i, value);
            }
        }

        /* Arithmetic expression */
        else if (strlen(arr[i].op) == 1 &&
                 is_const(arr[i].op1) &&
                 is_const(arr[i].op2)) {

            a = atoi(arr[i].op1);
            b = atoi(arr[i].op2);

            switch (arr[i].op[0]) {

                case '+':
                    result = a + b;
                    break;

                case '-':
                    result = a - b;
                    break;

                case '*':
                    result = a * b;
                    break;

                case '/':
                    if (b == 0) {
                        printf("\nError: Division by zero at expression %d\n",
                               i + 1);
                        return;
                    }

                    result = a / b;
                    break;

                default:
                    continue;
            }

            /*
             * Replace:
             *
             * + 5 10 b
             *
             * with:
             *
             * = 15 . b
             */
            strcpy(arr[i].op, "=");

            sprintf(arr[i].op1, "%d", result);

            strcpy(arr[i].op2, ".");

            /*
             * Propagate the calculated constant.
             */
            sprintf(value, "%d", result);
            propagate(i, value);
        }
    }
}

/* Display optimized code */
void output(void)
{
    int i;

    printf("\nOptimized code is:\n");

    for (i = 0; i < n; i++) {
        printf("%s %s %s %s\n",
               arr[i].op,
               arr[i].op1,
               arr[i].op2,
               arr[i].res);
    }
}

int main(void)
{
    input();

    constant_fold();

    output();

    return 0;
}

