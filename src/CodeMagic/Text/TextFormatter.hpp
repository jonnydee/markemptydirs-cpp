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
#ifndef CODEMAGIC_TEXT_FORMATTER_HPP
#define CODEMAGIC_TEXT_FORMATTER_HPP

#include "../codemagic_global.hpp"

#include <QList>
#include <QString>


namespace CodeMagic
{

namespace Text
{

class CODEMAGICSHARED_EXPORT TextFormatter
{
public:
    TextFormatter();

    QString format(const QString& paragraph) const;

    void setFirstLineLeftIndent(int count);
    int firstLineLeftIndent() const;

    void setFirstLineRightIndent(int count);
    int firstLineRightIndent() const;

    void setLinesAfterParagraph(int count);
    int linesAfterParagraph() const;

    void setLinesBeforeParagraph(int count);
    int linesBeforeParagraph() const;

    void setMaxLineLength(int maxLength);
    int maxLineLength() const;

    void setParagraphLeftIndent(int count);
    int paragraphLeftIndent() const;

    void setParagraphRightIndent(int count);
    int paragraphRightIndent() const;

private:
    int m_firstLineLeftIndent;
    int m_firstLineRightIndent;
    int m_linesAfterParagraph;
    int m_linesBeforeParagraph;
    int m_maxLineLength;
    int m_paragraphLeftIndent;
    int m_paragraphRightIndent;
};

}

}

#endif // CODEMAGIC_TEXT_FORMATTER_HPP
