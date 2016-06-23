@echo off
if "%1" == "" echo "plz input a file path" && exit
if not exist %1 echo "%1 not exist" && exit

set base=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\bin
"%base%\makecert.exe" -a sha512 -r -n "CN=Greedysky studio" -b 01/01/2016 -e 12/31/2019 -sv %temp%\CbipSetupKey.pvk %temp%\CbipSetupKey.cer
"%base%\cert2spc.exe" %temp%\CbipSetupKey.cer %temp%\CbipSetupKey.spc
"%base%\pvk2pfx.exe" -pvk %temp%\CbipSetupKey.pvk -spc %temp%\CbipSetupKey.spc -f -pfx %userprofile%\Desktop\TTKMusicplayer.pfx
"%base%\signtool.exe" sign /f %userprofile%\Desktop\TTKMusicplayer.pfx  /t "http://timestamp.verisign.com/scripts/timstamp.dll" /d "TTKMusicplayer" %1
del %temp%\CbipSetupKey.* >nul 2>nul
del %userprofile%\Desktop\TTKMusicplayer.pfx >nul 2>nul
