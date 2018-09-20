/*
 * TestEventTarget2.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTEVENTTARGET2_H_
#define UNITTEST_TESTEVENTTARGET2_H_

#include "EventCommon.h"
#include "EventType.h"


class TestEventTarget2 : public EventTarget {
public:
    int event_handle(EventType type, EventContent content)
    {
        printf("TestEventTarget2 event_handle event type:%s\n", type.c_str());
        return 0;
    }

    void object_test() {
        printf("TestEventTarget2 object_test publish_event EVENT_TEST_TARGET_2\n");

        EventType type = EVENT_TEST_TARGET_2;
        EventContent content;

        publish_event(type, content);
    }

    TestEventTarget2() {
        subscribe(EVENT_TEST_TARGET_1, *this);
        subscribe(EVENT_SYSTEM_STARTUP, *this);
    };

    ~TestEventTarget2() {
        unsubscribe(EVENT_TEST_TARGET_1, *this);
        unsubscribe(EVENT_SYSTEM_TIME_1, *this);
    };

    virtual void SetUp() {};

    virtual void TearDown() {};
};


#endif /* UNITTEST_TESTEVENTTARGET2_H_ */
