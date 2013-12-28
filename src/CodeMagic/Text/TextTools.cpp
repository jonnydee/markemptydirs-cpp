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

#include "TextTools.hpp"

#include <QString>


namespace CodeMagic
{

namespace Text
{

int adjustToMaxLen(QStringList& strings, QChar paddingChar)
{
    int maxLen = 0;
    foreach (const auto& str, strings)
        maxLen = qMax(maxLen, str.length());
    for (int i = 0; i < strings.size(); i++)
        strings[i] += QString(maxLen - strings[i].length(), paddingChar);
    return maxLen;
}

void indent(QStringList& strings, int count, QChar paddingChar)
{
    prepend(strings, QString(count, paddingChar));
}

QStringList join(const QList<QStringList>& columns, const QString& separator)
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

void prepend(QStringList& strings, const QString& prefix)
{
    for (int i = 0; i < strings.size(); i++)
        strings[i].prepend(prefix);
}

void trimLeft(QString& str)
{
    for (int i = 0; i < str.length() - 1; i++)
        if (!str[i].isSpace())
        {
            str.remove(0, i);
            break;
        }
}

void trimLeft(QStringList& strings)
{
    for (int i = 0; i < strings.size(); i++)
        trimLeft(strings[i]);
}

void trimRight(QString& str)
{
    for (int i = str.length() - 1; i >= 0; --i)
        if (!str[i].isSpace())
        {
            str.truncate(i + 1);
            break;
        }
}

void trimRight(QStringList& strings)
{
    for (int i = 0; i < strings.size(); i++)
        trimRight(strings[i]);
}

}

}
