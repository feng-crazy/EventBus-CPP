/******************************************************************************
作者：		何登锋
功能说明：
	整个消息系统原理：每个线程都会有自己的消息客户系统，消息客户系统由消息客户对象完成相关功能，
	消息客户对象由类MsgClient刻画。每个消息客户系统都有一个ID，该ID等于它从属线程的ID号。
	在构造消息客户对象的时候，它就向消息中心注册（MsgCenter），消息中心将此消息客户对象的ID和
	其引用保存下来。MsgClient有消息映射表、消息发送队列和消息接收队列，映射表保存了CmdTarget
	和其关注ID的对应关系，消息发送队列保存了需要外发给其他线程的消息，消息接收队列保存了由其他线
	程发送来的消息。当一个(CmdTarget)对象发送一条消息的时候，分为发给本地线程中的对象，发给其他
	线程的中的对象。发给本地对象的消息会马上被其订阅者处理，发送给其他线程对象的消息会先保存在本地
	消息客户系统的发送队列，稍候发到其他线程。这里简单描述了消息系统的原理，如果想深入了解细节请
	阅读源代码，毕竟源代码也很少。
******************************************************************************/
#ifndef _EVENT_BUS_H
#define _EVENT_BUS_H

#include "StandardHead.h"

class EventBus
{
	public:

		EventBus();
		~EventBus();

		static const char* XSUB_ADDR_PORT;
		static const char* XPUB_ADDR_PORT;
		static void *ZmqContext;

		static void run(void *arg);
		void begin(void);

		thread::id this_id;

	private:

        void *_xpub_socket;
        void *_xsub_socket;

        thread *_self_thread;

};


#endif

