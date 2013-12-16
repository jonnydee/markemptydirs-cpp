#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:23:31
#
#-------------------------------------------------

QT       -= gui

TARGET = StringTemplate
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

DEFINES += STRINGTEMPLATE_LIBRARY

SOURCES += \
    stringtemplate_global.hpp \
    Engine.cpp \
    Variable.cpp

HEADERS += \
    Engine.hpp \
    Variable.hpp

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
