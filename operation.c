#include <stdio.h>
#include <stdlib.h>
#include "atm.h"

int main()
{
    Account *users = NULL;
    int count = 0;

    if (!loadAccounts(&users, &count))
    {
        printf("Error loading data file.\n");
        return 0;
    }

    int acc, pin;
    printf("Enter Account Number: ");
    scanf("%d", &acc);
    printf("Enter PIN: ");
    scanf("%d", &pin);

    int index = validateUser(users, count, acc, pin);
    if (index == -1)
    {
        printf("Invalid credentials.\n");
        free(users);
        return 0;
    }

    int choice;
    do
    {
        showMenu();
        scanf("%d", &choice);

        if (choice == 1)
        {
            float amt;
            printf("Enter amount to deposit: ");
            scanf("%f", &amt);
            deposit(&users[index], amt);
            saveAccounts(users, count);
        }
        else if (choice == 2)
        {
            float amt;
            printf("Enter amount to withdraw: ");
            scanf("%f", &amt);

            if (withdraw(&users[index], amt))
                printf("Transaction Successful.\n");
            else
                printf("Insufficient Balance or Invalid amount.\n");

            saveAccounts(users, count);
        }
        else if (choice == 3)
        {
            printMiniStatement(&users[index]);
        }
        else if (choice != 4)
            printf("Invalid option.\n");

    } while (choice != 4);

    printf("Thank you for using the ATM.\n");
    free(users);
    return 0;
}
