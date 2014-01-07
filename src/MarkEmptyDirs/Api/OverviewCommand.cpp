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

#include "Context.hpp"
#include "DirDescriptor.hpp"
#include "Logger.hpp"
#include "OverviewCommand.hpp"

#include <CodeMagic/FileSystem/FileSystemTools.hpp>
#include <CodeMagic/Text/TextTools.hpp>

#include <QStringList>


using namespace CodeMagic;

namespace MarkEmptyDirs
{

namespace Api
{

OverviewCommand::OverviewCommand()
{
}

void OverviewCommand::run(const PathMap& pathMap)
{
    QStringList paths(pathMap.keys());
    QStringList infos;
    infos.reserve(paths.size());

    qSort(paths);
    for (int i = 0; i < paths.length(); i++)
    {
        const auto& dirDescr = pathMap[paths[i]];

        const auto nativeDescrDirPath = FileSystem::toQuotedNativePath(paths[i]);
        const auto statistics = QObject::tr("[children: %1, marker: %2, subDirs: %3]")
                .arg(dirDescr.childCount(), 2)
                .arg(dirDescr.hasMarker() ? QObject::tr("yes") : QObject::tr("no"), 3)
                .arg(dirDescr.subDirCount(), 2);

        paths[i] = nativeDescrDirPath;
        infos << statistics;
    }

    Text::adjustToMaxLen(paths);
    const auto lines = Text::join(QList<QStringList>() << paths << infos, "  ");
    foreach (const auto& line, lines)
        context().logger().log(line, LogLevel::NONE);
}

}

}
