// Copyright 2013-2014 Johann Duscher (a.k.a. Jonny Dee). All rights reserved.
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

#include <QodeMagic/Cli/Command.hpp>
#include <QodeMagic/Cli/Option.hpp>
#include <QodeMagic/Text/Template/Variable.hpp>

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
    const QodeMagic::Cli::Command cleanCmd;
    const QodeMagic::Cli::Command helpCmd;
    const QodeMagic::Cli::Command listCmd;
    const QodeMagic::Cli::Command overviewCmd;
    const QodeMagic::Cli::Command purgeCmd;
    const QodeMagic::Cli::Command updateCmd;
    const QodeMagic::Cli::Command versionCmd;

    const QodeMagic::Cli::Option createHookOpt;
    const QodeMagic::Cli::Option deleteHookOpt;
    const QodeMagic::Cli::Option dereferenceOpt;
    const QodeMagic::Cli::Option dryRunOpt;
    const QodeMagic::Cli::Option excludeOpt;
    const QodeMagic::Cli::Option fileOpt;
    const QodeMagic::Cli::Option helpOpt;
    const QodeMagic::Cli::Option markerOpt;
    const QodeMagic::Cli::Option noDereferenceOpt;
    const QodeMagic::Cli::Option noSubstOpt;
    const QodeMagic::Cli::Option shortOpt;
    const QodeMagic::Cli::Option substOpt;
    const QodeMagic::Cli::Option textOpt;
    const QodeMagic::Cli::Option verboseOpt;
    const QodeMagic::Cli::Option versionOpt;

    Program();
    ~Program();

    QodeMagic::Cli::CommandList commands() const;

    bool init(const QStringList& args);

    QodeMagic::Cli::OptionList options() const;

    void run();

protected:
    std::unique_ptr<const Api::Config> createConfig(const QStringList& args, QStringList& errorMessages);

    QString createHelpText(const QString& execFileName,
                           const QodeMagic::Cli::CommandList& cmds,
                           const QodeMagic::Cli::OptionList& opts,
                           const QodeMagic::Text::Template::VariableList& templVars,
                           bool shortHelp) const;

    bool acceptExcludeOpt(const QodeMagic::Cli::Argument& markerName, QString& errorMessage);

    bool acceptMarkerFileOpt(const QodeMagic::Cli::Argument& markerFileName, QString& errorMessage);

    bool acceptMarkerNameOpt(const QodeMagic::Cli::Argument& markerName, QString& errorMessage);

    bool acceptVerboseOpt(const QodeMagic::Cli::Argument& verbose, QString& errorMessage);

private:
    QodeMagic::Cli::CommandList m_commands;
    std::unique_ptr<Api::Config> m_pConfig;
    std::unique_ptr<Api::Context> m_pContext;
    QodeMagic::Cli::OptionList m_options;
};

}

}

#endif // MARKEMPTYDIRS_CLI_PROGRAM_HPP
