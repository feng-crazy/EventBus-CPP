

/******************************************************************************
作者：	何登锋
功能说明：
	事件客户对象，该对象将置于线程中。对于线程来讲，该对象就是一个事件中心，
	对于整个系统来讲，它只属于事件系统的一部分。利用事件客户对象，线程中各个
	例程、方法和函数不再关心多线程安全等问题，在实现这些例程、方法时只认为它
	们运行在单线程情况下。
******************************************************************************/
#ifndef _EVENT_CLIENT_H
#define _EVENT_CLIENT_H


#include "StandardHead.h"
#include "EventDefine.h"
#include "zmq/zmq.h"

class EventTarget;
class EventClient
{
	public:
		// 注册、撤销对一条事件的关注。
		bool register_observer(Message id, EventTarget &object);
		bool unregister_observer(Message id, const EventTarget &object);
		
		// MsgClient的父对象可以被设置为任何对象，但是将其设置为事件中心最为合适。
		EventClient();
		~EventClient();

	private:
  
		// 每个事件Client处理的最大事件数量。
		static const unsigned int EVENT_NR = 1024;

		// 限制每条事件的订阅者数量。
		static const unsigned int SUBSCRIBER_NR = 100;

		// 事件和其订阅者映射表，该订阅表中只是保存了事件于事件处理对象的对应关系。
		// 事件可以是系统中所有的事件，包括本线程内部的，也包括其他线程发送来的事件，
		// 需要注意的是事件处理对象都属于该线程的运行空间。很讨厌这种结构，不过能提高
		// 效率。
		struct MsgMap
		{
			EventTarget *_subscriber_nr[SUBSCRIBER_NR];
			unsigned int subscriber_number;
		} _event_object_map[EVENT_NR];
		

		void *_zmq_context;
		void *_sub_socket;
		void *_pub_socket;
};


#endif // _EVENT_CLIENT_H
