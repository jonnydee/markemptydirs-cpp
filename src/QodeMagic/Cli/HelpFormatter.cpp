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

#include "HelpFormatter.hpp"
#include "Option.hpp"

#include <QodeMagic/Text/TextTools.hpp>
#include <QodeMagic/Text/Formatter.hpp>


#define WRAPPED_LINE_MIN_LENGTH             40
#define OPTION_COLUMN_PADDING_SIZE          1
#define DESCRIPTION_COLUMN_PADDING_SIZE     2


namespace QodeMagic
{

namespace Cli
{

namespace
{

    QStringList wrapLine(const QString& line, int maxLength, int newLineIndent = 0, int firstLineIdent = 0)
    {
        Text::Formatter formatter;
        formatter.setMaxLineLength(maxLength);
        formatter.setFirstLineLeftIndent(firstLineIdent - newLineIndent);
        formatter.setParagraphLeftIndent(newLineIndent);
        return formatter.format(line);
    }

}


HelpFormatter::HelpFormatter(int sectionIndent, int maxLineLength)
    : m_maxLineLength(maxLineLength)
    , m_sectionIndent(sectionIndent)
{
    if (m_maxLineLength - m_sectionIndent < WRAPPED_LINE_MIN_LENGTH)
    {
        m_sectionIndent = 0;

        if (m_maxLineLength < WRAPPED_LINE_MIN_LENGTH)
            m_maxLineLength = std::numeric_limits<int>::max();
    }
}

void HelpFormatter::setExecutableFileName(const QString& executableFileName)
{
    m_executableFileName = executableFileName;
}

void HelpFormatter::addCommandListSection(const QString& title, const CommandList& commands)
{
    m_commandsListSections << CommandListSection(title, commands);
    m_sectionList << SectionItem(SectionCommandList, m_commandsListSections.size() - 1);
}

void HelpFormatter::addOptionListSection(const QString& title, const OptionList& options)
{
    m_optionsListSections << OptionListSection(title, options);
    m_sectionList << SectionItem(SectionOptionList, m_optionsListSections.size() - 1);
}

void HelpFormatter::addTextSection(const QString& title, const QString& paragraph)
{
    addTextSection(title, QStringList() << paragraph);
}

void HelpFormatter::addTextSection(const QString& title, const QStringList& paragraphs)
{
    QList<TextSectionParagraph> paras;
    foreach (const auto& paragraph, paragraphs)
    {
        if (paragraph.trimmed().isEmpty())
        {
            paras << TextSectionParagraph(0, QString());
            continue;
        }

        for (int i = 0; i < paragraph.length(); i++)
            if (!paragraph[i].isSpace())
            {
                paras << TextSectionParagraph(i, paragraph.mid(i));
                break;
            }
    }
    m_textSections << TextSection(title, paras);
    m_sectionList << SectionItem(SectionText, m_textSections.size() - 1);
}

void HelpFormatter::addUsageSection(const QString& customUsage)
{
    m_usageSections << UsageSection(customUsage);
    m_sectionList << SectionItem(SectionUsage, m_usageSections.size() - 1);
}

QString HelpFormatter::formatHelpText() const
{
    QStringList sections;
    foreach (const auto& item, m_sectionList)
    {
        const auto& type = item.first;
        const auto& index = item.second;
        switch (type)
        {
        case SectionCommandList:
            sections << formatCommandListSection(m_commandsListSections[index]);
            break;
        case SectionOptionList:
            sections << formatOptionListSection(m_optionsListSections[index]);
            break;
        case SectionUsage:
            sections << formatUsageSection(m_usageSections[index]);
            break;
        case SectionText:
            sections << formatTextSection(m_textSections[index]);
            break;
        default:
            break;
        }
    }
    return sections.join("\n\n");
}

QString HelpFormatter::formatUsageSection(const UsageSection& section) const
{
    auto usageArgs = section.isNull()
            ? "*** TODO ***" //formatGenericUsageArgs()
            : section;

    return QString("%1: %2 %3").arg(QObject::tr("USAGE")).arg(m_executableFileName).arg(usageArgs);
}

QString HelpFormatter::formatCommandListSection(const CommandListSection& section) const
{
    const auto& title = section.first;
    const auto& commands = section.second;

    QStringList wrappedTextLines;

    wrappedTextLines << formatTitle(title);

    wrappedTextLines << formatCommands(commands);

    return wrappedTextLines.join('\n');
}

QString HelpFormatter::formatCommands(const CommandList& commands) const
{
    auto namesColumn = formatCommandsNamesColumn(commands);
    const int namesColumnWidth = Text::adjustToMaxLen(namesColumn);

    const auto descriptionColumn = formatCommandsDescriptionColumn(commands);

    auto textLines = Text::join(QList<QStringList>() << namesColumn << descriptionColumn, QString(DESCRIPTION_COLUMN_PADDING_SIZE, ' '));
    const int indent = namesColumnWidth + DESCRIPTION_COLUMN_PADDING_SIZE;

    QStringList wrappedTextLines;
    foreach (const auto textLine, textLines)
        wrappedTextLines << wrapLine(textLine, m_maxLineLength, indent);

    Text::indent(wrappedTextLines, m_sectionIndent);
    return wrappedTextLines.join('\n');
}

QStringList HelpFormatter::formatCommandsDescriptionColumn(const CommandList& commands) const
{
    QStringList column;
    foreach (const auto pCmd, commands)
        column << pCmd->description();
    return column;
}

QStringList HelpFormatter::formatCommandsNamesColumn(const CommandList& commands) const
{
    QStringList column;
    foreach (const auto pCmd, commands)
        column << pCmd->names().join(", ");
    return column;
}

QString HelpFormatter::formatOptionListSection(const OptionListSection& section) const
{
    const auto& title = section.first;
    const auto& options = section.second;

    QStringList wrappedTextLines;

    wrappedTextLines << formatTitle(title);

    wrappedTextLines << formatOptions(options);

    return wrappedTextLines.join('\n');
}

QString HelpFormatter::formatOptions(const OptionList& options) const
{
    auto shortNamesColumn = formatShortOptionsColumn(options);
    const int shortNamesColumnWidth = Text::adjustToMaxLen(shortNamesColumn);

    auto longNamesColumn = formatLongOptionsColumn(options);
    const int longNamesColumnWidth = Text::adjustToMaxLen(longNamesColumn);

    const auto descriptionsColumn = formatOptionsDescriptionColumn(options);

    auto textLines = Text::join(QList<QStringList>() << shortNamesColumn << longNamesColumn, QString(OPTION_COLUMN_PADDING_SIZE, ' '));
    textLines = Text::join(QList<QStringList>() << textLines << descriptionsColumn, QString(DESCRIPTION_COLUMN_PADDING_SIZE, ' '));
    const int indent = shortNamesColumnWidth + OPTION_COLUMN_PADDING_SIZE + longNamesColumnWidth + DESCRIPTION_COLUMN_PADDING_SIZE;

    QStringList wrappedTextLines;
    foreach (const auto textLine, textLines)
        wrappedTextLines << wrapLine(textLine, m_maxLineLength, indent);

    Text::indent(wrappedTextLines, m_sectionIndent);
    return wrappedTextLines.join('\n');
}

QString HelpFormatter::formatTextSection(const TextSection& section) const
{
    const auto& title = section.first;
    const auto& paragraphs = section.second;

    QStringList wrappedTextLines;

    wrappedTextLines << formatTitle(title);

    foreach (const auto& paragraph, paragraphs)
    {
        const auto& indent = paragraph.first;
        const auto& text = paragraph.second;

        foreach (const auto line, text.split('\n'))
            wrappedTextLines << wrapLine(line, m_maxLineLength, m_sectionIndent + indent, m_sectionIndent + indent);
    }

    return wrappedTextLines.join('\n');
}

QStringList HelpFormatter::formatTitle(const QString& title) const
{
    if (title.isEmpty())
        return QStringList();

    return wrapLine(title + ":", m_maxLineLength) << QString();
}

QStringList HelpFormatter::formatShortOptionsColumn(const OptionList& options) const
{
    QStringList shortOptionsColumn;
    foreach (const auto pOption, options)
    {
        auto shortNames = pOption->shortNames();
        qSort(shortNames);
        QStringList names;
        foreach (const auto& shortName, shortNames)
            names << QString(shortName);

        auto shortOptions = names.join("|");
        if (!shortOptions.isEmpty())
        {
            shortOptions.insert(0, '-');

            if (pOption->hasValue())
                shortOptions += QString(pOption->isValueMandatory() ? " %1" : " [%1]").arg(pOption->valueName().toUpper());

            if (!shortOptions.isEmpty() && !pOption->longNames().isEmpty())
                shortOptions += ",";
        }
        shortOptionsColumn << shortOptions;
    }
    return shortOptionsColumn;
}

QStringList HelpFormatter::formatLongOptionsColumn(const OptionList& options) const
{
    QStringList longOptionsColumn;
    foreach (const auto pOption, options)
    {
        auto names = pOption->longNames();
        qSort(names);

        auto longOptions = names.join('|');
        if (!longOptions.isEmpty())
        {
            longOptions.insert(0, "--");

            if (pOption->hasValue())
                longOptions += QString(pOption->isValueMandatory() ? "=%1" : "[=%1]").arg(pOption->valueName().toUpper());
        }
        longOptionsColumn << longOptions;
    }
    return longOptionsColumn;
}

QStringList HelpFormatter::formatOptionsDescriptionColumn(const OptionList& options) const
{
    QStringList descriptionColumn;
    foreach (const auto pOption, options)
    {
        auto description = pOption->description();
        if (pOption->hasValue() && !pOption->isValueMandatory())
            description = QString("%1 (default is '%2')").arg(description).arg(pOption->defaultValue());
        descriptionColumn << description;
    }
    return descriptionColumn;
}

}

}
