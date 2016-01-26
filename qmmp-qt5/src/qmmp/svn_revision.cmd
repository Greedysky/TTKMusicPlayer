@echo off

:: based on smplayer getrev.cmd script
:: Some SVN clients can use localized messages (e.g. SlikSVN), force English
set LC_ALL=C
set SVN_REV=

for /f "tokens=2" %%i in ('svn info ^| find "Revision:"') do set SVN_REV=%%i

if NOT "%SVN_REV%"=="" (
echo r%SVN_REV%
)


