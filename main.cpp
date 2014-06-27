#include <iostream>
#include <Poco/Process.h>
#include "NinjaTtyDaemon.h"
#include "MQTTClient.h"

int main(int argc, char** argv)
{
	std::string clientId( "ninjatty_" + Poco::Process::id() );
    NinjaTtyDaemon app( clientId.c_str() );
	return app.run(argc, argv);
}