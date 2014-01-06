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

#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QtTest>

#include <CodeMagic/Text/NumberTools.hpp>


using namespace CodeMagic::Text;

class CodeMagic_Text_NumberTools_Test : public QObject
{
    Q_OBJECT

public:
    CodeMagic_Text_NumberTools_Test();

private slots:
    void test_parse_bool_data();
    void test_parse_bool();
};

inline CodeMagic_Text_NumberTools_Test::CodeMagic_Text_NumberTools_Test()
{
}

inline void CodeMagic_Text_NumberTools_Test::test_parse_bool_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<bool>("okInit");
    QTest::addColumn<bool>("okExpected");
    QTest::addColumn<bool>("resultExpected");

    QTest::newRow("ok.false.01") << "0" << false << true << false;
    QTest::newRow("ok.false.02") << "0 " << false << true << false;
    QTest::newRow("ok.false.03") << " 0" << false << true << false;
    QTest::newRow("ok.false.04") << " 0x0 " << false << true << false;
    QTest::newRow("ok.false.05") << " fAlSe " << false << true << false;
    QTest::newRow("ok.false.06") << " No " << false << true << false;
    QTest::newRow("ok.false.07") << " oFf " << false << true << false;

    QTest::newRow("ok.true.01") << "1" << false << true << true;
    QTest::newRow("ok.true.02") << "1 " << false << true << true;
    QTest::newRow("ok.true.03") << " 1" << false << true << true;
    QTest::newRow("ok.true.04") << " 0x1 " << false << true << true;
    QTest::newRow("ok.true.05") << " tRuE " << false << true << true;
    QTest::newRow("ok.true.06") << " yEs " << false << true << true;
    QTest::newRow("ok.true.07") << " oN " << false << true << true;

    QTest::newRow("!ok.true.01") << " -1 " << true << false << true;
    QTest::newRow("!ok.true.02") << " 2 " << true << false << true;
    QTest::newRow("!ok.true.03") << " 0x2 " << true << false << true;

    QTest::newRow("!ok.false.01") << "" << true << false << false;
    QTest::newRow("!ok.false.02") << " " << true << false << false;
    QTest::newRow("!ok.false.07") << "0x" << true << false << false;
    QTest::newRow("!ok.false.08") << "1y" << true << false << false;
    QTest::newRow("!ok.false.09") << "0y" << true << false << false;
    QTest::newRow("!ok.false.10") << "y1" << true << false << false;
    QTest::newRow("!ok.false.11") << "y0" << true << false << false;
    QTest::newRow("!ok.false.12") << "0 x" << true << false << false;
    QTest::newRow("!ok.false.13") << "1 x" << true << false << false;
    QTest::newRow("!ok.false.14") << "true x" << true << false << false;
    QTest::newRow("!ok.false.15") << "false x" << true << false << false;
}

inline void CodeMagic_Text_NumberTools_Test::test_parse_bool()
{
    QFETCH(QString, string);
    QFETCH(bool, okInit);
    QFETCH(bool, okExpected);
    QFETCH(bool, resultExpected);

    bool ok = okInit;
    auto result = parse<bool>(string, &ok);

    QCOMPARE(ok, okExpected);
    QCOMPARE(result, resultExpected);
}
