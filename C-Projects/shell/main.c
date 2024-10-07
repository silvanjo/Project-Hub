#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

/* Delimiters for tokenizing the input */
#define DELIM " \t\r\n\a"

char* read_line(void) {
    char* line = NULL;
    size_t buffer_size = 0;
    ssize_t line_size = 0;

    line_size = getline(&line, &buffer_size, stdin);

    if (line_size == -1) {
        printf("Error reading line\n");
        free(line);
        return NULL;
    }

    return line;
}

char** split_line(char* line) {
    int buffer_size = MAX_ARGS;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token;
    int position = 0;

    if (!tokens) {
        printf("Error allocating memory\n");
        return NULL;
    }

    token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= buffer_size) {
            buffer_size += MAX_ARGS;
            tokens = realloc(tokens, buffer_size * sizeof(char*));

            if (!tokens) {
                printf("Error allocating memory\n");
                return NULL;
            }
        }

        token = strtok(NULL, DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

/* Function to execute the command like ls, cat, etc. */
int execute_builtin(char** args) {
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) != 0) {
            printf("Error changing directory\n");
        }

        return 1;
    }

    /* Not a built-in command */
    return -1;
}

int execute_command(char** args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        /* Child process */
        if (execvp(args[0], args) == -1) {
            printf("Error executing command\n");
        }

        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        /* Error forking */
        printf("Error forking\n");
    } else {
        /* Parent process waits for the child to finish */
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void shell_loop() {
    char* line;
    char** args;
    int status = 1;

    while (1) {
        printf("> ");
        line = read_line();
        args = split_line(line);

        if (args[0] == NULL) {
            free(line);
            free(args);
            continue;
        }

        status = execute_builtin(args);

        if (status == -1) {
            status = execute_command(args);
        }

        free(line);
        free(args);

        if (status == 0) {
            break;
        }

    };
}

int main() {
    shell_loop();
    return 0;
}