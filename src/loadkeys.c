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
#include <fcntl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "../include/colors.h"

// Simple write wrapper
static void write_str(const char *str) {
    ssize_t ret = write(1, str, strlen(str));
    (void)ret;  // Ignore return value intentionally
}

// Turkish Q layout modifications (scan code -> key code mapping)
static void load_turkish_q(int fd) {
    struct kbentry entry;
    
    // Turkish specific keys for Turkish Q layout
    // These are common modifications from US layout
    
    // Scan code 0x1a (US: [) -> Turkish: ğ
    entry.kb_table = 0; // Normal
    entry.kb_index = 0x1a;
    entry.kb_value = 0xf0; // ğ
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x1b (US: ]) -> Turkish: ü
    entry.kb_index = 0x1b;
    entry.kb_value = 0xfc; // ü
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x27 (US: ;) -> Turkish: ş
    entry.kb_index = 0x27;
    entry.kb_value = 0xfe; // ş
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x28 (US: ') -> Turkish: i
    entry.kb_index = 0x28;
    entry.kb_value = 'i';
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x29 (US: `) -> Turkish: "
    entry.kb_index = 0x29;
    entry.kb_value = '"';
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x2b (US: \) -> Turkish: ,
    entry.kb_index = 0x2b;
    entry.kb_value = ',';
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x33 (US: ,) -> Turkish: ö
    entry.kb_index = 0x33;
    entry.kb_value = 0xf6; // ö
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x34 (US: .) -> Turkish: ç
    entry.kb_index = 0x34;
    entry.kb_value = 0xe7; // ç
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x35 (US: /) -> Turkish: .
    entry.kb_index = 0x35;
    entry.kb_value = '.';
    ioctl(fd, KDSKBENT, &entry);
}

// Turkish F layout modifications
static void load_turkish_f(int fd) {
    struct kbentry entry;
    entry.kb_table = 0; // Normal
    
    // Turkish F is a different layout with keys rearranged
    // This is a simplified version
    
    // Row 1 (QWERTY row becomes FGĞIOD)
    entry.kb_index = 0x10; entry.kb_value = 'f'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = 'g'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = 0xf0; ioctl(fd, KDSKBENT, &entry); // ğ
    entry.kb_index = 0x13; entry.kb_value = 0xfd; ioctl(fd, KDSKBENT, &entry); // ı
    entry.kb_index = 0x14; entry.kb_value = 'o'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = 'd'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = 'r'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = 'n'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = 'h'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = 'p'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = 'q'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = 'w'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 (ASDF row becomes UİEAÜT)
    entry.kb_index = 0x1e; entry.kb_value = 'u'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 'i'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = 'e'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'a'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 0xfc; ioctl(fd, KDSKBENT, &entry); // ü
    entry.kb_index = 0x23; entry.kb_value = 't'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'k'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'm'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'l'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = 'y'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = 0xfe; ioctl(fd, KDSKBENT, &entry); // ş
    entry.kb_index = 0x2b; entry.kb_value = 'x'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 (ZXCV row becomes JÖVCÇ)
    entry.kb_index = 0x2c; entry.kb_value = 'j'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 0xf6; ioctl(fd, KDSKBENT, &entry); // ö
    entry.kb_index = 0x2e; entry.kb_value = 'v'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'c'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 0xe7; ioctl(fd, KDSKBENT, &entry); // ç
    entry.kb_index = 0x31; entry.kb_value = 'z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 's'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = 'b'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
}

// US English layout (restore defaults)
static void load_us_english(int fd) {
    struct kbentry entry;
    entry.kb_table = 0; // Normal
    
    // Restore standard US QWERTY layout
    // Row 1
    entry.kb_index = 0x10; entry.kb_value = 'q'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = 'w'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = 'e'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x13; entry.kb_value = 'r'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x14; entry.kb_value = 't'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = 'y'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = 'u'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = 'i'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = 'o'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = 'p'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = '['; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = ']'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2
    entry.kb_index = 0x1e; entry.kb_value = 'a'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 's'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = 'd'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'f'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 'g'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = 'h'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'j'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'k'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'l'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = ';'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = '\''; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x29; entry.kb_value = '`'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2b; entry.kb_value = '\\'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3
    entry.kb_index = 0x2c; entry.kb_value = 'z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 'x'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = 'c'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'v'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 'b'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = 'n'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 'm'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = '/'; ioctl(fd, KDSKBENT, &entry);
}

// Load keyboard layout
int main(int argc, char *argv[]) {
    if (argc != 2) {
        write_str(ERDEMOS_ERROR_COLOR "Usage: loadkeys [us|trq|trf]\n" COLOR_RESET);
        write_str(ERDEMOS_PRIMARY_COLOR "  us  - English (US) keyboard layout\n");
        write_str(ERDEMOS_PRIMARY_COLOR "  trq - Turkish Q keyboard layout\n");
        write_str(ERDEMOS_PRIMARY_COLOR "  trf - Turkish F keyboard layout\n" COLOR_RESET);
        return 1;
    }

    const char *layout = argv[1];
    
    // Validate layout parameter
    if (strcmp(layout, "us") != 0 && 
        strcmp(layout, "trq") != 0 && 
        strcmp(layout, "trf") != 0) {
        write_str(ERDEMOS_ERROR_COLOR "loadkeys: invalid layout: " COLOR_RESET);
        write_str(layout);
        write_str("\n");
        write_str(ERDEMOS_PRIMARY_COLOR "Valid layouts: us, trq, trf\n" COLOR_RESET);
        return 1;
    }

    // Open console device
    int fd = open("/dev/console", O_RDWR);
    if (fd < 0) {
        // Try /dev/tty0 as fallback
        fd = open("/dev/tty0", O_RDWR);
        if (fd < 0) {
            // Try current tty
            fd = open("/dev/tty", O_RDWR);
            if (fd < 0) {
                write_str(ERDEMOS_ERROR_COLOR "loadkeys: cannot open console device\n" COLOR_RESET);
                write_str(ERDEMOS_WARNING_COLOR "Note: This command requires console access\n" COLOR_RESET);
                return 1;
            }
        }
    }

    // Load the appropriate keyboard layout
    if (strcmp(layout, "us") == 0) {
        load_us_english(fd);
        write_str(ERDEMOS_PRIMARY_COLOR "Keyboard layout set to: " ERDEMOS_COMMAND_COLOR "English (US)" COLOR_RESET "\n");
    } else if (strcmp(layout, "trq") == 0) {
        // First load US as base, then apply Turkish Q modifications
        load_us_english(fd);
        load_turkish_q(fd);
        write_str(ERDEMOS_PRIMARY_COLOR "Keyboard layout set to: " ERDEMOS_COMMAND_COLOR "Turkish Q" COLOR_RESET "\n");
    } else if (strcmp(layout, "trf") == 0) {
        load_turkish_f(fd);
        write_str(ERDEMOS_PRIMARY_COLOR "Keyboard layout set to: " ERDEMOS_COMMAND_COLOR "Turkish F" COLOR_RESET "\n");
    }

    close(fd);
    return 0;
}
