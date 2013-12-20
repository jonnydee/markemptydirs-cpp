#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:23:31
#
#-------------------------------------------------

QT       -= gui

TARGET = StringMagic
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

DEFINES += STRINGMAGIC_LIBRARY

SOURCES += \
    Template/Engine.cpp \
    Template/Variable.cpp \
    Template/VariableFactory.cpp \
    FileSystem.cpp \
    TextFormatter.cpp \
    Tools.cpp

HEADERS += stringmagic_global.hpp \
    Template/Engine.hpp \
    Template/Variable.hpp \
    Template/VariableFactory.hpp \
    FileSystem.hpp \
    TextFormatter.hpp \
    Tools.hpp

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
