/******************************************************************************
作者：		何登锋
功能说明:

******************************************************************************/
#include "ClientCenter.h"
#include "EventClient.h"

#include "zmq/zmq.h"
//const char* ClientCenter::MESSAGE_CENTER_ENDPOINT = "inproc://message_center";


/**************************************************************************
作者: 何登锋
功能描述:
	注册消息客户系统。
参数说明:
返回值:
**************************************************************************/
bool ClientCenter::register_client(thread::id id, EventClient &client)
{
	pair<ThreadClientMap::iterator, bool> ret;

	lock_guard<std::recursive_mutex> lock(_mutex);

	ret = _client_pool.insert(ThreadClientPair(id, &client));

	return ret.second;
}



/**************************************************************************
作者: 何登锋
功能描述:
	返回消息客户系统。
参数说明:
返回值:
**************************************************************************/
EventClient *ClientCenter::find_client(thread::id id)
{

	ThreadClientMap::iterator it;

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
ClientCenter::ClientCenter()
{

}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
ClientCenter::~ClientCenter()
{
	// 析够所有的消息客户系统。
	ThreadClientMap::iterator it = _client_pool.begin();
	for(; it != _client_pool.end(); ++it)
	{
		delete (*it).second;
	}
}

