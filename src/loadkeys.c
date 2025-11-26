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

// Helper macros for key types
#define LETTER(c) K(KT_LETTER, (c))

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
    
    // Normal (unshifted) keys - Table 0
    entry.kb_table = 0;
    
    // Scan code 0x1a (US: [) -> Turkish: ğ (letter type)
    entry.kb_index = 0x1a;
    entry.kb_value = LETTER(0xf0); // ğ
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x1b (US: ]) -> Turkish: ü (letter type)
    entry.kb_index = 0x1b;
    entry.kb_value = LETTER(0xfc); // ü
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x27 (US: ;) -> Turkish: ş (letter type)
    entry.kb_index = 0x27;
    entry.kb_value = LETTER(0xfe); // ş
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x28 (US: ') -> Turkish: i (letter type)
    entry.kb_index = 0x28;
    entry.kb_value = LETTER('i');
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x29 (US: `) -> Turkish: "
    entry.kb_index = 0x29;
    entry.kb_value = '"';
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x2b (US: \) -> Turkish: ,
    entry.kb_index = 0x2b;
    entry.kb_value = ',';
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x33 (US: ,) -> Turkish: ö (letter type)
    entry.kb_index = 0x33;
    entry.kb_value = LETTER(0xf6); // ö
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x34 (US: .) -> Turkish: ç (letter type)
    entry.kb_index = 0x34;
    entry.kb_value = LETTER(0xe7); // ç
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x35 (US: /) -> Turkish: .
    entry.kb_index = 0x35;
    entry.kb_value = '.';
    ioctl(fd, KDSKBENT, &entry);
    
    // Numbers row - unshifted (Table 0)
    entry.kb_index = 0x02; entry.kb_value = '1'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '2'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '3'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '4'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '5'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '6'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '7'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '8'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = '9'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = '0'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '*'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '-'; ioctl(fd, KDSKBENT, &entry);
    
    // Shifted keys - Table 1
    entry.kb_table = 1;
    
    // Numbers row - shifted (Turkish Q specific)
    entry.kb_index = 0x02; entry.kb_value = '!'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '\''; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '^'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '+'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '%'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '&'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '/'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '('; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = ')'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = '='; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '?'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '_'; ioctl(fd, KDSKBENT, &entry);
    
    // Letter keys shifted (use letter type)
    
    // Scan code 0x1a -> Ğ (uppercase letter type)
    entry.kb_index = 0x1a;
    entry.kb_value = LETTER(0xd0); // Ğ
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x1b -> Ü (uppercase letter type)
    entry.kb_index = 0x1b;
    entry.kb_value = LETTER(0xdc); // Ü
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x27 -> Ş (uppercase letter type)
    entry.kb_index = 0x27;
    entry.kb_value = LETTER(0xde); // Ş
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x28 -> İ (uppercase letter type)
    entry.kb_index = 0x28;
    entry.kb_value = LETTER(0xdd); // İ
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x29 -> é
    entry.kb_index = 0x29;
    entry.kb_value = 0xe9; // é
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x2b -> ;
    entry.kb_index = 0x2b;
    entry.kb_value = ';';
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x33 -> Ö (uppercase letter type)
    entry.kb_index = 0x33;
    entry.kb_value = LETTER(0xd6); // Ö
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x34 -> Ç (uppercase letter type)
    entry.kb_index = 0x34;
    entry.kb_value = LETTER(0xc7); // Ç
    ioctl(fd, KDSKBENT, &entry);
    
    // Scan code 0x35 -> :
    entry.kb_index = 0x35;
    entry.kb_value = ':';
    ioctl(fd, KDSKBENT, &entry);
    
    // Caps Lock table (Table 2) - uppercase for letters, normal for symbols
    entry.kb_table = 2;
    
    // All standard letter keys uppercase (QWERTY row)
    entry.kb_index = 0x10; entry.kb_value = 'Q'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = 'W'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = 'E'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x13; entry.kb_value = 'R'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x14; entry.kb_value = 'T'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = 'Y'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = 'U'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = 'I'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = 'O'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = 'P'; ioctl(fd, KDSKBENT, &entry);
    
    // Turkish-specific keys (uppercase)
    entry.kb_index = 0x1a; entry.kb_value = 0xd0; ioctl(fd, KDSKBENT, &entry); // Ğ
    entry.kb_index = 0x1b; entry.kb_value = 0xdc; ioctl(fd, KDSKBENT, &entry); // Ü
    
    // ASDF row uppercase
    entry.kb_index = 0x1e; entry.kb_value = 'A'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 'S'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = 'D'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'F'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 'G'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = 'H'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'J'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'K'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'L'; ioctl(fd, KDSKBENT, &entry);
    
    // Turkish-specific keys
    entry.kb_index = 0x27; entry.kb_value = 0xde; ioctl(fd, KDSKBENT, &entry); // Ş
    entry.kb_index = 0x28; entry.kb_value = 0xdd; ioctl(fd, KDSKBENT, &entry); // İ
    
    // ZXCV row uppercase
    entry.kb_index = 0x2c; entry.kb_value = 'Z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 'X'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = 'C'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'V'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 'B'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = 'N'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 'M'; ioctl(fd, KDSKBENT, &entry);
    
    // Turkish-specific keys
    entry.kb_index = 0x33; entry.kb_value = 0xd6; ioctl(fd, KDSKBENT, &entry); // Ö
    entry.kb_index = 0x34; entry.kb_value = 0xc7; ioctl(fd, KDSKBENT, &entry); // Ç
    
    // Symbols stay normal with Caps Lock
    entry.kb_index = 0x29; entry.kb_value = '"'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2b; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    
    // Numbers stay normal with Caps Lock
    entry.kb_index = 0x02; entry.kb_value = '1'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '2'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '3'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '4'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '5'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '6'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '7'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '8'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = '9'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = '0'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '*'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '-'; ioctl(fd, KDSKBENT, &entry);
    
    // Shift+Caps Lock table (Table 3) - inverts: lowercase letters, shifted symbols
    entry.kb_table = 3;
    
    // All standard letter keys lowercase (QWERTY row)
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
    
    // Turkish-specific keys (lowercase)
    entry.kb_index = 0x1a; entry.kb_value = 0xf0; ioctl(fd, KDSKBENT, &entry); // ğ
    entry.kb_index = 0x1b; entry.kb_value = 0xfc; ioctl(fd, KDSKBENT, &entry); // ü
    
    // ASDF row lowercase
    entry.kb_index = 0x1e; entry.kb_value = 'a'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 's'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = 'd'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'f'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 'g'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = 'h'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'j'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'k'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'l'; ioctl(fd, KDSKBENT, &entry);
    
    // Turkish-specific keys
    entry.kb_index = 0x27; entry.kb_value = 0xfe; ioctl(fd, KDSKBENT, &entry); // ş
    entry.kb_index = 0x28; entry.kb_value = 'i'; ioctl(fd, KDSKBENT, &entry);
    
    // ZXCV row lowercase
    entry.kb_index = 0x2c; entry.kb_value = 'z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 'x'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = 'c'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'v'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 'b'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = 'n'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 'm'; ioctl(fd, KDSKBENT, &entry);
    
    // Turkish-specific keys
    entry.kb_index = 0x33; entry.kb_value = 0xf6; ioctl(fd, KDSKBENT, &entry); // ö
    entry.kb_index = 0x34; entry.kb_value = 0xe7; ioctl(fd, KDSKBENT, &entry); // ç
    
    // Shifted symbols with Caps Lock
    entry.kb_index = 0x29; entry.kb_value = 0xe9; ioctl(fd, KDSKBENT, &entry); // é
    entry.kb_index = 0x2b; entry.kb_value = ';'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = ':'; ioctl(fd, KDSKBENT, &entry);
    
    // Numbers become shifted symbols with Shift+Caps
    entry.kb_index = 0x02; entry.kb_value = '!'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '\''; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '^'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '+'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '%'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '&'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '/'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '('; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = ')'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = '='; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '?'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '_'; ioctl(fd, KDSKBENT, &entry);
}

// Turkish F layout modifications
static void load_turkish_f(int fd) {
    struct kbentry entry;
    
    // Turkish F is a different layout with keys rearranged
    
    // Normal (unshifted) keys - Table 0
    entry.kb_table = 0;
    
    // Row 1 (QWERTY row becomes FGĞIOD) - use letter type
    entry.kb_index = 0x10; entry.kb_value = LETTER('f'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = LETTER('g'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = LETTER(0xf0); ioctl(fd, KDSKBENT, &entry); // ğ
    entry.kb_index = 0x13; entry.kb_value = LETTER(0xfd); ioctl(fd, KDSKBENT, &entry); // ı
    entry.kb_index = 0x14; entry.kb_value = LETTER('o'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = LETTER('d'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = LETTER('r'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = LETTER('n'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = LETTER('h'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = LETTER('p'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = LETTER('q'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = LETTER('w'); ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 (ASDF row becomes UİEAÜT) - use letter type
    entry.kb_index = 0x1e; entry.kb_value = LETTER('u'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = LETTER('i'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = LETTER('e'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = LETTER('a'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = LETTER(0xfc); ioctl(fd, KDSKBENT, &entry); // ü
    entry.kb_index = 0x23; entry.kb_value = LETTER('t'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = LETTER('k'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = LETTER('m'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = LETTER('l'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = LETTER('y'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = LETTER(0xfe); ioctl(fd, KDSKBENT, &entry); // ş
    entry.kb_index = 0x2b; entry.kb_value = LETTER('x'); ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 (ZXCV row becomes JÖVCÇ) - use letter type
    entry.kb_index = 0x2c; entry.kb_value = LETTER('j'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = LETTER(0xf6); ioctl(fd, KDSKBENT, &entry); // ö
    entry.kb_index = 0x2e; entry.kb_value = LETTER('v'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = LETTER('c'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = LETTER(0xe7); ioctl(fd, KDSKBENT, &entry); // ç
    entry.kb_index = 0x31; entry.kb_value = LETTER('z'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = LETTER('s'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = LETTER('b'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
    
    // Shifted keys - Table 1
    entry.kb_table = 1;
    
    // Row 1 (shifted: FGĞIOD -> uppercase) - use letter type
    entry.kb_index = 0x10; entry.kb_value = LETTER('F'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = LETTER('G'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = LETTER(0xd0); ioctl(fd, KDSKBENT, &entry); // Ğ
    entry.kb_index = 0x13; entry.kb_value = LETTER('I'); ioctl(fd, KDSKBENT, &entry); // I (capital dotless)
    entry.kb_index = 0x14; entry.kb_value = LETTER('O'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = LETTER('D'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = LETTER('R'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = LETTER('N'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = LETTER('H'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = LETTER('P'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = LETTER('Q'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = LETTER('W'); ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 (shifted: UİEAÜT -> uppercase) - use letter type
    entry.kb_index = 0x1e; entry.kb_value = LETTER('U'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = LETTER(0xdd); ioctl(fd, KDSKBENT, &entry); // İ
    entry.kb_index = 0x20; entry.kb_value = LETTER('E'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = LETTER('A'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = LETTER(0xdc); ioctl(fd, KDSKBENT, &entry); // Ü
    entry.kb_index = 0x23; entry.kb_value = LETTER('T'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = LETTER('K'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = LETTER('M'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = LETTER('L'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = LETTER('Y'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = LETTER(0xde); ioctl(fd, KDSKBENT, &entry); // Ş
    entry.kb_index = 0x2b; entry.kb_value = LETTER('X'); ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 (shifted: JÖVCÇ -> uppercase) - use letter type
    entry.kb_index = 0x2c; entry.kb_value = LETTER('J'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = LETTER(0xd6); ioctl(fd, KDSKBENT, &entry); // Ö
    entry.kb_index = 0x2e; entry.kb_value = LETTER('V'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = LETTER('C'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = LETTER(0xc7); ioctl(fd, KDSKBENT, &entry); // Ç
    entry.kb_index = 0x31; entry.kb_value = LETTER('Z'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = LETTER('S'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = LETTER('B'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = ':'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = ';'; ioctl(fd, KDSKBENT, &entry);
    
    // Caps Lock table (Table 2) - uppercase for letters, normal for symbols
    entry.kb_table = 2;
    
    // Row 1 - uppercase
    entry.kb_index = 0x10; entry.kb_value = 'F'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = 'G'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = 0xd0; ioctl(fd, KDSKBENT, &entry); // Ğ
    entry.kb_index = 0x13; entry.kb_value = 'I'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x14; entry.kb_value = 'O'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = 'D'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = 'R'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = 'N'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = 'H'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = 'P'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = 'Q'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = 'W'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 - uppercase
    entry.kb_index = 0x1e; entry.kb_value = 'U'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 0xdd; ioctl(fd, KDSKBENT, &entry); // İ
    entry.kb_index = 0x20; entry.kb_value = 'E'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'A'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 0xdc; ioctl(fd, KDSKBENT, &entry); // Ü
    entry.kb_index = 0x23; entry.kb_value = 'T'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'K'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'M'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'L'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = 'Y'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = 0xde; ioctl(fd, KDSKBENT, &entry); // Ş
    entry.kb_index = 0x2b; entry.kb_value = 'X'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 - uppercase
    entry.kb_index = 0x2c; entry.kb_value = 'J'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 0xd6; ioctl(fd, KDSKBENT, &entry); // Ö
    entry.kb_index = 0x2e; entry.kb_value = 'V'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'C'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 0xc7; ioctl(fd, KDSKBENT, &entry); // Ç
    entry.kb_index = 0x31; entry.kb_value = 'Z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 'S'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = 'B'; ioctl(fd, KDSKBENT, &entry);
    
    // Symbols stay normal
    entry.kb_index = 0x34; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
    
    // Shift+Caps Lock table (Table 3) - inverts: lowercase with Shift+Caps
    entry.kb_table = 3;
    
    // Row 1 - lowercase
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
    
    // Row 2 - lowercase
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
    
    // Row 3 - lowercase
    entry.kb_index = 0x2c; entry.kb_value = 'j'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 0xf6; ioctl(fd, KDSKBENT, &entry); // ö
    entry.kb_index = 0x2e; entry.kb_value = 'v'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'c'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 0xe7; ioctl(fd, KDSKBENT, &entry); // ç
    entry.kb_index = 0x31; entry.kb_value = 'z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 's'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = 'b'; ioctl(fd, KDSKBENT, &entry);
    
    // Symbols become shifted versions
    entry.kb_index = 0x34; entry.kb_value = ':'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = ';'; ioctl(fd, KDSKBENT, &entry);
}

// US English layout (restore defaults)
static void load_us_english(int fd) {
    struct kbentry entry;
    
    // Normal (unshifted) keys - Table 0
    entry.kb_table = 0;
    
    // Row 1 (letter type for a-z)
    entry.kb_index = 0x10; entry.kb_value = LETTER('q'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = LETTER('w'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = LETTER('e'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x13; entry.kb_value = LETTER('r'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x14; entry.kb_value = LETTER('t'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = LETTER('y'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = LETTER('u'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = LETTER('i'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = LETTER('o'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = LETTER('p'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = '['; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = ']'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 (letter type for a-z)
    entry.kb_index = 0x1e; entry.kb_value = LETTER('a'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = LETTER('s'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = LETTER('d'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = LETTER('f'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = LETTER('g'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = LETTER('h'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = LETTER('j'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = LETTER('k'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = LETTER('l'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = ';'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = '\''; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x29; entry.kb_value = '`'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2b; entry.kb_value = '\\'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 (letter type for a-z)
    entry.kb_index = 0x2c; entry.kb_value = LETTER('z'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = LETTER('x'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = LETTER('c'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = LETTER('v'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = LETTER('b'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = LETTER('n'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = LETTER('m'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = '/'; ioctl(fd, KDSKBENT, &entry);
    
    // Shifted keys - Table 1
    entry.kb_table = 1;
    
    // Row 1 (shifted - letter type)
    entry.kb_index = 0x10; entry.kb_value = LETTER('Q'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = LETTER('W'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = LETTER('E'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x13; entry.kb_value = LETTER('R'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x14; entry.kb_value = LETTER('T'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = LETTER('Y'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = LETTER('U'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = LETTER('I'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = LETTER('O'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = LETTER('P'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = '{'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = '}'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 (shifted - letter type)
    entry.kb_index = 0x1e; entry.kb_value = LETTER('A'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = LETTER('S'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = LETTER('D'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = LETTER('F'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = LETTER('G'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = LETTER('H'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = LETTER('J'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = LETTER('K'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = LETTER('L'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = ':'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = '"'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x29; entry.kb_value = '~'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2b; entry.kb_value = '|'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 (shifted - letter type)
    entry.kb_index = 0x2c; entry.kb_value = LETTER('Z'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = LETTER('X'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = LETTER('C'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = LETTER('V'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = LETTER('B'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = LETTER('N'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = LETTER('M'); ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = '<'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '>'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = '?'; ioctl(fd, KDSKBENT, &entry);
    
    // Numbers row - unshifted (Table 0)
    entry.kb_table = 0;
    entry.kb_index = 0x02; entry.kb_value = '1'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '2'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '3'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '4'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '5'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '6'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '7'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '8'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = '9'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = '0'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '-'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '='; ioctl(fd, KDSKBENT, &entry);
    
    // Numbers row - shifted (Table 1)
    entry.kb_table = 1;
    entry.kb_index = 0x02; entry.kb_value = '!'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '@'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '#'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '$'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '%'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '^'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '&'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '*'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = '('; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = ')'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '_'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '+'; ioctl(fd, KDSKBENT, &entry);
    
    // Caps Lock table (Table 2) - uppercase for letters, normal for symbols/numbers
    entry.kb_table = 2;
    
    // Row 1 - uppercase letters
    entry.kb_index = 0x10; entry.kb_value = 'Q'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x11; entry.kb_value = 'W'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x12; entry.kb_value = 'E'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x13; entry.kb_value = 'R'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x14; entry.kb_value = 'T'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x15; entry.kb_value = 'Y'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x16; entry.kb_value = 'U'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x17; entry.kb_value = 'I'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x18; entry.kb_value = 'O'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x19; entry.kb_value = 'P'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1a; entry.kb_value = '['; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = ']'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 - uppercase letters
    entry.kb_index = 0x1e; entry.kb_value = 'A'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 'S'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = 'D'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'F'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 'G'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = 'H'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'J'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'K'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'L'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = ';'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = '\''; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x29; entry.kb_value = '`'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2b; entry.kb_value = '\\'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 - uppercase letters
    entry.kb_index = 0x2c; entry.kb_value = 'Z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 'X'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = 'C'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'V'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 'B'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = 'N'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 'M'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = ','; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '.'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = '/'; ioctl(fd, KDSKBENT, &entry);
    
    // Numbers stay normal with Caps Lock
    entry.kb_index = 0x02; entry.kb_value = '1'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '2'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '3'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '4'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '5'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '6'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '7'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '8'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = '9'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = '0'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '-'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '='; ioctl(fd, KDSKBENT, &entry);
    
    // Shift+Caps Lock table (Table 3) - lowercase letters, shifted symbols
    entry.kb_table = 3;
    
    // Row 1 - lowercase letters (inverted)
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
    entry.kb_index = 0x1a; entry.kb_value = '{'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1b; entry.kb_value = '}'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 2 - lowercase letters (inverted)
    entry.kb_index = 0x1e; entry.kb_value = 'a'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x1f; entry.kb_value = 's'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x20; entry.kb_value = 'd'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x21; entry.kb_value = 'f'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x22; entry.kb_value = 'g'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x23; entry.kb_value = 'h'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x24; entry.kb_value = 'j'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x25; entry.kb_value = 'k'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x26; entry.kb_value = 'l'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x27; entry.kb_value = ':'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x28; entry.kb_value = '"'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x29; entry.kb_value = '~'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2b; entry.kb_value = '|'; ioctl(fd, KDSKBENT, &entry);
    
    // Row 3 - lowercase letters (inverted)
    entry.kb_index = 0x2c; entry.kb_value = 'z'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2d; entry.kb_value = 'x'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2e; entry.kb_value = 'c'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x2f; entry.kb_value = 'v'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x30; entry.kb_value = 'b'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x31; entry.kb_value = 'n'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x32; entry.kb_value = 'm'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x33; entry.kb_value = '<'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x34; entry.kb_value = '>'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x35; entry.kb_value = '?'; ioctl(fd, KDSKBENT, &entry);
    
    // Numbers become shifted symbols with Shift+Caps
    entry.kb_index = 0x02; entry.kb_value = '!'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x03; entry.kb_value = '@'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x04; entry.kb_value = '#'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x05; entry.kb_value = '$'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x06; entry.kb_value = '%'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x07; entry.kb_value = '^'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x08; entry.kb_value = '&'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x09; entry.kb_value = '*'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0a; entry.kb_value = '('; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0b; entry.kb_value = ')'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0c; entry.kb_value = '_'; ioctl(fd, KDSKBENT, &entry);
    entry.kb_index = 0x0d; entry.kb_value = '+'; ioctl(fd, KDSKBENT, &entry);
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
