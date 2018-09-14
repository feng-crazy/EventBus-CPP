/******************************************************************************
作者：		何登锋
功能说明:

******************************************************************************/
#include "EventBus.h"
#include "EventClient.h"


const char* EventBus::MESSAGE_CENTER_ENDPOINT = "inproc://message_center";


/**************************************************************************
作者: 何登锋
功能描述:
	注册消息客户系统。
参数说明:
返回值:
**************************************************************************/
bool EventBus::register_client(thread::id id, EventClient &client)
{
	pair<MsgCenterMap::iterator, bool> ret;

	lock_guard<std::recursive_mutex> lock(_mutex);

	ret = _client_pool.insert(MsgCenterPair(id, &client));

	return ret.second;
}



/**************************************************************************
作者: 何登锋
功能描述:
	返回消息客户系统。
参数说明:
返回值:
**************************************************************************/
EventClient *EventBus::find_client(thread::id id)
{

	MsgCenterMap::iterator it;

	lock_guard<std::recursive_mutex> lock(_mutex);

	it = _client_pool.find(id);

	if(it == _client_pool.end())
	{
		return NULL;
	}

	return (*it).second;
}


/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventBus::EventBus()
{

}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventBus::~EventBus()
{
	// 析够所有的消息客户系统。
	MsgCenterMap::iterator it = _client_pool.begin();
	for(; it != _client_pool.end(); ++it)
	{
		delete (*it).second;
	}
}

