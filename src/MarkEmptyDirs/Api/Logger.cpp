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
#include "Logger.hpp"


namespace MarkEmptyDirs
{

namespace Api
{

std::unique_ptr<Logger> Logger::create()
{
    return std::unique_ptr<Logger>(new Logger);
}

Logger::Logger()
    : m_pContext(nullptr)
    , m_sysOut(stdout)
    , m_sysErr(stderr)
{
}

void Logger::setContext(Context& ctx)
{
    m_pContext = &ctx;
}

Context& Logger::context()
{
    Q_ASSERT(m_pContext);
    return *m_pContext;
}

const Context& Logger::context() const
{
    Q_ASSERT(m_pContext);
    return *m_pContext;
}

void Logger::log(const QString& msg, LogLevel logLevel)
{
    static const QString LOG_TEMPLATE("%1: %2\n");

    auto& config = context().config();

    switch (logLevel)
    {
    case LogLevel::WARNING:
        sysErr() << LOG_TEMPLATE.arg("WARNING").arg(msg) << flush;
        break;
    case LogLevel::ERROR:
        sysErr() << LOG_TEMPLATE.arg("ERROR").arg(msg) << flush;
        break;
    case LogLevel::NONE:
        if (config.logLevel() >= LogLevel::NONE)
            sysOut() << msg << '\n' << flush;
        break;
    case LogLevel::INFO:
        if (config.logLevel() >= LogLevel::INFO)
        {
            if (config.shortMessages())
                sysOut() << msg << '\n' << flush;
            else
                sysOut() << LOG_TEMPLATE.arg("INFO").arg(msg) << flush;
        }
        break;
    case LogLevel::DEBUG:
        if (config.logLevel() >= LogLevel::DEBUG)
        {
            if (config.shortMessages())
                sysOut() << msg << '\n' << flush;
            else
                sysOut() << LOG_TEMPLATE.arg("DEBUG").arg(msg) << flush;
        }
        break;
    }
}

QTextStream& Logger::sysOut()
{
    return m_sysOut;
}

QTextStream& Logger::sysErr()
{
    return m_sysErr;
}

}

}
