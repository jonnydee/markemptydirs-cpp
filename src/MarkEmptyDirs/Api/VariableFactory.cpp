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

#include "Context.hpp"
#include "VariableFactory.hpp"

#include <CodeMagic/Text/Template/Variable.hpp>


namespace MarkEmptyDirs
{

namespace Api
{

VariableFactory::VariableFactory()
{
}

VariableFactory::Variable* VariableFactory::createDirVariable(const Context& ctx) const
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
    pVariable->setArgumentSpec("base|base.abs|base.rel|cur|cur.abs|cur.rel");
    pVariable->setDefaultArgument("base.abs");
    pVariable->setDescription(QObject::tr("get the base or current directory"));
    return pVariable;
}

}

}
