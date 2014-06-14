#include <Poco/Task.h>
#include "NinjaTtyDaemon.h"

#ifndef NTPS_tasks_included
#define NTPS_tasks_included

using Poco::Task;

class PubTask : public Task
{

public:
	PubTask( NinjaTtyDaemon& app ) : Task( "PubTask" ), logger( app.logger() ), app( app )
	{
	}

	void runTask();

private:
	Poco::Logger& logger;
	NinjaTtyDaemon& app;

};

class SubTask : public Task
{

public:
	SubTask( NinjaTtyDaemon& app ) : Task( "SubTask" ), logger( app.logger() ), app( app )
	{
	}

	void runTask();

	void handleMessage( char* message );

private:
	//std::fstream ttyWrite;
	Poco::Logger& logger;
	NinjaTtyDaemon& app;
	std::ostream* ttyWrite;
};

#endif