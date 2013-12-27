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
#ifndef MARKEMPTYDIRS_API_CCONTEXT_HPP
#define MARKEMPTYDIRS_API_CCONTEXT_HPP

#include "markemptydirsapi_global.hpp"

#include <QDir>


namespace CodeMagic
{
namespace Text
{
namespace Template
{
    class Engine;
}
}
}


namespace MarkEmptyDirs
{

namespace Api
{

class Config;
class Logger;

class MARKEMPTYDIRSAPISHARED_EXPORT Context
{
public:
    static std::unique_ptr<Context> create(Logger* pLogger, CodeMagic::Text::Template::Engine* pTemplateEngine);
    static std::unique_ptr<Context> create();

    ~Context();

    void setBaseDir(const QDir& baseDir);

    QDir baseDir() const;

    void setConfig(const Config& config);

    const Config& config() const;

    void setCurrentDir(const QDir& currentDir);

    QDir currentDir() const;

    Logger& logger();

    CodeMagic::Text::Template::Engine& templateEngine();

protected:
    Context(Logger* pLogger, CodeMagic::Text::Template::Engine* pTemplateEngine);

private:
    QDir m_baseDir;
    const Config* m_pConfig;
    QDir m_currentDir;
    Logger* m_pLogger;
    CodeMagic::Text::Template::Engine* m_pTemplateEngine;
};

}

}

#endif // CONTEXT_HPP
