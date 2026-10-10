#include <stdio.h>

int n, m;
int nfa[10][10];
int dst[1024];
int dt[1024][10];
int nd = 1;
char sym[10];

int main(void)
{
    int i, j, k, t, from, to, nt, s, nf;
    int fin = 0, mask;
    char c;

    printf("Number of states and symbols: ");
    scanf("%d %d", &n, &m);

    printf("Symbols: ");
    for (k = 0; k < m; k++)
        scanf(" %c", &sym[k]);

    printf("Number of transitions, then 'from symbol to' for each:\n");
    scanf("%d", &nt);

    while (nt--)
    {
        scanf("%d %c %d", &from, &c, &to);

        for (k = 0; k < m; k++)
        {
            if (sym[k] == c)
            {
                nfa[from][k] |= (1 << to);
                break;
            }
        }
    }

    printf("Start state, number of final states, final states: ");
    scanf("%d %d", &s, &nf);

    while (nf--)
    {
        scanf("%d", &t);
        fin |= (1 << t);
    }

    /* Initial DFA state = set containing the NFA start state */
    dst[0] = (1 << s);

    /* Subset construction */
    for (i = 0; i < nd; i++)
    {
        for (k = 0; k < m; k++)
        {
            mask = 0;

            /* Find all NFA states reachable on symbol k */
            for (j = 0; j < n; j++)
            {
                if ((dst[i] >> j) & 1)
                    mask |= nfa[j][k];
            }

            /* Check whether this DFA state already exists */
            for (t = 0; t < nd && dst[t] != mask; t++)
                ;

            /* Add a new DFA state if necessary */
            if (t == nd && mask != 0)
                dst[nd++] = mask;

            dt[i][k] = (mask != 0) ? t : -1;
        }
    }

    /* Print DFA */
    printf("\nDFA:\n");
    printf("State\tNFA set\t\t");

    for (k = 0; k < m; k++)
        printf("%c\t", sym[k]);

    printf("\n");

    for (i = 0; i < nd; i++)
    {
        printf("%s%c\t{",
               (dst[i] & fin) ? "*" : " ",
               'A' + i);

        for (j = 0; j < n; j++)
        {
            if ((dst[i] >> j) & 1)
                printf("q%d ", j);
        }

        printf("}\t\t");

        for (k = 0; k < m; k++)
        {
            if (dt[i][k] < 0)
                printf("-\t");
            else
                printf("%c\t", 'A' + dt[i][k]);
        }

        printf("\n");
    }

    printf("\n(* = final state, A = start state, - = no move)\n");

    return 0;
}

