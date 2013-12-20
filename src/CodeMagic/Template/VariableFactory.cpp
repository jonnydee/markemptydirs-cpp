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

#include "Variable.hpp"
#include "VariableFactory.hpp"

#include "CodeMagic/FileSystem.hpp"

#include <QChar>
#include <QDateTime>
#include <QObject>
#include <QUuid>


namespace CodeMagic
{

namespace Template
{

VariableFactory::VariableFactory()
{
}

Variable* VariableFactory::createCharRepeaterVariable(const QString& name, const QChar& ch, const QString& description) const
{
    auto pVariable = new Variable(name,
        [ch](const Variable::Context& ctx)
        {
            bool ok;
            const auto countStr = ctx.argument.trimmed();
            const auto count = countStr.toUInt(&ok);
            return ok ? QString(count, ch) : QString();
        });
    pVariable->setArgumentSpec("count");
    pVariable->setDefaultArgument("1");
    pVariable->setDescription(description);
    return pVariable;
}

Variable* VariableFactory::createDateTimeVariable() const
{
    auto pVariable = new Variable("datetime",
        [](const Variable::Context& ctx)
        {
            const auto currentDateTime = QDateTime::currentDateTimeUtc();
            return currentDateTime.toString(ctx.argument);
        });
    pVariable->setArgumentSpec("format-pattern");
    pVariable->setDefaultArgument("yyyy-MM-ddThh:mm:ssZ");
    pVariable->setDescription(QObject::tr("get UTC time"));
    return pVariable;
}

Variable* VariableFactory::createEnvironmentVariable() const
{
    auto pVariable = new Variable("env",
        [](const Variable::Context& ctx)
        {
            const auto varName = ctx.argument.trimmed().toUtf8().append('\0');
            const auto varValue = qgetenv(varName.constData());
            return QString::fromUtf8(varValue);
        });
    pVariable->setArgumentSpec("env-var-name");
    pVariable->setDescription(QObject::tr("get the value from an environment variable"));
    return pVariable;
}

Variable* VariableFactory::createGuidVariable() const
{
    auto pVariable = new Variable("guid",
        [](const Variable::Context&)
        {
            return QUuid::createUuid().toString();
        });
    pVariable->setDescription(QObject::tr("get a new globally unique identifier"));
    return pVariable;
}

Variable* VariableFactory::createLinefeedVariable() const
{
    return createCharRepeaterVariable(
        "lf", '\n',
        QObject::tr("get line feed character(s)"));
}

Variable* VariableFactory::createSeparatorVariable() const
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
    pVariable->setArgumentSpec("dir|path|vol");
    pVariable->setDescription(QObject::tr("get platform specific directory, path, or volume separator"));
    return pVariable;
}

Variable* VariableFactory::createSpaceVariable() const
{
    return createCharRepeaterVariable(
        "sp", ' ',
        QObject::tr("get space character(s)"));
}

}

}
