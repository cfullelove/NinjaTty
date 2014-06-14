#include <iostream>
#include <fstream>
#include <Poco/NumberFormatter.h>
#include "mqtt_client.h"
#include "tasks.h"


void PubTask::runTask()
{

	std::istream* ttyRead;

	if ( *(app.getTtyFilename()) == "-" )
	{
		logger.information( "Publishing from stdin" );

		ttyRead = &std::cin;
	}
	else
	{
		logger.information( "Publishing from : " + (*(app.getTtyFilename())) );
		ttyRead = new std::ifstream( app.getTtyFilename()->c_str(), std::fstream::in );
	}

	std::string line;
	
	mqtt_client& mosq = app.getClient();

	while ( ! isCancelled() )
	{
		std::getline( *ttyRead, line );

		if ( line.compare( 0, 37, "{\"DEVICE\":[{\"G\":\"0\",\"V\":0,\"D\":2,\"DA\":" ) == 0)
			continue;

		mosq.publish( line );
	}
}

void SubTask::runTask()
{
	mqtt_client& mosq = app.getClient();		
	
	MessageCallback<SubTask> onMessage( this, &SubTask::handleMessage );
	mosq.setMessageHandler( onMessage );

	logger.information( "Starting MQTT Client");

	if ( *(app.getTtyFilename()) == "-" )
	{
		logger.information( "Writing to stdin" );

		ttyWrite = &std::cout;
	}
	else
	{
		logger.information( "Writing to : " + (*(app.getTtyFilename())) );
		ttyWrite = new std::ofstream( app.getTtyFilename()->c_str(), std::fstream::out );
	}

	int rv = mosq.connect();
	if ( rv > 0 )
	{
		logger.error( "Failed to connect " + Poco::NumberFormatter::formatHex( rv ) );
	}
	else
	{
		logger.information( "Connection successful" );
		while ( ! isCancelled() )
		{
			rv = mosq.loop( -1 );
			if ( rv != MOSQ_ERR_SUCCESS )
			{
				mosq.reconnect();
			}
		}

		logger.information( "Loop finished" );
	}

}

void SubTask::handleMessage( char* message )
{
	(*ttyWrite) << message << std::endl;
}