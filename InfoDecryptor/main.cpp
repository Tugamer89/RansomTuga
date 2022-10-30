// TODO:
// InfoFile starts with '$$$\n ': line 85

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

    string line, content;
    ifstream inFile(fileName);
    while (getline(inFile, line))
        content += line;
    inFile.close();

    bool founded = false;
    string contentDecryptedOnce = aes_decrypt(KEY, content, IV);
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

    vector<string> dividedFileName = split(fileName, '.');
    string writeFileName;

    for (int i = 0; i < dividedFileName.size(); i++) {
        if (i == dividedFileName.size() - 1)
            writeFileName += string(skCrypt("decrypted.txt")) /* + dividedFileName[i]; //(uncomment this part if you want to keep original extension)*/;
        else
            writeFileName += dividedFileName[i] + string(skCrypt("."));
    }

    ofstream outFile(writeFileName, ios::out | ios::binary);
    unsigned char smarker[] = { 0xEF, 0xBB, 0xBF };
    outFile << smarker << contentDecryptedTwice;
    outFile.close();

    cout << skCrypt(" File decrypted in \"") << writeFileName << skCrypt("\"\n\n ");

    system(skCrypt("pause"));

    return 0;
}
