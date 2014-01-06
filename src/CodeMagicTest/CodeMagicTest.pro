#-------------------------------------------------
#
# Project created by QtCreator 2013-12-16T21:09:36
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = test_CodeMagic
DESTDIR = $$_PRO_FILE_PWD_/../../bin

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

macx {
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
}

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,_debug)
}

DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += CODEMAGICSTATIC

HEADERS += \
    test_CodeMagic_Text_Formatter.hpp \
    test_CodeMagic_Text_NumberTools.hpp \
    test_CodeMagic_Text_Template.hpp

SOURCES += \
    main.cpp

INCLUDEPATH += \
    ..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../bin

win32 {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lCodeMagic_debug
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/CodeMagic_debug.lib
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lCodeMagic
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/CodeMagic.lib
    }
} else {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lCodeMagic_debug
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/libCodeMagic_debug.a
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lCodeMagic
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/libCodeMagic.a
    }
}
