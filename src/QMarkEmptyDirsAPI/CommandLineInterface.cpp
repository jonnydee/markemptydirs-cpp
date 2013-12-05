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

#include "CommandLineInterface.hpp"
#include "OptionParser.hpp"

#define DEFAULT_COMMAND             Config::UPDATE
#define DEFAULT_EXCLUDE_DIRS        ".bzr:CVS:.git:.hg:.svn"
#define DEFAULT_MARKER_FILENAME     ".emptydir"

#ifdef Q_OS_WIN32
 #define PATH_LIST_SEPARATOR        ';'
#else
 #define PATH_LIST_SEPARATOR        ':'
#endif

namespace MarkEmptyDirs
{

namespace Api
{

CommandLineInterface::CommandLineInterface()
    : dryRunOpt(
          QStringList() << "n" << "dry-run",
          QObject::tr("simulate command execution without any side effects", "dry-run"))
    , shortOpt(
          QStringList() << "short",
          QObject::tr("output short verbose messages", "short"))
    , verboseOpt(
          QStringList() << "v" << "verbose",
          QObject::tr("output verbose messages", "verbose"))
    , cleanOpt(
          QStringList() << "c" << "clean",
          QObject::tr("delete all placeholder files", "clean"))
    , helpOpt(
          QStringList() << "h" << "help",
          QObject::tr("print help information", "help"))
    , createHookOpt(
          QStringList() << "create-hook",
          QObject::tr("invoke command after placeholder creation (use template variables)", "create-hook"),
          QObject::tr("COMMAND", "create-hook"))
    , deleteHookOpt(
          QStringList() << "delete-hook",
          QObject::tr("invoke command before placeholder deletion (use template variables)", "delete-hook"),
          QObject::tr("COMMAND", "delete-hook"))
    , listOpt(
          QStringList() << "l" << "list",
          QObject::tr("list all placeholder files", "list"))
    , purgeOpt(
          QStringList() << "purge",
          QObject::tr("delete everything within directories containing placeholders", "purge"))
    , excludeOpt(
          QStringList() << "x" << "exclude",
          QObject::tr("skip excluded dirs", "exclude"),
          QObject::tr("DIRS", "exclude"),
          DEFAULT_EXCLUDE_DIRS)
    , placeHolderOpt(
          QStringList() << "p" << "place-holder",
          QObject::tr("use another name for placeholder files", "place-holder"),
          QObject::tr("NAME", "place-holder"))
    , textOpt(
          QStringList() << "text",
          QObject::tr("create placeholder files with the specified text as content", "text"),
          QObject::tr("CONTENT", "text"))
    , fileOpt(
          QStringList() << "F" << "file",
          QObject::tr("create placeholder files using the specified template file as content", "file"),
          QObject::tr("NAME", "file"))
    , substOpt(
          QStringList() << "subst",
          QObject::tr("use variable subsitution", "subst"))
    , noSubstOpt(
          QStringList() << "no-subst",
          QObject::tr("do not use variable subsitution", "subst"))
    , followSymLinksOpt(
          QStringList() << "L" << "dereference",
          QObject::tr("follow symbolic links", "dereference"))
    , noFollowSymLinksOpt(
          QStringList() << "no-dereference",
          QObject::tr("do not follow symbolic links", "dereference"))
    , overviewOpt(
          QStringList() << "overview",
          QObject::tr("scan directory and show some overview statistics", "overview"))
    , updateOpt(
          QStringList() << "u" << "update",
          QObject::tr("create and delete placeholder files where necessary", "update"))
    , versionOpt(
          QStringList() << "version",
          QObject::tr("show version information", "version"))
{
    m_options
        << &dryRunOpt
        << &shortOpt
        << &verboseOpt
        << &cleanOpt
        << &helpOpt
        << &createHookOpt
        << &deleteHookOpt
        << &listOpt
        << &purgeOpt
        << &excludeOpt
        << &placeHolderOpt
        << &fileOpt
        << &substOpt
        << &noSubstOpt
        << &textOpt
        << &followSymLinksOpt
        << &noFollowSymLinksOpt
        << &updateOpt
        << &versionOpt;
}

Config CommandLineInterface::createConfig(const QStringList& args) const
{
    OptionParser parser;
    parser.addOptions(options());
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
        else if (arg.isBasedOn(excludeOpt))
        {
            Config::DirList dirs;
            foreach (auto dir, arg.value.split(PATH_LIST_SEPARATOR, QString::SkipEmptyParts))
                dirs.push_back(dir);
            config.setExcludeDirs(dirs);
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
        else if (arg.isBasedOn(noFollowSymLinksOpt))
        {
            config.setResolveSymLinks(false);
        }
        else if (arg.isBasedOn(substOpt))
        {
            config.setSubstituteVariables(true);
        }
        else if (arg.isBasedOn(noSubstOpt))
        {
            config.setSubstituteVariables(false);
        }
        else if (arg.isBasedOn(helpOpt))
        {
            config.setCommand(Config::Command::HELP);
        }
        else if (arg.isBasedOn(updateOpt))
        {
            config.setCommand(Config::Command::UPDATE);
        }
        else if (arg.isBasedOn(listOpt))
        {
            config.setCommand(Config::Command::CLEAN);
            config.setDryRun(true);
            config.setShortMessages(true);
            config.setLogLevel(LogLevel::INFO);
        }
        else if (arg.isBasedOn(cleanOpt))
        {
            config.setCommand(Config::Command::CLEAN);
        }
        else if (arg.isBasedOn(overviewOpt))
        {
            config.setCommand(Config::Command::OVERVIEW);
        }
        else if (arg.isBasedOn(versionOpt))
        {
            config.setCommand(Config::Command::VERSION);
        }
        else if (i > 0)
        {
            config.addRootDir(QDir(arg.value));
        }
    }

    return config;
}
const OptionList& CommandLineInterface::options() const
{
    return m_options;
}

}

}
