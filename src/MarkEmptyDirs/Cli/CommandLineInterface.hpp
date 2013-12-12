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

#include <ArgumentTools/Option.hpp>
#include <MarkEmptyDirs/Api/Config.hpp>


namespace MarkEmptyDirs
{

namespace Cli
{

class CommandLineInterface
{
public:
    const ArgumentTools::Option dryRunOpt;
    const ArgumentTools::Option shortOpt;
    const ArgumentTools::Option verboseOpt;
    const ArgumentTools::Option cleanOpt;
    const ArgumentTools::Option helpOpt;
    const ArgumentTools::Option createHookOpt;
    const ArgumentTools::Option deleteHookOpt;
    const ArgumentTools::Option listOpt;
    const ArgumentTools::Option purgeOpt;
    const ArgumentTools::Option excludeOpt;
    const ArgumentTools::Option markerOpt;
    const ArgumentTools::Option textOpt;
    const ArgumentTools::Option fileOpt;
    const ArgumentTools::Option substOpt;
    const ArgumentTools::Option noSubstOpt;
    const ArgumentTools::Option followSymLinksOpt;
    const ArgumentTools::Option noFollowSymLinksOpt;
    const ArgumentTools::Option overviewOpt;
    const ArgumentTools::Option updateOpt;
    const ArgumentTools::Option versionOpt;

    CommandLineInterface();

    ArgumentTools::OptionList options() const;
    ArgumentTools::OptionList commandOptions() const;
    ArgumentTools::OptionList otherOptions() const;

    Api::Config createConfig(const QStringList& args) const;

private:
    ArgumentTools::OptionList m_commandOptions;
    ArgumentTools::OptionList m_otherOptions;
};

}

}

#endif // COMMANDLINEINTERFACE_HPP
