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

#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QtTest>

#include <CodeMagic/TextFormatter.hpp>


using namespace CodeMagic;

class CodeMagic_TextFormatter_Test : public QObject
{
    Q_OBJECT

public:
    CodeMagic_TextFormatter_Test();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_format_1();
};

inline CodeMagic_TextFormatter_Test::CodeMagic_TextFormatter_Test()
{
}

inline void CodeMagic_TextFormatter_Test::initTestCase()
{
}

inline void CodeMagic_TextFormatter_Test::cleanupTestCase()
{
}

inline void CodeMagic_TextFormatter_Test::test_format_1()
{
    TextFormatter sut;
    sut.setFirstLineLeftIndent(2);
    sut.setFirstLineRightIndent(5);
    sut.setParagraphLeftIndent(4);
    sut.setParagraphRightIndent(10);
    sut.setMaxLineLength(30);

    QString text("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "sdfl skgjskjfghkd f djkfshgkdfskg kdfh kdf kdjfgh dk jfsdkfgh dkjfgh kdjfhgksjdh "
                 "kdsjfghk jhdfgjsdhfkgj hsdkfjgh ksjdhgksdjhg kjdhsgkj ksjd dkfgd,fmgldkfmgl kdmfg"
                 " dlfkgl kdfjlgk jdflgkjdl kfgdslkfg sdfgsdlökjfgl sdlkfgjl sdkfjgl sdkfgjldskfgj "
                 "lfkgjdl kjdflgkdjlgk jdlkfgjld jlfgj fwe.");

    qDebug() << QString("RESULT:\n%1").arg(sut.format(text));
}
