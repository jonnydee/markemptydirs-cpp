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

#include <QMarkEmptyDirsAPI/CommandLineInterface.hpp>
#include <QMarkEmptyDirsAPI/Config.hpp>
#include <QMarkEmptyDirsAPI/CleanCommand.hpp>
#include <QMarkEmptyDirsAPI/HelpCommand.hpp>
#include <QMarkEmptyDirsAPI/OverviewCommand.hpp>
#include <QMarkEmptyDirsAPI/UpdateCommand.hpp>

#include <QCoreApplication>
#include <QDebug>

using namespace MarkEmptyDirs::Api;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CommandLineInterface cli;

    const auto config = cli.createConfig(app.arguments());

    std::unique_ptr<ICommand> pCmd;
    switch (config.command())
    {
    case Config::CLEAN:
        pCmd.reset(new CleanCommand);
        break;
    case Config::UPDATE:
        pCmd.reset(new UpdateCommand);
        break;
    case Config::OVERVIEW:
        pCmd.reset(new OverviewCommand);
        break;
    case Config::HELP:
    default:
        pCmd.reset(new HelpCommand(cli));
        break;
    }

    pCmd->init(config);
    pCmd->run();

    return 0;
//    return app.exec();
}
