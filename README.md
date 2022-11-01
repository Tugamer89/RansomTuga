# RansomTuga
<img src=".github/images/main.jpg" alt="mainImage">

It is an advanced Ransomware with a lot of options and fully customizable.\
It encrypts files in AES CBC with a random generated 256 bit key and with an hardcoded IV.\
After the encryption it sends the key and some pc informations to the attacker via email.\
It also drops a readme.txt file and the decryptor and change the wallpaper of the victim computer.\
It could also change the extension of crypted files and set a custom icon for those.\
At the end it can delete the executable from which it was started.\


# Table of Contents
- [Features](#Features)
- [How to compile](#How-to-compile)
- [How to use](#How-to-use)
- [Todo list](#Todo)
- [How does it work](#How-does-it-work)


# Features
- **AES CBC 256** file encryption
- **Debug mode** for fast testing
- **Trojan mode**
- **Task remover**
- **Steal system informations** (HWID, IP, CPU brand and threads, RAM, GPU brand, Host name and username, screen resolution, windows version and language)
- **Delete restore points**
- **Custom file icon** for crypted files
- **Change wallpaper**
- **Send email** with data encrypted in AES CBC 256
- Send email also if pc is not connected to internet with a `.ps1` file obfuscated thanks to [Chimera](https://github.com/tokyoneon/Chimera)
- **Self-delete** after execution
- **Fully customizable** with more than 40 different options easy to change
And many more!


# How to compile
- Install latest version of Visual Studio 2022 [here](https://visualstudio.microsoft.com/downloads/)
- Install latest version of Git [here](https://git-scm.com/download/win)
- Install vcpkg and CryptoPP using Git Bash:
  - Open  Git Bash
  - Run `git clone https://github.com/microsoft/vcpkg`
  - Run `.\\vcpkg\\bootstrap-vcpkg.bat`
  - Run `.\\vcpkg\\vcpkg.exe install cryptopp:x64-windows`
  - Run `.\\vcpkg\\vcpkg.exe integrate install`
- Download the project and extract it
- Open the project double clicking on `RansomTuga.sln`
- Customize everything you want in `commons/common.h`, you **must** change `SENDERMAIL` `SENDERPSW` and `RECEIVERMAIL`
- Compile the project in `Release` and `x64`


# How to use
- Open `RansomTuga.exe` on the victim pc, if it is in debug mode it encrypts all files in `.\debugFolder\` or in the path defined in arguments else in `C:\Users\`
- Download the attachment from the received email
- Open `InfoDecryptor.exe` and select the attachment just downloaded, it decrypts the crypted info-file
- Open the decrypted file and copy the key
- Open `DataDecryptor.exe` and paste the key, it decrypts all encrypted files


# Todo
[ ] Fastest encryption modes (HeadOnly, DotPattern, SmartPattern, AdvancedSmartPattern)
[ ] Random IV
[ ] Whitelist & Blacklist file extensions
[ ] AntiViruses evasion
[ ] Anti-Debug functions
[ ] More anti-reverse functions (i.e. function names)
[ ] Do more tests in more environments


# How does it work
<details>
  <summary>RansomTuga Workflow</summary>
  <p align="center">
    &nbsp;
    <img src=".github/images/workflow_RansomTuga.png" alt="RansomTugaWorkflow">
  </p>
</details>
<details>
  <summary>InfoDecryptor Workflow</summary>
  <p align="center">
    &nbsp;
    <img src=".github/images/workflow_InfoDecryptor.png" alt="InfoDecryptorWorkflow">
  </p>
</details>
<details>
  <summary>DataDecryptor Workflow</summary>
  <p align="center">
    &nbsp;
    <img src=".github/images/workflow_DataDecryptor.png" alt="DataDecryptorWorkflow">
  </p>
</details>
