/*=============================================
inject.c - main injecting functions.
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

// A function for opening a process by PID
HANDLE OpenProcessByPID(DWORD pid) {
    HANDLE hProcess = OpenProcess(
        PROCESS_ACCESS_FLAGS,
        FALSE,
        pid);
    if (hProcess == NULL) {
        set_color(FOREGROUND_RED);
        printf("[!] Failed to open process with PID %lu\n", (unsigned long)pid);
        return INVALID_HANDLE_VALUE;
    }
    return hProcess;
}

// Main injecting function
int InjectDLL(DWORD pid, const char* dll_path) {
    HANDLE hProcess = OpenProcessByPID(pid);
    if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Can't open process with PID %lu.\n", (unsigned long)pid);
        return DIAMOND_ERROR;
    }

    // Checking that the path is correct (it should be a file, not a dir)
    size_t dllPathLen = strlen(dll_path);
    if (dllPathLen == 0 || dllPathLen > MAX_PATH_LEN - 1) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Invalid DLL path length.\n");
        CloseHandle(hProcess);
        return DIAMOND_INVALID_PATH;
    }

    // Reserve memory in the target process for the DLL name
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, dllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (remoteMem == NULL) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Failed to allocate memory in target process.\n");
        CloseHandle(hProcess);
        return DIAMOND_ERROR;
    }

    // Passing the DLL path to the process memory
    if (!WriteProcessMemory(hProcess, remoteMem, dll_path, (DWORD)dllPathLen, NULL)) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Failed to write DLL path to target process.\n");
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return DIAMOND_ERROR;
    }

    // Getting the LoadLibraryA address
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (hKernel32 == NULL) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Failed to get kernel32 handle.\n");
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return DIAMOND_ERROR;
    }

    FARPROC loadLibAddr = GetProcAddress(hKernel32, "LoadLibraryA");
    if (loadLibAddr == NULL) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Failed to get LoadLibraryA address.\n");
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return DIAMOND_ERROR;
    }

    // Creating a remote thread to load a DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, remoteMem, 0, NULL);
    if (hThread == NULL) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Failed to create remote thread.\n");
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return DIAMOND_ERROR;
    }

    // Waiting for the end of the thread
    WaitForSingleObject(hThread, INFINITE);
    DWORD retCode = 0;

    if (!GetExitCodeThread(hThread, &retCode)) {
        set_color(FOREGROUND_RED);
        printf("[ERROR] Failed to get thread exit code.\n");
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hThread);
        CloseHandle(hProcess);
        return DIAMOND_ERROR;
    }

    // Cleanup
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    //If the return code is not 0, LoadLibraryA returned an error
    if (retCode != 0) {
        set_color(7);
        printf("[WARNING] LoadLibraryA returned non-zero error code: %lu\n", retCode);
    }
    set_color(FOREGROUND_GREEN);
    printf("DLL injection completed into PID %lu.\n", (unsigned long)pid);
    return DIAMOND_OK;
}
