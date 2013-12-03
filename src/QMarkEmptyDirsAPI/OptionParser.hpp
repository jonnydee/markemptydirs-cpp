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

#ifndef OPTIONPARSER_HPP
#define OPTIONPARSER_HPP

#include <QStringList>


namespace MarkEmptyDirs
{

namespace Api
{

class Option
{
public:
    Option(const QStringList& names, const QString& description = "", const QString& valueName = "", const QString& defaultValue = "");

    QString defaultValue() const;
    QString description() const;
    bool hasValue() const;
    QStringList longNames() const;
    QStringList names() const;
    QString valueName() const;
    QList<QChar> shortNames() const;

private:
    QStringList m_names;
    QString m_description;
    QString m_valueName;
    QString m_defaultValue;
};
typedef QList<const Option*> OptionList;

struct Token
{
    enum Type
    {
        LONGNAME,
        SHORTNAME,
        ASSIGN,
        OTHER
    };

    Type type;
    QString payload;

    Token(Type aType, const QString& aPayload = QString::null)
        : type(aType), payload(aPayload)
    {}
};
typedef QList<Token> TokenList;

class Scanner
{
public:
    Scanner();

    void scan(const QStringList& args);

    TokenList tokens() const;

protected:
    int scanShortOptions(const QStringList& args, int startIndex);
    int scanLongOption(const QStringList& args, int startIndex);
    int scanOther(const QStringList& args, int startIndex);

private:
    TokenList m_tokens;
};

struct Argument
{
    const Option* option;
    QString name;
    QString value;
    QString errorMessage;

    Argument() : option(nullptr) {}

    bool isKnown() const { return nullptr != option; }
    bool isNull() const { return nullptr == option && name.isNull() && value.isNull(); }
};

class OptionParser
{
public:
    typedef QList<Argument> ArgumentList;

    OptionParser();

    void addOption(const Option& option);

    ArgumentList arguments() const;
    Argument findUnknownArgument() const;
    Argument findArgument(const Option& option) const;
    ArgumentList findUnknownArguments() const;
    ArgumentList findArguments(const Option& option) const;

    OptionList options() const;

    void parse(const QStringList& args);

protected:
    int parseShortOption(const TokenList& tokens, int startIndex);
    int parseLongOption(const TokenList& tokens, int startIndex);
    int parseOther(const TokenList& tokens, int startIndex);

private:
    OptionList m_options;
    ArgumentList m_arguments;
};

}

}

#endif // OPTIONPARSER_HPP
