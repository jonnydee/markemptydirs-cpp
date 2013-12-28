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
#include "VariableFactory.hpp"

#include <CodeMagic/Text/Template/Engine.hpp>
#include <CodeMagic/Text/Template/Variable.hpp>


using namespace CodeMagic::Text;

namespace MarkEmptyDirs
{

namespace Api
{

std::unique_ptr<Context> Context::create(std::unique_ptr<Logger> pLogger,
                                         std::unique_ptr<Template::Engine> pTemplateEngine)
{
    if (!pLogger)
        pLogger.reset(new Logger);
    if (!pTemplateEngine)
        pTemplateEngine.reset(new Template::Engine);

    auto pContext = new Context(std::move(pLogger), std::move(pTemplateEngine));

    // Add variables to template engine.
    {
        auto& templateEngine = pContext->templateEngine();
        VariableFactory variableFactory;
        templateEngine.addVariable(variableFactory.createDateTimeVariable());
        templateEngine.addVariable(variableFactory.createDirVariable(*pContext));
        templateEngine.addVariable(variableFactory.createEnvironmentVariable());
        templateEngine.addVariable(variableFactory.createGuidVariable());
        templateEngine.addVariable(variableFactory.createLinefeedVariable());
        templateEngine.addVariable(variableFactory.createMarkerVariable(*pContext));
        templateEngine.addVariable(variableFactory.createSpaceVariable());
    }

    return std::unique_ptr<Context>(pContext);
}

std::unique_ptr<Context> Context::create()
{
    return create(nullptr, nullptr);
}

Context::Context(std::unique_ptr<Logger> pLogger, std::unique_ptr<Template::Engine> pTemplateEngine)
    : m_pConfig(nullptr)
    , m_pLogger(pLogger.release())
    , m_pTemplateEngine(pTemplateEngine.release())
{
    Q_ASSERT(m_pLogger);
    Q_ASSERT(m_pTemplateEngine);

    m_pLogger->setContext(*this);
}

Context::~Context()
{
    delete m_pTemplateEngine;
    delete m_pLogger;
    delete m_pConfig;
}

void Context::setBaseDir(const QDir& baseDir)
{
    m_baseDir = baseDir;
}

QDir Context::baseDir() const
{
    return m_baseDir;
}

void Context::setConfig(std::unique_ptr<const Config> pConfig)
{
    m_pConfig = pConfig.release();
    Q_ASSERT(m_pConfig);
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
    return *m_pLogger;
}

Template::Engine& Context::templateEngine()
{
    return *m_pTemplateEngine;
}

const Template::Engine& Context::templateEngine() const
{
    return *m_pTemplateEngine;
}

}

}
