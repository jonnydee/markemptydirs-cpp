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

#include "Variable.hpp"
#include "VariableFactory.hpp"

#include "CodeMagic/FileSystem/FileSystemTools.hpp"

#include <QChar>
#include <QDateTime>
#include <QObject>
#include <QUuid>


namespace CodeMagic
{

namespace Text
{

namespace Template
{

VariableFactory::VariableFactory()
{
}

VariableFactory::VariablePtr VariableFactory::createCharRepeaterVariable(const QString& name, const QChar& ch, const QString& description) const
{
    auto pVariable = new Variable(name,
        [ch](const Variable::Context& ctx)
        {
            bool ok;
            const auto countStr = ctx.argument.trimmed();
            const auto count = countStr.toUInt(&ok);
            return ok ? QString(count, ch) : QString();
        });
    pVariable->setDescription(description);
    pVariable->setArgumentSpec("COUNT");
    pVariable->setDefaultArgument("1");
    pVariable->addArgumentDescription("COUNT", QObject::tr("integer denoting how often character is repeated"));
    return VariablePtr(pVariable);
}

VariableFactory::VariablePtr VariableFactory::createDateTimeVariable() const
{
    auto pVariable = new Variable("datetime",
        [](const Variable::Context& ctx)
        {
            const auto currentDateTime = QDateTime::currentDateTimeUtc();
            return currentDateTime.toString(ctx.argument);
        });
    pVariable->setDescription(QObject::tr("get UTC time"));
    pVariable->setArgumentSpec("FORMAT");
    pVariable->setDefaultArgument("yyyy-MM-ddThh:mm:ssZ");
    pVariable->addArgumentDescription("FORMAT", QObject::tr("custom format string (see documentation for details)"));
    return VariablePtr(pVariable);
}

VariableFactory::VariablePtr VariableFactory::createEnvironmentVariable() const
{
    auto pVariable = new Variable("env",
        [](const Variable::Context& ctx)
        {
            const auto varName = ctx.argument.trimmed().toUtf8().append('\0');
            const auto varValue = qgetenv(varName.constData());
            return QString::fromUtf8(varValue);
        });
    pVariable->setDescription(QObject::tr("get the value from an environment variable"));
    pVariable->setArgumentSpec("NAME");
    pVariable->addArgumentDescription("NAME", QObject::tr("the environment variable's name"));
    return VariablePtr(pVariable);
}

VariableFactory::VariablePtr VariableFactory::createGuidVariable() const
{
    auto pVariable = new Variable("guid",
        [](const Variable::Context&)
        {
            return QUuid::createUuid().toString();
        });
    pVariable->setDescription(QObject::tr("get a new globally unique identifier"));
    return VariablePtr(pVariable);
}

VariableFactory::VariablePtr VariableFactory::createLinefeedVariable() const
{
    return createCharRepeaterVariable(
        "lf", '\n',
        QObject::tr("get line feed character(s)"));
}

VariableFactory::VariablePtr VariableFactory::createSeparatorVariable() const
{
    auto pVariable = new Variable("separator",
        [](const Variable::Context& ctx) -> QString
        {
            if ("dir" == ctx.argument)
                return FileSystem::dirSeparator();
            if ("path" == ctx.argument)
                return FileSystem::pathSeparator();
            if ("vol" == ctx.argument)
                return FileSystem::volumeSeparator();
            return QString();
        });
    pVariable->setDescription(QObject::tr("get platform specific directory, path, or volume separator"));
    pVariable->setArgumentSpec("dir|path|vol");
    pVariable->addArgumentDescription("dir", QObject::tr("directory separator ('%1')").arg(FileSystem::dirSeparator()));
    pVariable->addArgumentDescription("path", QObject::tr("path separator ('%1')").arg(FileSystem::pathSeparator()));
    pVariable->addArgumentDescription("vol", QObject::tr("volume separator ('%1')").arg(FileSystem::volumeSeparator()));
    return VariablePtr(pVariable);
}

VariableFactory::VariablePtr VariableFactory::createSpaceVariable() const
{
    return createCharRepeaterVariable(
        "sp", ' ',
        QObject::tr("get space character(s)"));
}

}

}

}
