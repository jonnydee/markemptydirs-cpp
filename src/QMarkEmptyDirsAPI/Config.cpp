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
#define DEFAULT_FILE_FILENAME       "placeholder.txt"
#define DEFAULT_MARKER_FILENAME     ".emptydir"
#define DEFAULT_TEXT_CONTENT        ""


namespace MarkEmptyDirs
{

namespace Api
{

Config Config::createFromCommandLineArguments(const QStringList& args)
{
    Option dryRunOpt(QStringList() << "d" << "dry-run", "simulate command execution without any side effects");
    Option shortOpt(QStringList() << "s" << "short", "output short verbose messages");
    Option verboseOpt(QStringList() << "v" << "verbose", "output verbose messages", "level", "1");
    Option cleanOpt(QStringList() << "c" << "clean", "delete all placeholder files");
    Option helpOpt(QStringList() << "h" << "help", "print help information");
    Option createHookOpt(QStringList() << "a" << "create-hook", "invoke command after placeholder creation (use template variables)");
    Option deleteHookOpt(QStringList() << "r" << "delete-hook", "invoke command before placeholder deletion (use template variables)");
    Option listOpt(QStringList() << "l" << "list", "list all placeholder files");
    Option purgeOpt(QStringList() << "g" << "purge", "delete everything within directories containing placeholders");
    Option excludeOpt(QStringList() << "x" << "exclude", "skip excluded dirs", "dirs", ".bzr:CVS:.git:.hg:.svn");
    Option placeHolderOpt(QStringList() << "p" << "place-holder", "use another name for placeholder files", "name", DEFAULT_MARKER_FILENAME);
    Option textOpt(QStringList() << "t" << "text", "create placeholder files with the specified text as content", "content", DEFAULT_TEXT_CONTENT);
    Option fileOpt(QStringList() << "f" << "file", "create placeholder files using the specified template file as content", "name", DEFAULT_FILE_FILENAME);
    Option substOpt(QStringList() << "b" << "subst", "use variable subsitution");
    Option followSymLinksOpt(QStringList() << "m" << "follow-symlinks", "follow symbolic links");
    Option overviewOpt(QStringList() << "o" << "overview", "scan directory and show some overview statistics");
    Option updateOpt(QStringList() << "u" << "update", "create and delete placeholder files where necessary");

    OptionParser parser;
    parser.addOption(dryRunOpt);
    parser.addOption(shortOpt);
    parser.addOption(verboseOpt);
    parser.addOption(cleanOpt);
    parser.addOption(helpOpt);
    parser.addOption(createHookOpt);
    parser.addOption(deleteHookOpt);
    parser.addOption(listOpt);
    parser.addOption(purgeOpt);
    parser.addOption(excludeOpt);
    parser.addOption(placeHolderOpt);
    parser.addOption(fileOpt);
    parser.addOption(substOpt);
    parser.addOption(textOpt);
    parser.addOption(followSymLinksOpt);
    parser.addOption(updateOpt);

    parser.parse(args);

    Config config;

    const auto arguments = parser.arguments();
    for (int i = 0; i < arguments.size(); i++)
    {
        const auto& arg = arguments[i];

        if (arg.isBasedOn(dryRunOpt))
        {
            config.setDryRun(true);
        }
        else if (arg.isBasedOn(shortOpt))
        {
            config.setShortMessages(true);
        }
        else if (arg.isBasedOn(verboseOpt))
        {
            switch (config.logLevel())
            {
            case LogLevel::NONE:
                config.setLogLevel(LogLevel::INFO);
                break;
            case LogLevel::INFO:
            default:
                config.setLogLevel(LogLevel::DEBUG);
            }
        }
        else if (arg.isBasedOn(placeHolderOpt))
        {
            config.setMarkerFileName(arg.value);
        }
        else if (arg.isBasedOn(followSymLinksOpt))
        {
            config.setResolveSymLinks(true);
        }
        else if (arg.isBasedOn(helpOpt))
        {
            config.setCommand(Command::HELP);
        }
        else if (arg.isBasedOn(updateOpt))
        {
            config.setCommand(Command::UPDATE);
        }
        else if (arg.isBasedOn(listOpt))
        {
            config.setCommand(Command::CLEAN);
            config.setDryRun(true);
            config.setShortMessages(true);
            config.setLogLevel(LogLevel::INFO);
        }
        else if (arg.isBasedOn(cleanOpt))
        {
            config.setCommand(Command::CLEAN);
        }
        else if (arg.isBasedOn(overviewOpt))
        {
            config.setCommand(Command::OVERVIEW);
        }
        else if (i > 0)
        {
            config.setRootDir(QDir(arg.value));
        }
    }

    return config;
}

Config::Config()
    : m_command(DEFAULT_COMMAND)
    , m_dryRun(false)
    , m_logLevel(LogLevel::NONE)
    , m_markerFileName(DEFAULT_MARKER_FILENAME)
    , m_resolveSymLinks(false)
    , m_rootDir(".")
    , m_shortMessages(false)
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

void Config::setLogLevel(LogLevel logLevel)
{
    m_logLevel = logLevel;
}

LogLevel Config::logLevel() const
{
    return m_logLevel;
}

void Config::setMarkerFileName(const QString& fileName)
{
    m_markerFileName = fileName;
}

QString Config::markerFileName() const
{
    return m_markerFileName;
}

void Config::setResolveSymLinks(bool resolve)
{
    m_resolveSymLinks = resolve;
}

bool Config::resolveSymLinks() const
{
    return m_resolveSymLinks;
}

void Config::setRootDir(const QDir& rootDir)
{
    m_rootDir = rootDir;
}

QDir Config::rootDir() const
{
    return m_rootDir;
}

void Config::setShortMessages(bool shortMessages)
{
    m_shortMessages = shortMessages;
}

bool Config::shortMessages() const
{
    return m_shortMessages;
}

template <typename T>
static QString nameValueStr(const QString& name, const T& value)
{
    auto valueStr = QString("%1").arg(value);
    valueStr = valueStr.replace(QChar('\"'), "\\\"");
    return QString("%1: \"%2\"").arg(name).arg(valueStr);
}

QString Config::toString() const
{
    return QString("[%1]").arg(
        (QStringList()
            << nameValueStr("command", command())
            << nameValueStr("dryRun", dryRun())
            << nameValueStr("logLevel", logLevel())
            << nameValueStr("markerFileName", markerFileName())
            << nameValueStr("resolveSymLinks", resolveSymLinks())
            << nameValueStr("rootDir", rootDir().canonicalPath())
            << nameValueStr("shortMessages", shortMessages())
        ).join(", "));
}

}

}
