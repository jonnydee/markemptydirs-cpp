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

VERSION = 2.0.0
DESTDIR = $$_PRO_FILE_PWD_/../../../bin

TEMPLATE = app
CONFIG   += console
CONFIG   += static

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++
CONFIG += c++11

SOURCES += \
    main.cpp \
    Program.cpp

INCLUDEPATH += \
    ../..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../../bin

CONFIG(debug, debug|release) {
    LIBS += \
        -lMarkEmptyDirsApi_debug \
        -lCodeMagic_debug
    PRE_TARGETDEPS += \
        $$QMAKE_LIBDIR/libMarkEmptyDirsApi_debug.a \
        $$QMAKE_LIBDIR/libCodeMagic_debug.a
}
CONFIG(release, debug|release) {
    LIBS += \
        -lMarkEmptyDirsApi \
        -lCodeMagic
    PRE_TARGETDEPS += \
        $$QMAKE_LIBDIR/libMarkEmptyDirsApi.a \
        $$QMAKE_LIBDIR/libCodeMagic.a
}

HEADERS += \
    Program.hpp

macx {
    include(osx/osx.pri)
}
