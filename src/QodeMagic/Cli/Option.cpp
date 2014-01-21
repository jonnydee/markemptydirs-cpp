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

#include "Option.hpp"


namespace QodeMagic
{

namespace Cli
{

Option::Option(const QStringList& names, const QString& description, const Handler& handler)
    : m_names(names)
    , m_description(description)
    , m_handler(handler)
{
}

Option::Option(const QStringList& names, const QString& description, const QString& valueName, const QString& defaultValue, const Handler& handler)
    : m_names(names)
    , m_description(description)
    , m_valueName(valueName)
    , m_defaultValue(defaultValue)
    , m_handler(handler)
{
}

QStringList Option::names() const
{
    return m_names;
}

QString Option::defaultValue() const
{
    return m_defaultValue;
}

bool Option::hasValue() const
{
    return !m_valueName.isNull();
}

bool Option::hasHandler() const
{
    return nullptr != m_handler;
}

bool Option::isValueMandatory() const
{
    return hasValue() && m_defaultValue.isNull();
}

QString Option::description() const
{
    return m_description;
}

QString Option::valueName() const
{
    return m_valueName;
}

Option::Handler Option::handler() const
{
    return m_handler;
}

QList<QChar> Option::shortNames() const
{
    QList<QChar> foundNames;
    foreach (auto name, names())
        if (name.length() == 1)
            foundNames.push_back(name[0]);
    return foundNames;
}

QStringList Option::longNames() const
{
    QStringList foundNames;
    foreach (auto name, names())
        if (name.length() > 1)
            foundNames.push_back(name);
    return foundNames;
}

}

}
