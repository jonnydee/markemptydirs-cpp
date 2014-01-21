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

#include "Config.hpp"
#include "Context.hpp"
#include "VariableFactory.hpp"

#include <QodeMagic/Text/Template/Variable.hpp>


using namespace QodeMagic::Text::Template;

namespace MarkEmptyDirs
{

namespace Api
{

VariableFactory::VariableFactory()
{
}

VariableFactory::VariablePtr VariableFactory::createDirVariable(const Context& ctx) const
{
    const auto pMarkEmptyDirsApiCtx = &ctx;

    auto pVariable = new Variable("dir",
        [pMarkEmptyDirsApiCtx](const Variable::Context& ctx) -> QString
        {
            if ("base" == ctx.argument)
                return pMarkEmptyDirsApiCtx->baseDir().dirName();
            if ("base.abs" == ctx.argument)
                return pMarkEmptyDirsApiCtx->baseDir().absolutePath();
            if ("base.rel" == ctx.argument)
                return pMarkEmptyDirsApiCtx->currentDir().relativeFilePath(pMarkEmptyDirsApiCtx->baseDir().absolutePath());
            if ("cur" == ctx.argument)
                return pMarkEmptyDirsApiCtx->currentDir().dirName();
            if ("cur.abs" == ctx.argument)
                return pMarkEmptyDirsApiCtx->currentDir().absolutePath();
            if ("cur.rel" == ctx.argument)
                return pMarkEmptyDirsApiCtx->baseDir().relativeFilePath(pMarkEmptyDirsApiCtx->currentDir().absolutePath());
            return QString();
        });
    pVariable->setDescription(QObject::tr("get base or current directory"));
    pVariable->setArgumentSpec("base|base.abs|base.rel|cur|cur.abs|cur.rel");
    pVariable->setDefaultArgument("base.abs");
    pVariable->addArgumentDescription("base", QObject::tr("name of base directory"));
    pVariable->addArgumentDescription("base.abs", QObject::tr("absolute path of base directory"));
    pVariable->addArgumentDescription("base.rel", QObject::tr("path of base directory relative to current directory"));
    pVariable->addArgumentDescription("cur", QObject::tr("name of current directory"));
    pVariable->addArgumentDescription("cur.abs", QObject::tr("absolute path of current directory"));
    pVariable->addArgumentDescription("cur.rel", QObject::tr("path of current directory relative to base directory"));
    return VariablePtr(pVariable);
}

VariableFactory::VariablePtr VariableFactory::createMarkerVariable(const Context& ctx) const
{
    const auto pMarkEmptyDirsApiCtx = &ctx;

    auto pVariable = new Variable("marker",
        [pMarkEmptyDirsApiCtx](const Variable::Context& ctx) -> QString
        {
            if ("name" == ctx.argument)
                return pMarkEmptyDirsApiCtx->config().markerName();

            const auto markerFilePath = pMarkEmptyDirsApiCtx->currentDir().absoluteFilePath(pMarkEmptyDirsApiCtx->config().markerName());

            if ("name.abs" == ctx.argument)
                return markerFilePath;
            if ("name.rel" == ctx.argument)
                return pMarkEmptyDirsApiCtx->baseDir().relativeFilePath(markerFilePath);

            return QString();
        });
    pVariable->setDescription(QObject::tr("get marker name"));
    pVariable->setArgumentSpec("name|name.abs|name.rel");
    pVariable->setDefaultArgument("name");
    pVariable->addArgumentDescription("name", QObject::tr("name of marker file"));
    pVariable->addArgumentDescription("name.abs", QObject::tr("absolute path of marker file"));
    pVariable->addArgumentDescription("name.rel", QObject::tr("path of marker file relative to base directory"));
    return VariablePtr(pVariable);
}

}

}
