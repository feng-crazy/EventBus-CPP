

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
#include "MZmq.h"

class EventTarget;
class EventClient
{
	public:
		// 注册、撤销对一条事件的关注。
		bool register_observer(EventType type, EventTarget &object);
		bool unregister_observer(EventType type, const EventTarget &object);

		void handle_event(void);

		void publish_event(EventType type, EventContent content);

		void publish_loc_event(EventType type, EventContent content);

		zmq_pollitem_t sub_items;
		
		// MsgClient的父对象可以被设置为任何对象，但是将其设置为事件中心最为合适。
		EventClient();
		~EventClient();

	private:

		typedef std::multimap<EventType, EventTarget*> EventObjectMap;
		typedef std::pair<EventType, EventTarget*> EventObjectPair;

		EventObjectMap _event_object_map;
		

		void *_zmq_context;
		void *_sub_socket;
		void *_pub_socket;
};


#endif // _EVENT_CLIENT_H
