/*
 * TestEventTarget2.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTEVENTTARGET2_H_
#define UNITTEST_TESTEVENTTARGET2_H_

#include "EventCommon.h"
#include "EvemtType.h"


class TestEventTarget2 : public EventTarget {
public:
    int message_handle(const MsgEntity &msg) {
        printf("TestEventTarget2 message_handle msg id:%d ,%s\n", msg.id, msg.wparam);
        return 0;
    }

    void object_test() {
        printf("TestEventTarget2 object_test publish_thread_message\n");

        MsgEntity msg;
        msg.id = MSG_TEST_TARGET_2;
        string str = "MobjectTest2";
        msg.wparam = (WParam) (str.c_str());
        msg.wsize = str.size() + 1;
        msg.priority = MP_NORMAL;
        msg.lparam = 0;
        msg.lsize = 0;
        publish_thread_message(msg);
    }

    TestEventTarget2() {
        subscribe(MSG_TEST_TARGET_1, *this);
        subscribe(MSG_SYSTEM_STARTUP, *this);
    };

    ~TestEventTarget2() {
        unsubscribe(MSG_TEST_TARGET_1, *this);
        unsubscribe(MSG_SYSTEM_TIME_1, *this);
    };

    virtual void SetUp() {};

    virtual void TearDown() {};
};


#endif /* UNITTEST_TESTEVENTTARGET2_H_ */
