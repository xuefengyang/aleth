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

namespace po = boost::program_options;

using namespace dev;
using namespace dev::p2p;

namespace
{
unsigned const c_lineWidth = 160;
}

int main(int argc, char** argv)
{
    LoggingOptions loggingOptions;
     po::options_description loggingProgramOptions(
        createLoggingProgramOptions(c_lineWidth, loggingOptions));
    
    Host h("aleth-bootnode");

    return 0;
}
