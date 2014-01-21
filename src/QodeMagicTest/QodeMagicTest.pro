#-------------------------------------------------
#
# Project created by QtCreator 2013-12-16T21:09:36
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = test_QodeMagic
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
DEFINES += QODEMAGICSTATIC

HEADERS += \
    test_QodeMagic_Text_Formatter.hpp \
    test_QodeMagic_Text_NumberTools.hpp \
    test_QodeMagic_Text_Template.hpp

SOURCES += \
    main.cpp

INCLUDEPATH += \
    ..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../bin

win32 {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lQodeMagic_debug
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/QodeMagic_debug.lib
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lQodeMagic
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/QodeMagic.lib
    }
} else {
    CONFIG(debug, debug|release) {
        LIBS += \
            -lQodeMagic_debug
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/libQodeMagic_debug.a
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lQodeMagic
        PRE_TARGETDEPS += \
            $$QMAKE_LIBDIR/libQodeMagic.a
    }
}
