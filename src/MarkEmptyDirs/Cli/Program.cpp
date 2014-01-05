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

#include "Program.hpp"

#include <MarkEmptyDirs/Api/CommandFactory.hpp>
#include <MarkEmptyDirs/Api/Config.hpp>
#include <MarkEmptyDirs/Api/Context.hpp>
#include <MarkEmptyDirs/Api/ICommand.hpp>
#include <MarkEmptyDirs/Api/Logger.hpp>
#include <MarkEmptyDirs/Api/LogLevel.hpp>

#include <CodeMagic/Cli/HelpFormatter.hpp>
#include <CodeMagic/Cli/ArgumentParser.hpp>
#include <CodeMagic/FileSystem/FileSystemTools.hpp>
#include <CodeMagic/Text/Template/Engine.hpp>
#include <CodeMagic/Text/Template/Variable.hpp>

#include <QFile>
#include <QStringList>

#include <functional>


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

Program::Program()
    : cleanCmd(
          QStringList() << "clean",
          QObject::tr("Delete all marker files.", "clean"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::CLEAN); return true; })
    , helpCmd(
          QStringList() << "help",
          QObject::tr("Print help information.", "help"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::HELP); return true; })
    , listCmd(
          QStringList() << "list",
          QObject::tr("List all marker files.", "list"),
          [this](const Argument&, QString&)
            {
                m_pConfig->setCommand(Config::Command::CLEAN);
                m_pConfig->setDryRun(true);
                m_pConfig->setShortMessages(true);
                m_pConfig->setLogLevel(LogLevel::INFO);
                return true;
            })
    , overviewCmd(
          QStringList() << "overview",
          QObject::tr("Scan directory and show some overview statistics.", "overview"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::OVERVIEW); return true; })
    , purgeCmd(
          QStringList() << "purge",
          QObject::tr("Delete everything within directories containing markers.", "purge"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::PURGE); return true; })
    , updateCmd(
          QStringList() << "update",
          QObject::tr("Create and delete marker files where necessary.", "update"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::UPDATE); return true; })
    , versionCmd(
          QStringList() << "version",
          QObject::tr("Show version information.", "version"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::VERSION); return true; })
    , createHookOpt(
          QStringList() << "create-hook",
          QObject::tr("Invoke command after marker creation.", "create-hook"),
          QObject::tr("COMMAND", "create-hook"), QString(),
          [this](const Argument& hookCmd, QString&){ m_pConfig->setCreateHookCommand(hookCmd.value); return true; })
    , deleteHookOpt(
          QStringList() << "delete-hook",
          QObject::tr("Invoke command before marker deletion.", "delete-hook"),
          QObject::tr("COMMAND", "delete-hook"), QString(),
          [this](const Argument& hookCmd, QString&){ m_pConfig->setDeleteHookCommand(hookCmd.value); return true; })
    , dryRunOpt(
          QStringList() << "n" << "dry-run",
          QObject::tr("Simulate command execution without any side effects.", "dry-run"),
          [this](const Argument&, QString&){ m_pConfig->setDryRun(true); return true; })
    , excludeOpt(
          QStringList() << "x" << "exclude",
          QObject::tr("Skip excluded dirs.", "exclude"),
          QObject::tr("DIRS", "exclude"), DEFAULT_EXCLUDE_DIRS,
          std::bind(&Program::acceptExcludeOpt, this, std::placeholders::_1, std::placeholders::_2))
    , fileOpt(
          QStringList() << "F" << "file",
          QObject::tr("Create marker files using the specified template file as content.", "file"),
          QObject::tr("NAME", "file"), DEFAULT_MARKER_CONTENT_FILENAME,
          std::bind(&Program::acceptMarkerFileOpt, this, std::placeholders::_1, std::placeholders::_2))
    , followSymLinksOpt(
          QStringList() << "L" << "dereference",
          QObject::tr("Follow symbolic links.", "dereference"),
          [this](const Argument&, QString&){ m_pConfig->setDereferenceSymLinks(true); return true; })
    , helpOpt(
          QStringList() << "h" << "help",
          QObject::tr("Print help information.", "help"),
          [this](const Argument&, QString&){ m_pConfig->setCommand(Config::Command::HELP); return true; })
    , markerOpt(
          QStringList() << "m" << "marker-name",
          QObject::tr("Use another name for marker files.", "marker-name"),
          QObject::tr("NAME", "marker-name"), QString(),
          std::bind(&Program::acceptMarkerNameOpt, this, std::placeholders::_1, std::placeholders::_2))
    , noFollowSymLinksOpt(
          QStringList() << "no-dereference",
          QObject::tr("Do not follow symbolic links.", "dereference"),
          [this](const Argument&, QString&){ m_pConfig->setDereferenceSymLinks(false); return true; })
    , noSubstOpt(
          QStringList() << "no-subst",
          QObject::tr("Do not use variable subsitution.", "subst"),
          [this](const Argument&, QString&){ m_pConfig->setSubstituteVariables(false); return true; })
    , shortOpt(
          QStringList() << "short",
          QObject::tr("Output short verbose messages.", "short"),
          [this](const Argument&, QString&){ m_pConfig->setShortMessages(true); return true; })
    , substOpt(
          QStringList() << "subst",
          QObject::tr("Use variable subsitution.", "subst"),
          [this](const Argument&, QString&){ m_pConfig->setSubstituteVariables(true); return true; })
    , textOpt(
          QStringList() << "text",
          QObject::tr("Create marker files with the specified text as content.", "text"),
          QObject::tr("CONTENT", "text"), QString(),
          [this](const Argument& markerText, QString&){ m_pConfig->setMarkerText(markerText.value); return true; })
    , verboseOpt(
          QStringList() << "v" << "verbose",
          QObject::tr("Output verbose messages.", "verbose"),
          std::bind(&Program::acceptVerboseOpt, this, std::placeholders::_1, std::placeholders::_2))
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

Program::~Program()
{
}

bool Program::acceptExcludeOpt(const Argument& excludeDirs, QString& errorMessage)
{
    Q_UNUSED(errorMessage);

    Config::DirList dirs;
    foreach (auto dir, excludeDirs.value.split(FileSystem::pathSeparator(), QString::SkipEmptyParts))
        dirs.push_back(dir);
    m_pConfig->setExcludeDirs(dirs);
    return true;
}

bool Program::acceptMarkerFileOpt(const Argument& markerFileName, QString& errorMessage)
{
    QFile file(markerFileName.value);
    if (!file.exists())
    {
        errorMessage = QObject::tr("File does not exist: '%1'").arg(file.fileName());
        return false;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        errorMessage = QObject::tr("Cannot open file: '%1'").arg(file.fileName());
        return false;
    }
    m_pConfig->setMarkerText(QString::fromUtf8(file.readAll()));
    return true;
}

bool Program::acceptMarkerNameOpt(const Argument& markerName, QString& errorMessage)
{
    if (!FileSystem::validateFileName(markerName.value, &errorMessage))
        return false;
    m_pConfig->setMarkerName(markerName.value);
    return true;
}

bool Program::acceptVerboseOpt(const CodeMagic::Cli::Argument& verbose, QString& errorMessage)
{
    Q_UNUSED(verbose);
    Q_UNUSED(errorMessage);

    switch (m_pConfig->logLevel())
    {
    case LogLevel::NONE:
        m_pConfig->setLogLevel(LogLevel::INFO);
        break;
    case LogLevel::INFO:
    default:
        m_pConfig->setLogLevel(LogLevel::DEBUG);
    }
    return true;
}

std::unique_ptr<const Config> Program::createConfig(const QStringList& args, QStringList& errorMessages)
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

    m_pConfig.reset(new Config);
    m_pConfig->setApplicationInfo(appInfo);
    m_pConfig->setExecutableFile(args[0]);

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
        if (Argument::COMMAND == arg.type && !arg.isKnown() && !parser.findArgument(helpOpt).isNull())
        {
            m_pConfig->setCommand(Config::Command::HELP);
            continue;
        }

        if (!arg.errorMessage.isNull())
            errorMessages << arg.errorMessage;

        if (Argument::OTHER == arg.type)
        {
            m_pConfig->addRootDir(QDir(arg.value));
        }
    }

    // Set help text.
    {
        auto helpText = createHelpText(m_pConfig->executableFile().fileName(), commands(), options(), m_pContext->templateEngine().variables());

        m_pConfig->setHelpText(helpText);
    }

    return std::move(m_pConfig);
}

CommandList Program::commands() const
{
    return m_commands;
}

QString Program::createHelpText(const QString& execFileName,
                                const CommandList& cmds,
                                const OptionList& opts,
                                const Template::VariableList& templVars) const
{
    HelpFormatter formatter;

    formatter.setExecutableFileName(execFileName);

    formatter.addUsageSection("COMMAND [OPTION]... DIR...");

    formatter.addCommandListSection(
                QObject::tr("Commands"),
                cmds);

    formatter.addOptionListSection(
                QObject::tr("Options"),
                opts);

    formatter.addTextSection(
                QObject::tr("Template variables"),
                formatVariableParagraphs(templVars));

    return formatter.formatHelpText();
}

bool Program::init(const QStringList& args)
{
    m_pContext = Api::Context::create();

    QStringList errorMessages;
    m_pContext->setConfig(createConfig(args, errorMessages));
    foreach (const auto& errorMessage, errorMessages)
        m_pContext->logger().log(errorMessage, Api::LogLevel::ERROR);
    if (!errorMessages.isEmpty())
        return false;

    return true;
}

OptionList Program::options() const
{
    return m_options;
}

void Program::run()
{
    Q_ASSERT(m_pContext);
    Api::CommandFactory commandFactory;
    auto pCmd = commandFactory.createCommand(*m_pContext);
    pCmd->run();
}

}

}
