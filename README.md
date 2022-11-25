# RansomTuga
<p align="center">
    <a href="https://en.wikipedia.org/wiki/Microsoft_Windows" target="_blank"><img src="https://img.shields.io/badge/OS-windows-informational?style=plastic-square&logo=windows&logoColor=white" alt="os"></a>
    <a href="https://en.wikipedia.org/wiki/Visual_Studio" target="_blank"><img src="https://img.shields.io/badge/Editor-VisualStudio-blue?style=plastic-square&logo=visual-studio&logoColor=white" alt="os"></a>
    <a href="https://github.com/Tugamer89/RansomTuga" target="_blank"><img src="https://visitor-badge.laobi.icu/badge?page_id=tugamer89.RansomTuga" alt="os"></a>
</p>
<img src=".github/images/main.jpg" alt="logo">

It is an advanced Ransomware with a lot of options and fully customizable.\
It uses anti-dumping functions to evade some anti-viruses.\
It encrypts files in AES CBC with a random generated 256 bit key and with an hardcoded IV.\
It has a whitelist and a blacklist for the extension of files.\
It take photos from all the webcams connected to the pc.\
After the encryption it sends the key and some pc informations to the attacker via email and/or via a Telegram bot.\
It retrieves various information from the victim's pc and sends them to the attacker.\
It also drops a readme.txt file and the decryptor and change the wallpaper of the victim computer.\
It could also change the extension of crypted files and set a custom icon for those.\
It is able to upload files on AnonFiles before encryption.\
At the end it can delete the executable from which it was started.


# Table of Contents
- [Features](#Features)
- [How to compile](#How-to-compile)
- [How to use](#How-to-use)
- [Todo list](#Todo)
- [How does it work](#How-does-it-work)
- [Thanks](#Thanks)
- [Donations](#Donations)


# Features
- **AES CBC 256** file encryption
- **Debug mode** for fast testing
- **Trojan mode**
- **Custom icon** for the compiled executable
- **Tasks remover**
- **Steal system informations** (HWID, IP, CPU brand and threads, RAM, GPU brand, Host name and username, screen resolution, screenshot, clipboard, wifi passwords, windows version and language)
- **Take photos** of **all** the webcams
- **Delete restore points**
- **Custom file icon** for crypted files
- **Change wallpaper**
- **Whitelist** & **Blacklist** for files extensions (whitelist have priority)
- **File uploader** before encryption
- **Send email** with data encrypted in AES CBC 256
- Send email also if pc is not connected to internet with a `.ps1` file obfuscated thanks to [Chimera](https://github.com/tokyoneon/Chimera)
- **Self-delete** after execution
- **Fully customizable** with more than 40 different options easy to change
And many more!


# How to compile
- Install latest version of Python 3 [here](https://www.python.org/downloads/)
- Install latest version of Visual Studio 2022 [here](https://visualstudio.microsoft.com/downloads/)
- Install latest version of Git [here](https://git-scm.com/download/win)
- Install vcpkg and CryptoPP using Git Bash:
  - Open  Git Bash
  - Run `git clone https://github.com/microsoft/vcpkg` 
  - Run `./vcpkg/bootstrap-vcpkg.bat`
  - Run `./vcpkg/vcpkg.exe install cryptopp:x64-windows-static`
  - Run `./vcpkg/vcpkg.exe integrate install`
- Download the project and extract it
- Change in the first line of `./RansomTuga/preCompilation.bat` and `./RansomTuga/postCompilation.bat` the word `py` with whatever command you use to open python in command prompt
- Open the project double clicking on `./RansomTuga.sln`
- Customize everything you want in `./commons/common.h`, you **must** change `SENDERMAIL` `SENDERPSW` and `RECEIVERMAIL` in order to receive victim data
- Compile the project in `Release` and `x64`


# How to use
- Open `RansomTuga.exe` on the victim pc, if it is in debug mode it encrypts all files in `.\debugFolder\` or in the path defined in arguments else in `C:\Users\`
- Download the attachment from the received email
- Open `InfoDecryptor.exe` and select the attachment just downloaded, it decrypts the crypted info-file
- Open the decrypted file and copy the key
- Open `DataDecryptor.exe` and paste the key, it decrypts all encrypted files


# Todo
- [ ] Fastest encryption modes (HeadOnly, DotPattern, SmartPattern, AdvancedSmartPattern)
- [ ] Random IV
- [x] Get clipboard
- [x] Get screenshot
- [x] Get webcam pictures
- [x] Get Wi-Fi password
- [x] Drop custom file
- [x] Drop & launch custom file for trojan mode
- [x] Whitelist & Blacklist file extensions
- [x] Add ip geolocation
- [x] File sender
- [x] Info file sender through telegram bot
- [ ] AntiViruses evasion
- [ ] Anti-Debug functions
- [ ] More anti-reverse functions (i.e. function names)
- [ ] Do more tests in more environments


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


# Thanks
- [SafeSig](https://bf.hn/uid/105399)


# Donations
You are not obliged to donate but if you do I really appreciate it very much!\
If you donate write me everywhere so I will add you among the thanks.\
Those who donate and want to propose an improvement or the addition of some new function will have priority over the others.\
\
XMR: `41ge57zKUGccYiLiZh7RzuUTFZysUrFBz2wL5kkPQo5QJYHbXFVy5rmKxq4PAas365GPygFa9GiaERiNUpg3sYYnUPBbapt`\
ETH: `0x73D0b82F7D8c2899a54aA57c4075cca5e1CcbD03`\
BTC: `bc1qj7kuy9mlutjapq76dc368yrsmqmg3k7p8y4g6n`\
TRX: `TYpjEtunbiTsrS4p1JXyGxQikNGajkkWtq`\
ADA: `addr1qyhjrhxz425mwvn2x4yfa8t2xc70qr4ftg8xp7qf7u0903p0y8wv924fkuex5d2gn6wk5d3u7q82jkswvruqnac72lzq45ne0n`\
SOL: `HNKN5MiEBzUNk2yY2Q2MicPs8M4baaGMRdL9BeY5QtUw`\
LTC: `LZUdiSiXbU9AUZNjNKwVxNhVNSCHHSQkBm`\
DOGE: `DRdEj9EvMMrEC3WKyjdyfeAzZQdBCHC4cH`
