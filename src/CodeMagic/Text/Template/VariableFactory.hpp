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
#ifndef CODEMAGIC_TEXT_TEMPLATE_VARIABLEFACTORY_HPP
#define CODEMAGIC_TEXT_TEMPLATE_VARIABLEFACTORY_HPP

#include "../../codemagic_global.hpp"

#include <memory>


class QChar;

namespace CodeMagic
{

namespace Text
{

namespace Template
{

class Variable;

class CODEMAGICSHARED_EXPORT VariableFactory
{
public:
    typedef std::unique_ptr<const Variable> VariablePtr;

    VariableFactory();

    VariablePtr createDateTimeVariable() const;
    VariablePtr createEnvironmentVariable() const;
    VariablePtr createGuidVariable() const;
    VariablePtr createLinefeedVariable() const;
    VariablePtr createSeparatorVariable() const;
    VariablePtr createSpaceVariable() const;

protected:
    VariablePtr createCharRepeaterVariable(const QString& name, const QChar& ch, const QString& description) const;
};

}

}

}

#endif // CODEMAGIC_TEXT_TEMPLATE_VARIABLEFACTORY_HPP
