#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:23:31
#
#-------------------------------------------------

QT       -= gui

TARGET = ArgumentTools
VERSION = 0.7.0
DESTDIR = $$_PRO_FILE_PWD_/../../bin

TEMPLATE = lib
CONFIG += staticlib
CONFIG += static

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
CONFIG += c++11

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,_debug)
}

DEFINES += ARGUMENTTOOLS_LIBRARY

SOURCES += \
    ArgumentParser.cpp \
    ArgumentScanner.cpp \
    HelpFormatter.cpp \
    Option.cpp

HEADERS += \
    argumenttools_global.hpp \
    ArgumentParser.hpp \
    ArgumentScanner.hpp \
    HelpFormatter.hpp \
    Option.hpp

INCLUDEPATH += \
    ../..

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
