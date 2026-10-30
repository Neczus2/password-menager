#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_STR_LEN 50
#define DATA_FILE "passwords.txt"

typedef struct {
    char website[MAX_STR_LEN];
    char username[MAX_STR_LEN];
    char password[MAX_STR_LEN];
} Account;

Account accounts[MAX_ACCOUNTS];
int account_count = 0;

static void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Reads one full line. Returns 0 on EOF, 1 on success. */
static int read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        return 0;
    }

    size_t len = strcspn(buf, "\n");
    if (buf[len] == '\n') {
        buf[len] = '\0';
    } else {
        /* Line was longer than the buffer: discard the rest. */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    return 1;
}

/* Case-insensitive substring search (both strings are null-terminated). */
static int contains_ci(const char *haystack, const char *needle) {
    char h[MAX_STR_LEN];
    char n[MAX_STR_LEN];

    strcpy(h, haystack);
    strcpy(n, needle);

    for (int i = 0; h[i] != '\0'; i++) {
        h[i] = (char)tolower((unsigned char)h[i]);
    }
    for (int i = 0; n[i] != '\0'; i++) {
        n[i] = (char)tolower((unsigned char)n[i]);
    }
    return strstr(h, n) != NULL;
}

static void load_accounts() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) return;

    account_count = 0;
    while (account_count < MAX_ACCOUNTS &&
           fscanf(file, "%49[^|]|%49[^|]|%49[^\n]\n",
                  accounts[account_count].website,
                  accounts[account_count].username,
                  accounts[account_count].password) == 3) {
        account_count++;
    }
    fclose(file);
}

static void save_accounts() {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) {
        printf("Error opening file for saving!\n");
        return;
    }

    for (int i = 0; i < account_count; i++) {
        fprintf(file, "%s|%s|%s\n",
                accounts[i].website,
                accounts[i].username,
                accounts[i].password);
    }
    fclose(file);
}

static void add_account() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Database is full!\n");
        return;
    }

    Account acc = {0};
    char answer[MAX_STR_LEN];

    printf("Enter website: ");
    if (!read_line(acc.website, MAX_STR_LEN) || acc.website[0] == '\0') {
        printf("Cancelled (empty website).\n\n");
        return;
    }

    printf("Do you want to add a username/email? (y/n): ");
    read_line(answer, MAX_STR_LEN);

    if (answer[0] == 'y' || answer[0] == 'Y') {
        printf("Enter username/email: ");
        read_line(acc.username, MAX_STR_LEN);
        if (acc.username[0] == '\0') {
            strcpy(acc.username, "N/A");
        }
    } else {
        strcpy(acc.username, "N/A");
    }

    printf("Enter password: ");
    if (!read_line(acc.password, MAX_STR_LEN) || acc.password[0] == '\0') {
        printf("Cancelled (empty password).\n\n");
        return;
    }

    accounts[account_count++] = acc;
    save_accounts();
    printf("Account added successfully and saved to file!\n\n");
}

static void list_accounts() {
    if (account_count == 0) {
        printf("No accounts stored yet.\n\n");
        return;
    }

    printf("\n=== SAVED ACCOUNTS ===\n");
    for (int i = 0; i < account_count; i++) {
        printf("%d. Website: %s | Username/Email: %s | Password: %s\n", 
               i + 1, accounts[i].website, accounts[i].username, accounts[i].password);
    }
    printf("\n");
}

static void search_account() {
    if (account_count == 0) {
        printf("No accounts stored yet.\n\n");
        return;
    }

    char query[MAX_STR_LEN];
    printf("Enter website to search: ");
    if (!read_line(query, MAX_STR_LEN) || query[0] == '\0') {
        printf("Search cancelled (empty query).\n\n");
        return;
    }

    int found = 0;
    printf("\n=== SEARCH RESULTS ===\n");
    for (int i = 0; i < account_count; i++) {
        if (contains_ci(accounts[i].website, query)) {
            printf("Website: %s | Username/Email: %s | Password: %s\n", 
                   accounts[i].website, accounts[i].username, accounts[i].password);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching accounts found.\n");
    }
    printf("\n");
}

static void generate_password() {
    int length;
    printf("Enter password length (8-32): ");
    if (scanf("%d", &length) != 1 || length < 8 || length > 32) {
        printf("Invalid length.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    int charset_size = sizeof(charset) - 1;

    char generated[MAX_STR_LEN];
    for (int i = 0; i < length; i++) {
        int index = rand() % charset_size;
        generated[i] = charset[index];
    }
    generated[length] = '\0';

    printf("Generated Password: %s\n\n", generated);
}

static void delete_account() {
    if (account_count == 0) {
        printf("No accounts stored yet.\n\n");
        return;
    }

    list_accounts();
    int choice;
    printf("Enter account number to delete: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (choice < 1 || choice > account_count) {
        printf("Invalid account number.\n\n");
        return;
    }

    for (int i = choice - 1; i < account_count - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    account_count--;
    save_accounts();
    printf("Account deleted successfully and changes saved!\n\n");
}

int main() {
    load_accounts();
    srand(time(NULL));
    int choice;

    do {
        printf("=== PASSWORD MANAGER ===\n");
        printf("1. Add account\n");
        printf("2. List accounts\n");
        printf("3. Search account\n");
        printf("4. Generate password\n");
        printf("5. Delete account\n");
        printf("6. Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            choice = 0;
            printf("\n");
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_account();
                break;
            case 2:
                list_accounts();
                break;
            case 3:
                search_account();
                break;
            case 4:
                generate_password();
                break;
            case 5:
                delete_account();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n\n");
        }
    } while (choice != 6);

    return 0;
}
