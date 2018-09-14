/*
 * TestMthread2.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTMTHREAD2_H_
#define UNITTEST_TESTMTHREAD2_H_

#include "EventCommon.h"
#include "EvemtType.h"

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
		unsubscribe(MSG_TEST_THREAD_1, *this);
		unsubscribe(MSG_SYSTEM_TIME_1, *this);
		unsubscribe(MSG_SYSTEM_STARTUP, *this);
	};

	int message_handle(const MsgEntity &msg)
	{
		printf("TestMthread2 message_handle msg id:%d ,%s\n",msg.id,msg.wparam);
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
	//下面的函数已经在线程内调用
	 void setup_thread()
	 {
//		 printf("TestMthread2..setup_thread.%p,pthread id:%d\n",this_thread::get_id(),pthread_self());
		 _test_setup_thread_flag = true;
		 _object_test2 = new TestEventTarget2();

		subscribe(MSG_TEST_THREAD_1, *this);
		subscribe(MSG_SYSTEM_TIME_1, *this);
		subscribe(MSG_SYSTEM_STARTUP, *this);

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
