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
#ifndef CODEMAGIC_CLI_ARGUMENT_HPP
#define CODEMAGIC_CLI_ARGUMENT_HPP

#include "../codemagic_global.hpp"

#include <QList>
#include <QString>


namespace CodeMagic
{

namespace Cli
{

class Command;
typedef QList<const Command*> CommandList;

class Option;

struct CODEMAGICSHARED_EXPORT Argument
{
    enum Type
    {
        UNKNOWN,
        COMMAND,
        OPTION,
        OTHER
    };

    Type type;
    CommandList commands;
    const Option* option;
    QString name;
    QString value;
    QString errorMessage;

    Argument(Type _type = UNKNOWN) : type(_type), option(nullptr) {}

    bool isKnown() const { return !commands.isEmpty() || nullptr != option; }
    bool isNull() const { return UNKNOWN == type; }
    bool isBasedOn(const Command& cmd) const { return commands.contains(&cmd); }
    bool isBasedOn(const Option& opt) const { return &opt == option; }
};

typedef QList<Argument> ArgumentList;

}

}

#endif // CODEMAGIC_CLI_ARGUMENT_HPP
