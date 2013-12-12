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

SOURCES += \
    ../ArgumentTools/HelpFormatter.cpp \
    ../ArgumentTools/Option.cpp \
    ../ArgumentTools/OptionParser.cpp \
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
    CommandLineInterface.cpp

HEADERS += \
    ../ArgumentTools/HelpFormatter.hpp \
    ../ArgumentTools/Option.hpp \
    ../ArgumentTools/OptionParser.hpp \
    ../ArgumentTools/OptionParser_p.hpp \
    qmarkemptydirsapi_global.hpp \
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
    CommandLineInterface.hpp

INCLUDEPATH += \
    ..

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
