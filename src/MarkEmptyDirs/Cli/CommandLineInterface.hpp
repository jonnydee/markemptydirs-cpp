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

#ifndef COMMANDLINEINTERFACE_HPP
#define COMMANDLINEINTERFACE_HPP

#include <CodeMagic/Cli/Option.hpp>
#include <MarkEmptyDirs/Api/Config.hpp>

#include <memory>


namespace MarkEmptyDirs
{

namespace Cli
{

class CommandLineInterface
{
public:
    const CodeMagic::Cli::Option dryRunOpt;
    const CodeMagic::Cli::Option shortOpt;
    const CodeMagic::Cli::Option verboseOpt;
    const CodeMagic::Cli::Option cleanOpt;
    const CodeMagic::Cli::Option helpOpt;
    const CodeMagic::Cli::Option createHookOpt;
    const CodeMagic::Cli::Option deleteHookOpt;
    const CodeMagic::Cli::Option listOpt;
    const CodeMagic::Cli::Option purgeOpt;
    const CodeMagic::Cli::Option excludeOpt;
    const CodeMagic::Cli::Option markerOpt;
    const CodeMagic::Cli::Option textOpt;
    const CodeMagic::Cli::Option fileOpt;
    const CodeMagic::Cli::Option substOpt;
    const CodeMagic::Cli::Option noSubstOpt;
    const CodeMagic::Cli::Option followSymLinksOpt;
    const CodeMagic::Cli::Option noFollowSymLinksOpt;
    const CodeMagic::Cli::Option overviewOpt;
    const CodeMagic::Cli::Option updateOpt;
    const CodeMagic::Cli::Option versionOpt;

    CommandLineInterface();

    CodeMagic::Cli::OptionList options() const;
    CodeMagic::Cli::OptionList commandOptions() const;
    CodeMagic::Cli::OptionList otherOptions() const;

    std::unique_ptr<const Api::Config> createConfig(const QStringList& args) const;

private:
    CodeMagic::Cli::OptionList m_commandOptions;
    CodeMagic::Cli::OptionList m_otherOptions;
};

}

}

#endif // COMMANDLINEINTERFACE_HPP
