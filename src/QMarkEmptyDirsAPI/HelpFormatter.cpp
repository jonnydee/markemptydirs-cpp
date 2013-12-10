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

#include "HelpFormatter.hpp"
#include "Option.hpp"

#define WRAPPED_LINE_MIN_LENGTH             40
#define OPTION_COLUMN_PADDING_SIZE          1
#define DESCRIPTION_COLUMN_PADDING_SIZE     2

namespace MarkEmptyDirs
{

namespace Api
{

HelpFormatter::HelpFormatter(int optionListIndent, int maxLineLength)
    : m_optionListIndent(optionListIndent)
    , m_maxLineLength(maxLineLength)
{
    if (m_maxLineLength - m_optionListIndent < WRAPPED_LINE_MIN_LENGTH)
    {
        m_optionListIndent = 0;

        if (m_maxLineLength < WRAPPED_LINE_MIN_LENGTH)
            m_maxLineLength = std::numeric_limits<int>::max();
    }
}

void HelpFormatter::addUsageSection(const QString& executableFileName, const QString& customUsage)
{
    m_executableFileName = executableFileName;
    m_customUsage = customUsage;
}

void HelpFormatter::addOptionListSection(const QString& title, const OptionList& options)
{
    m_optionsListSections << OptionListSection(title, options);
}

QString HelpFormatter::formatHelpText() const
{
    QStringList sections;
    sections << formatUsageSection();
    foreach (auto section, m_optionsListSections)
        sections << formatOptionListSection(section);
    return sections.join("\n\n");
}

QString HelpFormatter::formatUsageSection() const
{
    auto usageArgs = m_customUsage.isNull()
            ? "*** TODO ***" //formatGenericUsageArgs()
            : m_customUsage;

    return QString("%1: %2 %3").arg(QObject::tr("USAGE")).arg(m_executableFileName).arg(usageArgs);
}

QString HelpFormatter::formatOptionListSection(const OptionListSection& section) const
{
    return QString("%1:\n%2")
            .arg(section.first)
            .arg(formatOptions(section.second));
}

QString HelpFormatter::formatOptions(const OptionList& options) const
{
    auto shortNamesColumn = formatShortOptionsColumn(options);
    const int shortNamesColumnWidth = adjustToMaxLen(shortNamesColumn);

    auto longNamesColumn = formatLongOptionsColumn(options);
    const int longNamesColumnWidth = adjustToMaxLen(longNamesColumn);

    const auto descriptionsColumn = formatDescriptionColumn(options);

    auto textLines = joinColumns(QList<QStringList>() << shortNamesColumn << longNamesColumn, QString(OPTION_COLUMN_PADDING_SIZE, ' '));
    textLines = joinColumns(QList<QStringList>() << textLines << descriptionsColumn, QString(DESCRIPTION_COLUMN_PADDING_SIZE, ' '));
    const int indent = shortNamesColumnWidth + OPTION_COLUMN_PADDING_SIZE + longNamesColumnWidth + DESCRIPTION_COLUMN_PADDING_SIZE;

    QStringList wrappedTextLines;
    foreach (const auto textLine, textLines)
        wrappedTextLines << wrapLine(textLine, m_maxLineLength, indent);

    indentLines(wrappedTextLines, m_optionListIndent);
    return wrappedTextLines.join('\n');
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

QStringList HelpFormatter::formatDescriptionColumn(const OptionList& options) const
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

int HelpFormatter::adjustToMaxLen(QStringList& strings) const
{
    int maxLen = 0;
    foreach (const auto& str, strings)
        maxLen = qMax(maxLen, str.length());
    for (int i = 0; i < strings.size(); i++)
        strings[i] += QString(maxLen - strings[i].length(), ' ');
    return maxLen;
}

void HelpFormatter::indentLines(QStringList& strings, int count) const
{
    const QString left(count , ' ');
    for (int i = 0; i < strings.size(); i++)
        strings[i] = left + strings[i];
}

QStringList HelpFormatter::joinColumns(const QList<QStringList>& columns, const QString& separator) const
{
    if (columns.isEmpty())
        return QStringList();

    QStringList joined;

    const int numRows = columns.first().size();
    for (int i = 0; i < numRows; i++)
    {
        QStringList row;
        foreach (const auto& column, columns)
        {
            Q_ASSERT(numRows == column.size());
            row << column[i];
        }
        joined << row.join(separator);
    }

    return joined;
}

static void strTrimRight(QString& str)
{
    for (int i = str.size() - 1; i >= 0; --i)
        if (!str[i].isSpace())
        {
            str.truncate(i + 1);
            break;
        }
}

void HelpFormatter::trimRight(QStringList& strings) const
{
    for (int i = 0; i < strings.size(); i++)
        strTrimRight(strings[i]);
}

QStringList HelpFormatter::wrapLine(const QString& line, int maxLength, int newLineIndent) const
{
    QStringList wrappedLines;

    auto currentLine = line;
    while (currentLine.length() > maxLength)
    {
        int i = maxLength;
        while (i >= 0 && !currentLine[i].isSpace())
            --i;

        auto newLine = currentLine.left(i);
        strTrimRight(newLine);
        wrappedLines << newLine;

        currentLine = QString(newLineIndent, ' ') + currentLine.mid(i++).trimmed();
    }
    wrappedLines << currentLine;

    return wrappedLines;
}

}

}
