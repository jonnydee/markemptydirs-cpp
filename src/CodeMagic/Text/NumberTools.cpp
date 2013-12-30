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

#include "NumberTools.hpp"

#include <QString>
#include <QStringList>

#include <limits>
#include <type_traits>


namespace CodeMagic
{

namespace Text
{

template <>
bool parse<bool>(const QString& str, bool* pOk, int)
{
    static bool devNull;
    bool& ok = pOk ? *pOk : devNull;

    const auto number = str.toShort(&ok, 0);
    if (ok)
    {
        ok = number == 0 || number == 1;
        return 0 != number;
    }

    static const auto trueConstants = QStringList() << "true" << "yes" << "on";
    static const auto falseConstants = QStringList() << "false" << "no" << "off";

    const auto numStr = str.trimmed();

    foreach (const auto& boolConstant, trueConstants)
        if (0 == numStr.compare(boolConstant, Qt::CaseInsensitive))
        {
            ok = true;
            return true;
        }

    foreach (const auto& boolConstant, falseConstants)
        if (0 == numStr.compare(boolConstant, Qt::CaseInsensitive))
        {
            ok = true;
            return false;
        }

    ok = false;
    return false;
}

template <>
float parse<float>(const QString& str, bool* pOk, int)
{
    auto number = str.toFloat(pOk);
    return number;
}

template <>
double parse<double>(const QString& str, bool* pOk, int)
{
    auto number = str.toDouble(pOk);
    return number;
}

template <>
qint8 parse<qint8>(const QString& str, bool* pOk, int base)
{
    static bool devNull;
    bool& ok = pOk ? *pOk : devNull;

    auto number = str.toShort(&ok, base);
    if (!ok || number < (std::numeric_limits<qint8>::min)() || number > (std::numeric_limits<qint8>::max)())
    {
        ok = false;
        return 0;
    }

    return number;
}

template <>
quint8 parse<quint8>(const QString& str, bool* pOk, int base)
{
    static bool devNull;
    bool& ok = pOk ? *pOk : devNull;

    auto number = str.toUShort(&ok, base);
    if (!ok || number < (std::numeric_limits<quint8>::min)() || number > (std::numeric_limits<quint8>::max)())
    {
        ok = false;
        return 0;
    }

    return number;
}

template <>
qint16 parse<qint16>(const QString& str, bool* pOk, int base)
{
    auto number = str.toShort(pOk, base);
    static_assert((std::is_same<qint16, decltype(number)>::value), "Types incompatible.");
    return number;
}

template <>
quint16 parse<quint16>(const QString& str, bool* pOk, int base)
{
    auto number = str.toUShort(pOk, base);
    static_assert((std::is_same<quint16, decltype(number)>::value), "Types incompatible.");
    return number;
}

template <>
qint32 parse<qint32>(const QString& str, bool* pOk, int base)
{
    auto number = str.toInt(pOk, base);
    static_assert((std::is_same<qint32, decltype(number)>::value), "Types incompatible.");
    return number;
}

template <>
quint32 parse<quint32>(const QString& str, bool* pOk, int base)
{
    auto number = str.toUInt(pOk, base);
    static_assert((std::is_same<quint32, decltype(number)>::value), "Types incompatible.");
    return number;
}

template <>
qint64 parse<qint64>(const QString& str, bool* pOk, int base)
{
    auto number = str.toLongLong(pOk, base);
    static_assert((std::is_same<qint64, decltype(number)>::value), "Types incompatible.");
    return number;
}

template <>
quint64 parse<quint64>(const QString& str, bool* pOk, int base)
{
    auto number = str.toULongLong(pOk, base);
    static_assert((std::is_same<quint64, decltype(number)>::value), "Types incompatible.");
    return number;
}

}

}
