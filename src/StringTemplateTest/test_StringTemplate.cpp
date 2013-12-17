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

#include <QString>
#include <QtTest>

#include <StringTemplate/Variable.hpp>

using namespace StringTemplate;

class StringTemplateTest : public QObject
{
    Q_OBJECT

public:
    StringTemplateTest();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_Variable_custom_expand();
};

StringTemplateTest::StringTemplateTest()
{
}

void StringTemplateTest::initTestCase()
{
}

void StringTemplateTest::cleanupTestCase()
{
}

void StringTemplateTest::test_Variable_custom_expand()
{
    const Variable sut("var",
        [](const Variable::Context& ctx) -> QString
        {
            if (1 == ctx.count)
                return ctx.argument;
            else if (ctx.argument.isNull())
                return QString("null");
            else if (ctx.argument.isEmpty())
                return QString("empty");
            else
                return QString("FAIL");
        });

    QString text("This is a §var:test case§ with a §var§ and an §var:§ variable.");
    qDebug() << "ORIGINAL:" << text;
    sut.expand(text);
    qDebug() << "EXPANDED:" << text;

    QVERIFY2(text == "This is a test case with a null and an empty variable.", "Failure");
}

QTEST_APPLESS_MAIN(StringTemplateTest)

#include "test_StringTemplate.moc"
