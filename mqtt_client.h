#include <mosquittopp.h>
#include <iostream>

#ifndef NTPS_mqtt_client_included
#define NTPS_mqtt_client_included

class MessageCallbackBase
{
public:
	virtual void invoke( char* message ) {};
};

template <class C>
class MessageCallback : public MessageCallbackBase
{

public:
	typedef void (C::*Callback)( char* message );

	MessageCallback( C* object, Callback method ): _object( object ), _method( method )
	{		
	}

	void invoke( char* message )
	{
		(_object->*_method)( message );
	}

private:
	C* _object;
	Callback _method;
};

class mqtt_client : public mosquittopp::mosquittopp
{

public:
	

	mqtt_client( const char* id ) : mosquittopp( id )
	{
	};

	void set_host(const std::string& name, const std::string& value);

	void set_topic_base(const std::string& name, const std::string& value);

	bool connected();

	int publish( std::string& message );

	int connect();

	void on_connect( int rc );

	void on_message( const struct mosquitto_message *message );

	void setMessageHandler( MessageCallbackBase& callback );

	void on_subscribe( uint16_t mid, int qos_count, const uint8_t *granted_qos);

private:
    std::string ttyFilename;
    std::string host;
    std::string topic_base;
    std::string read_topic;
    std::string write_topic;

    bool _connected;

    MessageCallbackBase* messageCallback;

};

#endif