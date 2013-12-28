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

#include "Formatter.hpp"
#include "TextTools.hpp"


namespace CodeMagic
{

namespace Text
{

namespace
{

    QString wrapSingleLine(QString& line, int maxLength)
    {
        if (line.length() <= maxLength)
            return QString();

        int i = maxLength - 1;
        while (i >= 0 && !line[i].isSpace())
            --i;

        if (i < 0)
            i = maxLength;

        auto tail = line.mid(i);
        trimLeft(tail);
        line.truncate(i);
        trimRight(line);
        return tail;
    }

}

Formatter::Formatter()
    : m_firstLineLeftIndent(0)
    , m_firstLineRightIndent(0)
    , m_linesAfterParagraph(0)
    , m_linesBeforeParagraph(0)
    , m_maxLineLength(0)
    , m_paragraphLeftIndent(0)
    , m_paragraphRightIndent(0)
{
}

QStringList Formatter::format(const QString& paragraphs) const
{
    QStringList wrappedLines;

    foreach (auto line, paragraphs.split('\n'))
    {
        auto LeftIndent = qMax(firstLineLeftIndent() + paragraphLeftIndent(), 0);
        auto rightIndent = qMax(firstLineRightIndent() + paragraphRightIndent(), 0);
        int textWidth = maxLineLength() - LeftIndent - rightIndent;
        Q_ASSERT(textWidth > 0);
        while (line.length() > textWidth)
        {
            auto nextLine = wrapSingleLine(line, textWidth);
            wrappedLines << QString(LeftIndent, ' ') + line;

            line = nextLine;
            LeftIndent = paragraphLeftIndent();
            rightIndent = paragraphRightIndent();
            textWidth = m_maxLineLength - LeftIndent - rightIndent;
            Q_ASSERT(textWidth > 0);
        }
        if (line.isNull() || !line.isEmpty())
            wrappedLines << QString(LeftIndent, ' ') + line;
    }

    return wrappedLines;
}

void Formatter::setFirstLineLeftIndent(int count)
{
    m_firstLineLeftIndent = count;
}

int Formatter::firstLineLeftIndent() const
{
    return m_firstLineLeftIndent;
}

void Formatter::setFirstLineRightIndent(int count)
{
    m_firstLineRightIndent = count;
}

int Formatter::firstLineRightIndent() const
{
    return m_firstLineRightIndent;
}

void Formatter::setLinesAfterParagraph(int count)
{
    m_linesAfterParagraph = count;
}

int Formatter::linesAfterParagraph() const
{
    return m_linesAfterParagraph;
}

void Formatter::setLinesBeforeParagraph(int count)
{
    m_linesBeforeParagraph = count;
}

int Formatter::linesBeforeParagraph() const
{
    return m_linesBeforeParagraph;
}

void Formatter::setMaxLineLength(int maxLength)
{
    m_maxLineLength = maxLength;
}

int Formatter::maxLineLength() const
{
    return m_maxLineLength;
}

void Formatter::setParagraphLeftIndent(int count)
{
    m_paragraphLeftIndent = count;
}

int Formatter::paragraphLeftIndent() const
{
    return m_paragraphLeftIndent;
}

void Formatter::setParagraphRightIndent(int count)
{
    m_paragraphRightIndent = count;
}

int Formatter::paragraphRightIndent() const
{
    return m_paragraphRightIndent;
}

}

}
