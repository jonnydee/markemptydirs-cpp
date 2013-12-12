#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T21:11:13
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QMarkEmptyDirs
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
CONFIG += c++11

SOURCES += \
    ../../ArgumentTools/ArgumentParser.cpp \
    ../../ArgumentTools/ArgumentScanner.cpp \
    ../../ArgumentTools/HelpFormatter.cpp \
    ../../ArgumentTools/Option.cpp \
    ../Api/FileSystemCrawler.cpp \
    ../Api/Config.cpp \
    ../Api/UpdateCommand.cpp \
    ../Api/ADirCommand.cpp \
    ../Api/OverviewCommand.cpp \
    ../Api/PurgeCommand.cpp \
    ../Api/VersionCommand.cpp \
    ../Api/CleanCommand.cpp \
    ../Api/Logger.cpp \
    ../Api/ACommand.cpp \
    ../Api/HelpCommand.cpp \
    ../Api/CommandFactory.cpp \
    CommandLineInterface.cpp \
    main.cpp

INCLUDEPATH += \
    ../..

#QMAKE_LIBDIR += \
#    /Users/jonnydee/Projekte/QMarkEmptyDirs/bin/Debug/Qt5_64bit

#LIBS += \
#    -lQMarkEmptyDirsAPI

HEADERS += \
    ../../ArgumentTools/ArgumentParser.hpp \
    ../../ArgumentTools/ArgumentScanner.hpp \
    ../../ArgumentTools/HelpFormatter.hpp \
    ../../ArgumentTools/Option.hpp \
    ../Api/qmarkemptydirsapi_global.hpp \
    ../Api/FileSystemCrawler.hpp \
    ../Api/Config.hpp \
    ../Api/UpdateCommand.hpp \
    ../Api/ADirCommand.hpp \
    ../Api/OverviewCommand.hpp \
    ../Api/PurgeCommand.hpp \
    ../Api/VersionCommand.hpp \
    ../Api/CleanCommand.hpp \
    ../Api/Logger.hpp \
    ../Api/ACommand.hpp \
    ../Api/HelpCommand.hpp \
    ../Api/CommandFactory.hpp \
    CommandLineInterface.hpp
