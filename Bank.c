/*
    BANK MANAGEMENT SYSTEM
            made by:
                Himanshu Kumar: AP25110070319
                Aaryansh polkondwar: AP25110071275
                Aayhan Panta: AP25110071423 
                Shaik Abdul Rashid Ali: AP25110070518
                                                            */



//importing header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//struct used in the program:
struct passbook {   //used to store transaction details
    int trans_id;
    char type[15];
    float amount;
    float new_balance;
    char from[50];
    char to[50];
};

struct account {    //used to store account details
    int acc_num;    //1000 to 1009
    char name[30];  //account holder's name
    int pin;        //4-digit PIN
    long long phone; //10-digit phone number 
    float balance;   //account balance
    struct passbook history[5]; //last 5 transactions
    int trans_count; //number of transactions made by this account
};

//initializing global variables
struct account accounts[10]; //maximum 10 accounts in the bank
int total_accounts = 0; //total number of accounts created so far
int loginid = -1;   //index of currently loginidged in account in accounts array, -1 if none
int transc_num = 1; //shows number of transactions done in bank, also used as transaction ID

//declaring all functions, definations are after main function 
void menu(int n);
void create_acc();
void login();
void deposit();
void withdraw();
void transfer();
void show_history();
void edit_passbook(int acc_index, char type[], float amt, float bal, char from[], char to[]);
int confirm();

//main function
int main() {
    while (1){
        menu(1); // calling main menu
    }
    return 0;
}

// function definations:

//menu function
void menu(int n) {
    int ch;

    if (n == 1) {                                               //main menu, shown at start
        printf("\n============= V503-T BANK ==============\n"
               "1. login\n"
               "2. Create Account\n"
               "3. Exit\n"
               "========================================\n"
               "Enter choice: ");

        while (scanf("%d", &ch) != 1 || ch < 1 || ch > 3) { 
            printf("Invalid input. Enter 1-3: ");
            while (getchar() != '\n');
        }
        printf("========================================\n");

        if (ch == 1) login();                               //login to account
        else if (ch == 2) create_acc();                     //create a new account
        else { printf("Thank you for using V503-T BANK!\n"); exit(0); }   //exit program
    }

    else if (n == 2) {                                          //account menu, shown after login
        printf("\n============== ACCOUNT MENU ==============\n"
               "1. Deposit\n"
               "2. Withdraw\n"
               "3. Transfer\n"
               "4. View Passbook\n"
               "5. Logout\n"
               "===========================================\n"
               "Enter choice: ");

        while (scanf("%d", &ch) != 1 || ch < 1 || ch > 5) {
            printf("Invalid input. Enter 1–5: ");
            while (getchar() != '\n');
        }

        printf("========================================\n");

        if (ch == 1) deposit(); 
        else if (ch == 2) withdraw();
        else if (ch == 3) transfer();
        else if (ch == 4) show_history(); 
        else { //logout
            loginid = -1;
            printf("\nlogged out successfully.\n");
            return;
        }

        if (loginid != -1) menu(2); //show account menu again if still logged in
    }
}

//creates a account
void create_acc() {
    if (total_accounts >= 10) {     //maximum 10 accounts
        printf("Bank full! Cannot create more accounts.\n");
        return;
    }

    accounts[total_accounts].acc_num = 1000 + total_accounts;  //account numbers from 1000 to 1009, incremented by 1 for each new account
    printf("\nEnter your name: ");
    scanf(" %[^\n]", accounts[total_accounts].name); //reads string with spaces, [^\n] means read until newline

    int pin;
    do {
        printf("Set a 4-digit PIN: ");
        scanf("%d", &pin);
        if (pin < 1000 || pin > 9999)   //PIN must be 4-digt
            printf("Invalid PIN! Must be 4 digits.\n");
    } while (pin < 1000 || pin > 9999); //repeat until valid PIN
    accounts[total_accounts].pin = pin;

    long long phone;
    do {
        printf("Enter 10-digit phone number: ");
        scanf("%lld", &phone); //%lld for long long int
        if (phone < 1000000000 || phone > 9999999999)   //phone number must be 10-digit
            printf("Invalid phone number.\n");
    } while (phone < 1000000000 || phone > 9999999999);
    accounts[total_accounts].phone = phone;

    accounts[total_accounts].balance = 0;
    accounts[total_accounts].trans_count = 0;

    printf("\nAccount created successfully!\nYour Account Number: %d\n",
           accounts[total_accounts].acc_num);
    total_accounts++;
}

//login to account
void login() {
    int acc, pin;
    printf("\nEnter Account Number: ");
    scanf("%d", &acc);
    printf("Enter PIN: ");
    scanf("%d", &pin);

    for (int i = 0; i < total_accounts; i++) {  //searching for account with given acc number and pin
        if (accounts[i].acc_num == acc && accounts[i].pin == pin) {
            loginid = i;                     //setting loginid to index of found account, so we can access it later
            printf("\nWelcome, %s!\n", accounts[i].name);
            menu(2);
            return;
        }
    }
    printf("Invalid account or PIN.\n");
}

//to confirm a transaction
int confirm() {
    char choice;
    printf("Confirm transaction? (y/n): ");
    scanf(" %c", &choice);
    return (choice == 'y' || choice == 'Y');
}

//to deposit money
void deposit() {
    float amt;  
    printf("Enter amount to deposit: "); 
    scanf("%f", &amt);
    if (amt <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    if (!confirm()) {   //confirming transaction from user 
        printf("Transaction cancelled.\n");
        return;
    }

    accounts[loginid].balance += amt;
    edit_passbook(loginid, "Deposit", amt, accounts[loginid].balance, "Bank", accounts[loginid].name);

    printf("Rs. %.2f deposited successfully. Balance: Rs. %.2f\n", amt, accounts[loginid].balance);
}

//to withdraw money
void withdraw() {
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);

    if (amt <= 0 || amt > accounts[loginid].balance) {  //checking for valid amount and if sufficient balance in account
        printf("Invalid or insufficient balance.\n");
        return;
    }

    if (!confirm()) {  //confirming transaction from user
        printf("Transaction cancelled.\n");
        return;
    }

    accounts[loginid].balance -= amt;
    edit_passbook(loginid, "Withdraw", amt, accounts[loginid].balance, accounts[loginid].name, "ATM");  //updating passbook after withdrawal

    printf("Rs. %.2f withdrawn. Balance: Rs. %.2f\n", amt, accounts[loginid].balance); 
}

//to transfer money to another account
void transfer() {
    int recv_acc, recv_index = -1;              //receiver account number and its index in accounts array
    float amt;  
    printf("Enter receiver account number: ");
    scanf("%d", &recv_acc);
    printf("Enter amount: ");
    scanf("%f", &amt);

    for (int i = 0; i < total_accounts; i++) { //finding receiver account index
        if (accounts[i].acc_num == recv_acc) recv_index = i;
    }

    if (recv_index == -1) { //if receiver account not found
        printf("Receiver not found.\n");
        return;
    }

    if (amt <= 0 || amt > accounts[loginid].balance) { //checking for valid amount and if sufficient balance in account
        printf("Invalid or insufficient balance.\n");
        return;
    }

    if (!confirm()) {
        printf("Transaction cancelled.\n");
        return;
    }

    accounts[loginid].balance -= amt; 
    accounts[recv_index].balance += amt;

    edit_passbook(loginid, "Transfer", amt, accounts[loginid].balance, 
                  accounts[loginid].name, accounts[recv_index].name);//updating sender's passbook

    edit_passbook(recv_index, "Received", amt, accounts[recv_index].balance,
                  accounts[loginid].name, accounts[recv_index].name);//updating receiver passbook

    printf("Transferred Rs. %.2f to %s successfully.\n", amt, accounts[recv_index].name);
}

//for showing last 5 transaction history
void show_history() {
    struct account *acc = &accounts[loginid];
    printf("\n======== LAST TRANSACTIONS ========\n");

    if (acc->trans_count == 0) {
        printf("No transactions yet.\n");
        return;
    }

    int start = acc->trans_count > 5 ? acc->trans_count - 5 : 0;
    for (int i = acc->trans_count - 1; i >= start; i--) {
        struct passbook *p = &acc->history[i % 5];
        printf("#%d | %-8s | Rs. %.2f | From: %s | To: %s | Bal: Rs. %.2f\n",
               p->trans_id, p->type, p->amount, p->from, p->to, p->new_balance);
    }
}

//to edit passbook after every transaction
void edit_passbook(int acc_index, char type[], float amt, float bal, char from[], char to[]) {
    struct account *acc = &accounts[acc_index];
    int i = acc->trans_count % 5;

    acc->history[i].trans_id = transc_num++;   //transc number incremented globally 
    strcpy(acc->history[i].type, type);        //transaction type, deposit/withdraw/transfer/received
    acc->history[i].amount = amt;              //transaction amount
    acc->history[i].new_balance = bal;         //new balance 
    strcpy(acc->history[i].from, from);        //sender name
    strcpy(acc->history[i].to, to);            //receiver name

    acc->trans_count++;
}

//END OF CODE
//Thank you for using V503-T BANK :)

