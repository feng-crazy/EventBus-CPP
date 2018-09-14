/******************************************************************************
版权所有：	深圳市理邦精密仪器有限公司
项目名称：	Mx
版本号：		1.0
文件名：		CmdTarget.h
生成日期：	2008.12.09
作者：		何登锋
功能说明：
	该类封装了消息系统的细节，使用者不再关注消息系统本身，只需要从该类派生出新类就
	可以注册消息、接受和发送消息。
******************************************************************************/
#ifndef _EVENT_TARGET_H
#define _EVENT_TARGET_H

#include "StandardHead.h"

extern const int NONE_HANDLE; // 随机数据没有任何意义，不要追究为什么。


class EventClient;
class EventTarget
{
	public:
		// 重载==。
		bool operator ==(const EventTarget &object) const;

		// 订阅一条消息。
		bool subscribe(Message id, EventTarget &object);

		// 订阅一个范围内的消息。
		void subscribe(Message start_id, Message end_id, EventTarget &object);

		// 撤销对一条消息的订阅。
		bool unsubscribe(Message id, EventTarget &object);

		// 撤销对一组消息的订阅。
		void unsubscribe(Message start_id, Message end_id, EventTarget &object);

		// 通知消息给本线程内的订阅者。
		int publish_message(const MsgEntity &msg);

		// 将消息放到本消息的处理队列中，而不及时通知订阅者。
		void publish_message_queue(const MsgEntity &msg);

		// 通知本线程的消息订阅者，以及发送消息到其他线程。
		void publish_thread_message(const MsgEntity &msg);

		// 只将消息发送给其他线程，而不通知本地线程的订阅者。
		void publish_thread_message_only(const MsgEntity &msg);

		// 立即发送跨线程消息。
		void flush_thread_message(void);

		// 消息回调方法，当消息被处理返回值大于等于0，没处理返回NONE_HANDLE。
		virtual int message_handle(const MsgEntity &msg);

		EventTarget();
		virtual ~EventTarget();

	protected:
		// 关联对象所属的消息客户。不要直接调用它！如有问题请与张虎联系。
		void set_client(EventClient &client);

		// 清空发送队列和接受队列。
		void clear_message_queue(void);

	private:
		EventClient *_client;
};


#endif

