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

std::unique_ptr<const Config> CommandLineInterface::createConfig(const Context& ctx, const QStringList& args, QStringList& errorMessages) const
{
    ArgumentParser parser;
    parser.addOptions(options());
    parser.parse(args);

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

    const auto arguments = parser.arguments();
    for (int i = 1; i < arguments.size(); i++)
    {
        const auto& arg = arguments[i];

        if (!arg.errorMessage.isNull())
            errorMessages << arg.errorMessage;

        if (arg.isBasedOn(dryRunOpt))
        {
            pConfig->setDryRun(true);
        }
        else if (arg.isBasedOn(createHookOpt))
        {
            pConfig->setCreateHookCommand(arg.value);
        }
        else if (arg.isBasedOn(deleteHookOpt))
        {
            pConfig->setDeleteHookCommand(arg.value);
        }
        else if (arg.isBasedOn(excludeOpt))
        {
            Config::DirList dirs;
            foreach (auto dir, arg.value.split(CodeMagic::FileSystem::pathSeparator(), QString::SkipEmptyParts))
                dirs.push_back(dir);
            pConfig->setExcludeDirs(dirs);
        }
        else if (arg.isBasedOn(shortOpt))
        {
            pConfig->setShortMessages(true);
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
        else if (arg.isBasedOn(markerOpt))
        {
            pConfig->setMarkerName(arg.value);
        }
        else if (arg.isBasedOn(textOpt))
        {
            pConfig->setMarkerText(arg.value);
        }
        else if (arg.isBasedOn(followSymLinksOpt))
        {
            pConfig->setDereferenceSymLinks(true);
        }
        else if (arg.isBasedOn(noFollowSymLinksOpt))
        {
            pConfig->setDereferenceSymLinks(false);
        }
        else if (arg.isBasedOn(substOpt))
        {
            pConfig->setSubstituteVariables(true);
        }
        else if (arg.isBasedOn(noSubstOpt))
        {
            pConfig->setSubstituteVariables(false);
        }
        else if (arg.isBasedOn(helpOpt))
        {
            pConfig->setCommand(Config::Command::HELP);
        }
        else if (arg.isBasedOn(updateOpt))
        {
            pConfig->setCommand(Config::Command::UPDATE);
        }
        else if (arg.isBasedOn(listOpt))
        {
            pConfig->setCommand(Config::Command::CLEAN);
            pConfig->setDryRun(true);
            pConfig->setShortMessages(true);
            pConfig->setLogLevel(LogLevel::INFO);
        }
        else if (arg.isBasedOn(cleanOpt))
        {
            pConfig->setCommand(Config::Command::CLEAN);
        }
        else if (arg.isBasedOn(overviewOpt))
        {
            pConfig->setCommand(Config::Command::OVERVIEW);
        }
        else if (arg.isBasedOn(purgeOpt))
        {
            pConfig->setCommand(Config::Command::PURGE);
        }
        else if (arg.isBasedOn(versionOpt))
        {
            pConfig->setCommand(Config::Command::VERSION);
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

        formatter.addUsageSection("[COMMAND] [OPTION]... DIR...");

        formatter.addOptionListSection(
                    QObject::tr("Command options"),
                    commandOptions());

        formatter.addOptionListSection(
                    QObject::tr("Other options"),
                    otherOptions());

        formatter.addTextSection(
                    QObject::tr("Template variables"),
                    formatVariableParagraphs(ctx.templateEngine().variables()));

        auto helpText = formatter.formatHelpText();

        pConfig->setHelpText(helpText);
    }

    return std::unique_ptr<const Config>(pConfig);
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
