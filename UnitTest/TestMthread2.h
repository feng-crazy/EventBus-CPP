/*
 * TestMthread2.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTMTHREAD2_H_
#define UNITTEST_TESTMTHREAD2_H_

#include "EventCommon.h"
#include "EventType.h"

#include "TestEventTarget2.h"

class TestMthread2 : public MThread
{
public:
	TestMthread2() : MThread(),_test_setup_thread_flag(false),_object_test2(NULL)
	{
		MThread::init();
	};

	~TestMthread2()
	{
		unsubscribe(EVENT_TEST_THREAD_1, *this);
		unsubscribe(EVENT_SYSTEM_TIME_1, *this);
		unsubscribe(EVENT_SYSTEM_STARTUP, *this);
	};

	int event_handle(EventType type, EventContent content)
	{
		printf("TestMthread2 event_handle event type:%s\n", type.c_str());

		return 0;
	};

protected:
	//下面的函数已经在线程内调用
	 void setup_thread()
	 {
//		 printf("TestMthread2..setup_thread.%p,pthread id:%d\n",this_thread::get_id(),pthread_self());
		 _test_setup_thread_flag = true;
		 _object_test2 = new TestEventTarget2();

		subscribe(EVENT_TEST_THREAD_1, *this);
		subscribe(EVENT_SYSTEM_TIME_1, *this);
		subscribe(EVENT_SYSTEM_STARTUP, *this);

	 };

	 void thread_run()
	 {
		 printf("TestMthread2 is run ....\n");
		 _object_test2->object_test();
		 this_thread::sleep_for(std::chrono::seconds(1));
	 };
private:
	 bool _test_setup_thread_flag;
	 TestEventTarget2 *_object_test2;
};



#endif /* UNITTEST_TESTMTHREAD2_H_ */
