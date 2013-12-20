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
#ifndef STRINGMAGIC_TOOLS_H
#define STRINGMAGIC_TOOLS_H

#include "stringmagic_global.hpp"

#include <QString>
#include <QStringList>


namespace StringMagic
{

STRINGMAGICSHARED_EXPORT
int adjustToMaxLen(QStringList& strings, QChar paddingChar = ' ');

STRINGMAGICSHARED_EXPORT
void indent(QStringList& strings, int count, QChar paddingChar = ' ');

STRINGMAGICSHARED_EXPORT
QStringList join(const QList<QStringList>& columns, const QString& separator = QString());

STRINGMAGICSHARED_EXPORT
void prepend(QStringList& strings, const QString& prefix);

STRINGMAGICSHARED_EXPORT
void trimRight(QString& str);

STRINGMAGICSHARED_EXPORT
void trimRight(QStringList& strings);

//QStringList wrapLine(const QString& line, int maxLength, int newLineIndent = 0, int firstLineIdent = 0);

}

#endif // STRINGMAGIC_TOOLS_H