#!/bin/sh

pushd project
$QTDIR/bin/qmake -r
make
popd
pushd src/MarkEmptyDirs/Cli
make product
popd
