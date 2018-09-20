/*
 * main.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: hdf_123
 */


#include <signal.h>

#include "EventCommon.h"
//#include "TestEventTarget1.h"
//#include "TestEventTarget2.h"
#include "TestMthread1.h"
#include "TestMthread2.h"

class TestMain : public EventTarget
{
public:
	TestMain()
	{
		EventBus *eventbus = EventBusSingleton::instance();

		// 创建主线程的消息客户系统，并从这里开始创建所有在本线程中运行的对象。
		EventClient *_client = new EventClient();
		EventTarget::set_client(*_client);

		TestMthread1 test_thread1;
//		test_thread1.run();
		TestMthread2 test_thread2;
//		test_thread2.run();

		subscribe(EVENT_TEST_TARGET_1,*this);
		subscribe(EVENT_TEST_TARGET_2,*this);
		subscribe(EVENT_TEST_THREAD_1,*this);
		subscribe(EVENT_TEST_THREAD_2,*this);


		while(1)
		{
			this_thread::sleep_for(std::chrono::seconds(1));
//			printf("main thread loop .........\n");
			string input_str;
			cout << "input send msg(start,time,target1,target2,thread1,thread2)";
			cin >> input_str;

			EventType type;
			EventContent content;

			if (input_str == "start")
			{
				printf("TestMain... publish_thread_message :%s\n", input_str.c_str());
				type = EVENT_SYSTEM_STARTUP;
			}
			else if (input_str == "time")
			{
				printf("TestMain... publish_thread_message :%s\n", input_str.c_str());
				type = EVENT_SYSTEM_TIME_1;
			}
			else if (input_str == "target1")
			{
				printf("TestMain... publish_thread_message :%s\n", input_str.c_str());
				type = EVENT_TEST_TARGET_1;
			}
			else if (input_str == "target2")
			{
				printf("TestMain... publish_thread_message :%s\n", input_str.c_str());
				type = EVENT_TEST_TARGET_2;
			}
			else if (input_str == "thread1")
			{
				printf("TestMain... publish_thread_message :%s\n", input_str.c_str());
				type = EVENT_TEST_THREAD_1;
			}
			else if (input_str == "thread2")
			{
				printf("TestMain... publish_thread_message :%s\n", input_str.c_str());
				type = EVENT_TEST_THREAD_2;
			}
			else
			{
				break;
			}

			publish_event(type, content);

			_client->handle_event();
		}

		delete _client;
		delete eventbus;
	}
	~TestMain()
	{
		unsubscribe(EVENT_TEST_TARGET_1,*this);
		unsubscribe(EVENT_TEST_TARGET_2,*this);
		unsubscribe(EVENT_TEST_THREAD_1,*this);
		unsubscribe(EVENT_TEST_THREAD_2,*this);
	}

	int event_handle(EventType type, EventContent content)
	{
		printf("TestMain message_handle msg id: %s\n", type.c_str());
		return 0;
	}
};


int main()
{

	TestMain test;

	return 0;
}
