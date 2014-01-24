# Only Intel binaries are accepted so force this
CONFIG += x86

# Icon is mandatory for submission
ICON = osx/MarkEmptyDirs.icns

# Name of the application signing certificate
APPCERT = "jonny.dee@gmx.net"

# Name of the installer signing certificate
INSTALLERCERT = "jonny.dee@gmx.net"

# Bundle identifier for your application
BUNDLEID = name.jonnydee.MarkEmptyDirs

QMAKE_CFLAGS += -gdwarf-2
QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++
QMAKE_CXXFLAGS += -gdwarf-2
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_OBJECTIVE_CFLAGS_RELEASE =  $$QMAKE_OBJECTIVE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

QMAKE_INFO_PLIST = osx/MarkEmptyDirs.plist
OTHER_FILES += \
    osx/make_icns.sh
    osx/MarkEmptyDirs.plist

# Write some info in the Info.plist
QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Set :GIT_COMMIT_HASH $${GIT_COMMIT_HASH}\" $$DESTDIR/$${TARGET}.app/Contents/Info.plist;
QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Set :CFBundleShortVersionString $${VERSION}\" $$DESTDIR/$${TARGET}.app/Contents/Info.plist;

codesign.depends  += all
codesign.commands += macdeployqt $$DESTDIR/$${TARGET}.app -no-plugins;

# Build icon set.
iconset.commands = pushd $${_PRO_FILE_PWD_}/osx; ./make_icns.sh; popd;

# Extract debug symbols
codesign.commands += dsymutil $$DESTDIR/$${TARGET}.app/Contents/MacOS/$${TARGET} -o $$DESTDIR/$${TARGET}.app.dSYM;

# Sign frameworks and plug-ins (uncomment and change to suit your application)
codesign.commands += cp ${QTDIR}/lib/QtCore.framework/Contents/Info.plist $$DESTDIR/$${TARGET}.app/Contents/Frameworks/QtCore.framework/Resources/;
codesign.commands += codesign --force --verify --verbose --timestamp --sign "$${APPCERT}" -i $${BUNDLEID} $$DESTDIR/$${TARGET}.app/Contents/Frameworks/QtCore.framework/;
# Sign the application bundle
codesign.commands += codesign --force --verify --verbose --timestamp --sign $${APPCERT} -i $${BUNDLEID} $$DESTDIR/$${TARGET}.app;

# Build dmg product.
product.commands += macdeployqt $$DESTDIR/$${TARGET}.app -no-plugins -dmg;

# Build the product package
package.commands += productbuild –component $$DESTDIR/$${TARGET}.app /Applications –sign $${INSTALLERCERT} $$DESTDIR/$${TARGET}.pkg;

QMAKE_EXTRA_TARGETS += codesign iconset package product
