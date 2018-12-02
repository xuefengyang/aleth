/*
    This file is part of aleth.

    aleth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    aleth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with aleth.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <libdevcore/LoggingProgramOptions.h>
#include <libp2p/Host.h>
#include <iostream>
#include <thread>

namespace po = boost::program_options;

using namespace dev;
using namespace dev::p2p;
using namespace std;

namespace
{
unsigned const c_lineWidth = 160;

class ExitHandler
{
public:
    static void exitHandler(int) { s_shouldExit = true; }
    bool shouldExit() const { return s_shouldExit; }

private:
    static bool s_shouldExit;
};

bool ExitHandler::s_shouldExit = false;
}

int main(int argc, char** argv)
{
    setDefaultOrCLocale();

    po::options_description generalOptions("GENERAL OPTIONS", c_lineWidth);
    auto addGeneralOption = generalOptions.add_options();
    addGeneralOption("help,h", "Show this help message and exit\n");

    LoggingOptions loggingOptions;
     po::options_description loggingProgramOptions(
        createLoggingProgramOptions(c_lineWidth, loggingOptions));
    
    po::options_description allowedOptions("Allowed options");
    allowedOptions.add(generalOptions);
    allowedOptions.add(loggingProgramOptions);
    po::variables_map vm;
    try
    {
        po::parsed_options parsed = po::parse_command_line(argc, argv, allowedOptions);
        po::store(parsed, vm);
        po::notify(vm);
    }
    catch (po::error const& e)
    {
        cout << e.what() << endl;
        return -1;
    }
    
    if (vm.count("help"))
    {
        cout << "NAME:\n"
             << "   aleth-bootnode\n"
             << "USAGE:\n"
             << "   aleth-bootnode [options]\n\n";
        cout << generalOptions << loggingProgramOptions;
        return 0;
    }

    setupLogging(loggingOptions);

    if (loggingOptions.verbosity > 0)
        cout << EthGrayBold "aleth-bootnode, a C++ Ethereum bootnode implementation" EthReset << "\n";

    Host h("aleth-bootnode");
    h.start();

    cout << "Node ID: " << h.enode() << endl;

    ExitHandler exitHandler;
    signal(SIGTERM, &ExitHandler::exitHandler);
    signal(SIGABRT, &ExitHandler::exitHandler);
    signal(SIGINT, &ExitHandler::exitHandler);
   
    while (!exitHandler.shouldExit())
        this_thread::sleep_for(chrono::milliseconds(1000));

    h.stop();

    return 0;
}
