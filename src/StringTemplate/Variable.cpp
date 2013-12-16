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

#include "Variable.hpp"

#include <QDebug>
#include <QRegExp>


#define PATTERN     QString("§%1(?:(\\:)([^§]*))?§")


namespace StringTemplate
{

Variable::Variable(const QString& name)
    : m_name(name)
    , m_pattern(PATTERN.arg(name))
{
}

QString Variable::name() const
{
    return m_name;
}

void Variable::expand(QString& str, const EvalFn& eval) const
{
    int count = 0;
    int index = m_pattern.indexIn(str, 0, QRegExp::CaretAtOffset);
    while (index >= 0)
    {
        auto components = m_pattern.capturedTexts();
        const auto& match = components[0];
        if (!components[1].isEmpty() && components[2].isNull())
            components[2] = "";
        const auto& argument = components[2];

        const Context ctx(str, index, match, ++count, m_name, argument);
        const auto value = eval(ctx);

        str.replace(index, match.length(), value);
        index += value.length();

        index = m_pattern.indexIn(str, index, QRegExp::CaretAtOffset);
    }
}

}
