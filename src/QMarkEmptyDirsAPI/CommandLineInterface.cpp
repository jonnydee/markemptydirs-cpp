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
#include "HelpFormatter.hpp"
#include "OptionParser.hpp"

#ifdef Q_OS_WIN32
 #define PATH_LIST_SEPARATOR        ';'
#else
 #define PATH_LIST_SEPARATOR        ':'
#endif

#define DEFAULT_COMMAND             Config::UPDATE
#define DEFAULT_EXCLUDE_DIRS        ((QStringList() << ".bzr" << "CVS" << ".git" << ".hg" << ".svn").join(PATH_LIST_SEPARATOR))
#define DEFAULT_MARKER_CONTENT_FILENAME "marker.txt"
#define DEFAULT_MARKER_FILENAME     ".emptydir"


namespace MarkEmptyDirs
{

namespace Api
{

CommandLineInterface::CommandLineInterface()
    : dryRunOpt(
          QStringList() << "n" << "dry-run",
          QObject::tr("Simulate command execution without any side effects.", "dry-run"))
    , shortOpt(
          QStringList() << "short",
          QObject::tr("Output short verbose messages.", "short"))
    , verboseOpt(
          QStringList() << "v" << "verbose",
          QObject::tr("Output verbose messages.", "verbose"))
    , cleanOpt(
          QStringList() << "c" << "clean",
          QObject::tr("Delete all marker files.", "clean"))
    , helpOpt(
          QStringList() << "h" << "help",
          QObject::tr("Print help information.", "help"))
    , createHookOpt(
          QStringList() << "create-hook",
          QObject::tr("Invoke command after marker creation.", "create-hook"),
          QObject::tr("COMMAND", "create-hook"))
    , deleteHookOpt(
          QStringList() << "delete-hook",
          QObject::tr("Invoke command before marker deletion.", "delete-hook"),
          QObject::tr("COMMAND", "delete-hook"))
    , listOpt(
          QStringList() << "l" << "list",
          QObject::tr("List all marker files.", "list"))
    , purgeOpt(
          QStringList() << "purge",
          QObject::tr("Delete everything within directories containing markers.", "purge"))
    , excludeOpt(
          QStringList() << "x" << "exclude",
          QObject::tr("Skip excluded dirs.", "exclude"),
          QObject::tr("DIRS", "exclude"),
          DEFAULT_EXCLUDE_DIRS)
    , markerOpt(
          QStringList() << "m" << "marker-name",
          QObject::tr("Use another name for marker files.", "marker-name"),
          QObject::tr("NAME", "marker-name"))
    , textOpt(
          QStringList() << "text",
          QObject::tr("Create marker files with the specified text as content.", "text"),
          QObject::tr("CONTENT", "text"))
    , fileOpt(
          QStringList() << "F" << "file",
          QObject::tr("Create marker files using the specified template file as content.", "file"),
          QObject::tr("NAME", "file"),
          DEFAULT_MARKER_CONTENT_FILENAME)
    , substOpt(
          QStringList() << "subst",
          QObject::tr("Use variable subsitution.", "subst"))
    , noSubstOpt(
          QStringList() << "no-subst",
          QObject::tr("Do not use variable subsitution.", "subst"))
    , followSymLinksOpt(
          QStringList() << "L" << "dereference",
          QObject::tr("Follow symbolic links.", "dereference"))
    , noFollowSymLinksOpt(
          QStringList() << "no-dereference",
          QObject::tr("Do not follow symbolic links.", "dereference"))
    , overviewOpt(
          QStringList() << "overview",
          QObject::tr("Scan directory and show some overview statistics.", "overview"))
    , updateOpt(
          QStringList() << "u" << "update",
          QObject::tr("Create and delete marker files where necessary.", "update"))
    , versionOpt(
          QStringList() << "version",
          QObject::tr("Show version information.", "version"))
{
    m_commandOptions
        << &cleanOpt
        << &helpOpt
        << &listOpt
        << &purgeOpt
        << &overviewOpt
        << &updateOpt
        << &versionOpt;

    m_otherOptions
        << &dryRunOpt
        << &shortOpt
        << &verboseOpt
        << &createHookOpt
        << &deleteHookOpt
        << &excludeOpt
        << &markerOpt
        << &fileOpt
        << &substOpt
        << &noSubstOpt
        << &textOpt
        << &followSymLinksOpt
        << &noFollowSymLinksOpt;
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
        else if (arg.isBasedOn(markerOpt))
        {
            config.setMarkerName(arg.value);
        }
        else if (arg.isBasedOn(followSymLinksOpt))
        {
            config.setDereferenceSymLinks(true);
        }
        else if (arg.isBasedOn(noFollowSymLinksOpt))
        {
            config.setDereferenceSymLinks(false);
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

    // Set help text.
    {
        HelpFormatter formatter;

        formatter.addOptionListSection(
                    QObject::tr("Command options"),
                    commandOptions());

        formatter.addOptionListSection(
                    QObject::tr("Other options"),
                    otherOptions());

        auto helpText = formatter.formatHelpText();

        config.setHelpText(helpText);
    }

    return config;
}
OptionList CommandLineInterface::options() const
{
    return OptionList() << commandOptions() << otherOptions();
}

OptionList CommandLineInterface::commandOptions() const
{
    return m_commandOptions;
}

OptionList CommandLineInterface::otherOptions() const
{
    return m_otherOptions;
}

}

}
