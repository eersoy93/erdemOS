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
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/kd.h>
#include "../include/colors.h"
#include "../include/version.h"

// Add proper process reaping
void sigchld_handler(int sig) {
    (void)sig;  // Unused parameter
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void) {
    // Set console to Unicode (UTF-8) mode
    int console_fd = open("/dev/console", O_RDWR);
    if (console_fd < 0) {
        console_fd = open("/dev/tty", O_RDWR);
        if (console_fd < 0) {
            console_fd = open("/dev/tty0", O_RDWR);
        }
    }
    
    if (console_fd >= 0) {
        // Set UTF-8 mode (KD_UNICODE)
        ioctl(console_fd, KDSKBMODE, K_UNICODE);
        close(console_fd);
    }
    
    // Clear screen using ANSI escape code
    const char clear[] = "\033[2J\033[H";
    ssize_t ret = write(1, clear, sizeof(clear) - 1);
    if (ret < 0) {
        return 1;
    }
    
    // Print message
    const char msg[] = ERDEMOS_PRIMARY_COLOR "Welcome to erdemOS " ERDEMOS_VERSION "!\n";
    ret = write(1, msg, sizeof(msg) - 1);
    if (ret < 0) {
        return 1;
    }
    
    // Setup signal handlers
    signal(SIGCHLD, sigchld_handler);
    
    // Load English keyboard layout before starting shell
    pid_t kbd_pid = fork();
    if (kbd_pid == 0) {
        // Child process - load keyboard
        execl("/bin/loadkeys", "loadkeys", "us", NULL);
        _exit(1);
    } else if (kbd_pid > 0) {
        // Wait for loadkeys to complete
        waitpid(kbd_pid, NULL, 0);
    }
    
    // Fork and exec a shell
    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/ersh", "ersh", NULL);
        _exit(1);
    }
    
    // Wait for children instead of infinite loop
    while(1) {
        pause();
    }
}
