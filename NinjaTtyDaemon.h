#include <iostream>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Process.h>
#include "MQTTClient.h"

#ifndef NTPS_NinjaTtyDeamon_included
#define NTPS_NinjaTtyDeamon_included

using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

class NinjaTtyDaemon : public ServerApplication
{

public:
	NinjaTtyDaemon( const char* _clientId ):
		_helpRequested( false ),
		mosq( _clientId ),
		topicBase( "" )
	{
	}

	~NinjaTtyDaemon()
	{
	}

	MQTTClient& getClient()
	{
		return mosq;
	}

	std::string* getTtyFilename()
	{
		return ttyFilename;
	}

	std::string* getTopicRead()
	{
		return new std::string( topicBase + "/read" );
	}

	std::string* getTopicWrite()
	{
		return new std::string( topicBase + "/write" );
	}

protected:

	void initialize(Application& self);

	void uninitialize();

	void handleHelp(const std::string& name, const std::string& value);

	void defineOptions(OptionSet& options);

	void set_filename(const std::string& name, const std::string& value);
	void set_topic_base(const std::string& name, const std::string& value);

	void displayHelp();

	int main(const std::vector<std::string>& args);

private:
	bool _helpRequested;

	MQTTClient mosq;
	std::string topicBase;

	std::string *ttyFilename;

};

#endif