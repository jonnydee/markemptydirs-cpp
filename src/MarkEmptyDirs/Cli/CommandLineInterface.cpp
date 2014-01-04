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

#include <MarkEmptyDirs/Api/Config.hpp>
#include <MarkEmptyDirs/Api/Context.hpp>

#include <CodeMagic/Cli/HelpFormatter.hpp>
#include <CodeMagic/Cli/ArgumentParser.hpp>
#include <CodeMagic/FileSystem/FileSystemTools.hpp>
#include <CodeMagic/Text/Template/Engine.hpp>
#include <CodeMagic/Text/Template/Variable.hpp>

#define APPLICATION_DISCLAIMER          "This is free software; see the source for copying conditions. There is NO" "\n" \
                                        "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
#define APPLICATION_LICENSE             "BSD"
#define APPLICATION_NAME                "MarkEmptyDirs"
#define APPLICATION_SITE                "https://github.com/jonnydee/markemptydirs"
#define APPLICATION_VERSION_MAJOR       2
#define APPLICATION_VERSION_MINOR       0
#define APPLICATION_VERSION_BUGFIX      0
#define APPLICATION_VERSION_SUFFIX      "beta1"
#define APPLICATION_VENDOR_NAME         "Johann Duscher (a.k.a. Jonny Dee)"
#define APPLICATION_VENDOR_EMAIL        "jonny.dee@gmx.net"
#define APPLICATION_COPYRIGHT           "Copyright (C) 2013 " APPLICATION_VENDOR_NAME

#define DEFAULT_COMMAND                 Config::UPDATE
#define DEFAULT_EXCLUDE_DIRS            ((QStringList() << ".bzr" << "CVS" << ".git" << ".hg" << ".svn").join(CodeMagic::FileSystem::pathSeparator()))
#define DEFAULT_MARKER_CONTENT_FILENAME "marker.txt"
#define DEFAULT_MARKER_FILENAME         ".emptydir"


using namespace CodeMagic;
using namespace CodeMagic::Cli;
using namespace CodeMagic::Text;
using namespace MarkEmptyDirs::Api;

namespace MarkEmptyDirs
{

namespace Cli
{

namespace
{
    QStringList formatVariableParagraph(const Template::Variable& variable)
    {
        QStringList lines;
        // Format title.
        {
            auto title = QString("%1 --> %2").arg(variable.toString()).arg(variable.description());
            if (variable.hasArgument() && !variable.isArgumentMandatory())
                title += " (" + QObject::tr("default is '%1'").arg(variable.defaultArgument()) + ")";

            lines << title;
        }
        // Format argument descriptions.
        {
            foreach (const auto& argDescr, variable.argumentDescriptions())
                lines << QString("    %1 : %2").arg(argDescr.argument).arg(argDescr.description);
        }

        return lines;
    }

    QStringList formatVariableParagraphs(const Template::VariableList& variables)
    {
        QStringList paragraphs;
        foreach (const auto pVariable, variables)
            paragraphs << formatVariableParagraph(*pVariable) << QString();
        if (!paragraphs.isEmpty())
            paragraphs.removeLast();
        return paragraphs;
    }
}

CommandLineInterface::CommandLineInterface()
    : cleanCmd(
          QStringList() << "clean",
          QObject::tr("Delete all marker files.", "clean"))
    , helpCmd(
          QStringList() << "help",
          QObject::tr("Print help information.", "help"))
    , listCmd(
          QStringList() << "list",
          QObject::tr("List all marker files.", "list"))
    , overviewCmd(
          QStringList() << "overview",
          QObject::tr("Scan directory and show some overview statistics.", "overview"))
    , purgeCmd(
          QStringList() << "purge",
          QObject::tr("Delete everything within directories containing markers.", "purge"))
    , updateCmd(
          QStringList() << "update",
          QObject::tr("Create and delete marker files where necessary.", "update"))
    , versionCmd(
          QStringList() << "version",
          QObject::tr("Show version information.", "version"))
    , createHookOpt(
          QStringList() << "create-hook",
          QObject::tr("Invoke command after marker creation.", "create-hook"),
          QObject::tr("COMMAND", "create-hook"))
    , deleteHookOpt(
          QStringList() << "delete-hook",
          QObject::tr("Invoke command before marker deletion.", "delete-hook"),
          QObject::tr("COMMAND", "delete-hook"))
    , dryRunOpt(
          QStringList() << "n" << "dry-run",
          QObject::tr("Simulate command execution without any side effects.", "dry-run"))
    , excludeOpt(
          QStringList() << "x" << "exclude",
          QObject::tr("Skip excluded dirs.", "exclude"),
          QObject::tr("DIRS", "exclude"),
          DEFAULT_EXCLUDE_DIRS)
    , fileOpt(
          QStringList() << "F" << "file",
          QObject::tr("Create marker files using the specified template file as content.", "file"),
          QObject::tr("NAME", "file"),
          DEFAULT_MARKER_CONTENT_FILENAME)
    , followSymLinksOpt(
          QStringList() << "L" << "dereference",
          QObject::tr("Follow symbolic links.", "dereference"))
    , helpOpt(
          QStringList() << "h" << "help",
          QObject::tr("Print help information.", "help"))
    , markerOpt(
          QStringList() << "m" << "marker-name",
          QObject::tr("Use another name for marker files.", "marker-name"),
          QObject::tr("NAME", "marker-name"),
          QString(),
          &FileSystem::validateFileName)
    , noFollowSymLinksOpt(
          QStringList() << "no-dereference",
          QObject::tr("Do not follow symbolic links.", "dereference"))
    , noSubstOpt(
          QStringList() << "no-subst",
          QObject::tr("Do not use variable subsitution.", "subst"))
    , shortOpt(
          QStringList() << "short",
          QObject::tr("Output short verbose messages.", "short"))
    , substOpt(
          QStringList() << "subst",
          QObject::tr("Use variable subsitution.", "subst"))
    , textOpt(
          QStringList() << "text",
          QObject::tr("Create marker files with the specified text as content.", "text"),
          QObject::tr("CONTENT", "text"))
    , verboseOpt(
          QStringList() << "v" << "verbose",
          QObject::tr("Output verbose messages.", "verbose"))
{
    m_commands
        << &cleanCmd
        << &helpCmd
        << &listCmd
        << &overviewCmd
        << &purgeCmd
        << &updateCmd
        << &versionCmd;

    m_options
        << &createHookOpt
        << &deleteHookOpt
        << &dryRunOpt
        << &excludeOpt
        << &fileOpt
        << &followSymLinksOpt
        << &helpOpt
        << &markerOpt
        << &noFollowSymLinksOpt
        << &noSubstOpt
        << &shortOpt
        << &substOpt
        << &textOpt
        << &verboseOpt;
}

std::unique_ptr<const Config> CommandLineInterface::createConfig(const Context& ctx, const QStringList& args, QStringList& errorMessages) const
{
    ApplicationInfo appInfo;
    appInfo.copyright = APPLICATION_COPYRIGHT;
    appInfo.disclaimer = APPLICATION_DISCLAIMER;
    appInfo.license = APPLICATION_LICENSE;
    appInfo.name = APPLICATION_NAME;
    appInfo.site = APPLICATION_SITE;
    appInfo.vendorEMail = APPLICATION_VENDOR_EMAIL;
    appInfo.vendorName = APPLICATION_VENDOR_NAME;
    appInfo.version.major = APPLICATION_VERSION_MAJOR;
    appInfo.version.minor = APPLICATION_VERSION_MINOR;
    appInfo.version.bugfix = APPLICATION_VERSION_BUGFIX;
    appInfo.version.suffix = APPLICATION_VERSION_SUFFIX;

    auto pConfig = new Config;
    pConfig->setApplicationInfo(appInfo);
    pConfig->setExecutableFile(args[0]);

    auto argsOnly = args;
    // Remove first argument, because it is the executable itself.
    argsOnly.removeFirst();

    ArgumentParser parser;
    parser.addCommands(commands());
    parser.addOptions(options());
    parser.parse(argsOnly);

    const auto arguments = parser.arguments();
    for (int i = 0; i < arguments.size(); i++)
    {
        const auto& arg = arguments[i];

        // If no command has been specified, but help option is present we configure HELP command.
        if (Argument::COMMAND == arg.type && arg.name.isNull() && !parser.findArgument(helpOpt).isNull())
        {
            pConfig->setCommand(Config::Command::HELP);
            continue;
        }

        if (!arg.errorMessage.isNull())
            errorMessages << arg.errorMessage;

        if (arg.isBasedOn(cleanCmd))
        {
            pConfig->setCommand(Config::Command::CLEAN);
        }
        else if (arg.isBasedOn(helpCmd))
        {
            pConfig->setCommand(Config::Command::HELP);
        }
        else if (arg.isBasedOn(listCmd))
        {
            pConfig->setCommand(Config::Command::CLEAN);
            pConfig->setDryRun(true);
            pConfig->setShortMessages(true);
            pConfig->setLogLevel(LogLevel::INFO);
        }
        else if (arg.isBasedOn(overviewCmd))
        {
            pConfig->setCommand(Config::Command::OVERVIEW);
        }
        else if (arg.isBasedOn(purgeCmd))
        {
            pConfig->setCommand(Config::Command::PURGE);
        }
        else if (arg.isBasedOn(updateCmd))
        {
            pConfig->setCommand(Config::Command::UPDATE);
        }
        else if (arg.isBasedOn(versionCmd))
        {
            pConfig->setCommand(Config::Command::VERSION);
        }
        else if (arg.isBasedOn(createHookOpt))
        {
            pConfig->setCreateHookCommand(arg.value);
        }
        else if (arg.isBasedOn(deleteHookOpt))
        {
            pConfig->setDeleteHookCommand(arg.value);
        }
        else if (arg.isBasedOn(dryRunOpt))
        {
            pConfig->setDryRun(true);
        }
        else if (arg.isBasedOn(excludeOpt))
        {
            Config::DirList dirs;
            foreach (auto dir, arg.value.split(CodeMagic::FileSystem::pathSeparator(), QString::SkipEmptyParts))
                dirs.push_back(dir);
            pConfig->setExcludeDirs(dirs);
        }
        else if (arg.isBasedOn(followSymLinksOpt))
        {
            pConfig->setDereferenceSymLinks(true);
        }
        else if (arg.isBasedOn(helpOpt))
        {
            pConfig->setCommand(Config::Command::HELP);
        }
        else if (arg.isBasedOn(markerOpt))
        {
            pConfig->setMarkerName(arg.value);
        }
        else if (arg.isBasedOn(noFollowSymLinksOpt))
        {
            pConfig->setDereferenceSymLinks(false);
        }
        else if (arg.isBasedOn(noSubstOpt))
        {
            pConfig->setSubstituteVariables(false);
        }
        else if (arg.isBasedOn(shortOpt))
        {
            pConfig->setShortMessages(true);
        }
        else if (arg.isBasedOn(substOpt))
        {
            pConfig->setSubstituteVariables(true);
        }
        else if (arg.isBasedOn(textOpt))
        {
            pConfig->setMarkerText(arg.value);
        }
        else if (arg.isBasedOn(verboseOpt))
        {
            switch (pConfig->logLevel())
            {
            case LogLevel::NONE:
                pConfig->setLogLevel(LogLevel::INFO);
                break;
            case LogLevel::INFO:
            default:
                pConfig->setLogLevel(LogLevel::DEBUG);
            }
        }
        else
        {
            pConfig->addRootDir(QDir(arg.value));
        }
    }

    // Set help text.
    {
        HelpFormatter formatter;

        formatter.setExecutableFileName(pConfig->executableFile().fileName());

        formatter.addUsageSection("COMMAND [OPTION]... DIR...");

        formatter.addCommandListSection(
                    QObject::tr("Commands"),
                    commands());

        formatter.addOptionListSection(
                    QObject::tr("Options"),
                    options());

        formatter.addTextSection(
                    QObject::tr("Template variables"),
                    formatVariableParagraphs(ctx.templateEngine().variables()));

        auto helpText = formatter.formatHelpText();

        pConfig->setHelpText(helpText);
    }

    return std::unique_ptr<const Config>(pConfig);
}

CommandList CommandLineInterface::commands() const
{
    return m_commands;
}

OptionList CommandLineInterface::options() const
{
    return m_options;
}

}

}
