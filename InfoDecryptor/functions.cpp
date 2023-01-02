#include "headers.h"

using namespace std;

bool FileExists(const string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

string OpenFilename() {
    string titleStr = (string)skCrypt("Select a File");
    wstring title(titleStr.length(), L' ');
    copy(titleStr.begin(), titleStr.end(), title.begin());

    string filterStr = (string)skCrypt("Text files (.txt)") + '\0' + (string)skCrypt("*.txt") + '\0';
    wstring filter(filterStr.length(), L' ');
    copy(filterStr.begin(), filterStr.end(), filter.begin());

    wstring filename(MAX_PATH, L'\0');

    OPENFILENAMEW ofn = { };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = &filename[0];
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_HIDEREADONLY | OFN_FORCESHOWHIDDEN | OFN_NOREADONLYRETURN;

    string fileNameStr = (string)skCrypt("");

    if (GetOpenFileName(&ofn))
        fileNameStr = string(filename.begin(), filename.end());

    return fileNameStr;
}

vector<string> Split(const string& s, const char& delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

void CopyToClipboard(const std::string& s) {
    OpenClipboard(0);
    EmptyClipboard();

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);

    memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);

    CloseClipboard();
    GlobalFree(hg);
}
