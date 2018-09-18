/******************************************************************************
作者：		何登锋
功能说明：
	消息客户端实现。
******************************************************************************/
#include "EventClient.h"
#include "EventBus.h"
#include "EventTarget.h"
#include <algorithm>



/******************************************************************************
作者: 何登锋
功能描述:
	订阅一条消息，实际上是在消息映射表中注册CmdTarget对象和消息的对应关系。
参数说明:
返回值:
******************************************************************************/
bool EventClient::register_observer(Message id, EventTarget &object)
{
	if(id >= EVENT_NR)
	{
		return false;
	}

	EventTarget *cmd_target = NULL;

	// 确定该对象没有订阅该消息。
	for(unsigned int i=0; i<SUBSCRIBER_NR; i++)
	{
		cmd_target = _event_object_map[id]._subscriber_nr[i];
		if((cmd_target != NULL) && (*cmd_target == object))
		{
			return false;
		}
	}

	for(unsigned int i=0; i<SUBSCRIBER_NR; i++)
	{
		cmd_target = _event_object_map[id]._subscriber_nr[i];
		if(cmd_target == NULL)
		{
			_event_object_map[id]._subscriber_nr[i] = &object;
			_event_object_map[id].subscriber_number++;
			return true;
		}
	}

	// 订阅者已经满了。
	printf("register_observer failed, and msgid = %d\n", id);
//	stack_trace();
	return false;
}



/******************************************************************************
作者: 何登锋
功能描述:
	撤销对一条消息的订阅。
参数说明:
返回值:
******************************************************************************/
bool EventClient::unregister_observer(Message id, const EventTarget &object)
{
	if(id >= EVENT_NR)
	{
		return false;
	}

	if(_event_object_map[id].subscriber_number == 0)
	{
		return false;
	}

	EventTarget *cmd_target = NULL;

	for(unsigned int i=0; i<SUBSCRIBER_NR; i++)
	{
		cmd_target = _event_object_map[id]._subscriber_nr[i];
		if(cmd_target == NULL)
		{
			continue;
		}

		if(*cmd_target == object)
		{
			_event_object_map[id]._subscriber_nr[i] = NULL;
			_event_object_map[id].subscriber_number--;
			return true;
		}
	}

	return false;
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

	EventBus *event_bus = EventSingleton::instance();

	_zmq_context = event_bus->getZmqContext();

	thread::id self_id= this_thread::get_id();
	printf("self_id = %p,%d, _zmq_context = %p \n",self_id,self_id,_zmq_context);


	//创建zmq的套接字，设置套接字为请求应答模式
	_sub_socket = zmq_socket(_zmq_context, ZMQ_SUB);
	assert(_sub_socket);

	int rc = zmq_connect(_sub_socket, EventBus::MESSAGE_CENTER_ENDPOINT);
	assert(rc == 0);

	rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, "boy", 3);
	assert(rc == 0);
	rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, "girl", 4);
	assert(rc == 0);

	_pub_socket = zmq_socket(_zmq_context,ZMQ_PUB);
	assert(_pub_socket);

	rc = zmq_bind(_pub_socket, EventBus::MESSAGE_CENTER_ENDPOINT);
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





