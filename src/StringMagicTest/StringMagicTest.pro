#-------------------------------------------------
#
# Project created by QtCreator 2013-12-16T21:09:36
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = test_StringMagic
DESTDIR = $$_PRO_FILE_PWD_/../../bin

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
CONFIG += c++11

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,_debug)
}

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    test_StringMagic_Template.hpp \
    test_StringMagic_TextFormatter.hpp

SOURCES += \
    main.cpp

INCLUDEPATH += \
    ..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../bin

CONFIG(debug, debug|release) {
    LIBS += \
        -lStringMagic_debug
}
CONFIG(release, debug|release) {
    LIBS += \
        -lStringMagic
}