/******************************************************************************
作者：		何登锋
功能说明：
	事件客户端实现。
******************************************************************************/
#include "EventClient.h"
#include "EventBus.h"
#include "EventTarget.h"
#include "EventDefine.h"
#include <algorithm>



/******************************************************************************
作者: 何登锋
功能描述:
	订阅一条事件，实际上是在事件映射表中注册EvnetTarget对象和事件的对应关系。
参数说明:
返回值:
******************************************************************************/
bool EventClient::register_observer(EventType type, EventTarget &object)
{

	auto search = _event_object_map.find(type);
	if(search != _event_object_map.end())
	{
		_event_object_map.insert(EventObjectPair(type, &object));
		return true;
	}
	else
	{
		_event_object_map.insert(EventObjectPair(type, &object));
		int rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, type, strlen(type));
		if(rc != 0)
		{
			printf("register_observer failure\n");
			return false;
		}
		return true;
	}

}



/******************************************************************************
作者: 何登锋
功能描述:
	撤销对一条事件的订阅。
参数说明:
返回值:
******************************************************************************/
bool EventClient::unregister_observer(EventType type, const EventTarget &object)
{
	for(auto it = _event_object_map.begin(); it != _event_object_map.end();)
	{
		if ((strcmp(it->first, type)==0) && it->second==&object)
		{
			it = _event_object_map.erase(it);

			// 最后一个订阅事件的目标则zmq取消接收该事件
			auto search = _event_object_map.find(type);
			if(search == _event_object_map.end())
			{
				int rc = zmq_setsockopt(_sub_socket, ZMQ_UNSUBSCRIBE, type, strlen(type));
				if(rc != 0)
				{
					printf("register_observer failure\n");
					return false;
				}
			}
			return true;
			
		}
		else 
		{
			++it;
		}
	}

	
	return true;
}


/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::handle_event(void)
{

}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventClient::EventClient()
{
	// 先初始化成员，再注册到EventBus。这是因为如果先注册会出现某个线程在构造的时候，
	// 方法中会操作_msg_object_map，因此在这里必须先初始化成员。
	for(unsigned int i=0; i<EVENT_NR; i++)
	{
		_event_object_map[i].subscriber_number = 0;
		for(unsigned int j=0; j<SUBSCRIBER_NR; j++)
		{
			_event_object_map[i]._subscriber_nr[j] = NULL;
		}
	}

	EventBus *event_bus = EventBusSingleton::instance();

	_zmq_context = EventBus::ZmqContext;

	thread::id self_id= this_thread::get_id();
	printf("self_id = %p,%d, _zmq_context = %p \n",self_id,self_id,_zmq_context);


	//创建zmq的套接字，设置套接字为请求应答模式
	_sub_socket = zmq_socket(_zmq_context, ZMQ_SUB);
	assert(_sub_socket);

	int rc = zmq_connect(_sub_socket, event_bus->XSUB_ADDR_PORT);
	assert(rc == 0);

//	rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, "boy", 3);
//	assert(rc == 0);
//	rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, "girl", 4);
//	assert(rc == 0);

	_pub_socket = zmq_socket(_zmq_context,ZMQ_PUB);
	assert(_pub_socket);

	rc = zmq_connect(_pub_socket, event_bus->XPUB_ADDR_PORT);
	assert(rc == 0);


	if(!event_bus->register_client(this_thread::get_id(), *this))
	{
		printf("Register client to event bus is failed!\n");
	}
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventClient::~EventClient()
{

}







