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

#include "FileSystemTools.hpp"

#include <QChar>
#include <QDir>
#include <QRegExp>
#include <QString>


namespace CodeMagic
{

namespace FileSystem
{

QChar dirSeparator()
{
    static const auto separator = QDir::separator();
    return separator;
}

bool validateFileName(const QString& fileName, QString* pErrorMsg)
{
    static QString devNull;
    auto& errorMsg = pErrorMsg ? *pErrorMsg : devNull;

    if (fileName.isEmpty())
    {
        errorMsg = QObject::tr("File name must not be empty.");
        return false;
    }

#ifdef Q_OS_WIN32
    static const QRegExp invalidPattern("[<>:\"/\\|?*]");
#else
    static const QRegExp invalidPattern("[<>:\"/\\|?*]");
#endif
    if (fileName.contains(invalidPattern))
    {
        errorMsg = QObject::tr("File name contains invalid characters.");
        return false;
    }

    return true;
}

QChar pathSeparator()
{
#ifdef Q_OS_WIN32
    return ';';
#else
    return ':';
#endif
}

QChar volumeSeparator()
{
#ifdef Q_OS_WIN32
    return ':';
#else
    return '/';
#endif
}

}

}
