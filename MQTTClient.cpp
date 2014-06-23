#include <mosquittopp.h>
#include "MQTTClient.h"

MQTTClient::MQTTClient( const char* id ) : mosquittopp( id )
{
	lib_init();
	log_init( MOSQ_LOG_ALL, MOSQ_LOG_STDOUT );
};

MQTTClient::~MQTTClient()
{
	lib_cleanup();
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
	return mosquittopp::publish( NULL, topic.c_str(), message.size(), (uint8_t*) message.c_str(),  0, false );
}

int MQTTClient::connect()
{
	return mosquittopp::connect( host.c_str() );
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
