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



Account *load_accounts(int *count) {
    FILE *fp;
    Account *accounts;

    fp = fopen(ACC_FILE, "r");
    if (fp == NULL) {
       
        *count = 3;
        accounts = (Account *) malloc((*count) * sizeof(Account));
        if (accounts == NULL) return NULL;

        accounts[0].account_no = 1001;
        accounts[0].pin = 1111;
        accounts[0].balance = 5000.0;

        accounts[1].account_no = 1002;
        accounts[1].pin = 2222;
        accounts[1].balance = 3000.0;

        accounts[2].account_no = 1003;
        accounts[2].pin = 3333;
        accounts[2].balance = 7000.0;

        save_accounts(accounts, *count);
        return accounts;
    }

    if (fscanf(fp, "%d", count) != 1) {
        fclose(fp);
        return NULL;
    }

    accounts = (Account *) malloc((*count) * sizeof(Account));
    if (accounts == NULL) {
        fclose(fp);
        return NULL;
    }

    {
        int i;
        for (i = 0; i < *count; i++) {
            if (fscanf(fp, "%d %d %lf",
                       &accounts[i].account_no,
                       &accounts[i].pin,
                       &accounts[i].balance) != 3) {
                fclose(fp);
                free(accounts);
                return NULL;
            }
        }
    }

    fclose(fp);
    return accounts;
}

void save_accounts(Account *accounts, int count) {
    FILE *fp;
    int i;

    fp = fopen(ACC_FILE, "w");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", ACC_FILE);
        return;
    }

    fprintf(fp, "%d\n", count);
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d %d %.2f\n",
                accounts[i].account_no,
                accounts[i].pin,
                accounts[i].balance);
    }

    fclose(fp);
}



void run_atm(Account *accounts, int count) {
    main_menu(accounts, count);
}

void main_menu(Account *accounts, int count) {
    int choice;
    int acc_no, pin;
    int index;

    while (1) {
        printf("\n==== ATM MAIN MENU ====\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        if (choice == 1) {
            printf("Enter Account Number: ");
            scanf("%d", &acc_no);
            printf("Enter PIN: ");
            scanf("%d", &pin);

            index = authenticate(accounts, count, acc_no, pin);
            if (index == -1) {
                printf("Login failed! Invalid account or PIN.\n");
            } else {
                printf("Login successful.\n");
                account_menu(accounts, index, count);
            }
        } else if (choice == 2) {
            printf("Thank you for using the ATM.\n");
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

void account_menu(Account *accounts, int index, int count) {
    int choice;
    int acc_no = accounts[index].account_no;

    while (1) {
        printf("\n==== ACCOUNT MENU (Acc No: %d) ====\n", acc_no);
        printf("1. Balance Enquiry\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Mini Statement\n");
        printf("5. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                show_balance(accounts, index);
                break;
            case 2:
                deposit_money(accounts, index, count);
                break;
            case 3:
                withdraw_money(accounts, index, count);
                break;
            case 4:
                show_mini_statement(acc_no);
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}



int find_account_index(Account *accounts, int count, int acc_no) {
    int i;
    for (i = 0; i < count; i++) {
        if (accounts[i].account_no == acc_no) {
            return i;
        }
    }
    return -1;
}

int authenticate(Account *accounts, int count, int acc_no, int pin) {
    int idx = find_account_index(accounts, count, acc_no);
    if (idx == -1) return -1;
    if (accounts[idx].pin != pin) return -1;
    return idx;
}

void show_balance(Account *accounts, int index) {
    printf("Current balance: Rs %.2f\n", accounts[index].balance);
}

void deposit_money(Account *accounts, int index, int count) {
    double amount;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1) {
        printf("Invalid amount.\n");
        return;
    }

    if (amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }

    accounts[index].balance += amount;
    save_accounts(accounts, count);
    add_transaction(accounts[index].account_no, "DEPOSIT", amount, accounts[index].balance);

    printf("Deposit successful. New balance: Rs %.2f\n", accounts[index].balance);
}

void withdraw_money(Account *accounts, int index, int count) {
    double amount;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1) {
        printf("Invalid amount.\n");
        return;
    }

    if (amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }

    if (amount > accounts[index].balance) {
        printf("Insufficient balance.\n");
        return;
    }

    accounts[index].balance -= amount;
    save_accounts(accounts, count);
    add_transaction(accounts[index].account_no, "WITHDRAW", amount, accounts[index].balance);

    printf("Withdrawal successful. New balance: Rs %.2f\n", accounts[index].balance);
}



void add_transaction(int acc_no, const char *type, double amount, double new_balance) {
    FILE *fp;

    fp = fopen(TXN_FILE, "a");
    if (fp == NULL) {
        printf("Could not open %s for writing.\n", TXN_FILE);
        return;
    }

    fprintf(fp, "%d %s %.2f %.2f\n", acc_no, type, amount, new_balance);
    fclose(fp);
}

void show_mini_statement(int acc_no) {
    FILE *fp;
    char type[20];
    double amt, bal;
    int fileAccNo;

    char last[MINI_STATEMENT_COUNT][200];
    int count = 0;
    char line[200];

    fp = fopen(TXN_FILE, "r");
    if (fp == NULL) {
        printf("No transactions found.\n");
        return;
    }

    while (fscanf(fp, "%d %s %lf %lf",
                  &fileAccNo, type, &amt, &bal) == 4) {
        if (fileAccNo == acc_no) {
            sprintf(line, "%s Rs %.2f, Balance after: Rs %.2f",
                    type, amt, bal);

            if (count < MINI_STATEMENT_COUNT) {
                strcpy(last[count], line);
                count++;
            } else {
                int i;
                for (i = 1; i < MINI_STATEMENT_COUNT; i++) {
                    strcpy(last[i - 1], last[i]);
                }
                strcpy(last[MINI_STATEMENT_COUNT - 1], line);
            }
        }
    }

    fclose(fp);

    if (count == 0) {
        printf("No transactions for this account yet.\n");
        return;
    }

    {
        int i;
        printf("\n--- MINI STATEMENT (last %d transactions) ---\n", count);
        for (i = 0; i < count; i++) {
            printf("%d) %s\n", i + 1, last[i]);
        }
    }
}
