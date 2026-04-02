/*=============================================
utils.h - a file with function prototypes and utilities.
Copyright(c) Kisushka IDG.
Licensed under GNU General Public License(GPL).
GitHub: https://github.com/KisushkaIDG
Telegram:         https://t.me/KisushkaGroup
Russian telegram: https://t.me/KisushkaRUS
==============================================*/

#ifndef DIAMOND_UTILS_H
#define DIAMOND_UTILS_H

#include <stdio.h>
#include "types.h"

/*=====================================
* FUNCTION: validate_dll_path(const har* path)
* Checks if the DLL file path exists.
*======================================*/
BOOL validate_dll_path(const char* path);

/*=====================================
* FUNCTION: is_process_alive(DWORD pid)
* Checks PID and opening a process.
*======================================*/
BOOL is_process_alive(DWORD pid);

// === INJECTING ===
/*=====================================
* FUNCTION: InjectDLL(DWORD pid, const char* dll_path)
* The main function for DLL injection.
*======================================*/
int InjectDLL(DWORD pid, const char* dll_path);

// === UTILS ===
/*=====================================
* FUNCTION: print_diamond_Logo()
* Printing diamond logo on start.
*======================================*/
void print_diamond_logo();

/*=====================================
* FUNCTION: clear_console()
* Cleaning the console window.
*======================================*/
void clear_console();

/*=====================================
* FUNCTION: set_color(int color)
* Set colors for TUI
*======================================*/
void set_color(int color);

#endif /* DIAMOND_UTILS_H */
