#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T21:11:13
#
#-------------------------------------------------

QT       += core
QT       -= gui

CONFIG(debug, debug|release) {
    TARGET = MarkEmptyDirs_debug
}
CONFIG(release, debug|release) {
    TARGET = MarkEmptyDirs
}

VERSION         = 2.0.0
VERSION_FULL    = $$VERSION-beta1

DESTDIR = $$_PRO_FILE_PWD_/../../../bin

TEMPLATE = app
CONFIG   += console
CONFIG   += static
CONFIG   += c++11
CONFIG   += debug_and_release

DEFINES += QODEMAGICSTATIC
DEFINES += MARKEMPTYDIRSAPISTATIC

QODEMAGIC_DIR = $$_PRO_FILE_PWD_/../../../externals-src/QodeMagic

HEADERS += \
    Program.hpp

SOURCES += \
    main.cpp \
    Program.cpp

INCLUDEPATH += \
    ../.. \
    $$QODEMAGIC_DIR/src

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../../bin \
    $$QODEMAGIC_DIR/bin

win32 {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi_debug \
            -lQodeMagic_debug
        PRE_TARGETDEPS += \
            $$DESTDIR/MarkEmptyDirsApi_debug.lib \
            $$QODEMAGIC_DIR/bin/QodeMagic_debug.lib
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi \
            -lQodeMagic
        PRE_TARGETDEPS += \
            $$DESTDIR/MarkEmptyDirsApi.lib \
            $$QODEMAGIC_DIR/bin/QodeMagic.lib
    }
} else {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi_debug \
            -lQodeMagic_debug
        PRE_TARGETDEPS += \
            $$DESTDIR/libMarkEmptyDirsApi_debug.a \
            $$QODEMAGIC_DIR/bin/libQodeMagic_debug.a
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi \
            -lQodeMagic
        PRE_TARGETDEPS += \
            $$DESTDIR/libMarkEmptyDirsApi.a \
            $$QODEMAGIC_DIR/bin/libQodeMagic.a
    }
}

# Get git commit hash
GIT_COMMIT_HASH = $$system(git rev-parse HEAD)

macx {
    include(osx/osx.pri)
}
win32 {
    include(win32/win32.pri)
}
