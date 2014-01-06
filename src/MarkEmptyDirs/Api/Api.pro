#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:23:31
#
#-------------------------------------------------

QT       -= gui

TARGET = MarkEmptyDirsApi
VERSION = 2.0.0
DESTDIR = $$_PRO_FILE_PWD_/../../../bin

TEMPLATE = lib
CONFIG += staticlib
CONFIG += static
CONFIG += c++11

macx {
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
}

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,_debug)
}

DEFINES += CODEMAGICSTATIC
DEFINES += MARKEMPTYDIRSAPI_LIBRARY

SOURCES += \
    FileSystemCrawler.cpp \
    ADirCommand.cpp \
    Config.cpp \
    UpdateCommand.cpp \
    OverviewCommand.cpp \
    PurgeCommand.cpp \
    VersionCommand.cpp \
    CleanCommand.cpp \
    Logger.cpp \
    ACommand.cpp \
    HelpCommand.cpp \
    CommandFactory.cpp \
    Context.cpp \
    VariableFactory.cpp

HEADERS += \
    markemptydirsapi_global.hpp \
    FileSystemCrawler.hpp \
    ADirCommand.hpp \
    DirDescriptor.hpp \
    Config.hpp \
    UpdateCommand.hpp \
    OverviewCommand.hpp \
    PurgeCommand.hpp \
    VersionCommand.hpp \
    CleanCommand.hpp \
    Logger.hpp \
    LogLevel.hpp \
    ICommand.hpp \
    ACommand.hpp \
    HelpCommand.hpp \
    CommandFactory.hpp \
    Context.hpp \
    VariableFactory.hpp

INCLUDEPATH += \
    ../..

QMAKE_LIBDIR += \
    $$_PRO_FILE_PWD_/../../../bin

CONFIG(debug, debug|release) {
    LIBS += \
        -lCodeMagic_debug
}
CONFIG(release, debug|release) {
    LIBS += \
        -lCodeMagic
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
