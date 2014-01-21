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
#ifndef QODEMAGIC_CLI_HELPFORMATTER_HPP
#define QODEMAGIC_CLI_HELPFORMATTER_HPP

#include "../qodemagic_global.hpp"
#include "Command.hpp"
#include "Option.hpp"

#include <QList>
#include <QPair>


namespace QodeMagic
{

namespace Cli
{

class QODEMAGICSHARED_EXPORT HelpFormatter
{
public:
    HelpFormatter(int sectionIndent = 2, int maxLineLength = 80);

    void setExecutableFileName(const QString& fileName);

    void addCommandListSection(const QString& title, const CommandList& commands);

    void addOptionListSection(const QString& title, const OptionList& options);

    void addTextSection(const QString& title, const QString& paragraph);
    void addTextSection(const QString& title, const QStringList& paragraphs);

    void addUsageSection(const QString& customUsage = QString());

    QString formatHelpText() const;

protected:
    enum SectionType
    {
        SectionCommandList,
        SectionOptionList,
        SectionUsage,
        SectionText
    };

    typedef QPair<SectionType, int> SectionItem;
    typedef QPair<QString, CommandList> CommandListSection;
    typedef QPair<QString, OptionList> OptionListSection;
    typedef QPair<int, QString> TextSectionParagraph;
    typedef QPair<QString, QList<TextSectionParagraph>> TextSection;
    typedef QString UsageSection;

    QString formatCommandListSection(const CommandListSection& section) const;
    QString formatCommands(const CommandList& commands) const;
    QStringList formatCommandsDescriptionColumn(const CommandList& commands) const;
    QStringList formatCommandsNamesColumn(const CommandList& commands) const;
    QString formatOptionListSection(const OptionListSection& section) const;
    QString formatOptions(const OptionList& options) const;
    QString formatUsageSection(const UsageSection& section) const;
    QString formatTextSection(const TextSection& section) const;
    QStringList formatTitle(const QString& title) const;
    QStringList formatShortOptionsColumn(const OptionList& options) const;
    QStringList formatLongOptionsColumn(const OptionList& options) const;
    QStringList formatOptionsDescriptionColumn(const OptionList& options) const;

private:
    QString m_executableFileName;
    int m_maxLineLength;
    int m_sectionIndent;
    QList<CommandListSection> m_commandsListSections;
    QList<OptionListSection> m_optionsListSections;
    QList<SectionItem> m_sectionList;
    QList<TextSection> m_textSections;
    QList<UsageSection> m_usageSections;
};

}

}

#endif // QODEMAGIC_CLI_HELPFORMATTER_HPP
