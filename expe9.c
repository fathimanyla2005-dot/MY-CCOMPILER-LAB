

#include <stdio.h>
#include <string.h>
#include <ctype.h>

char production[10][20];
char first[20];
int n;

void add(char c)
{
    int i;
    for(i=0; first[i]!='\0'; i++)
        if(first[i]==c)
            return;

    first[i]=c;
    first[i+1]='\0';
}

void FIRST(char c)
{
    int i,j,k;
    char temp[20];
    int epsilon;

    if(!isupper(c))
    {
        add(c);
        return;
    }

    for(i=0;i<n;i++)
    {
        if(production[i][0]==c)
        {
            if(production[i][2]=='$')
            {
                add('$');
            }
            else
            {
                j=2;

                while(production[i][j]!='\0')
                {
                    temp[0]='\0';
                    epsilon=0;

                    if(!isupper(production[i][j]))
                    {
                        add(production[i][j]);
                        break;
                    }

                    char oldFirst[20];
                    strcpy(oldFirst, first);

                    first[0]='\0';
                    FIRST(production[i][j]);
                    strcpy(temp, first);
                    strcpy(first, oldFirst);

                    for(k=0;temp[k]!='\0';k++)
                    {
                        if(temp[k]!='$')
                            add(temp[k]);
                    }

                    for(k=0;temp[k]!='\0';k++)
                    {
                        if(temp[k]=='$')
                        {
                            epsilon=1;
                            break;
                        }
                    }

                    if(!epsilon)
                        break;

                    j++;
                }

                if(production[i][j]=='\0')
                    add('$');
            }
        }
    }
}

int main()
{
    int i;
    char ch,choice;

    printf("Enter number of productions: ");
    scanf("%d",&n);

    printf("Enter productions (use $ for epsilon)\n");

    for(i=0;i<n;i++)
    {
        printf("Production %d: ",i+1);
        scanf("%s",production[i]);
    }

    do
    {
        first[0]='\0';

        printf("\nFind FIRST of: ");
        scanf(" %c",&ch);

        FIRST(ch);

        printf("FIRST(%c) = { ",ch);

        for(i=0;first[i]!='\0';i++)
            printf("%c ",first[i]);

        printf("}\n");

        printf("Continue (y/n): ");
        scanf(" %c",&choice);

    }while(choice=='y'||choice=='Y');

    return 0;
}
