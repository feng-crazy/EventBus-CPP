/*
 * ClientCenter.h
 *
 *  Created on: Apr 1, 2019
 *      Author: hdf_123
 */

#ifndef LTHUB_LTHUB_TERMINAL_CPP_EVENTBUS_CLIENTCENTER_H_
#define LTHUB_LTHUB_TERMINAL_CPP_EVENTBUS_CLIENTCENTER_H_

#include "Singleton.h"
#include "StandardHead.h"
#include "EventDefine.h"


class EventClient;

class ClientCenter
{
	public:
		~ClientCenter();
	public:
		// 注册消息客户对象。
		bool register_client(thread::id id, EventClient &client);

		// 根据id查找消息客户对象。
		EventClient *find_client(thread::id id);

	private:
		ClientCenter();
		DECLARE_SINGLETON_CLASS(ClientCenter);

		// 保存了所有的消息客户系统对象，它需要互斥保护。
		typedef std::map<thread::id, EventClient*> ThreadClientMap;
		typedef std::pair<thread::id, EventClient*> ThreadClientPair;

		ThreadClientMap _client_pool;

//        void *_xpub_socket;
//        void *_xsub_socket;

		recursive_mutex _mutex;  // 保护_client_pool。

};
typedef Singleton<ClientCenter> ClientCenterSingleton;

#endif /* LTHUB_LTHUB_TERMINAL_CPP_EVENTBUS_CLIENTCENTER_H_ */
