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


namespace MarkEmptyDirs
{

namespace Api
{

QString HelpFormatter::format(const OptionList& options) const
{
    auto detailList = formatOptionDetailList(options);
    indent(detailList, 4);
    return detailList.join('\n');
}

QStringList HelpFormatter::formatOptionDetailList(const OptionList& options) const
{
    auto shortNamesColumn = formatShortNames(options);
    adjustToMaxLen(shortNamesColumn);

    auto longNamesColumn = formatLongNames(options);

    auto optionsColumn = joinColumns(QList<QStringList>() << shortNamesColumn << longNamesColumn, " ");
    trimRight(optionsColumn);

    auto valueNamesColumn = formatValueNames(options);

    auto leftSide = joinColumns(QList<QStringList>() << optionsColumn << valueNamesColumn, " ");
    adjustToMaxLen(leftSide);

    auto descriptionsColumn = formatDescriptions(options);

    return joinColumns(QList<QStringList>() << leftSide << descriptionsColumn, " ");
}

QStringList HelpFormatter::formatShortNames(const OptionList& options) const
{
    QStringList allNames;
    foreach (const auto pOption, options)
    {
        auto shortNames = pOption->shortNames();
        qSort(shortNames);
        QStringList names;
        foreach (const auto& shortName, shortNames)
            names << QString("-%1").arg(shortName);

        auto joined = names.join(", ");
        if (!joined.isEmpty() && !pOption->longNames().isEmpty())
            joined += ",";

        allNames << joined;
    }
    return allNames;
}

QStringList HelpFormatter::formatLongNames(const OptionList& options) const
{
    QStringList allNames;
    foreach (const auto pOption, options)
    {
        auto names = pOption->longNames();
        qSort(names);
        for (int i = 0; i < names.size(); i++)
            names[i] = QString("--%1").arg(names[i]);
        allNames << names.join(", ");
    }
    return allNames;
}

QStringList HelpFormatter::formatValueNames(const OptionList& options) const
{
    QStringList allNames;
    foreach (const auto pOption, options)
    {
        if (pOption->hasValue())
            allNames << QString(pOption->isValueMandatory() ? "<%1>" : "[%1]").arg(pOption->valueName());
        else
            allNames << QString();
    }
    return allNames;
}

QStringList HelpFormatter::formatDescriptions(const OptionList& options) const
{
    QStringList allDescriptions;
    foreach (const auto pOption, options)
        allDescriptions << pOption->description();
    return allDescriptions;
}

void HelpFormatter::adjustToMaxLen(QStringList& strings) const
{
    int maxLen = 0;
    foreach (const auto& str, strings)
        maxLen = qMax(maxLen, str.length());
    for (int i = 0; i < strings.size(); i++)
        strings[i] += QString(maxLen - strings[i].length(), ' ');
}

void HelpFormatter::indent(QStringList& strings, int count) const
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


}

}
