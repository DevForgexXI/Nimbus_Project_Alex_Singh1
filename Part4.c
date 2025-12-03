#include "part3.c"

void show_balance(Account *accounts, int index) {
    printf("Balance: Rs %.2f\n", accounts[index].balance);
}

void deposit_money(Account *accounts, int index, int count) {
    double amount;
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    accounts[index].balance += amount;
    save_accounts(accounts, count);
    add_transaction(accounts[index].account_no, "DEPOSIT", amount, accounts[index].balance);

    printf("Deposit successful.\n");
}

void withdraw_money(Account *accounts, int index, int count) {
    double amount;
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0 || amount > accounts[index].balance) {
        printf("Invalid or insufficient amount.\n");
        return;
    }

    accounts[index].balance -= amount;
    save_accounts(accounts, count);
    add_transaction(accounts[index].account_no, "WITHDRAW", amount, accounts[index].balance);

    printf("Withdrawal successful.\n");
}

void add_transaction(int acc_no, const char *type, double amount, double new_balance) {
    FILE *fp = fopen(TXN_FILE, "a");
    if (!fp) return;

    fprintf(fp, "%d %s %.2f %.2f\n", acc_no, type, amount, new_balance);
    fclose(fp);
}

void show_mini_statement(int acc_no) {
    FILE *fp = fopen(TXN_FILE, "r");
    if (!fp) {
        printf("No transactions.\n");
        return;
    }

    char last[MINI_STATEMENT_COUNT][200];
    char type[20], line[200];
    double amt, bal;
    int fileAcc, count = 0;

    while (fscanf(fp, "%d %s %lf %lf", &fileAcc, type, &amt, &bal) == 4) {
        if (fileAcc == acc_no) {
            sprintf(line, "%s Rs %.2f | Balance: %.2f", type, amt, bal);

            if (count < MINI_STATEMENT_COUNT)
                strcpy(last[count++], line);
            else {
                for (int i = 1; i < MINI_STATEMENT_COUNT; i++)
                    strcpy(last[i - 1], last[i]);
                strcpy(last[MINI_STATEMENT_COUNT - 1], line);
            }
        }
    }

    fclose(fp);

    printf("\n--- MINI STATEMENT ---\n");
    for (int i = 0; i < count; i++)
        printf("%d) %s\n", i + 1, last[i]);
}
