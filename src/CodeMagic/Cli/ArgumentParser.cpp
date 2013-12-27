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

#include "Option.hpp"
#include "ArgumentParser.hpp"
#include "ArgumentScanner.hpp"

#include <QStringList>


namespace CodeMagic
{

namespace Cli
{

ArgumentParser::ArgumentParser()
{
}

void ArgumentParser::addOption(const Option& option)
{
    m_options.push_back(&option);
}

void ArgumentParser::addOptions(const OptionList& options)
{
    m_options << options;
}

ArgumentList ArgumentParser::arguments() const
{
    return m_arguments;
}

Argument ArgumentParser::findUnknownArgument() const
{
    foreach (auto& arg, m_arguments)
        if (!arg.isKnown())
            return arg;
    return Argument();
}

Argument ArgumentParser::findArgument(const Option& option) const
{
    foreach (auto& arg, m_arguments)
        if (&option == arg.option)
            return arg;
    return Argument();
}

ArgumentList ArgumentParser::findUnknownArguments() const
{
    ArgumentList args;
    foreach (auto& arg, m_arguments)
        if (!arg.isKnown())
            args.push_back(arg);
    return args;
}

ArgumentList ArgumentParser::findArguments(const Option& option) const
{
    ArgumentList args;
    foreach (auto& arg, m_arguments)
        if (&option == arg.option)
            args.push_back(arg);
    return args;
}

OptionList ArgumentParser::options() const
{
    return m_options;
}

void ArgumentParser::parse(const QStringList& args)
{
    ArgumentScanner scanner;
    scanner.scan(args);
    const auto tokens = scanner.tokens();

    int i = 0;
    while (i < tokens.size())
    {
        if (int tokensConsumed = parseLongOption(tokens, i))
        {
            i += tokensConsumed;
            continue;
        }

        if (int tokensConsumed = parseShortOption(tokens, i))
        {
            i += tokensConsumed;
            continue;
        }

        i += parseOther(tokens, i);
    }
}

int ArgumentParser::parseShortOption(const TokenList& tokens, int startIndex)
{
    Q_ASSERT(startIndex < tokens.size());

    if (tokens.isEmpty())
        return 0;

    if (Token::SHORTNAME != tokens[startIndex].type)
        return 0;

    Argument shortOption;
    shortOption.name = tokens[startIndex].payload;

    bool nextArgConsumed = false;
    for (int i = 0; i < m_options.size(); i++)
    {
        const auto& option = *m_options[i];

        if (!option.shortNames().contains(shortOption.name[0]))
            continue;

        shortOption.option = &option;

        if (option.hasValue())
        {
            if (startIndex < tokens.size() - 1 && Token::OTHER == tokens[startIndex + 1].type)
            {
                shortOption.value = tokens[startIndex + 1].payload;
                nextArgConsumed = true;
            }
            else
            {
                shortOption.value = option.defaultValue();
            }
        }

        break;
    }

    if (!shortOption.option)
    {
        shortOption.errorMessage = QObject::tr("Unknown short option.");
    }

    m_arguments.push_back(shortOption);

    return 1 + int(nextArgConsumed);
}

int ArgumentParser::parseLongOption(const TokenList& tokens, int startIndex)
{
    Q_ASSERT(startIndex < tokens.size());

    if (tokens.isEmpty())
        return 0;

    if (Token::LONGNAME != tokens[startIndex].type)
        return 0;

    Argument longOption;
    longOption.name = tokens[startIndex].payload;
    int tokensConsumed = 1;
    if (startIndex < tokens.size() - 2 && Token::ASSIGN == tokens[startIndex + 1].type)
    {
        Q_ASSERT(Token::OTHER == tokens[startIndex + 2].type);
        longOption.value = tokens[startIndex + 2].payload;
        tokensConsumed += 2;
    }

    for (int i = 0; i < m_options.size(); i++)
    {
        const auto& option = *m_options[i];

        if (!option.longNames().contains(longOption.name))
            continue;

        longOption.option = &option;

        if (option.hasValue())
        {
            if (longOption.value.isNull())
                longOption.value = option.defaultValue();
        }
        else if (!longOption.value.isNull())
        {
            longOption.errorMessage = QObject::tr("Long option does not support value assignment.");
        }

        break;
    }

    if (!longOption.option)
    {
        longOption.errorMessage = QObject::tr("Unknown long option.");
    }

    m_arguments.push_back(longOption);

    return tokensConsumed;
}

int ArgumentParser::parseOther(const TokenList& tokens, int startIndex)
{
    Q_ASSERT(startIndex < tokens.size());

    if (tokens.isEmpty())
        return 0;

    if (Token::OTHER != tokens[startIndex].type)
        return 0;

    Argument other;
    other.value = tokens[startIndex].payload;

    m_arguments.push_back(other);

    return 1;
}

}

}
