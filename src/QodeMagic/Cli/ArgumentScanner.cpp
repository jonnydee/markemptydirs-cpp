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

#include "ArgumentScanner.hpp"

#include <QStringList>


namespace QodeMagic
{

namespace Cli
{

ArgumentScanner::ArgumentScanner()
{
}

TokenList ArgumentScanner::tokens() const
{
    return m_tokens;
}

void ArgumentScanner::scan(const QStringList& args)
{
    int i = 0;
    while (i < args.length())
    {
        if (int argsConsumed = scanLongOption(args, i))
        {
            i += argsConsumed;
            continue;
        }

        if (int argsConsumed = scanShortOptions(args, i))
        {
            i += argsConsumed;
            continue;
        }

        i += scanOther(args, i);
    }
}

int ArgumentScanner::scanShortOptions(const QStringList& args, int startIndex)
{
    const auto& arg = args[startIndex];
    if (arg.length() < 2 || '-' != arg[0] || '-' == arg[1])
        return 0;

    for (int i = 1; i < arg.length(); i++)
    {
        auto& name = arg[i];

        Token shortName(Token::SHORTNAME, name);
        m_tokens.push_back(shortName);
    }

    return 1;
}

int ArgumentScanner::scanLongOption(const QStringList& args, int startIndex)
{
    const auto& arg = args[startIndex];
    if (arg.length() < 4 || !arg.startsWith("--") || '-' == arg[2])
        return 0;

    auto name = arg.mid(2);
    QString val = QString::null;

    int assignIndex = name.indexOf('=');
    if (assignIndex >= 0)
    {
        val = name.mid(assignIndex + 1);
        name = name.left(assignIndex);
    }
    if (name.length() < 2)
        return 0;

    Token longName(Token::LONGNAME, name);
    m_tokens.push_back(longName);

    if (assignIndex < 0)
        return 1;

    Token assign(Token::ASSIGN);
    m_tokens.push_back(assign);

    Token value(Token::OTHER, val);
    m_tokens.push_back(value);

    return 1;
}

int ArgumentScanner::scanOther(const QStringList& args, int startIndex)
{
    const auto& arg = args[startIndex];

    Token other(Token::OTHER, arg);
    m_tokens.push_back(other);

    return 1;
}

}

}
