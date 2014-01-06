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
#include "FileSystemCrawler.hpp"
#include "Logger.hpp"

#include <QDebug>
#include <QLinkedList>
#include <QFileInfo>
#include <QObject>


namespace MarkEmptyDirs
{

namespace Api
{

FileSystemCrawler::FileSystemCrawler(QObject *parent)
    : QObject(parent)
    , m_pContext(nullptr)
{
}

FileSystemCrawler::~FileSystemCrawler()
{
}

FileSystemCrawler::PathMap FileSystemCrawler::pathMap() const
{
    return m_pathMap;
}

void FileSystemCrawler::setContext(Context& ctx)
{
    m_pContext = &ctx;
}

Context& FileSystemCrawler::context()
{
    Q_ASSERT(m_pContext);
    return *m_pContext;
}

const Context& FileSystemCrawler::context() const
{
    Q_ASSERT(m_pContext);
    return *m_pContext;
}

bool FileSystemCrawler::isDirExcluded(const QDir& dir) const
{
    const auto& config = context().config();

    foreach (const auto& excludeDir, config.excludeDirs())
    {
        auto exPath = excludeDir.dirName();
        auto dirPath = dir.dirName();
        if (excludeDir.dirName() == dir.dirName())
            return true;
    }
    return false;
}

void FileSystemCrawler::run()
{
    const auto& config = context().config();
    auto& logger = context().logger();

    const QDir::Filters filter = QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot;

    QLinkedList<QDir> dirQueue;
    foreach (const auto& dir, config.rootDirs())
        dirQueue.push_back(dir);

    while (!dirQueue.isEmpty())
    {
        const auto dir = dirQueue.takeFirst();
        if (!dir.exists())
        {
            logger.log(QString("'%1': %2").arg(dir.absolutePath()).arg(QObject::tr("Not a directory, skipping it.")), LogLevel::WARNING);
            continue;
        }

        const auto dirPath = dir.canonicalPath();

        if (m_pathMap.contains(dirPath))
            continue;

        DirDescriptor dirDescr;
        dirDescr.setDir(dir);

        const auto children = dir.entryInfoList(filter);
        foreach (const auto child, children)
        {
            if ((config.dereferenceSymLinks() || !child.isSymLink()) && child.isDir())
            {
                const auto childPath = child.absoluteFilePath();
                const QDir subDir(childPath);
                if (isDirExcluded(subDir))
                    continue;

                dirQueue.append(subDir);
            }
            else if (child.fileName() == config.markerName() && child.isFile())
            {
                dirDescr.setHasMarker();
                continue;
            }

            dirDescr.children() << child;
        }

        m_pathMap[dirPath] = dirDescr;

        if (config.shortMessages())
        {
            logger.log(QObject::tr("Visited directory: '%1'")
                         .arg(dirDescr.dir().canonicalPath()),
                         LogLevel::DEBUG);
        }
        else
        {
            logger.log(QObject::tr("Visited directory: '%1' [children: %2, marker: %3]")
                         .arg(dirDescr.dir().canonicalPath())
                         .arg(dirDescr.childCount())
                         .arg(dirDescr.hasMarker() ? QObject::tr("yes") : QObject::tr("no")),
                         LogLevel::DEBUG);
        }
    }
}

}

}
