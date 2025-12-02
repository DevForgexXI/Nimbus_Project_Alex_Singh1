#ifndef ATM_H
#define ATM_H

typedef struct {
    int acc_no;
    int pin;
    float balance;
} Account;

int loadAccounts(Account **list, int *count);
void saveAccounts(Account *list, int count);

int validateUser(Account *list, int count, int acc, int pin);
void showMenu();
void deposit(Account *user, float amount);
int withdraw(Account *user, float amount);
void printMiniStatement(Account *user);

#endif
