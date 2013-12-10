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

#include "Config.hpp"
#include "OptionParser.hpp"

#include <QDebug>

#define DEFAULT_COMMAND             Config::UPDATE
#define DEFAULT_MARKER_NAME         ".emptydir"
#define DEFAULT_TEXT_CONTENT        ""


namespace MarkEmptyDirs
{

namespace Api
{

template <typename T>
static QString valueStr(const T& value)
{
    auto valueStr = QString("%1").arg(value);
    valueStr = valueStr.replace(QChar('\"'), "\\\"");
    valueStr = valueStr.replace(QChar('\n'), "\\n");
    return QString("\"%1\"").arg(valueStr);
}

template <typename T>
static QString listValueStr(const T& list, std::function<QString(const typename T::value_type&)> convert)
{
    QStringList strValues;
    foreach (const auto& value, list)
        strValues << valueStr(convert(value));
    return QString("[%1]").arg(strValues.join(", "));
}

template <typename T>
static QString nameValueStr(const QString& name, const T& value)
{
    return QString("%1: %2").arg(valueStr(name)).arg(value);
}



Config::Config()
    : m_command(DEFAULT_COMMAND)
    , m_dryRun(false)
    , m_excludeDirs()
    , m_logLevel(LogLevel::NONE)
    , m_markerName(DEFAULT_MARKER_NAME)
    , m_dereferenceSymLinks(false)
    , m_shortMessages(false)
    , m_substituteVariables(true)
{
}

Config::~Config()
{
}

void Config::setCommand(Command command)
{
    m_command = command;
}

Config::Command Config::command() const
{
    return m_command;
}

void Config::setDryRun(bool dryRun)
{
    m_dryRun = dryRun;
}

bool Config::dryRun() const
{
    return m_dryRun;
}

void Config::setExcludeDirs(const DirList& excludeDirs)
{
    m_excludeDirs = excludeDirs;
}

Config::DirList Config::excludeDirs() const
{
    return m_excludeDirs;
}

void Config::setExecutableFile(const QFileInfo& executableFile)
{
    m_executableFile = executableFile;
}

QFileInfo Config::executableFile() const
{
    return m_executableFile;
}

void Config::setHelpText(const QString& text)
{
    m_helpText = text;
}

QString Config::helpText() const
{
    return m_helpText;
}

void Config::setLogLevel(LogLevel logLevel)
{
    m_logLevel = logLevel;
}

LogLevel Config::logLevel() const
{
    return m_logLevel;
}

void Config::setMarkerName(const QString& fileName)
{
    m_markerName = fileName;
}

QString Config::markerName() const
{
    return m_markerName;
}

void Config::setDereferenceSymLinks(bool dereference)
{
    m_dereferenceSymLinks = dereference;
}

bool Config::dereferenceSymLinks() const
{
    return m_dereferenceSymLinks;
}

void Config::addRootDir(const QDir& rootDir)
{
    m_rootDirs.push_back(rootDir);
}

Config::DirList Config::rootDirs() const
{
    return m_rootDirs;
}

void Config::setShortMessages(bool shortMessages)
{
    m_shortMessages = shortMessages;
}

bool Config::shortMessages() const
{
    return m_shortMessages;
}

void Config::setSubstituteVariables(bool subst)
{
    m_substituteVariables = subst;
}

bool Config::substituteVariables() const
{
    return m_substituteVariables;
}

QString Config::toString() const
{
    return QString("{%1}").arg(
        (QStringList()
            << nameValueStr("applicationInfo", applicationInfo().toString())
            << nameValueStr("command", command())
            << nameValueStr("dryRun", dryRun())
            << nameValueStr("excludeDirs", listValueStr(excludeDirs(), [](const QDir& dir) { return dir.path(); }))
            << nameValueStr("executableFile", valueStr(executableFile().fileName()))
            << nameValueStr("logLevel", logLevel())
            << nameValueStr("markerName", valueStr(markerName()))
            << nameValueStr("dereferenceSymLinks", dereferenceSymLinks())
            << nameValueStr("rootDirs", listValueStr(rootDirs(), [](const QDir& dir) { return dir.path(); }))
            << nameValueStr("shortMessages", shortMessages())
            << nameValueStr("substituteVariables", substituteVariables())
        ).join(", "));
}

}

}
