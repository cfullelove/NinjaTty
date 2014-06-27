#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/TaskManager.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Format.h>
#include <Poco/AutoPtr.h>
#include <Poco/SyslogChannel.h>
#include "NinjaTtyDaemon.h"
#include "tasks.h"


using Poco::Util::Application;
using Poco::Util::ServerApplication;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::SyslogChannel;
using Poco::AutoPtr;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;



void NinjaTtyDaemon::initialize(Application& self)
{

	loadConfiguration();  // load default configuration files, if present

	ServerApplication::initialize(self);

	AutoPtr<SyslogChannel> pSC= new SyslogChannel( commandName() );
	logger().setChannel( pSC );

	logger().information("Starting up");

}

void NinjaTtyDaemon::uninitialize()
{
	logger().information("Shutting down");
	ServerApplication::uninitialize();
}

void NinjaTtyDaemon::handleHelp(const std::string& name, const std::string& value)
{
	_helpRequested = true;
	displayHelp();
	stopOptionsProcessing();
}

void NinjaTtyDaemon::defineOptions(OptionSet& options)
{
	ServerApplication::defineOptions(options);

	options.addOption(
		Option("help", "", "display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<NinjaTtyDaemon>(this, &NinjaTtyDaemon::handleHelp)));

	options.addOption(
		Option( "host", "h", "hostname" )
			.required( true )
			.repeatable( false )
			.argument( "host" )
			.callback( OptionCallback<MQTTClient>(&mosq, &MQTTClient::set_host ) ) );

	options.addOption(
		Option( "topic", "t", "base topic" )
			.required( true )
			.repeatable( false )
			.argument( "topic" )
			.callback( OptionCallback<NinjaTtyDaemon>(this, &NinjaTtyDaemon::set_topic_base ) ) );

	options.addOption(
		Option( "file", "f", "tty filename" )
			.required( true )
			.repeatable( false )
			.argument( "file" )
			.callback( OptionCallback<NinjaTtyDaemon>(this, &NinjaTtyDaemon::set_filename ) ) );
}

void NinjaTtyDaemon::set_filename(const std::string& name, const std::string& value)
{
	ttyFilename = new std::string( value );
}

void NinjaTtyDaemon::set_topic_base(const std::string& name, const std::string& value)
{
	topicBase = std::string( value );
}

void NinjaTtyDaemon::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.format(std::cout);
}

int NinjaTtyDaemon::main(const std::vector<std::string>& args)
{
	if (!_helpRequested)
	{
		TaskManager tm;
		tm.start(new SubTask( *this ) );
		tm.start(new PubTask( *this ) );
		waitForTerminationRequest();
		tm.cancelAll();
		tm.joinAll();
	}
	return Application::EXIT_OK;
}

