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

#include "Config.hpp"
#include "Context.hpp"
#include "Logger.hpp"

#include <CodeMagic/Text/Template/Engine.hpp>


using namespace CodeMagic::Text;

namespace MarkEmptyDirs
{

namespace Api
{

Context::Context(Logger* pLogger, Template::Engine* pTemplateEngine)
    : m_pConfig(nullptr)
    , m_pLogger(pLogger)
    , m_pTemplateEngine(pTemplateEngine)
{
    if (!pLogger)
        throw std::invalid_argument("pLogger");
    if (!pTemplateEngine)
        throw std::invalid_argument("pTemplateEngine");

    m_pLogger->setContext(*this);
}

Context::~Context()
{
    delete m_pTemplateEngine;
    delete m_pLogger;
}

void Context::setBaseDir(const QDir& baseDir)
{
    m_baseDir = baseDir;
}

QDir Context::baseDir() const
{
    return m_baseDir;
}

void Context::setConfig(const Config& config)
{
    m_pConfig = &config;
}

const Config& Context::config() const
{
    Q_ASSERT(m_pConfig);
    return *m_pConfig;
}

void Context::setCurrentDir(const QDir& currentDir)
{
    m_currentDir = currentDir;
}

QDir Context::currentDir() const
{
    return m_currentDir;
}

Logger& Context::logger()
{
    Q_ASSERT(m_pLogger);
    return *m_pLogger;
}

Template::Engine& Context::templateEngine()
{
    Q_ASSERT(m_pTemplateEngine);
    return *m_pTemplateEngine;
}

}

}
