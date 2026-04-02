/*=============================================
types.h - file with injection statuses and defenitions.
Copyright (c) Kisushka IDG.
Licensed under GNU General Public License (GPL).
GitHub: https://github.com/KisushkaIDG          
Telegram:         https://t.me/KisushkaGroup
Russian telegram: https://t.me/KisushkaRUS
==============================================*/

#ifndef DIAMOND_TYPES_H
#define DIAMOND_TYPES_H

#include <windows.h>

// Injection status codes
#define MAX_PATH_LEN           512

#define DIAMOND_OK             0
#define DIAMOND_ERROR          -1
#define DIAMOND_INVALID_PATH   -2
#define DIAMOND_NOT_FOUND      -3

#define CMD_SUCCESS_EXIT       0
#define CMD_FAIL_EXIT          1

// OpenProcess flags
#define PROCESS_ACCESS_FLAGS \
                             (PROCESS_CREATE_THREAD | \
                             PROCESS_QUERY_INFORMATION | \
                             PROCESS_VM_OPERATION | \
                             PROCESS_VM_WRITE | \
                             PROCESS_VM_READ)

#endif /* DIAMOND_TYPES_H */
