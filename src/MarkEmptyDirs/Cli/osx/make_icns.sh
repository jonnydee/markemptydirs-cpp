#!/bin/sh

mkdir -p MarkEmptyDirs.iconset
sips -z 16 16     ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_16x16.png
sips -z 32 32     ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_16x16@2x.png
sips -z 32 32     ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_32x32.png
sips -z 64 64     ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_32x32@2x.png
sips -z 128 128   ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_128x128.png
sips -z 256 256   ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_128x128@2x.png
sips -z 256 256   ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_256x256.png
sips -z 512 512   ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_256x256@2x.png
sips -z 512 512   ../MarkEmptyDirs.png --out MarkEmptyDirs.iconset/icon_512x512.png
cp ../MarkEmptyDirs.png MarkEmptyDirs.iconset/icon_512x512@2x.png
iconutil -c icns MarkEmptyDirs.iconset
rm -r MarkEmptyDirs.iconset
