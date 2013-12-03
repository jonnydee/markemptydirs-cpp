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

SOURCES += main.cpp \
    ../QMarkEmptyDirsAPI/FileSystemCrawler.cpp \
    ../QMarkEmptyDirsAPI/Config.cpp \
    ../QMarkEmptyDirsAPI/UpdateCommand.cpp \
    ../QMarkEmptyDirsAPI/ADirCommand.cpp \
    ../QMarkEmptyDirsAPI/OverviewCommand.cpp \
    ../QMarkEmptyDirsAPI/CleanCommand.cpp \
    ../QMarkEmptyDirsAPI/Logger.cpp \
    ../QMarkEmptyDirsAPI/OptionParser.cpp \
    ../QMarkEmptyDirsAPI/ACommand.cpp

INCLUDEPATH += \
    ..

#QMAKE_LIBDIR += \
#    /Users/jonnydee/Projekte/QMarkEmptyDirs/bin/Debug/Qt5_64bit

#LIBS += \
#    -lQMarkEmptyDirsAPI

HEADERS += \
    ../QMarkEmptyDirsAPI/qmarkemptydirsapi_global.hpp \
    ../QMarkEmptyDirsAPI/FileSystemCrawler.hpp \
    ../QMarkEmptyDirsAPI/Config.hpp \
    ../QMarkEmptyDirsAPI/UpdateCommand.hpp \
    ../QMarkEmptyDirsAPI/ADirCommand.hpp \
    ../QMarkEmptyDirsAPI/OverviewCommand.hpp \
    ../QMarkEmptyDirsAPI/CleanCommand.hpp \
    ../QMarkEmptyDirsAPI/Logger.hpp \
    ../QMarkEmptyDirsAPI/OptionParser.hpp \
    ../QMarkEmptyDirsAPI/ACommand.hpp
