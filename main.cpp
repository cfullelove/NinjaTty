#include <iostream>
#include <Poco/Process.h>
#include "NinjaTtyDaemon.h"
#include "MQTTClient.h"

int main(int argc, char** argv)
{
	std::string clientId( "ninjatty" );
	NinjaTtyDaemon app( clientId );
	return app.run(argc, argv);
}
