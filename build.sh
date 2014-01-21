#!/bin/sh

mkdir -p bin
pushd bin
$QTDIR/bin/qmake -r ../project
make
popd
