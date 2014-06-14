#include <Poco/Util/Application.h>
#include <mosquittopp.h>
#include "mqtt_client.h"

using Poco::Util::Application;

void mqtt_client::set_host(const std::string& name, const std::string& value)
{
	this->host = std::string( value );
}

void mqtt_client::set_topic_base(const std::string& name, const std::string& value)
{
	this->topic_base = std::string( value );
	this->read_topic = std::string( value + "/read" );
	this->write_topic = std::string( value + "/write" );
}

bool mqtt_client::connected()
{
	return _connected;
}

int mqtt_client::publish( std::string& message )
{
	return mosquittopp::publish( NULL, read_topic.c_str(), message.size(), (uint8_t*) message.c_str(),  0, false );
}

int mqtt_client::connect()
{
	return mosquittopp::connect( host.c_str() );
}

void mqtt_client::on_connect( int rc )
{
	_connected = true;
	Application::instance().logger().information( "MQTT Client Connected" );
    subscribe( NULL, write_topic.c_str() );
};

void mqtt_client::on_message( const struct mosquitto_message *message )
{
        char *payload;
        payload = (char *)message->payload;
        messageCallback->invoke( payload );
};

void mqtt_client::setMessageHandler( MessageCallbackBase& callback )
{
	messageCallback = &callback;
};

void mqtt_client::on_subscribe( uint16_t mid, int qos_count, const uint8_t *granted_qos)
{
    Application::instance().logger().information( "Subscription Successful" );
};