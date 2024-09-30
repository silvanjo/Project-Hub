#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;  // File pointer
    char filename[] = "example.txt";
    
    // 1. Open the file for writing ("w" mode creates or overwrites the file)
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    // 2. Write some text to the file
    fprintf(file, "Hello, World!\n");
    fprintf(file, "This is a test file.\n");

    // 3. Close the file
    fclose(file);

    // 4. Open the file again, this time for reading ("r" mode)
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    // 5. Read and display the content from the file line by line
    char buffer[100];  // Buffer to hold each line
    printf("Reading from file '%s':\n", filename);
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);  // Print each line to the console
    }

    // 6. Close the file
    fclose(file);

    return 0;
}

