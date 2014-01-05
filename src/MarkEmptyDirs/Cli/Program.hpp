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
#ifndef MARKEMPTYDIRS_CLI_PROGRAM_HPP
#define MARKEMPTYDIRS_CLI_PROGRAM_HPP

#include <CodeMagic/Cli/Command.hpp>
#include <CodeMagic/Cli/Option.hpp>
#include <CodeMagic/Text/Template/Variable.hpp>

#include <QList>

#include <memory>


class QStringList;

namespace MarkEmptyDirs
{

namespace Api
{
    class Config;
    class Context;
}

namespace Cli
{

class Program
{
public:
    const CodeMagic::Cli::Command cleanCmd;
    const CodeMagic::Cli::Command helpCmd;
    const CodeMagic::Cli::Command listCmd;
    const CodeMagic::Cli::Command overviewCmd;
    const CodeMagic::Cli::Command purgeCmd;
    const CodeMagic::Cli::Command updateCmd;
    const CodeMagic::Cli::Command versionCmd;

    const CodeMagic::Cli::Option createHookOpt;
    const CodeMagic::Cli::Option deleteHookOpt;
    const CodeMagic::Cli::Option dryRunOpt;
    const CodeMagic::Cli::Option excludeOpt;
    const CodeMagic::Cli::Option fileOpt;
    const CodeMagic::Cli::Option followSymLinksOpt;
    const CodeMagic::Cli::Option helpOpt;
    const CodeMagic::Cli::Option markerOpt;
    const CodeMagic::Cli::Option noFollowSymLinksOpt;
    const CodeMagic::Cli::Option noSubstOpt;
    const CodeMagic::Cli::Option shortOpt;
    const CodeMagic::Cli::Option substOpt;
    const CodeMagic::Cli::Option textOpt;
    const CodeMagic::Cli::Option verboseOpt;

    Program();
    ~Program();

    CodeMagic::Cli::CommandList commands() const;

    bool init(const QStringList& args);

    CodeMagic::Cli::OptionList options() const;

    void run();

protected:
    std::unique_ptr<const Api::Config> createConfig(const QStringList& args, QStringList& errorMessages);

    QString createHelpText(const QString& execFileName,
                           const CodeMagic::Cli::CommandList& cmds,
                           const CodeMagic::Cli::OptionList& opts,
                           const CodeMagic::Text::Template::VariableList& templVars) const;

    bool acceptExcludeOpt(const CodeMagic::Cli::Argument& markerName, QString& errorMessage);

    bool acceptMarkerFileOpt(const CodeMagic::Cli::Argument& markerFileName, QString& errorMessage);

    bool acceptMarkerNameOpt(const CodeMagic::Cli::Argument& markerName, QString& errorMessage);

    bool acceptVerboseOpt(const CodeMagic::Cli::Argument& verbose, QString& errorMessage);

private:
    CodeMagic::Cli::CommandList m_commands;
    std::unique_ptr<Api::Config> m_pConfig;
    std::unique_ptr<Api::Context> m_pContext;
    CodeMagic::Cli::OptionList m_options;
};

}

}

#endif // MARKEMPTYDIRS_CLI_PROGRAM_HPP
