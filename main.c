#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024    // Max size for the buffer
#define FILENAME "editor.txt"    // File in the current directory

// Function to read the contents of the file and store in buffer
void readFileToBuffer(const char *filename, char buffer[][MAX_SIZE], int *lineCount)
{
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        printf("Could not open file %s for reading.\n", filename);
        return;
    }
    *lineCount = 0;
    while (fgets(buffer[*lineCount], MAX_SIZE, file) != NULL) {  // Read each line from the file
        (*lineCount)++;
    }
    fclose(file);  // Close the file
}

// Function to write the buffer content to the file
void writeBufferToFile(const char *filename, char buffer[][MAX_SIZE], int lineCount)
{
	int i;
    FILE *file = fopen(filename, "w");  // Open the file for writing
    if (file == NULL) {
        printf("Could not open file %s for writing.\n", filename);
        return;
    }
    
    for ( i = 0; i < lineCount; i++) {
        fprintf(file, "%s", buffer[i]);  // Write each line to the file
    }
    fclose(file);  // Close the file
}

// Function to edit a specific line in the buffer
void editLine(char buffer[][MAX_SIZE], int lineCount)
{
    int lineNumber;
    printf("Enter the line number to edit (1-%d): ", lineCount);
    scanf("%d", &lineNumber);
    getchar();  // Clear the newline character from the input buffer
    if (lineNumber < 1 || lineNumber > lineCount) {
        printf("Invalid line number.\n");
        return;
    }
    printf("Enter new text for line %d: ", lineNumber);
    fgets(buffer[lineNumber - 1], MAX_SIZE, stdin);  // Read the new text
}

// Function to delete a specific line from the buffer
void deleteLine(char buffer[][MAX_SIZE], int *lineCount)
{
    int lineNumber;
    int i;
    printf("Enter the line number to delete (1-%d): ", *lineCount);
    scanf("%d", &lineNumber);
    getchar();  // Clear the newline character from the input buffer
    if (lineNumber < 1 || lineNumber > *lineCount) {
        printf("Invalid line number.\n");
        return;
    }
    for ( i = lineNumber - 1; i < *lineCount - 1; i++) {
        strcpy(buffer[i], buffer[i + 1]);  // Shift lines up
    }
    (*lineCount)--;
}

// Function to search for specific text in the buffer
void searchInBuffer(char buffer[][MAX_SIZE], int lineCount)
{
    char searchText[MAX_SIZE];
    int i;
    printf("Enter text to search: ");
    fgets(searchText, MAX_SIZE, stdin);
    searchText[strcspn(searchText, "\n")] = 0;  // Remove the newline character

    printf("Search results for \"%s\":\n", searchText);
    for ( i = 0; i < lineCount; i++) {
        if (strstr(buffer[i], searchText) != NULL) {
            printf("Line %d: %s", i + 1, buffer[i]);
        }
    }
}

// Function to read and display the file contents with line numbers
void readFileWithLineNumbers(const char *filename)
{
    char buffer[MAX_SIZE][MAX_SIZE];
    int lineCount = 0;
    int i;
    readFileToBuffer(filename, buffer, &lineCount);
    for ( i = 0; i < lineCount; i++) {
        printf("%d: %s", i + 1, buffer[i]);  // Print each line with its line number
    }
}

// Function to get user input and store it in a buffer
void getUserInput(char *buffer, size_t size)
{
    char line[256];  // Temporary buffer for each line of input
    size_t len = 0;  // Current length of the buffer
    printf("Enter text (end with a single dot '.' on a new line):\n");
    while (fgets(line, sizeof(line), stdin) != NULL) {  // Read a line of input from the user
        if (line[0] == '.' && line[1] == '\n') {  // Check for termination condition
            break;
        }
        size_t lineLen = strlen(line);  // Get the length of the current line
        if (len + lineLen < size - 1) {  // Check if the buffer has enough space
            strcpy(&buffer[len], line);  // Copy the line to the buffer
            len += lineLen;  // Update the total length
        } else {
            printf("Buffer full. Cannot add more text.\n");
            break;  // Exit if the buffer is full
        }
    }
    buffer[len] = '\0';  // Null-terminate the buffer
}

// Function to count the number of lines, characters, words, and sentences in the buffer
void countStats(char buffer[][MAX_SIZE], int lineCount, int *charCount, int *wordCount, int *sentenceCount)
{
    *charCount = *wordCount = *sentenceCount = 0;
    int i,j;
    for ( i = 0; i < lineCount; i++) {
        int len = strlen(buffer[i]);
        *charCount += len;
        int inWord = 0;
        for ( j = 0; j < len; j++) {
            if (isspace(buffer[i][j])) {
                inWord = 0;
            } else {
                if (!inWord) {
                    (*wordCount)++;
                    inWord = 1;
                }
                if (buffer[i][j] == '.' || buffer[i][j] == '!' || buffer[i][j] == '?') {
                    (*sentenceCount)++;
                }
            }
        }
    }
}

// Function to handle new features like edit, delete, search, copy, and stats
void newFeatures(char buffer[][MAX_SIZE], int *lineCount)
{
    int num;
    int i;
    while (1) {
        printf("Enter 1 for editing text\nEnter 2 for deleting text\nEnter 3 for searching text\nEnter 4 to copy a line\nEnter 5 to count statistics\nEnter 0 to go back to the main menu: ");
        scanf("%d", &num);
        getchar();  // Clear the newline character from the input buffer
        if (num == 1) {
            editLine(buffer, *lineCount);
            writeBufferToFile(FILENAME, buffer, *lineCount);
            printf("Line edited successfully.\n\n");
        } else if (num == 2) {
            deleteLine(buffer, lineCount);
            writeBufferToFile(FILENAME, buffer, *lineCount);
            printf("Line deleted successfully.\n\n");
        } else if (num == 3) {
            searchInBuffer(buffer, *lineCount);
        } else if (num == 4) {
            int lineNumber;
            printf("Enter the line number to copy (1-%d): ", *lineCount);
            scanf("%d", &lineNumber);
            getchar();  // Clear the newline character from the input buffer
            if (lineNumber < 1 || lineNumber > *lineCount) {
                printf("Invalid line number.\n");
            } else {
                // Shift lines down to make space for the new line
                for ( i = *lineCount - 1; i >= lineNumber; i--) {
                    strcpy(buffer[i + 1], buffer[i]);
                }
                // Copy the line
                strcpy(buffer[lineNumber], buffer[lineNumber - 1]);
                (*lineCount)++;
                printf("Line %d copied successfully.\n\n", lineNumber);
            }
        } else if (num == 5) {
            int charCount, wordCount, sentenceCount;
            countStats(buffer, *lineCount, &charCount, &wordCount, &sentenceCount);
            printf("Statistics:\n");
            printf("Characters: %d\n", charCount);
            printf("Words: %d\n", wordCount);
            printf("Sentences: %d\n", sentenceCount);
        } else if (num == 0) {
            break;  // Exit the loop and return to the main menu
        } else {
            printf("Invalid option. Please enter a valid option.\n");
        }
    }
}
int main() {
    char buffer[MAX_SIZE][MAX_SIZE];  // Buffer to hold the file content
    int lineCount = 0;
    int num;
    printf("Welcome to the editor!\n");

    while (1) {  // Infinite loop to keep the program running until the user chooses to exit
        printf("\nEnter 1 for writing text\nEnter 2 for reading text\nEnter 3 for more options\nEnter 0 to exit: ");
        scanf("%d", &num);
        getchar();  // Clear the newline character from the input buffer

        if (num == 1) {
            printf("Writing contents of %s:\n", FILENAME);
            char inputBuffer[MAX_SIZE] = {0};
            getUserInput(inputBuffer, MAX_SIZE);  // Get input content from the user
            FILE *file = fopen(FILENAME, "a");  // Open the file for appending
            if (file != NULL) {
                fprintf(file, "\n%s", inputBuffer);  // Append the content to the file
                fclose(file);  // Close the file
                printf("File saved successfully.\n\n");
            } else {
                printf("Could not open file %s for writing.\n", FILENAME);
            }
        } else if (num == 2) {
            printf("Reading contents of %s:\n\n", FILENAME);
            readFileWithLineNumbers(FILENAME);  // Read content from the file
        } else if (num == 3) {
            readFileToBuffer(FILENAME, buffer, &lineCount);  // Read file content to buffer
            newFeatures(buffer, &lineCount);  // Perform additional operations like edit, delete, search, and copy
        } else if (num == 0) {
            printf("Exiting the program. Thank you!\n");
            break;  // Exit the loop and terminate the program
        } else {
            printf("Invalid option. Please enter 1, 2, 3, or 0.\n");
        }
    }

    return 0;
}

