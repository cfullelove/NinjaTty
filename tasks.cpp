#include <iostream>
#include <fstream>
#include <Poco/NumberFormatter.h>
#include <Poco/Delegate.h>
#include "MQTTClient.h"
#include "tasks.h"

using Poco::Delegate;

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
	std::string *topicRead = app.getTopicRead();

	MQTTClient *mosq = app.getClient();

	while ( ! isCancelled() )
	{
		if ( ttyRead->rdbuf()->in_avail() > 0 )
		{
			std::getline( *ttyRead, line );

			if ( line.compare( 0, 37, "{\"DEVICE\":[{\"G\":\"0\",\"V\":0,\"D\":2,\"DA\":" ) == 0)
				continue;

			mosq->publish( *topicRead, line );
		}
	}

	logger.debug( "Finished Publish task" );
}

void SubTask::runTask()
{
	MQTTClient *mosq = app.getClient();		
	

	mosq->clientMessage += Delegate<SubTask, MQTTMessageEventArgs>( this, &SubTask::handleMessage );
	mosq->clientConnect += Delegate<SubTask, MQTTConnectEventArgs>( this, &SubTask::handleConnect );
	mosq->clientSubscribe += Delegate<SubTask, MQTTSubscribeEventArgs>( this, &SubTask::handleSubscribe );

	logger.information( "Starting MQTT Client");

	if ( *(app.getTtyFilename()) == "-" )
	{
		logger.debug( "Writing to stdout" );

		ttyWrite = &std::cout;
	}
	else
	{
		logger.debug( "Writing to : " + (*(app.getTtyFilename())) );
		ttyWrite = new std::ofstream( app.getTtyFilename()->c_str(), std::fstream::out );
	}

	int rv = mosq->connect();
	if ( rv > 0 )
	{
		logger.error( "Failed to connect " + Poco::NumberFormatter::format( rv ) );
	}
	else
	{
		logger.information( "Connection successful" );
		while ( ! isCancelled() )
		{
			rv = mosq->loop( 0 );
			if ( rv != MOSQ_ERR_SUCCESS )
			{
				logger.information( "Reconnecting..." );
				logger.debug( "loop() returned: " + Poco::NumberFormatter::formatHex( rv ) );
				rv = mosq->reconnect();
			}
		}

		logger.debug( "Loop finished" );
	}
	logger.debug( "Finished Subscribe task" );

}


void SubTask::handleMessage( const void* pSender, MQTTMessageEventArgs& eventArgs )
{
	logger.debug( "handleMessage Start" );
	char *payload;
    payload = (char *)(eventArgs.message)->payload;
	(*ttyWrite) << payload << std::endl;
	logger.debug( "handleMessage End" );
}

void SubTask::handleConnect( const void* pSender, MQTTConnectEventArgs& eventArgs )
{
	logger.debug( "handleConnect Start" );
	std::string *topicWrite = app.getTopicWrite();
	logger.information( "Subscribing to: " + *topicWrite );
	app.getClient()->subscribe( NULL, topicWrite->c_str() );
	logger.debug( "handleConnect End" );
}

void SubTask::handleSubscribe( const void* pSender, MQTTSubscribeEventArgs& eventArgs )
{
	logger.debug( "handleSubscribe" );
	logger.debug( "Subscription Successful" );
}
