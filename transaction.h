#include <stdio.h>
#include "atm.h"

void deposit(Account *user, float amount)
{
    if (amount > 0)
    {
        user->balance += amount;
        printf("Amount Rs %.2f deposited successfully.\n", amount);
    }
    else
        printf("Invalid amount.\n");
}

int withdraw(Account *user, float amount)
{
    if (amount <= 0)
        return 0;

    if (amount > user->balance)
        return 0;

    user->balance -= amount;
    return 1;
}

void printMiniStatement(Account *user)
{
    printf("\n-------- MINI STATEMENT --------\n");
    printf("Account Number : %d\n", user->acc_no);
    printf("Balance        : Rs %.2f\n", user->balance);
    printf("--------------------------------\n");
}
