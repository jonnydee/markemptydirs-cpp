#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T21:11:13
#
#-------------------------------------------------

QT       += core
QT       -= gui

CONFIG(debug, debug|release) {
    TARGET = markemptydirs_debug
}
CONFIG(release, debug|release) {
    TARGET = markemptydirs
}

VERSION         = 2.0.0
VERSION_FULL    = $$VERSION-beta1

DESTDIR = $$_PRO_FILE_PWD_/../../../bin

TEMPLATE = app
CONFIG   += console
CONFIG   += static
CONFIG += c++11

DEFINES += QODEMAGICSTATIC
DEFINES += MARKEMPTYDIRSAPISTATIC

SOURCES += \
    main.cpp \
    Program.cpp

INCLUDEPATH += \
    ../..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../../bin

win32 {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi_debug \
            -lQodeMagic_debug
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/MarkEmptyDirsApi_debug.lib \
            $$QMAKE_LIBDIR/QodeMagic_debug.lib
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi \
            -lQodeMagic
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/MarkEmptyDirsApi.lib \
            $$QMAKE_LIBDIR/QodeMagic.lib
    }
} else {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi_debug \
            -lQodeMagic_debug
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/libMarkEmptyDirsApi_debug.a \
            $$QMAKE_LIBDIR/libQodeMagic_debug.a
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lMarkEmptyDirsApi \
            -lQodeMagic
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/libMarkEmptyDirsApi.a \
            $$QMAKE_LIBDIR/libQodeMagic.a
    }
}

HEADERS += \
    Program.hpp

# Get git commit hash
GIT_COMMIT_HASH = $$system(git rev-parse HEAD)

macx {
    include(osx/osx.pri)
}
win32 {
    include(win32/win32.pri)
}
