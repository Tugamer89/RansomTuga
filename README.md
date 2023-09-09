# RansomTuga
<p align="center">
    <a href="https://en.wikipedia.org/wiki/Microsoft_Windows" target="_blank"><img src="https://img.shields.io/badge/OS-windows-informational?style=plastic-square&logo=windows&logoColor=white" alt="os"></a>
    <a href="https://en.wikipedia.org/wiki/Visual_Studio" target="_blank"><img src="https://img.shields.io/badge/Editor-VisualStudio-blue?style=plastic-square&logo=visual-studio&logoColor=white" alt="editor"></a>
    <a href="https://github.com/Tugamer89/RansomTuga" target="_blank"><img src="https://img.shields.io/endpoint?url=https://hits.dwyl.com/Tugamer89/RansomTuga.svg%3Fcolor%3Dblue" alt="visitors"></a>
    <a href="https://wakatime.com/badge/github/Tugamer89/RansomTuga" target="_blank"><img src="https://wakatime.com/badge/github/Tugamer89/RansomTuga.svg" alt="time"></a>
</p>
<img src=".github/images/main.jpg" alt="logo">

The project called RansomTuga is an advanced ransomware and semi-stealer that offers numerous customizable options. Its main objective is to provide a self-contained solution, utilizing a single .exe file for encryption and another separate .exe file for decryption. These files do not require any external dependencies. The program employs anti-dumping functions to avoid detection by certain anti-virus software.\
\
For encryption, RansomTuga utilizes AES CBC encryption, employing a randomly generated 256-bit key and a 128-bit IV. The program includes a whitelist and blacklist feature for file extensions that will be subjected to encryption.\
\
Moreover, RansomTuga captures photos from all connected webcams, sending the encryption key, along with some PC information, to the attacker via email or Telegram. It also retrieves various information from the victim's PC, drops a readme.txt file and a decryptor, alters the victim's computer wallpaper, and can modify the file extension and apply a custom icon to the encrypted files. Additionally, it possesses the capability to upload files to AnonFiles prior to encryption and can delete the executable from which it was initially launched.


# Table of Contents
- [Features](#Features)
- [Compilation Instructions](#Compilation-Instructions)
- [Usage Instructions](#Usage-Instructions)
- [Todo list](#Todo-list)
- [Thanks](#Thanks)
- [Donations](#Donations)
- [Legal Disclaimer](#Legal-Disclaimer)


# Features
- File encryption is performed using the robust **AES CBC 256** algorithm, employing a randomly generated key and a 128-bit IV.
- **Anti-dumping** functions are implemented to elude detection by certain anti-virus software.
- The program incorporates a **whitelist** and **blacklist** feature, allowing for targeted or excluded file extensions during the encryption process.
- RansomTuga has the capability to **capture photos** from all connected webcams.
- It also steals various **system information**, including hardware ID (HWID), IP address, CPU and RAM specifications, GPU brand, screen resolution, screenshots, clipboard contents, GPS location and Wi-Fi passwords.
- The malware can **delete restore points** on the system.
- Encrypted files can be assigned a **custom file icon**.
- The victim's **wallpaper** can be altered by the malware.
- Before encryption, files can be **uploaded** to a remote server.
- The **encryption key**, along with stolen information, can be **sent to the attacker** through email or a Telegram bot.
- The malware has a **self-deletion** feature, removing its executable after execution.
- With over 70 different options, RansomTuga is **highly customizable**.
And many more features are available!


# Compilation Instructions
- Install the latest version of Python 3 from [here](https://www.python.org/downloads/).
- Install the latest version of Visual Studio 2022 from [here](https://visualstudio.microsoft.com/downloads/).
- Install the latest version of Git from [here](https://git-scm.com/download/win).
- Install vcpkg, CryptoPP and minhook using Git Bash:
  - Open Git Bash.
  - Run the command `git clone https://github.com/microsoft/vcpkg`.
  - Run the command `./vcpkg/bootstrap-vcpkg.bat`.
  - Run the command `./vcpkg/vcpkg.exe install cryptopp:x64-windows-static`.
  - Run the command `./vcpkg/vcpkg.exe install minhook:x64-windows-static`.
  - Run the command `./vcpkg/vcpkg.exe integrate install`.
- Download the project and extract it.
- In the first line of `./RansomTuga/preCompilation.bat` and `./RansomTuga/postCompilation.bat`, replace the word `py` with the command you use to open Python in the command prompt.
- Double click on `./RansomTuga.sln` to open the project.
- Customize any desired settings in `./commons/common.h`. Make sure to change `SENDERMAIL`, `SENDERPSW`, and `RECEIVERMAIL` to receive victim data.
- Compile the project in `Release` and `x64`.


# Usage Instructions
- Open `RansomTuga.exe` on the victim's PC. If it is in debug mode, it will encrypt all files in `./debugFolder/` or in the path specified in the arguments; otherwise, it will encrypt files in the current directory.
- Download the attachment from the received email or the received Telegram message.
- Open `InfoDecryptor.exe` and select the downloaded attachment. It will decrypt the encrypted info-file.
- Open the decrypted file and copy the key.
- Open `DataDecryptor.exe` and paste the key. It will decrypt all encrypted files.


# Todo list
- [x] Delete key after usage
- [ ] Substitute anonfiles with some other provider
- [ ] Clean up some hardcoded data
- [ ] Add a GUI for the Decryptor
- [ ] Improve anti-AV
- [ ] Split info-file into multiple if too big
- [ ] Chrome based browsers password stealer
- [ ] Chrome based browsers cookies stealer
- [ ] Add info-file sender in offline mode
- [ ] Do more tests for email sending


# Thanks
- [SafeSig](https://bf.hn/uid/105399)
- [FullCrypt](https://bf.hn/uid/216523)
- [Grief](https://breachforums.is/member?action=profile&uid=2212)
- [Adamhkt](https://github.com/adamhkt)


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


# Legal Disclaimer
Please read this disclaimer carefully before using RansomTuga.\
\
The information provided by RansomTuga is for general educational purposes only. It is not intended to promote or encourage any illegal or malicious activities. The use of ransomware or any similar software for unauthorized access, data theft, or any other illegal purposes is strictly prohibited. \
\
The creators and contributors of RansomTuga do not assume any responsibility for any consequences that may arise from the use or misuse of this software. Users are solely responsible for their actions and should exercise caution and comply with all applicable laws and regulations in their respective jurisdictions.\
\
The functionality and features provided by RansomTuga are intended for educational and research purposes, allowing users to understand the potential risks associated with ransomware and to develop effective countermeasures against such threats. It is important to ensure that you have the legal right and authorization before using RansomTuga on any system or network.\
\
By using RansomTuga, you agree that the creators and contributors shall not be held liable for any damages, losses, or legal repercussions resulting from the use or misuse of this software. The creators and contributors disclaim all warranties, express or implied, regarding the use, performance, or suitability of RansomTuga for any particular purpose.\
\
It is strongly advised to use RansomTuga only in controlled environments and with the explicit permission of the system owner or administrator. Unauthorized use of this software is strictly prohibited and may violate local, national, or international laws.\
\
If you are uncertain about the legal implications of using RansomTuga or have any questions regarding its usage, it is recommended to seek legal counsel from professionals specializing in cybersecurity and technology laws.\
\
Remember, always prioritize legal and ethical behavior when using any software or engaging in any online activities. Stay informed, stay responsible, and respect the rights and privacy of others.\
\
Please note that the addition of this statement is solely for legal purposes.
