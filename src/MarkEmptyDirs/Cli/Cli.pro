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
CONFIG   -= app_bundle
CONFIG   += static

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
CONFIG += c++11

SOURCES += \
    CommandLineInterface.cpp \
    main.cpp

INCLUDEPATH += \
    ../..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../../bin

CONFIG(debug, debug|release) {
    LIBS += \
        -lMarkEmptyDirsApi_debug \
        -lArgumentTools_debug \
        -lStringMagic_debug
}
CONFIG(release, debug|release) {
    LIBS += \
        -lMarkEmptyDirsApi \
        -lArgumentTools \
        -lStringMagic
}

HEADERS += \
    CommandLineInterface.hpp
