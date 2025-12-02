#include <stdio.h>
#include <stdlib.h>
#include "atm.h"

int loadAccounts(Account **list, int *count)
{
    FILE *fp = fopen("data.txt", "r");
    if (!fp) return 0;

    fscanf(fp, "%d", count);
    *list = (Account *)malloc(sizeof(Account) * (*count));

    for (int i = 0; i < *count; i++)
        fscanf(fp, "%d %d %f", &(*list)[i].acc_no, &(*list)[i].pin, &(*list)[i].balance);

    fclose(fp);
    return 1;
}

void saveAccounts(Account *list, int count)
{
    FILE *fp = fopen("data.txt", "w");
    if (!fp) return;

    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d %d %.2f\n", list[i].acc_no, list[i].pin, list[i].balance);

    fclose(fp);
}

int validateUser(Account *list, int count, int acc, int pin)
{
    for (int i = 0; i < count; i++)
        if (list[i].acc_no == acc && list[i].pin == pin)
            return i;

    return -1;
}
