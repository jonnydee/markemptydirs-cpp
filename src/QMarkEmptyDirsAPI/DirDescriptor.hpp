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

#ifndef DIRDESCRIPTOR_HPP
#define DIRDESCRIPTOR_HPP

#include "qmarkemptydirsapi_global.hpp"

#include <QDir>
#include <QString>


namespace MarkEmptyDirs
{

namespace Api
{

class QMARKEMPTYDIRSAPISHARED_EXPORT DirDescriptor
{
public:
    DirDescriptor()
        : m_dir()
        , m_childCount(0)
        , m_hasMarker(false)
        , m_subDirCount(0)
    {
    }

    void setDir(const QDir& dir) { m_dir = dir; }
    QDir dir() const { return m_dir; }

    void incChildCount() { ++m_childCount; }
    int childCount() const { return m_childCount; }
    bool hasChildren() const { return m_childCount > 0; }

    void setHasMarker() { m_hasMarker = true; }
    bool hasMarker() const { return m_hasMarker; }

    void incSubDirCount() { ++m_subDirCount; }
    int subDirCount() const { return m_subDirCount; }
    bool hasSubDirs() const { return m_subDirCount > 0; }

private:
    QDir m_dir;
    int m_childCount;
    bool m_hasMarker;
    int m_subDirCount;
};

}

}

#endif // DIRDESCRIPTOR_HPP
