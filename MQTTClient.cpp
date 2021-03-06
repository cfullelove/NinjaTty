#include <mosquittopp.h>
#include "MQTTClient.h"
#include <Poco/Mutex.h>

MQTTClient::MQTTClient( const char* id ) : mosquittopp( id )
{
	host = std::string( "" );
};

MQTTClient::~MQTTClient()
{
};

void MQTTClient::set_host(const std::string& name, const std::string& value)
{
	host = std::string( value );
}

void MQTTClient::set_host( const std::string value )
{
	host = std::string( value );
}

bool MQTTClient::connected()
{
	return _connected;
}

int MQTTClient::publish( std::string topic, std::string message )
{
	Poco::Mutex::ScopedLock lock( mutex );
	return mosquittopp::publish( NULL, topic.c_str(), message.size(), (uint8_t*) message.c_str(),  0, false );
}

int MQTTClient::connect()
{
	Poco::Mutex::ScopedLock lock( mutex );
	if ( host == "" )
		return false;

	return mosquittopp::connect( host.c_str() );
}

int MQTTClient::loop( int timeout = 1000 )
{
	Poco::Mutex::ScopedLock lock( mutex );
	return mosquittopp::loop( timeout );
}

void MQTTClient::on_connect( int rc )
{
	_connected = true;
	
    MQTTConnectEventArgs eventArgs( rc );
    clientConnect.notify( this, eventArgs );
};

void MQTTClient::on_disconnect()
{
	_connected = false;

	MQTTDisconnectEventArgs eventArgs;
	clientDisconnect.notify( this, eventArgs );

};

void MQTTClient::on_message( const struct mosquitto_message *message )
{

	MQTTMessageEventArgs eventArgs( message );

    clientMessage.notify( this, eventArgs );
};

void MQTTClient::on_publish(uint16_t mid)
{
	MQTTPublishEventArgs eventArgs( mid );

	clientPublish.notify( this, eventArgs );
};

void MQTTClient::on_subscribe( uint16_t mid, int qos_count, const uint8_t *granted_qos)
{
	MQTTSubscribeEventArgs eventArgs( mid, qos_count, granted_qos );

	clientSubscribe.notify( this, eventArgs );
};

void MQTTClient::on_unsubscribe(uint16_t mid)
{
	MQTTUnsubscribeEventArgs eventArgs;

	clientUnsubscribe.notify( this, eventArgs );
};

void MQTTClient::on_error()
{
	MQTTErrorEventArgs eventArgs;
	clientError.notify( this, eventArgs );
};
