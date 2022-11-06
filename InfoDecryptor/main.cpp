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

int main() {
    system(skCrypt("title RansomTuga - InfoDecryptor"));
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1060, 530, TRUE);

    cout << logo << skCrypt("\n\n\n");

    string fileName;
    cout << skCrypt(" Select crypted info-file: ");
    fileName = openfilename();
    cout << fileName << endl;

    if (!fileExists(fileName)) {
        cout << skCrypt("\n File \"") << fileName << skCrypt("\" does not exists\n");
        Sleep(1500);
        exit(0);
    }

    string filePath = split(fileName, '.')[0];
    if (!fileExists(filePath))
        system(((string)skCrypt("mkdir ") + filePath).c_str());


    string line;
    vector<string> content;
    ifstream inFile(fileName);
    while (getline(inFile, line))
        content.push_back(line);
    inFile.close();

    // info.txt
    bool founded = false;
    string contentDecryptedOnce = aes_decrypt(KEY, content[0], IV);
    string contentDecryptedTwice;
    vector<string> dividedContentDecryptedOnce = split(contentDecryptedOnce, ' ');
    for (int i = 0; i < dividedContentDecryptedOnce.size(); i++) {
        string part = dividedContentDecryptedOnce[i];
        if (founded) {
            vector<string> tmp = split(part, '\n');
            contentDecryptedTwice += aes_decrypt(KEYOFKEY, tmp[0], IV) + string(skCrypt("\n"));
            for (int j = 1; j < tmp.size(); j++) {
                contentDecryptedTwice += tmp[j];
                if (j != tmp.size() - 1)
                    contentDecryptedTwice += string(skCrypt("\n"));   
            }
            if (i != dividedContentDecryptedOnce.size() - 1)
                contentDecryptedTwice += string(skCrypt(" "));
            founded = false;
            continue;
        }
        if (part == string(skCrypt("Key:")) && !founded)
            founded = true;
        contentDecryptedTwice += part;
        if (i != dividedContentDecryptedOnce.size() - 1)
            contentDecryptedTwice += string(skCrypt(" "));
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
    cryptFile2 << aes_decrypt(KEY, content[1], IV);
    cryptFile2.close();
    cout << skCrypt(" File decrypted in \"") << cryptFileName << skCrypt("\"\n");


    // clipboard.txt
    string clipFileName = filePath + (string)skCrypt("\\clipboard.txt");
    ofstream clipFile(clipFileName, ios::out | ios::binary);
    clipFile << smarker;
    clipFile.close();
    ofstream clipFile2(clipFileName, ios::out | ios::binary);
    clipFile2 << aes_decrypt(KEY, content[2], IV);
    clipFile2.close();
    cout << skCrypt(" File decrypted in \"") << clipFileName << skCrypt("\"\n");


    cout << skCrypt("\n ");
    system(skCrypt("pause"));

    return 0;
}
