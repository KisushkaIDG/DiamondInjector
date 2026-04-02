/*=============================================
validate.c - file with validation of the DLL file path and PID logic.
Copyright(c) Kisushka IDG.
Licensed under GNU General Public License(GPL).
GitHub: https://github.com/KisushkaIDG
Telegram:         https://t.me/KisushkaGroup
Russian telegram: https://t.me/KisushkaRUS
==============================================*/

#include <windows.h>
#include <stdio.h>
#include "include\types.h"
#include "include\utils.h"

BOOL validate_dll_path(const char* path) {
    if (path == NULL || strlen(path) == 0) {
        return FALSE;
    }

    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        // File not found, not available or incorrect
        return FALSE;
    }

    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
        // Directory, not a file
        return FALSE;
    }

    return TRUE;
}

BOOL is_process_alive(DWORD pid) {
    if (pid == 0 || pid > 100000) { // Limit on a reasonable PID
        return FALSE;
    }

    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION,
        FALSE,
        pid);

    if (hProcess == NULL) {
        return FALSE;
    }

    CloseHandle(hProcess);
    return TRUE;
}
