#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:23:31
#
#-------------------------------------------------

QT       -= gui

TARGET = CodeMagic
VERSION = 0.1.0
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

DEFINES += CODEMAGIC_LIBRARY

SOURCES += \
    FileSystem/FileSystemTools.cpp \
    Text/Formatter.cpp \
    Text/Template/Engine.cpp \
    Text/Template/Variable.cpp \
    Text/Template/VariableFactory.cpp \
    Text/TextTools.cpp

HEADERS += codemagic_global.hpp \
    FileSystem/FileSystemTools.hpp \
    Text/Formatter.hpp \
    Text/Template/Engine.hpp \
    Text/Template/Variable.hpp \
    Text/Template/VariableFactory.hpp \
    Text/TextTools.hpp

INCLUDEPATH += \
    $$_PRO_FILE_PWD_/../../src

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
