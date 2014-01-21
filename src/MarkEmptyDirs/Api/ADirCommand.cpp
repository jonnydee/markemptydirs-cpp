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

#include "ADirCommand.hpp"
#include "Config.hpp"
#include "Context.hpp"
#include "DirDescriptor.hpp"
#include "FileSystemCrawler.hpp"
#include "Logger.hpp"

#include <QodeMagic/FileSystem/FileSystemTools.hpp>
#include <QodeMagic/Text/Template/Engine.hpp>

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QTextStream>


using namespace QodeMagic;

namespace MarkEmptyDirs
{

namespace Api
{

void ADirCommand::run()
{
    if (context().config().rootDirs().isEmpty())
    {
        context().logger().log(QObject::tr("No directory specified."), LogLevel::ERROR);
        return;
    }

    auto pathMap = crawlDir();
    run(pathMap);
}

QDir ADirCommand::baseDir(const QDir& dir) const
{
    const auto& config = context().config();

    const auto dirPath = dir.absolutePath();
    foreach (const auto& rootDir, config.rootDirs())
        if (dirPath.startsWith(rootDir.absolutePath()))
            return rootDir;
    return dir;
}

ADirCommand::PathMap ADirCommand::crawlDir()
{
    FileSystemCrawler crawler;
    crawler.setContext(context());
    crawler.run();
    return crawler.pathMap();
}

bool ADirCommand::createMarker(const QDir& dir)
{
    const auto& config = context().config();
    auto& logger = context().logger();

    // Update context.
    context().setBaseDir(baseDir(dir));
    context().setCurrentDir(dir);

    // Create marker.
    {
        const QFileInfo markerFileInfo(dir, config.markerName());
        const auto filePath = markerFileInfo.absoluteFilePath();
        QFile markerFile(filePath);

        const auto nativeFilePath = FileSystem::toQuotedNativePath(filePath);
        if (!config.dryRun() && !markerFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            logger.log(QObject::tr("Could not create marker: %1 (%2)").arg(nativeFilePath).arg(markerFile.errorString()),
                         LogLevel::ERROR);
            return false;
        }
        else
        {
            // Write marker file content (if any).
            if (!config.dryRun() && !config.markerText().isEmpty())
            {
                auto markerText = config.markerText();
                if (config.substituteVariables())
                    context().templateEngine().process(markerText);

                QTextStream out(&markerFile);
                out << markerText;
            }

            const auto logMsg = config.shortMessages()
                    ? nativeFilePath
                    : QObject::tr("Created marker: %1").arg(nativeFilePath);
            logger.log(logMsg, LogLevel::INFO);
        }
    }

    // Execute create hook (if any).
    if (!config.createHookCommand().isNull())
    {
        auto program = config.createHookCommand();
        if (context().config().substituteVariables())
            context().templateEngine().process(program);

        QString errorString;
        if (!config.dryRun() && !executeCommand(program, errorString))
        {
            logger.log(QObject::tr("Could not execute create hook: '%1' (%2)").arg(program).arg(errorString),
                         LogLevel::ERROR);
        }
        else
        {
            const auto logMsg = config.shortMessages()
                    ? program
                    : QObject::tr("Executed create hook: '%1'").arg(program);
            logger.log(logMsg, LogLevel::INFO);
        }
    }

    return true;
}

bool ADirCommand::deleteMarker(const QDir& dir)
{
    const auto& config = context().config();
    auto& logger = context().logger();

    // Update context.
    context().setBaseDir(baseDir(dir));
    context().setCurrentDir(dir);

    // Execute delete hook (if any).
    if (!config.deleteHookCommand().isNull())
    {
        auto program = config.deleteHookCommand();
        if (context().config().substituteVariables())
            context().templateEngine().process(program);

        QString errorString;
        if (!config.dryRun() && !executeCommand(program, errorString))
        {
            logger.log(QObject::tr("Could not execute delete hook: '%1' (%2)").arg(program).arg(errorString),
                         LogLevel::ERROR);
        }
        else
        {
            const auto logMsg = config.shortMessages()
                    ? program
                    : QObject::tr("Executed delete hook: '%1'").arg(program);
            logger.log(logMsg, LogLevel::INFO);
        }
    }

    // Delete marker.
    {
        const QFileInfo markerFileInfo(dir, config.markerName());
        const auto filePath = markerFileInfo.canonicalFilePath();
        QFile markerFile(filePath);

        const auto nativeFilePath = FileSystem::toQuotedNativePath(filePath);
        if (!config.dryRun() && !markerFile.remove())
        {
            logger.log(QObject::tr("Could not delete marker: %1 (%2)").arg(nativeFilePath).arg(markerFile.errorString()),
                         LogLevel::ERROR);
            return false;
        }
        else
        {
            const auto logMsg = config.shortMessages()
                    ? nativeFilePath
                    : QObject::tr("Deleted marker: %1").arg(nativeFilePath);
            logger.log(logMsg, LogLevel::INFO);
        }
    }

    return true;
}

bool ADirCommand::executeCommand(const QString& cmd, QString& errorMessage)
{
    auto program = cmd;
    if (context().config().substituteVariables())
        context().templateEngine().process(program);

    const int result = QProcess::execute(program);
    switch (result)
    {
    case -2:
        errorMessage = QObject::tr("Process could not be started.");
        return false;
    case -1:
        errorMessage = QObject::tr("Process crashed.");
        return false;
    default:
        break;
    }

    return true;
}

}

}
