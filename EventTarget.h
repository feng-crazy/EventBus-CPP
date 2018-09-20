/******************************************************************************
版权所有：	深圳市理邦精密仪器有限公司
项目名称：	Mx
版本号：		1.0
文件名：		CmdTarget.h
生成日期：	2008.12.09
作者：		何登锋
功能说明：
	该类封装了事件系统的细节，使用者不再关注事件系统本身，只需要从该类派生出新类就
	可以注册事件、接受和发送事件。
******************************************************************************/
#ifndef _EVENT_TARGET_H
#define _EVENT_TARGET_H

#include "StandardHead.h"
#include "EventDefine.h"
#include "zmq/zmq.h"
#include "MZmq.h"

extern const int NONE_HANDLE; // 随机数据没有任何意义，不要追究为什么。


class EventClient;
class EventTarget
{
	public:
		// 重载==。
		bool operator ==(const EventTarget &object) const;

		// 订阅一条事件。
		bool subscribe(EventType type, EventTarget &object);
		
		// 撤销对一条事件的订阅。
		bool unsubscribe(EventType type, EventTarget &object);

		// 通知事件给本线程内的订阅者。
		int publish_loc_event(EventType type, EventContent content);

		// 通知事件给订阅者。
		int publish_event(EventType type, EventContent content);
		
		// 事件回调方法，当事件被处理返回值大于等于0，没处理返回NONE_HANDLE。
		virtual int event_handle(EventType type, EventContent content);

		EventTarget();
		virtual ~EventTarget();

	protected:
		// 关联对象所属的事件客户。不要直接调用它
		void set_client(EventClient &client);
		

	private:
		EventClient *_client;
};


#endif

