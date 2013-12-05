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
#define DEFAULT_FILE_FILENAME       "placeholder.txt"
#define DEFAULT_MARKER_FILENAME     ".emptydir"
#define DEFAULT_TEXT_CONTENT        ""


namespace MarkEmptyDirs
{

namespace Api
{

CommandLineInterface::CommandLineInterface()
    : dryRunOpt(QStringList() << "d" << "dry-run", "simulate command execution without any side effects")
    , shortOpt(QStringList() << "s" << "short", "output short verbose messages")
    , verboseOpt(QStringList() << "v" << "verbose", "output verbose messages", "level", "1")
    , cleanOpt(QStringList() << "c" << "clean", "delete all placeholder files")
    , helpOpt(QStringList() << "h" << "help", "print help information")
    , createHookOpt(QStringList() << "a" << "create-hook", "invoke command after placeholder creation (use template variables)")
    , deleteHookOpt(QStringList() << "r" << "delete-hook", "invoke command before placeholder deletion (use template variables)")
    , listOpt(QStringList() << "l" << "list", "list all placeholder files")
    , purgeOpt(QStringList() << "g" << "purge", "delete everything within directories containing placeholders")
    , excludeOpt(QStringList() << "x" << "exclude", "skip excluded dirs", "dirs", ".bzr:CVS:.git:.hg:.svn")
    , placeHolderOpt(QStringList() << "p" << "place-holder", "use another name for placeholder files", "name", DEFAULT_MARKER_FILENAME)
    , textOpt(QStringList() << "t" << "text", "create placeholder files with the specified text as content", "content", DEFAULT_TEXT_CONTENT)
    , fileOpt(QStringList() << "f" << "file", "create placeholder files using the specified template file as content", "name", DEFAULT_FILE_FILENAME)
    , substOpt(QStringList() << "b" << "subst", "use variable subsitution")
    , followSymLinksOpt(QStringList() << "m" << "follow-symlinks", "follow symbolic links")
    , overviewOpt(QStringList() << "o" << "overview", "scan directory and show some overview statistics")
    , updateOpt(QStringList() << "u" << "update", "create and delete placeholder files where necessary")
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
        << &textOpt
        << &followSymLinksOpt
        << &updateOpt;
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
