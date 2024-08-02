#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE 1024
#define ACCOUNTS_FILE "accounts.dat"  // Using a binary file for account storage

struct account {
    char username[30];
    char password[65];
};

void divider() {
	int i;
    for ( i = 0; i < 50; i++) {
        printf("-");
    }
    printf("\n");
}

void take_password(char pass[20]) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter key
            pass[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (ch == 9 || ch == 32) { // Tab and space are ignored
            continue;
        } else {
            pass[i++] = ch;
            printf("*");
        }
    }
    printf("\n");
}

void basic_hash(const char *password, char *hash) {
    int i;
    for (i = 0; i < strlen(password); i++) {
        hash[i] = password[i] + 3; // Simple Caesar cipher for demonstration
    }
    hash[i] = '\0';
}

void signup() {
    struct account u;
    FILE *fp;
    struct account temp;
    int exists = 0;

    system("cls");
    printf("CREATING NEW ACCOUNT\n");
    divider();
    printf("Enter your username: ");
    scanf("%s", u.username);

    // Check if the username already exists
    fp = fopen(ACCOUNTS_FILE, "rb");
    if (fp) {
        while (fread(&temp, sizeof(struct account), 1, fp)) {
            if (strcmp(temp.username, u.username) == 0) {
                exists = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (exists) {
        printf("Username already exists. Please choose a different username.\n");
        return;
    }

    printf("Enter your password: ");
    take_password(u.password);

    char hashed_password[65];
    basic_hash(u.password, hashed_password);
    strcpy(u.password, hashed_password);

    fp = fopen(ACCOUNTS_FILE, "ab");
    if (fp == NULL) {
        printf("Could not open file %s for writing.\n", ACCOUNTS_FILE);
        return;
    }

    fwrite(&u, sizeof(struct account), 1, fp);
    fclose(fp);

    printf("ACCOUNT CREATED SUCCESSFULLY\n");
}

int login(struct account *logged_user) {
    char user[30], log_pass[20];
    struct account u;
    FILE *fp;
    int found = 0;

    system("cls");
    printf("LOGIN\n");
    divider();
    printf("Enter Username: ");
    scanf("%s", user);
    printf("Enter Password: ");
    take_password(log_pass);

    char hashed_password[65];
    basic_hash(log_pass, hashed_password);

    fp = fopen(ACCOUNTS_FILE, "rb");
    if (fp == NULL) {
        printf("Could not open file %s for reading.\n", ACCOUNTS_FILE);
        return 0;
    }

    while (fread(&u, sizeof(struct account), 1, fp)) {
        if (strcmp(u.username, user) == 0 && strcmp(u.password, hashed_password) == 0) {
            found = 1;
            strcpy(logged_user->username, u.username);
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("\n************ WELCOME %s ************\n", logged_user->username);
        return 1;
    } else {
        printf("\nInvalid username or password. Please try again.\n");
        return 0;
    }
}

void edit_file(const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Could not open file %s for editing.\n", filename);
        return;
    }

    char input[MAX_SIZE];
    printf("\nType your text (type -save to save and -exit to exit):\n");
    while (1) {
        fgets(input, MAX_SIZE, stdin);

        if (strcmp(input, "-save\n") == 0) {
            break;
        } else if (strcmp(input, "-exit\n") == 0) {
            fclose(file);
            return;
        }

        fputs(input, file);
    }

    fclose(file);
}

void display_help() {
    system("cls");
    printf("Commands:\n");
    printf("-login: Log in to your account\n");
    printf("-signup: Sign up for a new account\n");
    printf("-info_username: View file contents of the specified username\n");
    printf("-save: Save the current text in the editor\n");
    printf("-exit: Exit the editor or program\n");
    printf("-read: Read your file contents\n");
    printf("-help: Display this help message\n");
}

void view_user_file(const char *username) {
    char filename[50];
    snprintf(filename, sizeof(filename), "%s.txt", username);
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("No file found for user %s.\n", username);
        return;
    }

    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

int main() {
    struct account logged_user = {"", ""};
    char command[50];
    int logged_in = 0;

    system("cls");
    printf("Welcome to the Command-Based Text Editor!\n");
    display_help();

    while (1) {
        printf("\nEnter command: ");
        scanf("%s", command);
        getchar(); // Clear the newline character from input buffer

        if (strcmp(command, "-login") == 0) {
            logged_in = login(&logged_user); 
        } else if (strcmp(command, "-signup") == 0) {
            signup();
        } else if (strcmp(command, "-exit") == 0) {
            break;
        } else if (strcmp(command, "-save") == 0) {
            if (logged_in) {
                char filename[50];
                snprintf(filename, sizeof(filename), "%s.txt", logged_user.username);
                edit_file(filename);
            } else {
                printf("Please log in first.\n");
            }
        } else if (strcmp(command, "-read") == 0) {
            if (logged_in) {
                view_user_file(logged_user.username);
            } else {
                printf("Please log in first.\n");
            }
        } else if (strncmp(command, "-info_", 6) == 0) {
            char *username = command + 6;
            view_user_file(username);
        } else if (strcmp(command, "-help") == 0) {
            display_help();
        } else {
            printf("Invalid command. Type -help for a list of commands.\n");
        }
    }

    return 0;
}
