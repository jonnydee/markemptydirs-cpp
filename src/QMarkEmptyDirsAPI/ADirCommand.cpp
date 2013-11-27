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

#include "ADirCommand.hpp"
#include "DirDescriptor.hpp"

#include <QFile>
#include <QFileInfo>
#include <QObject>

#include <iostream>


namespace MarkEmptyDirs
{

namespace Api
{

ADirCommand::ADirCommand(const Config& config)
    : m_config(config)
    , m_logger(config)
{
}

ADirCommand::~ADirCommand()
{
}

const Config& ADirCommand::config() const
{
    return m_config;
}

bool ADirCommand::createMarker(const QDir& dir)
{
    QFileInfo markerFileInfo(dir, config().markerFileName());
    auto filePath = markerFileInfo.absoluteFilePath();
    QFile markerFile(filePath);
    if (config().dryRun() || markerFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        const auto logMsg = config().shortMessages()
                ? QString("%1: '%2'").arg(QObject::tr("Created")).arg(filePath)
                : QString("%1: '%2'").arg(QObject::tr("Created placeholder")).arg(filePath);
        logger().log(logMsg, LogLevel::INFO);
        return true;
    }
    else
    {
        logger().log(QString("%1: '%2' (%3)").arg(QObject::tr("Could not create placeholder")).arg(filePath).arg(markerFile.errorString()),
                     LogLevel::ERROR);
        return false;
    }
}

bool ADirCommand::removeMarker(const QDir &dir)
{
    QFileInfo markerFileInfo(dir, config().markerFileName());
    auto filePath = markerFileInfo.canonicalFilePath();
    QFile markerFile(filePath);
    if (config().dryRun() || markerFile.remove())
    {
        const auto logMsg = config().shortMessages()
                ? QString("%1: '%2'").arg(QObject::tr("Deleted")).arg(filePath)
                : QString("%1: '%2'").arg(QObject::tr("Deleted placeholder")).arg(filePath);
        logger().log(logMsg, LogLevel::INFO);
        return true;
    }
    else
    {
        logger().log(QString("%1: '%2' (%3)").arg(QObject::tr("Could not delete placeholder")).arg(filePath).arg(markerFile.errorString()),
                     LogLevel::ERROR);
        return false;
    }
}

void ADirCommand::setPathMap(const PathMap& pathMap)
{
    m_pathMap = pathMap;
}

ADirCommand::PathMap& ADirCommand::pathMap()
{
    return m_pathMap;
}

Logger& ADirCommand::logger()
{
    return m_logger;
}

}

}
