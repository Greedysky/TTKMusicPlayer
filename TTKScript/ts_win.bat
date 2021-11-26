@echo off & setlocal EnableDelayedExpansion
PUSHD %~dp0

cd /D ../TTKLanguage
set LCEXE="%1"
set OUTPUT="%2/GLanguage"
md %OUTPUT%
::make
for /F %%f in ('dir /A:A /s /b *.ts') do (
  %LCEXE% %%f -silent -nounfinished
)
::rename
ren *.qm *.ln
::move
for /F %%f in ('dir /A:A /s /b *.ln') do (
  move %%f %OUTPUT%
)
POPD %~dp0
