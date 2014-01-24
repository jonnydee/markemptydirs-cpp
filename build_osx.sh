#!/bin/sh

pushd src/MarkEmptyDirs/Cli/osx
./make_icns.sh
popd
pushd project
$QTDIR/bin/qmake -r
make
popd
pushd src/MarkEmptyDirs/Cli
make product
popd
