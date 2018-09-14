

/******************************************************************************
作者：	何登锋
功能说明：
	消息客户对象，该对象将置于线程中。对于线程来讲，该对象就是一个消息中心，
	对于整个系统来讲，它只属于消息系统的一部分。利用消息客户对象，线程中各个
	例程、方法和函数不再关心多线程安全等问题，在实现这些例程、方法时只认为它
	们运行在单线程情况下。
******************************************************************************/
#ifndef _EVENT_CLIENT_H
#define _EVENT_CLIENT_H


#include "StandardHead.h"

class EventTarget;
class EventClient
{
	public:
		// 重载==。
		bool operator==(const EventClient &client);

		// 注册、撤销对一条消息的关注。
		bool register_observer(Message id, EventTarget &object);
		bool unregister_observer(Message id, const EventTarget &object);

		// 通知本地的消息观察者。
		int notify_local_observer(const MsgEntity &msg);

		// 该方法在其它线程的空间中运行，需要加锁保护。
		// 它将消息插入到接收队列中。
		void add_message_to_receive_queue(const MsgEntity &msg);

		// 清除消息发送队列。
		void clear_message_queue(void);

		// 该方法将消息放到发送队列中。
		void add_message_to_send_queue(const MsgEntity &msg);

		// 线程调用该方法处理消息接受队列中的所有消息, 并将发送消息队列中的消息外发
		// 出去。在主线程中需要显示地调用它。
		void handle_message(void);

		// 处理发送队列和接受队列。
		void handle_receive_queue(void);
		void handle_send_queue(void);

		// MsgClient的父对象可以被设置为任何对象，但是将其设置为消息中心最为合适。
		EventClient();
		~EventClient();

	private:
		void _free_send_and_receive_space(void);

		// 每个消息Client处理的最大消息数量。
		static const unsigned int MESSAGE_NR = 1024;

		// 限制每条消息的订阅者数量。
		static const unsigned int SUBSCRIBER_NR = 100;

		// 消息和其订阅者映射表，该订阅表中只是保存了消息于消息处理对象的对应关系。
		// 消息可以是系统中所有的消息，包括本线程内部的，也包括其他线程发送来的消息，
		// 需要注意的是消息处理对象都属于该线程的运行空间。很讨厌这种结构，不过能提高
		// 效率。
		struct MsgMap
		{
			EventTarget *_subscriber_nr[SUBSCRIBER_NR];
			unsigned int subscriber_number;
		} _msg_object_map[MESSAGE_NR];

#if 0
		// 为了优化将消息映射表由数组实现，实际上使用数组的方式并不能提高太
		// 多效率，因此只是注释掉这段代码，而不是删掉它们。
		// 消息和其订阅者映射表，该订阅表中只是保存了消息于消息处理对象的对应关系。
		// 消息可以是系统中所有的消息，包括本线程内部的，也包括其他线程发送来的消息，
		// 需要注意的是消息处理对象都属于该线程的运行空间。
		typedef std::multimap<Message, EventTarget*> MsgObjectMap;
		typedef std::pair<Message, EventTarget*> MsgObjectPair;
		MsgObjectMap _msg_object_map;
#endif

		// 消息发送队列和接受队列
		typedef std::list<MessageEntity*> MsgEntityQueue;
		MsgEntityQueue _msg_send_queue;
		MsgEntityQueue _msg_receive_queue;
		// 因为访问_msg_receive_queue队列需要互斥，为了减少对_msg_receive_queue
		// 队列的访问时间，handle_message()会先将_msg_receive_queue中的消息复制到
		// _msg_back_queue队列中。
		MsgEntityQueue _msg_back_queue;

		// 接收队列需要保护起来，因为其他线程会来写它，这也是唯一需要互斥保护的地方。
		recursive_mutex _mutex;
		MsgPool _pool;
		void *_zmq_context;
		void *_sub_socket;
		void *_pub_socket;
};


#endif // _EVENT_CLIENT_H
