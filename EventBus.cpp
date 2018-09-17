/******************************************************************************
作者：		何登锋
功能说明:

******************************************************************************/
#include "EventBus.h"
#include "EventClient.h"

#include "zmq/zmq.h"
//const char* EventBus::MESSAGE_CENTER_ENDPOINT = "inproc://message_center";

void* EventBus::ZmqContext = zmq_ctx_new();


/**************************************************************************
作者: 何登锋
功能描述:
	注册消息客户系统。
参数说明:
返回值:
**************************************************************************/
bool EventBus::register_client(thread::id id, EventClient &client)
{
	pair<MsgCenterMap::iterator, bool> ret;

	lock_guard<std::recursive_mutex> lock(_mutex);

	ret = _client_pool.insert(MsgCenterPair(id, &client));

	return ret.second;
}



/**************************************************************************
作者: 何登锋
功能描述:
	返回消息客户系统。
参数说明:
返回值:
**************************************************************************/
EventClient *EventBus::find_client(thread::id id)
{

	MsgCenterMap::iterator it;

	lock_guard<std::recursive_mutex> lock(_mutex);

	it = _client_pool.find(id);

	if(it == _client_pool.end())
	{
		return NULL;
	}

	return (*it).second;
}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventBus::_thread_run(void *arg)
{
	EventBus *self = reinterpret_cast<EventBus*>(arg);
	zmq_proxy(self->_xsub_socket, self->_xpub_socket, NULL);
}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventBus::EventBus()
{
	assert(ZmqContext);
	_xpub_socket = zmq_socket(ZmqContext, ZMQ_XPUB);
	assert(_xpub_socket);
	int rc = zmq_bind(_xpub_socket, XPUB_ADDR_PORT);
	assert(rc==0);

	_xsub_socket = zmq_socket(ZmqContext, ZMQ_XSUB);
	assert(_xsub_socket);
	rc = zmq_bind(_xsub_socket, XSUB_ADDR_PORT);
	assert(rc==0);


	_bus_proxy_thread = new thread(_thread_run, this);
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventBus::~EventBus()
{
	// 析够所有的消息客户系统。
	MsgCenterMap::iterator it = _client_pool.begin();
	for(; it != _client_pool.end(); ++it)
	{
		delete (*it).second;
	}
}

