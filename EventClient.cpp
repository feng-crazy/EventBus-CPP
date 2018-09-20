/******************************************************************************
作者：		何登锋
功能说明：
	事件客户端实现。
******************************************************************************/
#include <algorithm>
#include "EventClient.h"
#include "EventBus.h"
#include "EventTarget.h"
#include "EventDefine.h"
#include "MZmq.h"



/******************************************************************************
作者: 何登锋
功能描述:
	订阅一条事件，实际上是在事件映射表中注册EvnetTarget对象和事件的对应关系。
参数说明:
返回值:
******************************************************************************/
bool EventClient::register_observer(EventType type, EventTarget &object)
{

	EventObjectMap::iterator it = _event_object_map.find(type);
	if(it != _event_object_map.end())
	{
		// 判断是否已经订阅
		for(;it != _event_object_map.end();it++)
		{
			if ((type.compare (it->first)==0) && it->second==&object)
				return true;
		}
		_event_object_map.insert(EventObjectPair(type, &object));
		return true;
	}
	else
	{
		_event_object_map.insert(EventObjectPair(type, &object));
		int rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, type.c_str (), type.length ());
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
		if ((type.compare(it->first)==0) && it->second==&object)
		{
			it = _event_object_map.erase(it);

			// 最后一个订阅事件的目标则zmq取消接收该事件
			auto search = _event_object_map.find(type);
			if(search == _event_object_map.end())
			{
				int rc = zmq_setsockopt(_sub_socket, ZMQ_UNSUBSCRIBE, type.c_str (), type.length ());
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
void EventClient::publish_event (EventType type, EventContent content)
{
	unsigned char *data = NULL;
	if(!content.empty())
	{
		data = &content[0];
	}

	MZmq::SendEventEntity(_pub_socket, type, data, content.size());
}

/******************************************************************************
作者：何登锋
功能描述：发布一个事件到本地客户端，不到EventBus上，在本客户端的订阅者将会直接调用
参数说明：
返回值：
******************************************************************************/
void EventClient::publish_loc_event (EventType type, EventContent content)
{
	auto search = _event_object_map.find(type);
	for(; search != _event_object_map.end ();++search)
	{
		search->second->event_handle(type, content);
	}
}


/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::handle_event(void)
{
	zmq_poll(&sub_items, 1, 0);

	if (sub_items.revents & ZMQ_POLLIN)
	{
		string title;
		vector<unsigned char> content;
		MZmq::RecvEventEntity (_sub_socket, title, content);

		auto search = _event_object_map.find(title);
		for(; search != _event_object_map.end ();++search)
		{
			search->second->event_handle(title, content);
		}
	}
}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventClient::EventClient()
{

	EventBus *event_bus = EventBusSingleton::instance();

	_zmq_context = EventBus::ZmqContext;

	thread::id self_id= this_thread::get_id();
	printf("self_id = %p,%d, _zmq_context = %p \n",self_id,self_id,_zmq_context);


	//创建zmq的套接字，设置套接字为请求应答模式
	_sub_socket = zmq_socket(_zmq_context, ZMQ_SUB);
	assert(_sub_socket);

	int rc = zmq_connect(_sub_socket, event_bus->XSUB_ADDR_PORT);
	assert(rc == 0);


	_pub_socket = zmq_socket(_zmq_context, ZMQ_PUB);
	assert(_pub_socket);

	rc = zmq_connect(_pub_socket, event_bus->XPUB_ADDR_PORT);
	assert(rc == 0);

//	sub_items = { _sub_socket, 0, ZMQ_POLLIN, 0 };
	sub_items.socket = _sub_socket;
	sub_items.fd = 0;
	sub_items.events = ZMQ_POLLIN;
	sub_items.revents = 0;


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








