@echo off

pushd project
%QTDIR%\bin\qmake -r
nmake
popd
pushd src\MarkEmptyDirs\Cli
nmake product
popd
