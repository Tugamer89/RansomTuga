# RansomTuga
<p align="center">
    <a href="https://en.wikipedia.org/wiki/Microsoft_Windows" target="_blank"><img src="https://img.shields.io/badge/OS-windows-informational?style=plastic-square&logo=windows&logoColor=white" alt="os"></a>
    <a href="https://en.wikipedia.org/wiki/Visual_Studio" target="_blank"><img src="https://img.shields.io/badge/Editor-VisualStudio-blue?style=plastic-square&logo=visual-studio&logoColor=white" alt="editor"></a>
    <a href="https://github.com/Tugamer89/RansomTuga" target="_blank"><img src="https://visitor-badge.glitch.me/badge?page_id=tugamer89.RansomTuga" alt="visitors"></a>
</p>
<img src=".github/images/main.jpg" alt="logo">

This project, RansomTuga, is an advanced ransomware and semi-stealer with many customizable options.
The main goal of the project is to achieve a self-contained solution by providing a single .exe file for encryption and another distinct .exe file for decryption, both of which require no external dependencies.
It uses anti-dumping functions to evade detection by some anti-viruses.
It encrypts files using AES CBC encryption with a randomly generated 256-bit key and 128-bit IV.
The program has a whitelist and blacklist for the extensions of files it will encrypt.
Additionally, it takes photos from all connected webcams, sends the encryption key and some PC information to the attacker via email or Telegram, retrieves various information from the victim's PC, drops a readme.txt file and a decryptor, changes the victim's computer wallpaper, and can change the extension and set a custom icon for the encrypted files.
It also has the capability to upload files to AnonFiles before encryption, and can delete the executable from which it was launched.


# Table of Contents
- [Features](#Features)
- [Compilation Instructions](#Compilation-Instructions)
- [Usage Instructions](#Usage-Instructions)
- [Todo list](#Todo-list)
- [Thanks](#Thanks)
- [Donations](#Donations)


# Features
- Encryption of files using the **AES CBC 256** algorithm with a randomly generated key and 128-bit IV.
- **Anti-dumping functions** to evade detection by some anti-viruses.
- A **whitelist and blacklist** for file extensions to target or exclude during encryption.
- **Taking photos** from all connected webcams.
- **Stealing system information** such as the hardware ID (HWID), IP address, CPU and RAM specs, GPU brand, host name, username, screen resolution, screenshots, clipboard contents, and Wi-Fi passwords.
- **Deleting restore points**
- **Custom file icon** for encrypted files
- **Changing the wallpaper**
- **Uploading files** to a remote server before encryption
- **Sending the encryption key and stolen information** to the attacker via email or Telegram bot.
- **Self-deletion** of the malware executable after execution.
- **Fully customizable** with over 60 different options.
And many more!


# Compilation Instructions
- Install latest version of Python 3 [here](https://www.python.org/downloads/)
- Install latest version of Visual Studio 2022 [here](https://visualstudio.microsoft.com/downloads/)
- Install latest version of Git [here](https://git-scm.com/download/win)
- Install vcpkg and CryptoPP using Git Bash:
  - Open  Git Bash
  - Run `git clone https://github.com/microsoft/vcpkg`
  - Run `./vcpkg/bootstrap-vcpkg.bat`
  - Run `./vcpkg/vcpkg.exe install cryptopp:x64-windows-static`
  - Run `./vcpkg/vcpkg.exe install minhook:x64-windows-static`
  - Run `./vcpkg/vcpkg.exe integrate install`
- Download the project and extract it
- Change in the first line of `./RansomTuga/preCompilation.bat` and `./RansomTuga/postCompilation.bat` the word `py` with whatever command you use to open python in command prompt
- Open the project double clicking on `./RansomTuga.sln`
- Customize everything you want in `./commons/common.h`, you **must** change `SENDERMAIL` `SENDERPSW` and `RECEIVERMAIL` in order to receive victim data
- Compile the project in `Release` and `x64`


# Usage Instructions
- Open `RansomTuga.exe` on the victim pc, if it is in debug mode it encrypts all files in `./debugFolder/` or in the path defined in arguments else in `C:/Users/`
- Download the attachment from the received email
- Open `InfoDecryptor.exe` and select the attachment just downloaded, it decrypts the crypted info-file
- Open the decrypted file and copy the key
- Open `DataDecryptor.exe` and paste the key, it decrypts all encrypted files


# Todo list
- [x] Pre-encrypt dropping files
- [ ] Stop shutdown
- [ ] Add new project: info-file sender in offline mode
- [ ] Create new branch with external depencies for more advanced features:
    - [ ] Chrome based browsers password stealer
    - [ ] Chrome based browsers cookies stealer


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
