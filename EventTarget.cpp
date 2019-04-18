/******************************************************************************
作者：		何登锋
功能说明:

******************************************************************************/
#include "EventTarget.h"
#include "EventClient.h"
#include "ClientCenter.h"
#include "EventDefine.h"

const int NONE_HANDLE = 0xA5A5A5EF; // 随机数据没有任何意义，不要追究为什么。

/******************************************************************************
作者: 何登锋
功能描述:
参数说明:
返回值:
******************************************************************************/
bool EventTarget::operator ==(const EventTarget &object) const
{
	return (this == &object);
}

/******************************************************************************
作者: 何登锋
功能描述:
	事件处理方法，需要处理事件的对象需要重写该方法，注意在处理的事件需要先订阅。
参数说明:
	type: 事件。
返回值:
	NONE_HANDLE: 未处理该事件。
	其他: 自定义。
******************************************************************************/
int EventTarget::event_handle(EventType type, EventContent content)
{
	return NONE_HANDLE;
}



/******************************************************************************
作者: 何登锋
功能描述:
	发布一条事件，事件订阅者对象的event_handle方法将被调用。
参数说明:
	type: 事件。
返回值:
	NONE_HANDLE: 订阅者未处理该事件。
	其他: 自定义。
******************************************************************************/
int EventTarget::publish_event(EventType type, EventContent content)
{
	_client->publish_event(type, content);
}


/******************************************************************************
作者: 何登锋
功能描述:
	发布一条事件，通知事件给本线程内的订阅者。
参数说明:
	type: 事件。
返回值:
	NONE_HANDLE: 订阅者未处理该事件。
	其他: 自定义。
******************************************************************************/
int EventTarget::publish_loc_event(EventType type, EventContent content)
{
	_client->publish_loc_event(type, content);
}

/******************************************************************************
作者: 何登锋
功能描述:
	订阅一条事件。
参数说明:
	id: 事件的标示号。
	object: 关注事件的对象引用。
返回值:
	订阅成功返回true，否则false，一个对象重复订阅一条事件的时候会返回false。
******************************************************************************/
bool EventTarget::subscribe(EventType type, EventTarget &object)
{
	if(_client == NULL)
	{
		return false;
	}

	return _client->register_observer(type, object);
}


/******************************************************************************
作者: 何登锋
功能描述:
	退订一条事件。
参数说明:
	type: 事件的type。
	object: 关注事件的对象引用。
返回值:	
	订阅成功返回true，否则false。
******************************************************************************/
bool EventTarget::unsubscribe(EventType type, EventTarget &object)
{
	if(_client == NULL)
	{
		return false;
	}

	return _client->unregister_observer(type, object);
}





/******************************************************************************
作者: 何登锋
功能描述:
	设置该CmdTarget所属的事件客户对象。该对象不是接口，应用层不应直接调用，要是导致CmdTarget
	关联到另外线程的客户对像，会造成非常麻烦的事情。请注意不要直接调用它！
参数说明:
	client: 事件客户对象引用。
返回值:	
******************************************************************************/
void EventTarget::set_client(EventClient &client)
{
	_client = &client;
}


/******************************************************************************
作者: 何登锋
功能描述:
	构造方法。
参数说明:
	parent: 父对象。
返回值:	
******************************************************************************/
EventTarget::EventTarget()
{
	// 得到当前线程的type号，这个type也是当前线程所对应事件客户对象的type。
	thread::id id = this_thread::get_id();
	
	// 从事件中心中检索出事件客户对象。
	ClientCenter *client_center = ClientCenterSingleton::instance();
	_client = client_center->find_client(id);
	
	if(_client == NULL)
	{
		mdebug("Can't find message client! %d\n");
	}
}



/******************************************************************************
作者: 何登锋
功能描述:
	析构方法。
参数说明:
返回值:	
******************************************************************************/
EventTarget::~EventTarget()
{

}

