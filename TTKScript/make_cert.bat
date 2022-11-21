@echo off
if "%1" == "" echo "plz input a file path" && exit
if not exist %1 echo "%1 not exist" && exit

set base=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\bin
"%base%\makecert.exe" -a sha512 -r -n "CN=Greedysky Studio" -b 01/01/2016 -e 12/31/2024 -sv %temp%\CbipSetupKey.pvk %temp%\CbipSetupKey.cer
"%base%\cert2spc.exe" %temp%\CbipSetupKey.cer %temp%\CbipSetupKey.spc
"%base%\pvk2pfx.exe" -pvk %temp%\CbipSetupKey.pvk -spc %temp%\CbipSetupKey.spc -f -pfx %userprofile%\Desktop\TTKMusicPlayer.pfx
"%base%\signtool.exe" sign /f %userprofile%\Desktop\TTKMusicPlayer.pfx  /t "http://time.certum.pl/" /d "TTKMusicPlayer" %1
del %temp%\CbipSetupKey.* >nul 2>nul
del %userprofile%\Desktop\TTKMusicPlayer.pfx >nul 2>nul
