// Copyright 2025 Erdem Ersoy (eersoy93)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../include/colors.h"

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// Simple write wrapper
static void write_str(const char *str) {
    ssize_t ret = write(1, str, strlen(str));
    (void)ret;  // Ignore return value intentionally
}

// Parse command line into arguments
static int parse_args(char *line, char **args) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return i;
}

// Built-in commands
static int builtin_cd(char **args) {
    if (args[1] == NULL) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: cd: missing argument" COLOR_RESET "\n");
        return 1;
    }
    if (chdir(args[1]) != 0) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: cd: cannot change directory" COLOR_RESET "\n");
        return 1;
    }
    return 0;
}

static int builtin_exit(char **args) {
    (void)args;
    exit(0);
}

static int builtin_help(char **args) {
    (void)args;
    write_str(ERDEMOS_PRIMARY_COLOR "ersh - Erdem Shell\n");
    write_str(ERDEMOS_PRIMARY_COLOR "Built-in commands:\n");
    write_str("  " ERDEMOS_COMMAND_COLOR "cd <dir>" ERDEMOS_PRIMARY_COLOR "  - Change directory\n");
    write_str("  " ERDEMOS_COMMAND_COLOR "pwd" ERDEMOS_PRIMARY_COLOR "       - Print working directory\n");
    write_str("  " ERDEMOS_COMMAND_COLOR "exit" ERDEMOS_PRIMARY_COLOR "      - Exit shell\n");
    write_str("  " ERDEMOS_COMMAND_COLOR "help" ERDEMOS_PRIMARY_COLOR "      - Show this help\n");
    write_str("  " ERDEMOS_COMMAND_COLOR "poweroff" ERDEMOS_PRIMARY_COLOR "  - Exit shell and power off system\n");
    return 0;
}

static int builtin_poweroff(char **args) {
    (void)args;
    write_str(ERDEMOS_WARNING_COLOR "Exiting shell and powering off..." COLOR_RESET "\n");
    sync();
    execl("/bin/poweroff", "poweroff", NULL);
    // If execl fails, just exit normally
    exit(0);
}

static int builtin_pwd(char **args) {
    (void)args;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write_str(ERDEMOS_PRIMARY_COLOR);
        write_str(cwd);
        write_str(COLOR_RESET "\n");
        return 0;
    } else {
        write_str(ERDEMOS_ERROR_COLOR "ersh: pwd: failed to get current directory" COLOR_RESET "\n");
        return 1;
    }
}

// Execute command
static int execute(char **args) {
    if (args[0] == NULL) {
        return 0;
    }

    // Check built-ins
    if (strcmp(args[0], "cd") == 0) {
        return builtin_cd(args);
    }
    if (strcmp(args[0], "exit") == 0) {
        return builtin_exit(args);
    }
    if (strcmp(args[0], "help") == 0) {
        return builtin_help(args);
    }
    if (strcmp(args[0], "poweroff") == 0) {
        return builtin_poweroff(args);
    }
    if (strcmp(args[0], "pwd") == 0) {
        return builtin_pwd(args);
    }

    // Fork and exec external command
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        write_str(ERDEMOS_ERROR_COLOR "ersh: command not found: " ERDEMOS_COMMAND_COLOR);
        write_str(args[0]);
        write_str("\n");
        _exit(127);
    } else if (pid < 0) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: fork failed" COLOR_RESET "\n");
        return 1;
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
}

int main(void) {
    char line[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    ssize_t n;

    write_str(ERDEMOS_PRIMARY_COLOR "ersh - Erdem Shell" COLOR_RESET "\n");
    write_str(ERDEMOS_PRIMARY_COLOR "Type " ERDEMOS_COMMAND_COLOR "'help'" ERDEMOS_PRIMARY_COLOR " for built-in commands" COLOR_RESET "\n\n");

    while (1) {
        // Print prompt
        write_str(ERDEMOS_PROMPT_COLOR "> " ERDEMOS_COMMAND_COLOR);

        // Read command
        n = read(0, line, sizeof(line) - 1);
        if (n <= 0) {
            break;
        }
        line[n] = '\0';

        // Parse and execute
        if (parse_args(line, args) > 0) {
            execute(args);
        }
    }

    write_str("\n");
    return 0;
}

