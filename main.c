#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------ CONFIG ------------------------ */
#define ACC_FILE "accounts.txt"
#define TXN_FILE "transactions.txt"
#define MINI_STATEMENT_COUNT 5

/* ------------------------ DATA STRUCTURE ------------------------ */
typedef struct {
    int account_no;
    int pin;
    double balance;
} Account;

/* ------------------------ PROTOTYPES ------------------------ */
Account *loadAccounts(int *count);
void saveAccounts(Account *accounts, int count);

int findAccountIndex(Account *accounts, int count, int acc_no);
int authenticate(Account *accounts, int count, int acc_no, int pin);

void mainMenu(Account *accounts, int count);
void accountMenu(Account *accounts, int index, int count);

void showBalance(Account *accounts, int index);
void deposit(Account *accounts, int index, int count);
void withdrawAmount(Account *accounts, int index, int count);

void addTransaction(int acc_no, const char *type, double amount, double new_balance);
void showMiniStatement(int acc_no);

/* ------------------------ MAIN ------------------------ */
int main(void) {
    int accountCount = 0;
    Account *accounts = loadAccounts(&accountCount);   // dynamic memory (pointers)

    if (accounts == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return 1;
    }

    mainMenu(accounts, accountCount);

    /* save final state and free memory */
    saveAccounts(accounts, accountCount);
    free(accounts);

    return 0;
}

/* ------------------------ MENUS ------------------------ */

void mainMenu(Account *accounts, int count) {
    int choice;
    int acc_no, pin, idx;

    while (1) {
        printf("\n========== ATM MAIN MENU ==========\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            return;
        }

        switch (choice) {
            case 1:
                printf("Enter Account Number: ");
                scanf("%d", &acc_no);
                printf("Enter PIN: ");
                scanf("%d", &pin);

                idx = authenticate(accounts, count, acc_no, pin);
                if (idx == -1) {
                    printf("Login failed! Invalid account number or PIN.\n");
                } else {
                    printf("Login successful.\n");
                    accountMenu(accounts, idx, count);
                }
                break;

            case 2:
                printf("Thank you for using the ATM.\n");
                return;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void accountMenu(Account *accounts, int index, int count) {
    int choice;
    int acc_no = accounts[index].account_no;

    while (1) {
        printf("\n======= ACCOUNT MENU (Acc No: %d) =======\n", acc_no);
        printf("1. Balance Enquiry\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Mini Statement\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showBalance(accounts, index);
                break;
            case 2:
                deposit(accounts, index, count);
                break;
            case 3:
                withdrawAmount(accounts, index, count);
                break;
            case 4:
                showMiniStatement(acc_no);
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

/* ------------------------ FILE HANDLING ------------------------ */

/* Loads accounts from file.
   If file does not exist, create 3 default accounts and save them. */
Account *loadAccounts(int *count) {
    FILE *fp = fopen(ACC_FILE, "r");
    int i;

    if (fp == NULL) {
        /* no existing file -> create default data */
        *count = 3;
        Account *accounts = (Account *)malloc((*count) * sizeof(Account));
        if (!accounts) return NULL;

        accounts[0].account_no = 1001; accounts[0].pin = 1111; accounts[0].balance = 5000.0;
        accounts[1].account_no = 1002; accounts[1].pin = 2222; accounts[1].balance = 3000.0;
        accounts[2].account_no = 1003; accounts[2].pin = 3333; accounts[2].balance = 7000.0;

        saveAccounts(accounts, *count);
        return accounts;
    }

    fscanf(fp, "%d", count);
    Account *accounts = (Account *)malloc((*count) * sizeof(Account));
    if (!accounts) {
        fclose(fp);
        return NULL;
    }

    for (i = 0; i < *count; i++) {
        fscanf(fp, "%d %d %lf",
               &accounts[i].account_no,
               &accounts[i].pin,
               &accounts[i].balance);
    }

    fclose(fp);
    return accounts;
}

/* Saves accounts to file */
void saveAccounts(Account *accounts, int count) {
    FILE *fp = fopen(ACC_FILE, "w");
    int i;
    if (fp == NULL) {
        printf("Error opening account file for writing.\n");
        return;
    }

    fprintf(fp, "%d\n", count);
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d %d %.2lf\n",
                accounts[i].account_no,
                accounts[i].pin,
                accounts[i].balance);
    }

    fclose(fp);
}

/* ------------------------ AUTH & SEARCH ------------------------ */

int findAccountIndex(Account *accounts, int count, int acc_no) {
    int i;
    for (i = 0; i < count; i++) {
        if (accounts[i].account_no == acc_no) {
            return i;
        }
    }
    return -1;
}

int authenticate(Account *accounts, int count, int acc_no, int pin) {
    int idx = findAccountIndex(accounts, count, acc_no);
    if (idx == -1) return -1;
    if (accounts[idx].pin != pin) return -1;
    return idx;
}

/* ------------------------ OPERATIONS ------------------------ */

void showBalance(Account *accounts, int index) {
    printf("Current balance: Rs %.2lf\n", accounts[index].balance);
}

void deposit(Account *accounts, int index, int count) {
    double amount;
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    accounts[index].balance += amount;
    saveAccounts(accounts, count);
    addTransaction(accounts[index].account_no, "DEPOSIT", amount,
                   accounts[index].balance);

    printf("Deposit successful. New balance: Rs %.2lf\n",
           accounts[index].balance);
}

void withdrawAmount(Account *accounts, int index, int count) {
    double amount;
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    if (amount > accounts[index].balance) {
        printf("Insufficient balance.\n");
        return;
    }

    accounts[index].balance -= amount;
    saveAccounts(accounts, count);
    addTransaction(accounts[index].account_no, "WITHDRAW", amount,
                   accounts[index].balance);

    printf("Withdrawal successful. New balance: Rs %.2lf\n",
           accounts[index].balance);
}

/* ------------------------ TRANSACTIONS & MINI STATEMENT ------------------------ */

/* Append single transaction record to file */
void addTransaction(int acc_no, const char *type, double amount, double new_balance) {
    FILE *fp = fopen(TXN_FILE, "a");
    if (fp == NULL) {
        printf("Error opening transaction file.\n");
        return;
    }

    /* Format: account_no TYPE amount balance_after */
    fprintf(fp, "%d %s %.2lf %.2lf\n",
            acc_no, type, amount, new_balance);

    fclose(fp);
}

/* Show last MINI_STATEMENT_COUNT transactions of a particular account */
void showMiniStatement(int acc_no) {
    FILE *fp = fopen(TXN_FILE, "r");
    if (fp == NULL) {
        printf("No transactions found.\n");
        return;
    }

    char buffer[200];
    char type[20];
    double amt, bal;
    int fileAccNo;

    char last[MINI_STATEMENT_COUNT][200];
    int count = 0;

    /* Read entire file; keep only last N txns for this account */
    while (fscanf(fp, "%d %s %lf %lf",
                  &fileAccNo, type, &amt, &bal) == 4) {

        if (fileAccNo == acc_no) {
            snprintf(buffer, sizeof(buffer),
                     "%s Rs %.2lf, Balance after: Rs %.2lf",
                     type, amt, bal);

            if (count < MINI_STATEMENT_COUNT) {
                strcpy(last[count], buffer);
                count++;
            } else {
                /* shift left */
                for (int i = 1; i < MINI_STATEMENT_COUNT; i++) {
                    strcpy(last[i - 1], last[i]);
                }
                strcpy(last[MINI_STATEMENT_COUNT - 1], buffer);
            }
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No transactions for this account yet.\n");
        return;
    }

    printf("\n------ MINI STATEMENT (last %d) ------\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d) %s\n", i + 1, last[i]);
    }
}
