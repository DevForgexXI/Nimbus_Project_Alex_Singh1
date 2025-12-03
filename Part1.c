#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACC_FILE "accounts.txt"
#define TXN_FILE "transactions.txt"
#define MINI_STATEMENT_COUNT 5

typedef struct {
    int account_no;
    int pin;
    double balance;
} Account;

Account *load_accounts(int *count);
void save_accounts(Account *accounts, int count);
void run_atm(Account *accounts, int count);

int find_account_index(Account *accounts, int count, int acc_no);
int authenticate(Account *accounts, int count, int acc_no, int pin);

void main_menu(Account *accounts, int count);
void account_menu(Account *accounts, int index, int count);

void show_balance(Account *accounts, int index);
void deposit_money(Account *accounts, int index, int count);
void withdraw_money(Account *accounts, int index, int count);

void add_transaction(int acc_no, const char *type, double amount, double new_balance);
void show_mini_statement(int acc_no);

int main(void) {
    int account_count = 0;
    Account *accounts = load_accounts(&account_count);

    if (accounts == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    run_atm(accounts, account_count);

    save_accounts(accounts, account_count);
    free(accounts);
    return 0;
}
