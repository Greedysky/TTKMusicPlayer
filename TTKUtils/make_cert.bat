@echo off
setlocal enabledelayedexpansion

if "%1" == "" echo "plz input a file path" && exit
if not exist %1 echo "%1 not exist" && exit

set sources[0]=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\bin
set sources[1]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.10240.0\x64
set sources[2]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.10586.0\x64
set sources[3]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.14393.0\x64
set sources[4]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.15063.0\x64
set sources[5]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64
set sources[6]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.17134.0\x64
set sources[7]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x64
set sources[8]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64
set sources[9]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x64
set sources[10]=C:\Program Files (x86)\Windows Kits\10\bin\10.0.20348.0\x64
set sources[11]=C:\Program Files (x86)\Windows Kits\11\bin\10.0.22000.0\x64
set sources[12]=C:\Program Files (x86)\Windows Kits\11\bin\10.0.22621.0\x64

for /L %%n in (0,1,12) do (
   if exist "!sources[%%n]!\makecert.exe" set base=!sources[%%n]!
)

"%base%\makecert.exe" -a sha512 -r -n "CN=Greedysky Studio" -b 01/01/2016 -e 12/31/2024 -sv %temp%\CbipSetupKey.pvk %temp%\CbipSetupKey.cer
"%base%\cert2spc.exe" %temp%\CbipSetupKey.cer %temp%\CbipSetupKey.spc
"%base%\pvk2pfx.exe" -pvk %temp%\CbipSetupKey.pvk -spc %temp%\CbipSetupKey.spc -f -pfx %userprofile%\Desktop\TTKMusicPlayer.pfx
"%base%\signtool.exe" sign /f %userprofile%\Desktop\TTKMusicPlayer.pfx  /t "http://time.certum.pl/" /d "TTKMusicPlayer" %1

del %temp%\CbipSetupKey.* >nul 2>nul
del %userprofile%\Desktop\TTKMusicPlayer.pfx >nul 2>nul
