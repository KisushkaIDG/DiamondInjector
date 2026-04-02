/*=============================================
main.c - main file.
Copyright(c) Kisushka IDG.
Licensed under GNU General Public License(GPL).
GitHub: https://github.com/KisushkaIDG
Telegram:         https://t.me/KisushkaGroup
Russian telegram: https://t.me/KisushkaRUSs
==============================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "include\types.h"
#include "include\utils.h"

// Set console color
void set_color(int color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void print_diamond_logo() {
    set_color(3);
    printf(" ####   #####   ###   #   #   ####   #   #  ####  \n");
    printf(" #   #    #    #   #  ## ##  #    #  ##  #  #   # \n");
    printf(" #   #    #    #####  # # #  #    #  # # #  #   # \n");
    printf(" #   #    #    #   #  #   #  #    #  #  ##  #   # \n");
    printf(" ####   #####  #   #  #   #   ####   #   #  ####  \n");
    printf("diamond inject Tool v1.0 | dev: KisushkaIDG \n");
}
void clear_console() {
#if defined _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main(int argc, char* argv[]) {
    int result = CMD_FAIL_EXIT;

    /* Enable Ctrl+C handling for proper termination */
    SetConsoleCtrlHandler(NULL, FALSE);

    //Setting the console window title
    SetConsoleTitleA("Diamond Inject v 1.0");

    if (argc >= 3) {
        /* === CLI MODE === */
        clear_console();
        set_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("Diamond Inject Tool v1.0\n");

        char* dll_path = argv[1];
        DWORD pid_str = atoi(argv[2]);

        if (pid_str == 0) {
            set_color(FOREGROUND_RED);
            printf("[ERROR] PID cannot be zero.\n");
            system("pause");
            return CMD_FAIL_EXIT;
        }

        // Validation in CLI mode
        if (!validate_dll_path(dll_path)) {
            set_color(FOREGROUND_RED);
            printf("[ERROR] Invalid DLL path: %s\n", dll_path);
            system("pause");
            return CMD_FAIL_EXIT;
        }

        if (!is_process_alive(pid_str)) {
            set_color(FOREGROUND_RED);
            printf("[ERROR] Process with PID %lu was not found.\n", (unsigned long)pid_str);
            system("pause");
            return CMD_FAIL_EXIT;
        }

        // Calling an inject in CLI mode
        result = InjectDLL((DWORD)pid_str, dll_path);

        if (result == DIAMOND_OK) {
            set_color(FOREGROUND_GREEN);
            printf("Injection successful!\n");
        }
        else {
            system("pause");
        }

    }
    else {
        /* === INTERACTING MODE === */
        while (1) {
            clear_console();
            print_diamond_logo();

            char dll_buffer[MAX_PATH_LEN];
            char pid_str_input[MAX_PATH_LEN];

            // Enter the DLL file path
            set_color(7);
            printf("Enter full path to DLL file >>> ");
            if (!fgets(dll_buffer, MAX_PATH_LEN, stdin)) {
                set_color(FOREGROUND_RED);
                printf("\n[ERROR] Input failed.\n");
                break;
            }
            // Clearing a line break
            char* newline = strchr(dll_buffer, '\n');
            if (newline) *newline = '\0';
            else if (strchr(dll_buffer, '\r')) *strchr(dll_buffer, '\r') = '\0';

            // Entering PIDs
            set_color(7);
            printf("Enter Process ID to inject into >>> ");
            fflush(stdin);
            if (!fgets(pid_str_input, MAX_PATH_LEN, stdin)) {
                break;
            }
            newline = strchr(pid_str_input, '\n');
            if (newline) *newline = '\0';
            else if (strchr(pid_str_input, '\r')) *strchr(pid_str_input, '\r') = '\0';

            DWORD pid = atol(pid_str_input);
            char status[512];

            if (pid <= 0) {
                set_color(FOREGROUND_RED);
                printf("\n[ERROR] Invalid PID format. Must be a positive integer.\n");
                continue;
            }

            // Checking for the existence of a process
            if (!is_process_alive(pid)) {
                set_color(FOREGROUND_RED);
                printf("[ERROR] Process with PID %lu does not exist.\n", (unsigned long)pid);
                continue;
            }

            // Validation of the DLL path
            if (!validate_dll_path(dll_buffer)) {
                set_color(FOREGROUND_RED);
                printf("[ERROR] Invalid DLL path or file does not exist: %s\n", dll_buffer);
                continue;
            }

            // Calling the injection
            int inject_status = InjectDLL(pid, dll_buffer);

            if (inject_status == DIAMOND_OK) {
                set_color(FOREGROUND_GREEN);
                printf("\nDLL injected successfully into PID %lu.\n\n", (unsigned long)pid);
                // We are waiting for pressing Enter to continue
                printf("Press Enter to inject next...\n");
                while (!getchar());
            }
            else {
                // An error, but we continue in the loop.
                set_color(6);
                printf("\n[WARNING] Injection failed or encountered error.\n\n");

                // Waiting for pressing Enter to continue after the error
                set_color(FOREGROUND_INTENSITY);
                printf("Press Enter to continue...\n");
                while (!getchar());
            }
        }
    }

    clear_console();
    return 0;
}
