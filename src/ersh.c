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
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/colors.h"
#include "../include/version.h"

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

// Write size right-aligned in a field of width 10
static void write_size_aligned(off_t size) {
    char buf[32];
    int i = 0;
    
    // Convert number to string (reverse)
    off_t n = size;
    if (n == 0) {
        buf[i++] = '0';
    } else {
        while (n > 0) {
            buf[i++] = '0' + (n % 10);
            n /= 10;
        }
    }
    
    // Pad with spaces (right-align to 10 characters)
    int padding = 10 - i;
    for (int j = 0; j < padding; j++) {
        write_str(" ");
    }
    
    // Write in correct order
    while (i > 0) {
        char c[2] = {buf[--i], '\0'};
        write_str(c);
    }
}

// Write file permissions in ls -l format
static void write_permissions(mode_t mode) {
    write_str((S_ISDIR(mode)) ? "d" : "-");
    write_str((mode & S_IRUSR) ? "r" : "-");
    write_str((mode & S_IWUSR) ? "w" : "-");
    write_str((mode & S_IXUSR) ? "x" : "-");
    write_str((mode & S_IRGRP) ? "r" : "-");
    write_str((mode & S_IWGRP) ? "w" : "-");
    write_str((mode & S_IXGRP) ? "x" : "-");
    write_str((mode & S_IROTH) ? "r" : "-");
    write_str((mode & S_IWOTH) ? "w" : "-");
    write_str((mode & S_IXOTH) ? "x" : "-");
}

// Built-in commands

// Forward declaration for recursive directory removal
static int remove_directory_recursive(const char *path);

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
    // If specific command requested, show detailed help
    if (args[1] != NULL) {
        const char *cmd = args[1];
        
        if (strcmp(cmd, "cd") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "cd" ERDEMOS_PRIMARY_COLOR " - Change directory\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "cd [directory]" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Changes the current working directory to the specified path.\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "exit") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "exit" ERDEMOS_PRIMARY_COLOR " - Exit shell\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "exit" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Exits the shell.\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "help") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "help" ERDEMOS_PRIMARY_COLOR " - Show help\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "help [command]" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Shows general help or detailed help for a specific command.\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "ls") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "ls" ERDEMOS_PRIMARY_COLOR " - List directory contents\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "ls [-al] [directory]" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Lists files and directories in the specified or current directory.\n");
            write_str("Options:\n");
            write_str("  -a      Show all files including hidden (starting with '.')\n");
            write_str("  -l      Use long listing format (permissions, size, name)\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "mkdir") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "mkdir" ERDEMOS_PRIMARY_COLOR " - Create directory\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "mkdir [directory]" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Creates a new directory with the specified name.\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "poweroff") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "poweroff" ERDEMOS_PRIMARY_COLOR " - Power off system\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "poweroff" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Exits the shell and powers off the system.\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "pwd") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "pwd" ERDEMOS_PRIMARY_COLOR " - Print working directory\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "pwd" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Displays the current working directory path.\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "rm") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "rm" ERDEMOS_PRIMARY_COLOR " - Remove file or directory\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "rm [-rf] [file/dir ...]" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Removes files or directories.\n");
            write_str("Options:\n");
            write_str("  -r, -R  Remove directories recursively\n");
            write_str("  -f      Force removal, ignore errors\n" COLOR_RESET);
            return 0;
        }
        if (strcmp(cmd, "touch") == 0) {
            write_str(ERDEMOS_COMMAND_COLOR "touch" ERDEMOS_PRIMARY_COLOR " - Create empty file\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Usage: " ERDEMOS_COMMAND_COLOR "touch [file]" COLOR_RESET "\n");
            write_str(ERDEMOS_PRIMARY_COLOR "Creates an empty file with the specified name.\n" COLOR_RESET);
            return 0;
        }
        
        // Unknown command
        write_str(ERDEMOS_ERROR_COLOR "ersh: help: unknown command: " COLOR_RESET);
        write_str(cmd);
        write_str("\n");
        return 1;
    }
    
    // Show general help
    write_str(ERDEMOS_PRIMARY_COLOR "ersh - Erdem Shell\n\n");
    write_str(ERDEMOS_PRIMARY_COLOR "Built-in commands:\n\n");
    write_str(ERDEMOS_COMMAND_COLOR "cd [dir]" ERDEMOS_PRIMARY_COLOR "            - Change directory\n");
    write_str(ERDEMOS_COMMAND_COLOR "exit" ERDEMOS_PRIMARY_COLOR "                - Exit shell\n");
    write_str(ERDEMOS_COMMAND_COLOR "help [command]" ERDEMOS_PRIMARY_COLOR "      - Show this help\n");
    write_str(ERDEMOS_COMMAND_COLOR "ls [-al] [dir]" ERDEMOS_PRIMARY_COLOR "      - List directory contents\n");
    write_str(ERDEMOS_COMMAND_COLOR "mkdir [dir]" ERDEMOS_PRIMARY_COLOR "         - Create directory\n");
    write_str(ERDEMOS_COMMAND_COLOR "poweroff" ERDEMOS_PRIMARY_COLOR "            - Exit shell and power off system\n");
    write_str(ERDEMOS_COMMAND_COLOR "pwd" ERDEMOS_PRIMARY_COLOR "                 - Print working directory\n");
    write_str(ERDEMOS_COMMAND_COLOR "rm [-rf] [file/dir]" ERDEMOS_PRIMARY_COLOR " - Remove file or directory\n");
    write_str(ERDEMOS_COMMAND_COLOR "touch [file]" ERDEMOS_PRIMARY_COLOR "        - Create empty file\n");
    write_str("\nType " ERDEMOS_COMMAND_COLOR "'help [command]'" ERDEMOS_PRIMARY_COLOR " for detailed help on a specific command.\n");
    return 0;
}

static int builtin_ls(char **args) {
    int show_all = 0;
    int long_format = 0;
    int arg_idx = 1;
    const char *path = ".";
    
    // Parse flags
    while (args[arg_idx] != NULL && args[arg_idx][0] == '-') {
        for (int i = 1; args[arg_idx][i] != '\0'; i++) {
            if (args[arg_idx][i] == 'a') {
                show_all = 1;
            } else if (args[arg_idx][i] == 'l') {
                long_format = 1;
            }
        }
        arg_idx++;
    }
    
    // Get path if provided
    if (args[arg_idx] != NULL) {
        path = args[arg_idx];
    }
    
    DIR *dir = opendir(path);
    
    if (dir == NULL) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: ls: cannot open directory: " COLOR_RESET);
        write_str(path);
        write_str("\n");
        return 1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files unless -a is specified
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }
        
        if (long_format) {
            // Build full path for stat
            char fullpath[1024];
            int written = 0;
            const char *p = path;
            while (*p && written < 1023) {
                fullpath[written++] = *p++;
            }
            if (written < 1023 && fullpath[written-1] != '/') {
                fullpath[written++] = '/';
            }
            p = entry->d_name;
            while (*p && written < 1023) {
                fullpath[written++] = *p++;
            }
            fullpath[written] = '\0';
            
            struct stat st;
            if (stat(fullpath, &st) == 0) {
                write_str(ERDEMOS_PRIMARY_COLOR);
                write_permissions(st.st_mode);
                write_str(" ");
                write_size_aligned(st.st_size);
                write_str("  ");
                write_str(ERDEMOS_COMMAND_COLOR);
                write_str(entry->d_name);
                write_str(COLOR_RESET "\n");
            } else {
                // Fallback if stat fails
                write_str(ERDEMOS_PRIMARY_COLOR);
                write_str(entry->d_name);
                write_str(COLOR_RESET "\n");
            }
        } else {
            write_str(ERDEMOS_PRIMARY_COLOR);
            write_str(entry->d_name);
            write_str("  " COLOR_RESET);
        }
    }
    
    if (!long_format) {
        write_str("\n");
    }
    
    closedir(dir);
    return 0;
}

static int builtin_mkdir(char **args) {
    if (args[1] == NULL) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: mkdir: missing argument" COLOR_RESET "\n");
        return 1;
    }
    
    if (mkdir(args[1], 0755) != 0) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: mkdir: cannot create directory: " COLOR_RESET);
        write_str(args[1]);
        write_str("\n");
        return 1;
    }
    
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

static int builtin_rm(char **args) {
    if (args[1] == NULL) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: rm: missing argument" COLOR_RESET "\n");
        return 1;
    }
    
    int force = 0;
    int recursive = 0;
    int arg_idx = 1;
    
    // Parse flags
    while (args[arg_idx] != NULL && args[arg_idx][0] == '-') {
        for (int i = 1; args[arg_idx][i] != '\0'; i++) {
            if (args[arg_idx][i] == 'f') {
                force = 1;
            } else if (args[arg_idx][i] == 'r' || args[arg_idx][i] == 'R') {
                recursive = 1;
            }
        }
        arg_idx++;
    }
    
    // Check if we have a path argument
    if (args[arg_idx] == NULL) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: rm: missing operand" COLOR_RESET "\n");
        return 1;
    }
    
    // Process each path argument
    while (args[arg_idx] != NULL) {
        const char *path = args[arg_idx];
        struct stat st;
        
        if (stat(path, &st) != 0) {
            if (!force) {
                write_str(ERDEMOS_ERROR_COLOR "ersh: rm: cannot stat: " COLOR_RESET);
                write_str(path);
                write_str("\n");
                return 1;
            }
            arg_idx++;
            continue;
        }
        
        if (S_ISDIR(st.st_mode)) {
            if (!recursive) {
                write_str(ERDEMOS_ERROR_COLOR "ersh: rm: cannot remove directory (use -r): " COLOR_RESET);
                write_str(path);
                write_str("\n");
                if (!force) return 1;
            } else {
                if (remove_directory_recursive(path) != 0) {
                    if (!force) {
                        write_str(ERDEMOS_ERROR_COLOR "ersh: rm: cannot remove directory: " COLOR_RESET);
                        write_str(path);
                        write_str("\n");
                        return 1;
                    }
                }
            }
        } else {
            if (unlink(path) != 0) {
                if (!force) {
                    write_str(ERDEMOS_ERROR_COLOR "ersh: rm: cannot remove file: " COLOR_RESET);
                    write_str(path);
                    write_str("\n");
                    return 1;
                }
            }
        }
        
        arg_idx++;
    }
    
    return 0;
}

static int remove_directory_recursive(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return -1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Build full path
        char fullpath[1024];
        int written = 0;
        const char *p = path;
        while (*p && written < 1023) {
            fullpath[written++] = *p++;
        }
        if (written < 1023 && fullpath[written-1] != '/') {
            fullpath[written++] = '/';
        }
        p = entry->d_name;
        while (*p && written < 1023) {
            fullpath[written++] = *p++;
        }
        fullpath[written] = '\0';
        
        struct stat st;
        if (stat(fullpath, &st) != 0) {
            closedir(dir);
            return -1;
        }
        
        if (S_ISDIR(st.st_mode)) {
            if (remove_directory_recursive(fullpath) != 0) {
                closedir(dir);
                return -1;
            }
        } else {
            if (unlink(fullpath) != 0) {
                closedir(dir);
                return -1;
            }
        }
    }
    
    closedir(dir);
    return rmdir(path);
}

static int builtin_touch(char **args) {
    if (args[1] == NULL) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: touch: missing argument" COLOR_RESET "\n");
        return 1;
    }
    
    int fd = open(args[1], O_WRONLY | O_CREAT | O_NOCTTY | O_NONBLOCK, 0666);
    if (fd < 0) {
        write_str(ERDEMOS_ERROR_COLOR "ersh: touch: cannot create file: " COLOR_RESET);
        write_str(args[1]);
        write_str("\n");
        return 1;
    }
    
    close(fd);
    return 0;
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
    if (strcmp(args[0], "ls") == 0) {
        return builtin_ls(args);
    }
    if (strcmp(args[0], "mkdir") == 0) {
        return builtin_mkdir(args);
    }
    if (strcmp(args[0], "poweroff") == 0) {
        return builtin_poweroff(args);
    }
    if (strcmp(args[0], "pwd") == 0) {
        return builtin_pwd(args);
    }
    if (strcmp(args[0], "rm") == 0) {
        return builtin_rm(args);
    }
    if (strcmp(args[0], "touch") == 0) {
        return builtin_touch(args);
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

    write_str(ERDEMOS_PRIMARY_COLOR "\n" "Type " ERDEMOS_COMMAND_COLOR "'help'" ERDEMOS_PRIMARY_COLOR " for built-in commands" COLOR_RESET "\n\n");

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

