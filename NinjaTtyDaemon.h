#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include "mqtt_client.h"

#ifndef NTPS_NinjaTtyDeamon_included
#define NTPS_NinjaTtyDeamon_included

using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

class NinjaTtyDaemon : public ServerApplication
{

public:
	NinjaTtyDaemon(): _helpRequested( false ), mosq( "ninjatty" )
	{
	}

	~NinjaTtyDaemon()
	{
	}

	mqtt_client& getClient()
	{
		return mosq;
	}

	std::string* getTtyFilename()
	{
		return ttyFilename;
	}

protected:

	void initialize(Application& self);

	void uninitialize();

	void handleHelp(const std::string& name, const std::string& value);

	void defineOptions(OptionSet& options);

	void set_filename(const std::string& name, const std::string& value);

	void displayHelp();

	int main(const std::vector<std::string>& args);

private:
	bool _helpRequested;

	mqtt_client mosq;

	std::string *ttyFilename;
};

#endif