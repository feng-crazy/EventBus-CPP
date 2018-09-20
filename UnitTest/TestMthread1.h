/*
 * TestMthread1.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTMTHREAD1_H_
#define UNITTEST_TESTMTHREAD1_H_

#include "EventCommon.h"
#include "EventType.h"


#include "TestEventTarget1.h"

class TestMthread1 : public MThread
{
public:
	TestMthread1() : MThread(),_test_setup_thread_flag(false),_object_test1(NULL)
	{
		MThread::init();
	};

	~TestMthread1()
	{
		unsubscribe(EVENT_TEST_THREAD_2, *this);
		unsubscribe(EVENT_SYSTEM_TIME_1, *this);
		unsubscribe(EVENT_SYSTEM_STARTUP, *this);
	};

	int event_handle(EventType type, EventContent content)
	{
		printf("TestMthread1 event_handle event type:%s\n", type.c_str());

		return 0;
	};

protected:
	 void setup_thread()
	 {
//		 printf("MthreadTest1.setup_thread..%p,pthread id:%d\n",this_thread::get_id(),pthread_self());
		 _test_setup_thread_flag = true;
		 _object_test1 = new TestEventTarget1();

		subscribe(EVENT_TEST_THREAD_2, *this);
		subscribe(EVENT_SYSTEM_TIME_1, *this);
		subscribe(EVENT_SYSTEM_STARTUP, *this);
	 };

	 void thread_run()
	 {
		 printf("MthreadTest1 is run ...\n");
		 _object_test1->object_test();
		 this_thread::sleep_for(std::chrono::seconds(1));
	 };

private:
	 bool _test_setup_thread_flag;
	 TestEventTarget1 *_object_test1;
};



#endif /* UNITTEST_TESTMTHREAD1_H_ */
