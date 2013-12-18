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

#include <QRegExp>
#include <QString>
#include <QtTest>

#include <StringTemplate/Engine.hpp>
#include <StringTemplate/Variable.hpp>
#include <StringTemplate/VariableFactory.hpp>

#define GUID_PATTERN        "\\{[a-fA-F\\d]{8}-[a-fA-F\\d]{4}-[a-fA-F\\d]{4}-[a-fA-F\\d]{4}-[a-fA-F\\d]{12}\\}"

#define DATE_PATTERN        "\\d{4}-\\d{2}-\\d{2}"
#define TIME_PATTERN        "\\d{2}\\:\\d{2}\\:\\d{2}"
#define DATETIME_PATTERN    DATE_PATTERN "T" TIME_PATTERN "Z"


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
    void test_Variable_datetime_expand();
    void test_Variable_env_expand();
    void test_Variable_guid_expand();
    void test_Variable_lf_expand();
    void test_Variable_sp_expand();

    void test_Engine_process();
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

    QString text("This is a§var: test case§ with a §var § and an §var :§ variable.");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = sut.expand(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 3);
    QVERIFY(text == "This is a test case with a null and an empty variable.");
}

void StringTemplateTest::test_Variable_datetime_expand()
{
    std::unique_ptr<const Variable> pSut(VariableFactory().createDateTimeVariable());

    QString text("Now we have §datetime§, or shorter §datetime:yyyy-MM-dd§, or wrong §datetime:§.");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = pSut->expand(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 2);
    const QRegExp timeRegExp("Now we have " DATETIME_PATTERN ", or shorter " DATE_PATTERN ", or wrong §datetime:§.");
    QVERIFY(timeRegExp.indexIn(text) == 0);
}

void StringTemplateTest::test_Variable_env_expand()
{
    std::unique_ptr<const Variable> pSut(VariableFactory().createEnvironmentVariable());

    qputenv("STRINGTEMPLATE_VARIABLE_ENV", "environment");

    QString text("This is a §env : STRINGTEMPLATE_VARIABLE_ENV § variable.");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = pSut->expand(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 1);
    QVERIFY(text == "This is a environment variable.");
}

void StringTemplateTest::test_Variable_guid_expand()
{
    std::unique_ptr<const Variable> pSut(VariableFactory().createGuidVariable());

    QString text("This is a §guid§ variable.");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = pSut->expand(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 1);
    const QRegExp guidRegExp("This is a " GUID_PATTERN " variable.");
    QVERIFY(guidRegExp.indexIn(text) == 0);
}

void StringTemplateTest::test_Variable_lf_expand()
{
    std::unique_ptr<const Variable> pSut(VariableFactory().createLinefeedVariable());

    QString text("Line 1§lf§Line 2§lf:2§Line 4§lf :  §.");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = pSut->expand(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 2);
    QVERIFY(text == "Line 1\nLine 2\n\nLine 4§lf :  §.");
}

void StringTemplateTest::test_Variable_sp_expand()
{
    std::unique_ptr<const Variable> pSut(VariableFactory().createSpaceVariable());

    QString text("123§sp§56§sp:2§9§sp :  §.");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = pSut->expand(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 2);
    QVERIFY(text == "123 56  9§sp :  §.");
}

void StringTemplateTest::test_Engine_process()
{
    VariableFactory factory;
    std::unique_ptr<Variable> pDateTimeVariable(factory.createDateTimeVariable());
    std::unique_ptr<Variable> pEnvVariable(factory.createEnvironmentVariable());
    std::unique_ptr<Variable> pGuidVariable(factory.createGuidVariable());
    std::unique_ptr<Variable> pLfVariable(factory.createLinefeedVariable());
    std::unique_ptr<Variable> pSpVariable(factory.createSpaceVariable());

    qputenv("STRINGTEMPLATE_AUTHOR", "Jonny Dee");

    Engine sut;
    sut.addVariable(*pDateTimeVariable);
    sut.addVariable(*pEnvVariable);
    sut.addVariable(*pGuidVariable);
    sut.addVariable(*pLfVariable);
    sut.addVariable(*pSpVariable);

    QString text("Id: §guid§§lf:2§§sp:4§Created on: §datetime:yyyy-MM-dd§§lf§§sp:4§Created by: §env:STRINGTEMPLATE_AUTHOR§§lf:2§§sp:4§File: .emptydir");
    qDebug() << "ORIGINAL:" << text;
    const auto expansionCount = sut.process(text);
    qDebug() << QString("EXPANDED (%1x):").arg(expansionCount) << text;

    QVERIFY(expansionCount == 9);
    const QRegExp guidRegExp("Id: " GUID_PATTERN "\n\n    Created on: " DATE_PATTERN "\n    Created by: Jonny Dee\n\n    File: .emptydir");
    QVERIFY(guidRegExp.indexIn(text) == 0);
}

QTEST_APPLESS_MAIN(StringTemplateTest)

#include "test_StringTemplate.moc"
