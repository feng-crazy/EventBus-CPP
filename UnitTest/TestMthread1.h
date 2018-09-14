/*
 * TestMthread1.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTMTHREAD1_H_
#define UNITTEST_TESTMTHREAD1_H_

#include "EventCommon.h"
#include "EvemtType.h"


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
		unsubscribe(MSG_TEST_THREAD_2, *this);
		unsubscribe(MSG_SYSTEM_TIME_1, *this);
		unsubscribe(MSG_SYSTEM_STARTUP, *this);
	};

	int message_handle(const MsgEntity &msg)
	{
		printf("TestMthread1 message_handle msg id:%d ,%s\n",msg.id,msg.wparam);
		switch(msg.id)
		{
			case MSG_SYSTEM_STARTUP:
			{
				run();
				return 0;
			}

		}
		return 0;
	};

protected:
	 void setup_thread()
	 {
//		 printf("MthreadTest1.setup_thread..%p,pthread id:%d\n",this_thread::get_id(),pthread_self());
		 _test_setup_thread_flag = true;
		 _object_test1 = new TestEventTarget1();

		subscribe(MSG_TEST_THREAD_2, *this);
		subscribe(MSG_SYSTEM_TIME_1, *this);
		subscribe(MSG_SYSTEM_STARTUP, *this);
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
