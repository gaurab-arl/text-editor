#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define MAX_SIZE 1024
#define ACCOUNTS_FILE "accounts.dat"

struct account {
    char username[30];
    char password[65];
};

void divider();
void display_help();
int login(struct account *user_data);
void take_password(char pass[20]);
int create_account(struct account *user_data);

int main() {
    struct account user_data = {"", ""};
    char command[20];
while(1)
{
    display_help();
    printf("\nEnter command: ");
    fgets(command, sizeof(command), stdin);

    // Remove the newline character if present
    command[strcspn(command, "\n")] = 0;

    if (strcmp(command, "-login") == 0) {
        login(&user_data);
    } else if(strcmp(command, "-signup") == 0) {
        create_account(&user_data);
    } else {
        printf("Unknown command. Use -help to see available commands.\n");
    }
    
    return 0;
}
}

void divider() {
	int i;
    for ( i = 0; i < 50; i++) {
        printf("-");
    }
    printf("\n");
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
    printf("-edit: Edit your file contents\n");
    printf("-search: Search text in file contents\n");
    printf("-help: Display this help message\n");
    
}

int login(struct account *user_data) {
    char pass[20];
    system("cls");
    printf("LOGIN\n");
    divider();
    printf("Enter your username: ");
    scanf("%s", user_data->username);
    printf("Enter password: ");
    take_password(pass);
    strcpy(user_data->password, pass);

    // Check credentials against stored accounts
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (file == NULL) {
        printf("Error opening accounts file.\n");
        return;
    }



    struct account temp;
    int found = 0;
    while (fread(&temp, sizeof(struct account), 1, file)) {
        if (strcmp(temp.username, user_data->username) == 0 && strcmp(temp.password, user_data->password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found) {
        printf("Login successful.\n");
    } else {
        printf("Invalid username or password.\n");
    }
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
        } else if (i < MAX_SIZE) { // Ensure we don't overflow the buffer
            pass[i++] = ch;
            printf("*");
        }
    }
    printf("\n");
}

int create_account(struct account *user_data) {
    system("cls");
    printf("SIGN UP\n");
    divider();
    printf("Enter your username: ");
    scanf("%s", user_data->username);
    printf("Enter password: ");
    take_password(user_data->password);

    FILE *file = fopen(ACCOUNTS_FILE, "ab");
    if (file == NULL) {
        printf("Error opening accounts file.\n");
        return;
    }

    // Check if the username already exists
    struct account temp;
    int exists = 0;
    FILE *readFile = fopen(ACCOUNTS_FILE, "rb");
    if (readFile != NULL) {
        while (fread(&temp, sizeof(struct account), 1, readFile)) {
            if (strcmp(temp.username, user_data->username) == 0) {
                exists = 1;
                break;
            }
        }
        fclose(readFile);
    }

    if (exists) {
        printf("Username already exists.\n");
    } else {
        fwrite(user_data, sizeof(struct account), 1, file);
        printf("Account successfully created.\n");
    }
    fclose(file);
    return;
}
