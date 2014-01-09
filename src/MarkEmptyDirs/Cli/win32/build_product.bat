@echo off

set VERSION=%1

set PROJDIR=..\..\..\..
set PRODUCTDIR=%PROJDIR%\build\MarkEmptyDirs
set PRODUCT=%PROJDIR%\build\MarkEmptyDirs_v%VERSION%_win32.exe

mkdir "%PRODUCTDIR%"
del "%PRODUCT%"
xcopy /Y %PROJDIR%\bin\markemptydirs.exe %PRODUCTDIR%
xcopy /Y %QTDIR%\bin\Qt5Core.dll %PRODUCTDIR%
xcopy /Y %QTDIR%\bin\icu*.dll %PRODUCTDIR%

7z a -sfx7z.sfx "%PRODUCT%" "%PRODUCTDIR%"
