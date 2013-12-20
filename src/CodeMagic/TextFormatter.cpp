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

#include "TextFormatter.hpp"

#include "Tools.hpp"


namespace CodeMagic
{

TextFormatter::TextFormatter()
    : m_firstLineLeftIndent(0)
    , m_firstLineRightIndent(0)
    , m_linesAfterParagraph(0)
    , m_linesBeforeParagraph(0)
    , m_maxLineLength(0)
    , m_paragraphLeftIndent(0)
    , m_paragraphRightIndent(0)
{
}

static QString wrapSingleLine(QString& line, int maxLength)
{
    line = line.trimmed();
    if (line.length() <= maxLength)
        return QString();

    int i = maxLength - 1;
    while (i >= 0 && !line[i].isSpace())
        --i;

    if (i < 0)
        i = maxLength;

    const auto tail = line.mid(i);
    line.truncate(i);
    trimRight(line);
    return tail.trimmed();
}

QString TextFormatter::format(const QString& paragraph) const
{
    QStringList wrappedLines;

    auto line = paragraph.trimmed();
    auto LeftIndent = firstLineLeftIndent();
    auto rightIndent = firstLineRightIndent();
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
    if (!line.isEmpty())
        wrappedLines << QString(LeftIndent, ' ') + line;

    return wrappedLines.join('\n');
}

void TextFormatter::setFirstLineLeftIndent(int count)
{
    m_firstLineLeftIndent = count;
}

int TextFormatter::firstLineLeftIndent() const
{
    return m_firstLineLeftIndent;
}

void TextFormatter::setFirstLineRightIndent(int count)
{
    m_firstLineRightIndent = count;
}

int TextFormatter::firstLineRightIndent() const
{
    return m_firstLineRightIndent;
}

void TextFormatter::setLinesAfterParagraph(int count)
{
    m_linesAfterParagraph = count;
}

int TextFormatter::linesAfterParagraph() const
{
    return m_linesAfterParagraph;
}

void TextFormatter::setLinesBeforeParagraph(int count)
{
    m_linesBeforeParagraph = count;
}

int TextFormatter::linesBeforeParagraph() const
{
    return m_linesBeforeParagraph;
}

void TextFormatter::setMaxLineLength(int maxLength)
{
    m_maxLineLength = maxLength;
}

int TextFormatter::maxLineLength() const
{
    return m_maxLineLength;
}

void TextFormatter::setParagraphLeftIndent(int count)
{
    m_paragraphLeftIndent = count;
}

int TextFormatter::paragraphLeftIndent() const
{
    return m_paragraphLeftIndent;
}

void TextFormatter::setParagraphRightIndent(int count)
{
    m_paragraphRightIndent = count;
}

int TextFormatter::paragraphRightIndent() const
{
    return m_paragraphRightIndent;
}

}
