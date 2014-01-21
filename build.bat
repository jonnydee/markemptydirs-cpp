@echo off

mkdir bin
pushd bin
%QTDIR%\bin\qmake ..\src
nmake
popd

