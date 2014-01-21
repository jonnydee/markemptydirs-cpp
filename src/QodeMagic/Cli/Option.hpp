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
#ifndef QODEMAGIC_CLI_OPTION_HPP
#define QODEMAGIC_CLI_OPTION_HPP

#include "../qodemagic_global.hpp"

#include <QList>
#include <QString>
#include <QStringList>

#include <functional>


namespace QodeMagic
{

namespace Cli
{

struct Argument;

class QODEMAGICSHARED_EXPORT Option
{
public:
    typedef std::function<bool(const Argument& optionArgument, QString& errorMessage)> Handler;

    Option(const QStringList& names, const QString& description,
           const Handler& handler = Handler());

    Option(const QStringList& names, const QString& description,
           const QString& valueName, const QString& defaultValue = QString(),
           const Handler& handler = Handler());

    QString defaultValue() const;
    QString description() const;
    bool hasHandler() const;
    bool hasValue() const;
    bool isValueMandatory() const;
    QStringList longNames() const;
    QStringList names() const;
    QString valueName() const;
    Handler handler() const;
    QList<QChar> shortNames() const;

private:
    QStringList m_names;
    QString m_description;
    QString m_valueName;
    QString m_defaultValue;
    Handler m_handler;
};

typedef QList<const Option*> OptionList;

}

}

#endif // QODEMAGIC_CLI_OPTION_HPP
