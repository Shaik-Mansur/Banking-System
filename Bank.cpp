#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    char name[30];
    long int accnum;
    int pin;
    double balance;
};

long int getLastAccountNumber() {
    long int lastAccountNumber = 0;
    long int an;
    char n[30];
    double b;
    int p;
    FILE *file = fopen("Accounts File.txt", "a+");
    if (file == NULL) {
        printf("Error opening file to get last account number.\n");
        printf("----------------------------------------------------\n\n\n");
        return lastAccountNumber;
    }
    while (fscanf(file, "Account Number : %ld\nAccount Holder Name : %s\nBalance : %lf\nPin : %d\n\n", &an, n, &b, &p) == 4);
     lastAccountNumber = an; 
    fclose(file);
    return lastAccountNumber;
}

void newAcc() {
    char ch;
    struct Account *temp = (struct Account *)malloc(sizeof(struct Account));
    if (temp == NULL) {
        printf("Error creating account. Try again.\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    long int lastAccountNum = getLastAccountNumber();
    temp->accnum = lastAccountNum + 1;
    temp->balance = 0;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("Enter your name:\n");
    fgets(temp->name, sizeof(temp->name), stdin);
    int len = strlen(temp->name);
    if (len > 0 && temp->name[len - 1] == '\n') {
        temp->name[len - 1] = '\0'; 
    }
    do {
        printf("Set your four-digit pin:\n");
        scanf("%d", &temp->pin);
    } while (temp->pin < 1000 || temp->pin > 9999);

    FILE *file = fopen("Accounts File.txt", "a+");
    if (file == NULL) {
        printf("Error creating account. Please try again.\n");
        free(temp);
        return;
    }

    fprintf(file, "Account Number : %ld\n", temp->accnum);
    fprintf(file, "Account Holder Name : %s\n", temp->name);
    fprintf(file, "Balance : %lf\n", temp->balance);
    fprintf(file, "Pin : %d\n\n", temp->pin);
    fclose(file);

    printf("Your Account Number : %ld\n", temp->accnum);
    printf("Account Created Successfully\n");
    printf("----------------------------------------------------\n\n\n");
    free(temp);
}

int checkUser(long int acc, int pin) {
    long int an = 0;
    int p = 0;
    char n[30];
    double b;
    FILE *file;
    file = fopen("Accounts File.txt", "r");
    if (file == NULL) {
        printf("Error Opening file\n");
        printf("----------------------------------------------------\n\n\n");
        return 0;
    }
    while (fscanf(file, "Account Number : %ld\nAccount Holder Name : %s\nBalance : %lf\nPin : %d\n\n", &an, n, &b, &p) == 4) {
        if (acc == an) {
            if (pin == p) {
                fclose(file);
                return 1;
            }
            fclose(file);
            printf("Incorrect Pin\n");
            printf("----------------------------------------------------\n\n\n");
            return 0;
        }
    }
    fclose(file);
    printf("Account not found\n");
    printf("----------------------------------------------------\n\n\n");
    return 0;
}

void pinChange() {
    long int an = 0, an1 = 0;
    int pin = 0, pin1 = 0;
    char n[30];
    double b = 0;
    FILE *file;
    file = fopen("Accounts File.txt", "r");
    if (file == NULL) {
        printf("Error opening File\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    printf("Enter Your Account Number\n");
    scanf("%ld", &an);
    printf("Enter Your pin\n");
    scanf("%d", &pin);
    if (checkUser(an, pin)) {
        FILE *tempfile;
        tempfile = fopen("Temp.txt", "a");
        if (tempfile == NULL) {
            printf("Error changing the pin\n");
            printf("----------------------------------------------------\n\n\n");
            return;
        }
        while (fscanf(file, "Account Number : %ld\nAccount Holder Name : %s\nBalance : %lf\nPin : %d\n\n", &an1, n, &b, &pin1) == 4) {
            fprintf(tempfile, "Account Number : %ld\n", an1);
            fprintf(tempfile, "Account Holder Name : %s\n", n);
            fprintf(tempfile, "Balance : %lf\n", b);
            if (an == an1) {
                do {
                    printf("Enter New pin\n");
                    scanf("%d", &pin);
                } while (pin < 1000 || pin > 9999);
                fprintf(tempfile, "Pin : %d\n\n", pin);
            } else {
                fprintf(tempfile, "Pin : %d\n\n", pin1);
            }
        }
        fclose(file);
        fclose(tempfile);
        remove("Accounts File.txt");
        rename("Temp.txt", "Accounts File.txt");
        printf("Pin changed successfully\n");
        printf("----------------------------------------------------\n\n\n");
    } else {
        fclose(file);
        printf("Invalid Account Number or PIN.\n");
        printf("----------------------------------------------------\n\n\n");
    }
}

void deposit() {
    long int acc = 0, acc1 = 0;
    int pin = 0, p = 0, amount = 0;
    char name[30], n[30];
    double balance = 0, b = 0;
    FILE *file;
    file = fopen("Accounts File.txt", "r");
    if (file == NULL) {
        printf("Error Opening File\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    FILE *temp;
    temp = fopen("temp.txt", "a");
    if (temp == NULL) {
        printf("Error Opening File2\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    printf("Enter Your Account Number\n");
    scanf("%ld", &acc);
    printf("Enter Your Pin\n");
    scanf("%d", &pin);
    if (checkUser(acc, pin)) {
        while (fscanf(file, "Account Number : %ld\nAccount Holder Name : %s\nBalance : %lf\nPin : %d\n\n", &acc1, n, &b, &p) == 4) {
            fprintf(temp, "Account Number : %ld\n", acc1);
            fprintf(temp, "Account Holder Name : %s\n", n);
            if (acc == acc1) {
                printf("Enter Amount to deposit 500 and 100 rupee Notes Only\n");
                scanf("%i", &amount);
                if (amount % 100 != 0 || amount < 100) {
                    printf("Invalid Amount\nTry again\n");
                    fclose(file);
                    fclose(temp);
                    remove("temp.txt");
                    printf("----------------------------------------------------\n\n\n");
                    return;
                }
                fprintf(temp, "Balance : %lf\n", b + amount);
                printf("Current Balance : %lf\n", b + amount);
            }
            fprintf(temp, "Pin : %d\n\n", p);
        }
        fclose(file);
        fclose(temp);
        remove("Accounts File.txt");
        rename("temp.txt", "Accounts File.txt");
        printf("Amount Added successfully\n");
        printf("----------------------------------------------------\n\n\n");
    } else {
        fclose(file);
        fclose(temp);
        remove("temp.txt");
        printf("Invalid Account Number or PIN.\n");
        printf("----------------------------------------------------\n\n\n");
    }
}

void withdraw() {
    long int acc = 0, acc1 = 0;
    int pin = 0, p = 0, amount = 0;
    char name[30], n[30];
    double balance = 0, b = 0;
    FILE *file;
    file = fopen("Accounts File.txt", "r");
    if (file == NULL) {
        printf("Error Opening File\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    FILE *temp;
    temp = fopen("temp.txt", "a");
    if (temp == NULL) {
        printf("Error Opening File2\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    printf("Enter Your Account Number\n");
    scanf("%ld", &acc);
    printf("Enter Your Pin\n");
    scanf("%d", &pin);
    if (checkUser(acc, pin)) {
        while (fscanf(file, "Account Number : %ld\nAccount Holder Name : %s\nBalance : %lf\nPin : %d\n\n", &acc1, n, &b, &p) == 4) {
            fprintf(temp, "Account Number : %ld\n", acc1);
            fprintf(temp, "Account Holder Name : %s\n", n);
            if (acc == acc1) {
                printf("Enter Amount to Withdraw 500 and 100 rupee Notes Only\n");
                scanf("%i", &amount);
                if (amount % 100 != 0 || amount < 100) {
                    printf("Invalid Amount\nTry again\n");
                    fclose(file);
                    fclose(temp);
                    remove("temp.txt");
                    printf("----------------------------------------------------\n\n\n");
                    return;
                }
                if (b < amount) {
                    printf("Insufficient Balance\n");
                    fclose(file);
                    fclose(temp);
                    remove("temp.txt");
                    printf("----------------------------------------------------\n\n\n");
                    return;
                }
                fprintf(temp, "Balance : %lf\n", b - amount);
                printf("Current Balance : %lf\n", b - amount);
            }
            fprintf(temp, "Pin : %d\n\n", p);
        }
        fclose(file);
        fclose(temp);
        remove("Accounts File.txt");
        rename("temp.txt", "Accounts File.txt");
        printf("Amount Withdrawn Successfully\n");
        printf("----------------------------------------------------\n\n\n");
    } else {
        fclose(file);
        fclose(temp);
        remove("temp.txt");
        printf("Invalid Account Number or PIN.\n");
        printf("----------------------------------------------------\n\n\n");
    }
}

void balanceCheck() {
    long int acc = 0, acc1 = 0;
    int pin = 0, p = 0;
    char name[30], n[30];
    double balance = 0, b = 0;
    FILE *file;
    file = fopen("Accounts File.txt", "r");
    if (file == NULL) {
        printf("Error Opening File\n");
        printf("----------------------------------------------------\n\n\n");
        return;
    }
    printf("Enter Your Account Number\n");
    scanf("%ld", &acc);
    printf("Enter Your Pin\n");
    scanf("%d", &pin);
    if (checkUser(acc, pin)) {
        while (fscanf(file, "Account Number : %ld\nAccount Holder Name : %s\nBalance : %lf\nPin : %d\n\n", &acc1, n, &b, &p) == 4) {
            if (acc == acc1) {
                printf("Account Number : %ld\n", acc1);
                printf("Account Holder Name : %s\n", n);
                printf("Account Balance : %lf\n", b);
                printf("----------------------------------------------------\n\n\n");
                break;
            }
        }
    } else {
        printf("Invalid Account Number or PIN.\n");
        printf("----------------------------------------------------\n\n\n");
    }
    fclose(file);
}

int main() {
    int choice;
    while (1) {
        printf("\nBanking System\n");
        printf("1. Create New Account\n");
        printf("2. Change Pin\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Check Balance\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                newAcc();
                break;
            case 2:
                pinChange();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 5:
                balanceCheck();
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }
    return 0;
}
