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

#include "Variable.hpp"

#include <QDebug>
#include <QRegExp>


#define PATTERN     QString("§%1\\s*(?:(\\:)([^§]*))?§")


namespace CodeMagic
{

namespace Text
{

namespace Template
{

Variable::Variable(const QString& name, const EvalFn& eval)
    : m_name(name)
    , m_pattern(PATTERN.arg(name))
    , m_eval(eval)
{
}

void Variable::addArgumentDescription(const QString& argument, const QString& description)
{
    m_argumentDescriptions << ArgumentDescription(argument, description);
}

const QList<Variable::ArgumentDescription>& Variable::argumentDescriptions() const
{
    return m_argumentDescriptions;
}

void Variable::setArgumentSpec(const QString& argumentSpec)
{
    m_argumentSpec = argumentSpec;
}

QString Variable::argumentSpec() const
{
    return m_argumentSpec;
}

void Variable::setDefaultArgument(const QString& defaultArgument)
{
    m_defaultArgument = defaultArgument;
}

QString Variable::defaultArgument() const
{
    return m_defaultArgument;
}

void Variable::setDescription(const QString& description)
{
    m_description = description;
}

QString Variable::description() const
{
    return m_description;
}

QString Variable::name() const
{
    return m_name;
}

int Variable::expand(QString& str) const
{
    int expansionCount = 0;
    // Look for variable occurences and replace them by values provided
    // by m_eval function.
    int index = m_pattern.indexIn(str, 0, QRegExp::CaretAtOffset);
    while (index >= 0)
    {
        auto components = m_pattern.capturedTexts();
        Q_ASSERT(components.size() == 3);

        // Get matched part.
        const auto& match = components[0];

        // Get argument part.
        auto& argument = components[2];
        {
            const bool separatorPresent = !components[1].isEmpty();
            // If variable name is followed by a separator make sure the argument part is not null.
            if (separatorPresent && argument.isNull())
                argument = "";
            // If argument part is null initialize it with default argument (which may again be null).
            if (argument.isNull())
                argument = defaultArgument();
        }

        // Create variable context and call evaluation function.
        QString value;
        if (m_eval)
        {
            const Context ctx(str, index, match, expansionCount + 1, m_name, argument);
            value = m_eval(ctx);
        }

        if (!value.isNull())
        {
            // Replace matched part by non-null value returned by m_eval function.
            str.replace(index, match.length(), value);
            index += value.length();
            ++expansionCount;
        }
        else
        {
            // m_eval function returned null value, so don't replace anything and
            // skip matched part.
            index += match.length();
        }

        // Look for next variable occurence.
        index = m_pattern.indexIn(str, index, QRegExp::CaretAtOffset);
    }
    return expansionCount;
}

bool Variable::hasArgument() const
{
    return !m_argumentSpec.isNull();
}

bool Variable::isArgumentMandatory() const
{
    return m_defaultArgument.isNull();
}

QString Variable::toString() const
{
    QString spec;
    if (hasArgument())
    {
        spec = isArgumentMandatory()
            ? QString(":%1").arg(argumentSpec())
            : QString("[:%1]").arg(argumentSpec());
    }
    return QString("§%1%2§").arg(name()).arg(spec);
}

}

}

}
