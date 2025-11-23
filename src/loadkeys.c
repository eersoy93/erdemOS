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
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include "../include/colors.h"

// Forward declaration for isatty if not available
#ifndef _UNISTD_H
extern int isatty(int fd);
#endif

// Simple write wrapper
static void write_str(const char *str) {
    ssize_t ret = write(2, str, strlen(str));
    (void)ret;
}

// Keymap structure to hold plain, shift, and altgr tables
struct keymap {
    unsigned short plain[128];
    unsigned short shift[128];
    unsigned short altgr[128];
};

// Turkish Q layout keymap
static struct keymap trq_keymap = {
    .plain = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '-', 127, 9,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 0x0131, 'o', 'p', 0x011f, 0x00fc, 13, 0,  // ı(U+0131), ğ(U+011F), ü(U+00FC)
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0x015f, 'i', '"', 0, '<',  // ş(U+015F)
        'z', 'x', 'c', 'v', 'b', 'n', 'm', 0x00f6, 0x00e7, '.', 0, '*', 0, ' ',   // ö(U+00F6), ç(U+00E7)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .shift = {
        0, 27, '!', '\'', '^', '+', '%', '&', '/', '(', ')', '=', '?', '_', 127, 9,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0x011e, 0x00dc, 13, 0,  // İ(U+0130), Ğ(U+011E), Ü(U+00DC)
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0x015e, 0x0130, 0x00e9, 0, '>',  // Ş(U+015E), İ(U+0130), é(U+00E9)
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', 0x00d6, 0x00c7, ':', 0, '*', 0, ' ',   // Ö(U+00D6), Ç(U+00C7)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .altgr = {
        0, 27, 0, '@', '#', '$', 0xbd, 0, '{', '[', ']', '}', '\\', '|', 127, 9,  // ½(U+00BD)
        '@', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '~', 13, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '`', 0, '|',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '*', 0, ' ',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

// Turkish F layout keymap
static struct keymap trf_keymap = {
    .plain = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '/', '-', 127, 9,
        'f', 'g', 0x011f, 0x0131, 'o', 'd', 'r', 'n', 'h', 'p', 'q', 'w', 13, 0,  // ğ(U+011F), ı(U+0131)
        'u', 'i', 'e', 'a', 0x00fc, 't', 'k', 'm', 'l', 'y', 0x015f, 'x', 0, '<',  // ü(U+00FC), ş(U+015F)
        'j', 0x00f6, 'v', 0x00e7, 'z', 's', 'b', '.', ',', 0, 0, '*', 0, ' ',   // ö(U+00F6), ç(U+00E7)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .shift = {
        0, 27, '!', '"', '^', '+', '%', '&', '\'', '(', ')', '=', '?', '_', 127, 9,
        'F', 'G', 0x011e, 0x0130, 'O', 'D', 'R', 'N', 'H', 'P', 'Q', 'W', 13, 0,  // Ğ(U+011E), İ(U+0130)
        'U', 0x0130, 'E', 'A', 0x00dc, 'T', 'K', 'M', 'L', 'Y', 0x015e, 'X', 0, '>',  // İ(U+0130), Ü(U+00DC), Ş(U+015E)
        'J', 0x00d6, 'V', 0x00c7, 'Z', 'S', 'B', ':', ';', 0, 0, '*', 0, ' ',   // Ö(U+00D6), Ç(U+00C7)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .altgr = {
        0, 27, 0, '@', '#', '$', 0xbd, 0, '{', '[', ']', '}', '\\', '|', 127, 9,
        '@', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '~', 13, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '`', 0, '|',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '*', 0, ' ',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

// US (English) layout keymap
static struct keymap us_keymap = {
    .plain = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 127, 9,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13, 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .shift = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 127, 9,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 13, 0,
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    .altgr = {
        0, 27, 0, '@', '#', '$', 0, 0, '{', '[', ']', '}', '\\', '|', 127, 9,
        '@', 0, 0, 0, 0, 0, 0, 0, 0, 0, '{', '}', 13, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '`', 0, '|',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '*', 0, ' ',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

static int load_keymap(int fd, struct keymap *keymap) {
    struct kbentry entry;
    
    // Linux kernel table mapping uses bitwise OR of (1 << KG_*) values:
    // Table 0 = 0: Plain (no modifiers)
    // Table 1 = (1 << KG_SHIFT): Shift only
    // Table 2 = (1 << KG_ALTGR): AltGr only  
    // Table 3 = (1 << KG_SHIFT) | (1 << KG_ALTGR): Shift+AltGr
    // Table 4 = (1 << KG_CTRL): Control only
    // Table 5 = (1 << KG_CTRL) | (1 << KG_SHIFT): Control+Shift
    // Etc.
    //
    // The standard loadkeys loads only essential tables and lets kernel handle combinations
    
    // Load plain keymap (table 0 - no modifiers)
    for (int i = 0; i < 128; i++) {
        entry.kb_table = 0;
        entry.kb_index = i;
        entry.kb_value = keymap->plain[i];
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            // Ignore errors for special keys
            continue;
        }
    }
    
    // Load shift keymap (table 1 = 1 << KG_SHIFT)
    for (int i = 0; i < 128; i++) {
        entry.kb_table = (1 << KG_SHIFT);
        entry.kb_index = i;
        entry.kb_value = keymap->shift[i];
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            // Ignore errors for special keys
            continue;
        }
    }
    
    // Load AltGr keymap (table 2 = 1 << KG_ALTGR)
    for (int i = 0; i < 128; i++) {
        entry.kb_table = (1 << KG_ALTGR);
        entry.kb_index = i;
        entry.kb_value = keymap->altgr[i];
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            // Ignore errors for special keys
            continue;
        }
    }
    
    // Load Shift+AltGr table (table 3 = (1 << KG_SHIFT) | (1 << KG_ALTGR))
    for (int i = 0; i < 128; i++) {
        entry.kb_table = (1 << KG_SHIFT) | (1 << KG_ALTGR);
        entry.kb_index = i;
        entry.kb_value = keymap->altgr[i];
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            // Ignore errors for special keys
            continue;
        }
    }
    
    // Load Control table (table 4 = 1 << KG_CTRL)
    for (int i = 0; i < 128; i++) {
        entry.kb_table = (1 << KG_CTRL);
        entry.kb_index = i;
        entry.kb_value = keymap->plain[i];
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            continue;
        }
    }
    
    // Load Control+Shift table (table 5 = (1 << KG_CTRL) | (1 << KG_SHIFT))
    for (int i = 0; i < 128; i++) {
        entry.kb_table = (1 << KG_CTRL) | (1 << KG_SHIFT);
        entry.kb_index = i;
        entry.kb_value = keymap->shift[i];
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            continue;
        }
    }
    
    // Load CapsShift table (table 8 - CapsLock ON, no other modifiers)
    // KG_CAPSSHIFT = 8, so we use it directly as table index
    // When CapsLock is active, letters should be uppercase
    for (int i = 0; i < 128; i++) {
        entry.kb_table = KG_CAPSSHIFT;
        entry.kb_index = i;
        entry.kb_value = keymap->shift[i];  // Use shift table for uppercase
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            continue;
        }
    }
    
    // Load CapsShift+Shift table (table 9 - CapsLock ON + Shift pressed)
    // When both are active, invert: show lowercase
    for (int i = 0; i < 128; i++) {
        entry.kb_table = KG_CAPSSHIFT | (1 << KG_SHIFT);
        entry.kb_index = i;
        entry.kb_value = keymap->plain[i];  // Use plain table for lowercase (inverted)
        
        if (ioctl(fd, KDSKBENT, &entry) < 0) {
            continue;
        }
    }
    
    // Set modifier keys to work in all important tables
    // These are the actual modifier key definitions
    int tables[] = {0, (1 << KG_SHIFT), (1 << KG_ALTGR), (1 << KG_CTRL), 
                    (1 << KG_SHIFT) | (1 << KG_ALTGR),
                    (1 << KG_CTRL) | (1 << KG_SHIFT),
                    KG_CAPSSHIFT,
                    KG_CAPSSHIFT | (1 << KG_SHIFT)};
    
    for (int t = 0; t < 8; t++) {
        int table = tables[t];
        
        // Left Shift is scancode 42
        entry.kb_table = table;
        entry.kb_index = 42;
        entry.kb_value = K(KT_SHIFT, KG_SHIFT);
        ioctl(fd, KDSKBENT, &entry);
        
        // Right Shift is scancode 54
        entry.kb_table = table;
        entry.kb_index = 54;
        entry.kb_value = K(KT_SHIFT, KG_SHIFT);
        ioctl(fd, KDSKBENT, &entry);
        
        // Set Control key (scancode 29 - left ctrl)
        entry.kb_table = table;
        entry.kb_index = 29;
        entry.kb_value = K(KT_SHIFT, KG_CTRL);
        ioctl(fd, KDSKBENT, &entry);
        
        // Set Alt key (scancode 56 - left alt)
        entry.kb_table = table;
        entry.kb_index = 56;
        entry.kb_value = K(KT_SHIFT, KG_ALT);
        ioctl(fd, KDSKBENT, &entry);
        
        // Set Right Alt (Alt Gr) key (scancode 100 - right alt)
        entry.kb_table = table;
        entry.kb_index = 100;
        entry.kb_value = K(KT_SHIFT, KG_ALTGR);
        ioctl(fd, KDSKBENT, &entry);
        
        // Set Caps Lock key (scancode 58)
        // Use KT_LOCK to make it toggle (not momentary)
        entry.kb_table = table;
        entry.kb_index = 58;
        entry.kb_value = K(KT_LOCK, KG_CAPSSHIFT);
        ioctl(fd, KDSKBENT, &entry);
    }
    
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        write_str(ERDEMOS_ERROR_COLOR "Usage: loadkeys [layout]" COLOR_RESET "\n");
        write_str(ERDEMOS_PRIMARY_COLOR "Supported layouts: " ERDEMOS_COMMAND_COLOR "trq" ERDEMOS_PRIMARY_COLOR ", " ERDEMOS_COMMAND_COLOR "trf" ERDEMOS_PRIMARY_COLOR ", " ERDEMOS_COMMAND_COLOR "us" COLOR_RESET "\n");
        return 1;
    }
    
    const char *layout = argv[1];
    struct keymap *keymap = NULL;
    const char *layout_name = NULL;
    
    // Select keymap based on layout
    if (strcmp(layout, "trq") == 0) {
        keymap = &trq_keymap;
        layout_name = "Turkish Q";
    } else if (strcmp(layout, "trf") == 0) {
        keymap = &trf_keymap;
        layout_name = "Turkish F";
    } else if (strcmp(layout, "us") == 0) {
        keymap = &us_keymap;
        layout_name = "English (US)";
    } else {
        write_str(ERDEMOS_ERROR_COLOR "loadkeys: unknown layout: " COLOR_RESET);
        write_str(layout);
        write_str("\n");
        write_str(ERDEMOS_PRIMARY_COLOR "Supported layouts: " ERDEMOS_COMMAND_COLOR "trq" ERDEMOS_PRIMARY_COLOR ", " ERDEMOS_COMMAND_COLOR "trf" ERDEMOS_PRIMARY_COLOR ", " ERDEMOS_COMMAND_COLOR "us" COLOR_RESET "\n");
        return 1;
    }
    
    // Open console - try multiple device files
    int fd = open("/dev/tty", O_RDWR);
    if (fd < 0) {
        fd = open("/dev/tty0", O_RDWR);
        if (fd < 0) {
            fd = open("/dev/console", O_RDWR);
            if (fd < 0) {
                // Try standard input/output
                fd = 0;
                // Check if it's a valid TTY
                if (!isatty(fd)) {
                    write_str(ERDEMOS_ERROR_COLOR "loadkeys: cannot open console device" COLOR_RESET "\n");
                    return 1;
                }
            }
        }
    }
    
    // Load the keymap
    int result = load_keymap(fd, keymap);
    
    if (fd > 0) {
        close(fd);
    }
    
    if (result < 0) {
        write_str(ERDEMOS_ERROR_COLOR "loadkeys: failed to load keymap" COLOR_RESET "\n");
        return 1;
    }
    
    // Success message
    write_str(ERDEMOS_PRIMARY_COLOR "Keyboard layout loaded: " ERDEMOS_COMMAND_COLOR);
    write_str(layout_name);
    write_str(COLOR_RESET "\n");
    
    return 0;
}
