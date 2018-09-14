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
参数说明:
返回值:
******************************************************************************/
bool EventClient::operator==(const EventClient &client)
{
	return ((int)this == (int)&client);
}



/******************************************************************************
作者: 何登锋
功能描述:
	订阅一条消息，实际上是在消息映射表中注册CmdTarget对象和消息的对应关系。
参数说明:
返回值:
******************************************************************************/
bool EventClient::register_observer(Message id, EventTarget &object)
{
	if(id >= MESSAGE_NR)
	{
		return false;
	}

	EventTarget *cmd_target = NULL;

	// 确定该对象没有订阅该消息。
	for(unsigned int i=0; i<SUBSCRIBER_NR; i++)
	{
		cmd_target = _msg_object_map[id]._subscriber_nr[i];
		if((cmd_target != NULL) && (*cmd_target == object))
		{
			return false;
		}
	}

	for(unsigned int i=0; i<SUBSCRIBER_NR; i++)
	{
		cmd_target = _msg_object_map[id]._subscriber_nr[i];
		if(cmd_target == NULL)
		{
			_msg_object_map[id]._subscriber_nr[i] = &object;
			_msg_object_map[id].subscriber_number++;
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
	if(id >= MESSAGE_NR)
	{
		return false;
	}

	if(_msg_object_map[id].subscriber_number == 0)
	{
		return false;
	}

	EventTarget *cmd_target = NULL;

	for(unsigned int i=0; i<SUBSCRIBER_NR; i++)
	{
		cmd_target = _msg_object_map[id]._subscriber_nr[i];
		if(cmd_target == NULL)
		{
			continue;
		}

		if(*cmd_target == object)
		{
			_msg_object_map[id]._subscriber_nr[i] = NULL;
			_msg_object_map[id].subscriber_number--;
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
	NONE_HANDLE未处理；消息处理对象的返回值，如果消息类型是MTT_MULTICAST，
	则返回最后一个处理对像的返回值。
******************************************************************************/
int EventClient::notify_local_observer(const MsgEntity &msg)
{
	if(msg.id >= MESSAGE_NR)
	{
		printf("Invalid Message ID: %d\n", msg.id);
		return NONE_HANDLE;
	}

	int re = NONE_HANDLE;
	EventTarget *cmd_target = NULL;

	// 没有订阅者。
	if(_msg_object_map[msg.id].subscriber_number == 0)
	{
		return NONE_HANDLE;
	}

	unsigned int notified_nr = 0;
	for(unsigned i = 0; i < SUBSCRIBER_NR; i++)
	{
		cmd_target = _msg_object_map[msg.id]._subscriber_nr[i];
		if(cmd_target == NULL)
		{
			continue;
		}

		re = cmd_target->message_handle(msg);

		if(++notified_nr >= _msg_object_map[msg.id].subscriber_number)
		{
			break;
		}
	}

	return re;
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::handle_receive_queue(void)
{
	_mutex.lock();

	if(_msg_receive_queue.empty() == true)
	{
		_mutex.unlock();
		return;
	}

	// 复制消息接收队列中的消息到_msg_back_queue队列中，这样可以更快地打开锁，减少
	// 其他线程写消息接收队列被挂起的几率，或者更快地唤醒已经被阻塞的线程。
	_msg_back_queue = _msg_receive_queue;
	_msg_receive_queue.clear();
	_mutex.unlock();

	// 下面通告订阅这些消息的对象。
	MessageEntity *entity = NULL;
	while(_msg_back_queue.empty() == false)
	{
		entity = _msg_back_queue.front();
		_msg_back_queue.pop_front();

		notify_local_observer(entity->msg);

		_mutex.lock();
		_pool.free_message(*entity);
		_mutex.unlock();
	}
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::handle_send_queue(void)
{
	if(_msg_send_queue.empty() == true)
	{
		return;
	}

	MessageEntity *entity = NULL;
	EventBus *msg_center = MsgCenterSingleton::instance();

	while(_msg_send_queue.empty() == false)
	{
		entity = _msg_send_queue.front();
		_msg_send_queue.pop_front();
		msg_center->dispatch_message(*this, entity->msg);

		lock_guard<std::recursive_mutex> lock(_mutex);
		_pool.free_message(*entity);
	}
}



/******************************************************************************
作者：何登锋
功能描述：
	将发送和接送消息队列中的消息实体控件归还给msgpool。
参数说明：
返回值：
******************************************************************************/
void EventClient::_free_send_and_receive_space(void)
{
	MessageEntity *entity = NULL;

	while(_msg_send_queue.empty() == false)
	{
		entity = _msg_send_queue.front();
		_msg_send_queue.pop_front();
		_pool.free_message(*entity);
	}

	while(_msg_receive_queue.empty() == false)
	{
		entity = _msg_receive_queue.front();
		_msg_receive_queue.pop_front();

		_pool.free_message(*entity);
	}
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::add_message_to_receive_queue(const MsgEntity &msg)
{
	if(msg.id >= MESSAGE_NR)
	{
		printf("Invalid Message ID: %d\n", msg.id);
		return;
	}

	lock_guard<std::recursive_mutex> lock(_mutex);

	// 如果该Client中对象不关注这条消息，则返回。
	if(_msg_object_map[msg.id].subscriber_number == 0)
	{

		return;
	}

	MessageEntity &msg_entity = _pool.alloc_message();
	msg_entity = msg;
	if(msg.priority == MP_NORMAL)
	{
		_msg_receive_queue.push_back(&msg_entity);
	}
	else if(msg.priority == MP_PRIORITY)
	{
		_msg_receive_queue.push_front(&msg_entity);
	}

#ifndef NDEBUG
	if(_msg_receive_queue.size() > 1000)
	{
		printf("Too many MSGs in thread(id = %ld) receive queue,",
				this_thread::get_id());
		printf(" and info: msg.id = %d, size = %d\n", msg.id,
				_msg_receive_queue.size());
	}
#endif
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::add_message_to_send_queue(const MsgEntity &msg)
{
	if(msg.id > MESSAGE_NR)
	{
		printf("Invalid Message ID: %d\n", msg.id);
		return;
	}

	lock_guard<std::recursive_mutex> lock(_mutex);
	MessageEntity &msg_entity = _pool.alloc_message();
	msg_entity = msg;

	_msg_send_queue.push_back(&msg_entity);


#ifndef NDEBUG
	if(_msg_send_queue.size() > 1000)
	{
		printf("There are too many data in %ld send queue\n", this_thread::get_id());
	}
#endif
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::clear_message_queue(void)
{
	lock_guard<std::recursive_mutex> lock(_mutex);
	_free_send_and_receive_space();
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventClient::handle_message(void)
{
	handle_receive_queue();
	handle_send_queue();
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventClient::EventClient()
{
	// 先初始化成员，再注册到MsgCenter。这是因为如果先注册会出现某个线程在构造的时候，
	// 就有消息传到该线程。EventClient::add_message_to_receive_queue被异步调用，在此
	// 方法中会操作_msg_object_map，因此在这里必须先初始化成员。
	for(unsigned int i=0; i<MESSAGE_NR; i++)
	{
		_msg_object_map[i].subscriber_number = 0;
		for(unsigned int j=0; j<SUBSCRIBER_NR; j++)
		{
			_msg_object_map[i]._subscriber_nr[j] = NULL;
		}
	}

	EventBus *msg_center = MsgCenterSingleton::instance();

//	_zmq_context = msg_center->getZmqContext();
//
//	thread::id self_id= this_thread::get_id();
//	printf("self_id = %p,%d, _zmq_context = %p \n",self_id,self_id,_zmq_context);
//
//
//	//创建zmq的套接字，设置套接字为请求应答模式
//	_sub_socket = zmq_socket(_zmq_context, ZMQ_SUB);
//	assert(_sub_socket);
//
//	int rc = zmq_connect(_sub_socket, EventBus::MESSAGE_CENTER_ENDPOINT);
//	assert(rc == 0);
//
//	rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, "boy", 3);
//	assert(rc == 0);
//	rc = zmq_setsockopt(_sub_socket, ZMQ_SUBSCRIBE, "girl", 4);
//	assert(rc == 0);
//
//	_pub_socket = zmq_socket(_zmq_context,ZMQ_PUB);
//	assert(_pub_socket);
//
//	rc = zmq_bind(_pub_socket, EventBus::MESSAGE_CENTER_ENDPOINT);
//	assert(rc == 0);


	if(msg_center->register_client(this_thread::get_id(), *this) == false)
	{
		printf("Register client to message center is failed!\n");
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
	_free_send_and_receive_space();
}





