/*
 * TestEventTarget1.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTEVENTTARGET1_H_
#define UNITTEST_TESTEVENTTARGET1_H_

#include "EventCommon.h"
#include "EventType.h"


class TestEventTarget1 : public EventTarget
{
public:
	int event_handle(EventType type, EventContent content)
	{
		printf("TestEventTarget1 event_handle event type:%s\n", type.c_str());
		return 0;
	};

	void object_test()
	{
		printf("TestEventTarget1 object_test publish_event EVENT_TEST_TARGET_1\n");

		EventType type = EVENT_TEST_TARGET_1;
		EventContent content;

		publish_event(type, content);
	};

	TestEventTarget1()
	{
		subscribe(EVENT_TEST_TARGET_2, *this);
		subscribe(EVENT_SYSTEM_TIME_1, *this);
	};

	~TestEventTarget1()
	{
		unsubscribe(EVENT_TEST_TARGET_2, *this);
		unsubscribe(EVENT_SYSTEM_TIME_1, *this);
	};

	virtual void SetUp(){ };

	virtual void TearDown() { };
};



#endif /* UNITTEST_TESTEVENTTARGET1_H_ */
