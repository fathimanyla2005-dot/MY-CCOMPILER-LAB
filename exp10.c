#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char prod[MAX][MAX];
char follow[MAX];
int n, m;

int visitedFollow[26];

void addResult(char c)
{
    int i;
    for(i = 0; i < m; i++)
        if(follow[i] == c)
            return;

    follow[m++] = c;
}

void FIRST(char c, char result[], int *k);

void FOLLOW(char c)
{
    int i, j;

    if(visitedFollow[c - 'A'])
        return;

    visitedFollow[c - 'A'] = 1;

    /* Start symbol */
    if(prod[0][0] == c)
        addResult('$');

    for(i = 0; i < n; i++)
    {
        int len = strlen(prod[i]);

        for(j = 2; j < len; j++)
        {
            if(prod[i][j] != c)
                continue;

            /* Symbol exists after c */
            if(j + 1 < len)
            {
                int x = 0;
                char temp[20];

                FIRST(prod[i][j + 1], temp, &x);

                int hasEpsilon = 0;

                for(int t = 0; t < x; t++)
                {
                    if(temp[t] == '$')
                        hasEpsilon = 1;
                    else
                        addResult(temp[t]);
                }

                if(hasEpsilon)
                {
                    if(prod[i][0] != c)
                        FOLLOW(prod[i][0]);
                }
            }
            else
            {
                /* Last symbol */
                if(prod[i][0] != c)
                    FOLLOW(prod[i][0]);
            }
        }
    }

    visitedFollow[c - 'A'] = 0;
}

void FIRST(char c, char result[], int *k)
{
    int i;

    if(!isupper(c))
    {
        result[(*k)++] = c;
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(prod[i][0] == c)
        {
            char x = prod[i][2];

            if(x == '$')
            {
                result[(*k)++] = '$';
            }
            else if(!isupper(x))
            {
                result[(*k)++] = x;
            }
            else
            {
                FIRST(x, result, k);
            }
        }
    }
}

int main()
{
    int i;
    char ch;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (Example: E=TR, A=$)\n");

    for(i = 0; i < n; i++)
        scanf("%s", prod[i]);

    do
    {
        char c;

        m = 0;
        memset(visitedFollow, 0, sizeof(visitedFollow));

        printf("\nEnter Non-Terminal: ");
        scanf(" %c", &c);

        FOLLOW(c);

        printf("FOLLOW(%c) = { ", c);

        for(i = 0; i < m; i++)
            printf("%c ", follow[i]);

        printf("}\n");

        printf("\nContinue? (y/n): ");
        scanf(" %c", &ch);

    } while(ch == 'y' || ch == 'Y');

    return 0;
}
