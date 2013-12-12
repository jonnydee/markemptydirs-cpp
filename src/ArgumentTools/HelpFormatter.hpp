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

#pragma once
#ifndef ARGUMENTTOOLS_HELPFORMATTER_HPP
#define ARGUMENTTOOLS_HELPFORMATTER_HPP

#include "ArgumentTools/Option.hpp"


namespace ArgumentTools
{

class HelpFormatter
{
public:
    HelpFormatter(int optionListIndent = 2, int maxLineLength = 80);

    void addUsageSection(const QString& executableFileName, const QString& customUsage = QString());

    void addOptionListSection(const QString& title, const OptionList& options);

    QString formatHelpText() const;

protected:
    typedef QPair<QString, OptionList> OptionListSection;

    QString formatUsageSection() const;
    QString formatOptionListSection(const OptionListSection& section) const;
    QString formatOptions(const OptionList& options) const;
    QStringList formatShortOptionsColumn(const OptionList& options) const;
    QStringList formatLongOptionsColumn(const OptionList& options) const;
    QStringList formatDescriptionColumn(const OptionList& options) const;

    int adjustToMaxLen(QStringList& strings) const;
    void indentLines(QStringList& strings, int count) const;
    QStringList joinColumns(const QList<QStringList>& columns, const QString& separator) const;
    void trimRight(QStringList& strings) const;
    QStringList wrapLine(const QString& line, int maxLength, int newLineIndent = 0) const;

private:
    int m_optionListIndent;
    int m_maxLineLength;

    QList<OptionListSection> m_optionsListSections;
    QString m_executableFileName;
    QString m_customUsage;
};

}

#endif // ARGUMENTTOOLS_HELPFORMATTER_HPP
