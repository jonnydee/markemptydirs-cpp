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
#ifndef STRINGMAGIC_TEMPLATE_VARIABLE_HPP
#define STRINGMAGIC_TEMPLATE_VARIABLE_HPP

#include "../stringmagic_global.hpp"

#include <QRegExp>
#include <QString>

#include <functional>


namespace StringMagic
{

namespace Template
{

class STRINGMAGICSHARED_EXPORT Variable
{
public:
    struct Context;

    typedef std::function<QString(const Context&)> EvalFn;

    Variable(const QString& name, const EvalFn& eval);

    void setArgumentDescription(const QString& description);
    QString argumentDescription() const;

    void setArgumentSpec(const QString& argumentSpec);
    QString argumentSpec() const;

    void setDefaultArgument(const QString& defaultArgument);
    QString defaultArgument() const;

    void setDescription(const QString& description);
    QString description() const;

    bool hasArgument() const;

    bool isArgumentMandatory() const;

    QString name() const;

    int expand(QString& str) const;

    QString toString() const;

private:
    QString m_name;
    QRegExp m_pattern;
    EvalFn m_eval;
    QString m_argumentDescription;
    QString m_argumentSpec;
    QString m_defaultArgument;
    QString m_description;
};

struct STRINGMAGICSHARED_EXPORT Variable::Context
{
    Context(const QString& text_,
            int index_, const QString& match_,
            int count_,
            const QString& name_, const QString& argument_)
        : text(text_)
        , index(index_)
        , match(match_)
        , count(count_)
        , name(name_)
        , argument(argument_)
    {}

    const QString& text;
    int index;
    const QString& match;
    int length;
    int count;
    const QString& name;
    const QString& argument;
};

}

}

#endif // STRINGMAGIC_TEMPLATE_VARIABLE_HPP
