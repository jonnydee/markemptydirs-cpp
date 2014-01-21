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

#include "Command.hpp"
#include "Option.hpp"
#include "ArgumentParser.hpp"
#include "ArgumentScanner.hpp"

#include <QStringList>

#define DEFAULT_MINIMUMCOMMANDLENGTH    2


namespace QodeMagic
{

namespace Cli
{

ArgumentParser::ArgumentParser()
    : m_minimumCommandLength(DEFAULT_MINIMUMCOMMANDLENGTH)
{
}

void ArgumentParser::addCommand(const Command& command)
{
    m_commands.push_back(&command);
}

void ArgumentParser::addCommands(const CommandList& commands)
{
    m_commands << commands;
}

CommandList ArgumentParser::commands() const
{
    return m_commands;
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

void ArgumentParser::setMinimumCommandLength(int minLen)
{
    m_minimumCommandLength = minLen;
}

int ArgumentParser::minimumCommandLength() const
{
    return m_minimumCommandLength;
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
    do
    {
        if (int tokensConsumed = parseCommand(tokens, i))
        {
            i += tokensConsumed;
            continue;
        }

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
    } while (i < tokens.size());
}

int ArgumentParser::parseCommand(const TokenList& tokens, int startIndex)
{
    if (m_commands.isEmpty())
        return 0;
    if (0 != startIndex)
        return 0;

    Argument command(Argument::COMMAND);

    if (tokens.isEmpty() || Token::OTHER != tokens[startIndex].type)
    {
        command.errorMessage = QObject::tr("Missing command.");
        m_arguments.push_back(command);
        return 0;
    }

    command.name = tokens[startIndex].payload;
    Q_ASSERT(!command.name.isEmpty());

    foreach (const auto pCmd, m_commands)
        foreach (const auto name, pCmd->names())
            if (name.startsWith(command.name))
                command.commands << pCmd;

    if (command.name.length() < m_minimumCommandLength)
        command.errorMessage = QString("%1: %2").arg(command.name).arg(QObject::tr("Command too short (must consist of at least %1 characters).").arg(m_minimumCommandLength));
    else if (command.commands.isEmpty())
        command.errorMessage = QString("%1: %2").arg(command.name).arg(QObject::tr("Unknown command."));
    else if (command.commands.size() > 1)
        command.errorMessage = QString("%1: %2").arg(command.name).arg(QObject::tr("Ambivalent command."));
    else if (command.commands.first()->hasHandler())
    {
        QString errorMessage;
        if (!command.commands.first()->handler()(command, errorMessage))
            command.errorMessage = QString("%1: %2")
                    .arg(command.name)
                    .arg(!errorMessage.isEmpty() ? errorMessage : QObject::tr("Command not accepted."));
    }

    m_arguments.push_back(command);

    return 1;
}

int ArgumentParser::parseShortOption(const TokenList& tokens, int startIndex)
{
    if (startIndex >= tokens.size())
        return 0;

    if (tokens.isEmpty())
        return 0;

    if (Token::SHORTNAME != tokens[startIndex].type)
        return 0;

    Argument shortOption(Argument::OPTION);
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
                if (option.isValueMandatory())
                    shortOption.errorMessage = QString("-%1: %2").arg(shortOption.name).arg(QObject::tr("Missing value."));
                else
                    shortOption.value = option.defaultValue();
            }
        }

        if (option.hasHandler())
        {
            QString errorMessage;
            if (!option.handler()(shortOption, errorMessage))
                shortOption.errorMessage = QString("-%1: %2")
                        .arg(shortOption.name)
                        .arg(!errorMessage.isEmpty() ? errorMessage : QObject::tr("Option not accepted."));
        }

        break;
    }

    if (!shortOption.option)
    {
        shortOption.errorMessage = QString("-%1: %2").arg(shortOption.name).arg(QObject::tr("Unknown option."));
    }

    m_arguments.push_back(shortOption);

    return 1 + int(nextArgConsumed);
}

int ArgumentParser::parseLongOption(const TokenList& tokens, int startIndex)
{
    if (startIndex >= tokens.size())
        return 0;

    if (tokens.isEmpty())
        return 0;

    if (Token::LONGNAME != tokens[startIndex].type)
        return 0;

    Argument longOption(Argument::OPTION);
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
            {
                if (option.isValueMandatory())
                    longOption.errorMessage = QString("--%1: %2").arg(longOption.name).arg(QObject::tr("Missing value."));
                else
                    longOption.value = option.defaultValue();
            }
        }
        else if (!longOption.value.isNull())
        {
            longOption.errorMessage = QString("--%1: %2").arg(longOption.name).arg(QObject::tr("Option does not support value assignment."));
        }

        if (option.hasHandler())
        {
            QString errorMessage;
            if (!option.handler()(longOption, errorMessage))
                longOption.errorMessage = QString("--%1: %2")
                        .arg(longOption.name)
                        .arg(!errorMessage.isEmpty() ? errorMessage : QObject::tr("Option not accepted."));
        }

        break;
    }

    if (!longOption.option)
    {
        longOption.errorMessage = QString("--%1: %2").arg(longOption.name).arg(QObject::tr("Unknown option."));
    }

    m_arguments.push_back(longOption);

    return tokensConsumed;
}

int ArgumentParser::parseOther(const TokenList& tokens, int startIndex)
{
    if (startIndex >= tokens.size())
        return 0;

    if (tokens.isEmpty())
        return 0;

    if (Token::OTHER != tokens[startIndex].type)
        return 0;

    Argument other(Argument::OTHER);
    other.value = tokens[startIndex].payload;

    m_arguments.push_back(other);

    return 1;
}

}

}
