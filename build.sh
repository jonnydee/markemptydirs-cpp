#!/bin/sh

pushd bin
$QTDIR/bin/qmake ../src
make
popd
