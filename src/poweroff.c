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
#include <sys/reboot.h>
#include <linux/reboot.h>
#include <string.h>
#include "../include/colors.h"

int main(void) {
    const char msg[] = ERDEMOS_ERROR_COLOR "Power off..." COLOR_RESET "\n";
    ssize_t ret = write(1, msg, sizeof(msg) - 1);
    (void)ret;
    
    sync();
    reboot(LINUX_REBOOT_CMD_POWER_OFF);
    
    return 0;
}
