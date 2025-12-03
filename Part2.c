#include "part1.c"

Account *load_accounts(int *count) {
    FILE *fp;
    Account *accounts;

    fp = fopen(ACC_FILE, "r");
    if (fp == NULL) {
        *count = 3;
        accounts = malloc((*count) * sizeof(Account));
        if (!accounts) return NULL;

        accounts[0] = (Account){1001, 1111, 5000.0};
        accounts[1] = (Account){1002, 2222, 3000.0};
        accounts[2] = (Account){1003, 3333, 7000.0};

        save_accounts(accounts, *count);
        return accounts;
    }

    fscanf(fp, "%d", count);
    accounts = malloc((*count) * sizeof(Account));
    if (!accounts) {
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < *count; i++) {
        fscanf(fp, "%d %d %lf",
               &accounts[i].account_no,
               &accounts[i].pin,
               &accounts[i].balance);
    }

    fclose(fp);
    return accounts;
}

void save_accounts(Account *accounts, int count) {
    FILE *fp = fopen(ACC_FILE, "w");
    if (!fp) {
        printf("Cannot open %s\n", ACC_FILE);
        return;
    }

    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
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
