#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:23:31
#
#-------------------------------------------------

QT       -= gui

TARGET = QMarkEmptyDirsAPI
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
CONFIG += c++11

DEFINES += QMARKEMPTYDIRSAPI_LIBRARY

SOURCES += QMarkEmptyDirsAPI.cpp \
    FileSystemCrawler.cpp \
    ADirCommand.cpp \
    Config.cpp \
    UpdateCommand.cpp \
    OverviewCommand.cpp \
    CleanCommand.cpp \
    Logger.cpp \
    OptionParser.cpp \
    ACommand.cpp \
    HelpCommand.cpp \
    HelpFormatter.cpp \
    CommandLineInterface.cpp \
    Option.cpp

HEADERS += QMarkEmptyDirsAPI.hpp\
        qmarkemptydirsapi_global.hpp \
    FileSystemCrawler.hpp \
    ADirCommand.hpp \
    DirDescriptor.hpp \
    Config.hpp \
    UpdateCommand.hpp \
    OverviewCommand.hpp \
    CleanCommand.hpp \
    Logger.hpp \
    LogLevel.hpp \
    OptionParser.hpp \
    OptionParser_p.hpp \
    ICommand.hpp \
    ACommand.hpp \
    HelpCommand.hpp \
    HelpFormatter.hpp \
    CommandLineInterface.hpp \
    Option.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
