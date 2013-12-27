// Copyright 2013 Johann Duscher (a.k.a. Jonny Dee). All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY JOHANN DUSCHER ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of Johann Duscher.

#pragma once
#ifndef MARKEMPTYDIRS_API_CONFIG_HPP
#define MARKEMPTYDIRS_API_CONFIG_HPP

#include "markemptydirsapi_global.hpp"
#include "LogLevel.hpp"

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QString>


namespace MarkEmptyDirs
{

namespace Api
{

struct MARKEMPTYDIRSAPISHARED_EXPORT ApplicationInfo
{
    struct Version
    {
        quint16 major;
        quint16 minor;
        quint16 bugfix;
        QString suffix;

        QString toString() const;
    };

    QString copyright;
    QString disclaimer;
    QString license;
    QString name;
    QString site;
    QString vendorEMail;
    QString vendorName;
    Version version;

    QString toString() const;
};

class MARKEMPTYDIRSAPISHARED_EXPORT Config
{
public:
    enum Command
    {
        HELP,
        UPDATE,
        CLEAN,
        OVERVIEW,
        PURGE,
        VERSION
    };

    typedef QList<QDir> DirList;

    Config();
    ~Config();

    void setApplicationInfo(const ApplicationInfo& applicationInfo);
    const ApplicationInfo& applicationInfo() const;

    void setCommand(Command command);
    Command command() const;

    void setCreateHookCommand(const QString& cmd);
    QString createHookCommand() const;

    void setDeleteHookCommand(const QString& cmd);
    QString deleteHookCommand() const;

    void setDryRun(bool dryRun);
    bool dryRun() const;

    void setExcludeDirs(const DirList& excludeDirs);
    DirList excludeDirs() const;

    void setExecutableFile(const QFileInfo& executableFile);
    QFileInfo executableFile() const;

    void setHelpText(const QString& text);
    QString helpText() const;

    void setLogLevel(LogLevel logLevel);
    LogLevel logLevel() const;

    void setMarkerName(const QString& fileName);
    QString markerName() const;

    void setMarkerText(const QString& text);
    QString markerText() const;

    void setDereferenceSymLinks(bool dereference);
    bool dereferenceSymLinks() const;

    void addRootDir(const QDir& rootDir);
    DirList rootDirs() const;

    void setShortMessages(bool shortMessages);
    bool shortMessages() const;

    void setSubstituteVariables(bool subst);
    bool substituteVariables() const;

    QString toString() const;

private:
    ApplicationInfo m_applicationInfo;
    Command m_command;
    QString m_createHookCommand;
    QString m_deleteHookCommand;
    bool m_dryRun;
    DirList m_excludeDirs;
    QFileInfo m_executableFile;
    QString m_helpText;
    LogLevel m_logLevel;
    QString m_markerName;
    QString m_markerText;
    bool m_dereferenceSymLinks;
    DirList m_rootDirs;
    bool m_shortMessages;
    bool m_substituteVariables;
};

}

}

#endif // MARKEMPTYDIRS_API_CONFIG_HPP
