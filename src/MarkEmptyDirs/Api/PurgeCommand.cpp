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
#include "DirDescriptor.hpp"
#include "Logger.hpp"
#include "PurgeCommand.hpp"

#include <QodeMagic/FileSystem/FileSystemTools.hpp>

#include <cerrno>


using namespace QodeMagic;

namespace MarkEmptyDirs
{

namespace Api
{

PurgeCommand::PurgeCommand()
{
}

void PurgeCommand::run(const PathMap& pathMap)
{
    const auto& config = context().config();
    auto& logger = context().logger();

    QStringList pathsToPurge;
    pathsToPurge.reserve(pathMap.size());
    for (auto it = pathMap.constBegin(); it != pathMap.constEnd(); it++)
    {
        const auto& path = it.key();
        const auto& dirDescr = it.value();

        if (!dirDescr.hasMarker())
            continue;

        if (!dirDescr.hasChildren())
            continue;

        if (pathsToPurge.isEmpty() || !path.startsWith(pathsToPurge.last()))
        {
            pathsToPurge << path;
            const auto nativePath = FileSystem::toQuotedNativePath(path);
            logger.log(QObject::tr("Found directory to purge: %1").arg(nativePath), LogLevel::INFO);
        }
    }

    foreach (const auto& path, pathsToPurge)
    {
        foreach (const auto& child, pathMap[path].children())
        {
            const auto childPath = child.canonicalFilePath();
            const auto nativeChildPath = FileSystem::toQuotedNativePath(childPath);
            if (child.isDir())
            {
                QDir childDir(childPath);
                if (config.dryRun() || childDir.removeRecursively())
                {
                    const auto logMsg = config.shortMessages()
                            ? nativeChildPath
                            : QObject::tr("Deleted directory: %1").arg(nativeChildPath);
                    logger.log(logMsg, LogLevel::INFO);
                }
                else
                {
                    logger.log(QObject::tr("Could not delete directory: %1 (%2)").arg(nativeChildPath).arg(QLatin1String(strerror(errno))),
                                 LogLevel::ERROR);
                }
            }
            else if (child.isFile())
            {
                QFile childFile(childPath);
                if (config.dryRun() || childFile.remove())
                {
                    const auto logMsg = config.shortMessages()
                            ? nativeChildPath
                            : QObject::tr("Deleted file: %1").arg(nativeChildPath);
                    logger.log(logMsg, LogLevel::INFO);
                }
                else
                {
                    logger.log(QObject::tr("Could not delete file: %1 (%2)").arg(nativeChildPath).arg(childFile.errorString()),
                                 LogLevel::ERROR);
                }
            }
        }
    }
}

}

}
