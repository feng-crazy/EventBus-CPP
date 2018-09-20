/*
 * EventType.h
 *
 *  Created on: Apr 12, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_EVENTTYPE_H_
#define UNITTEST_EVENTTYPE_H_

#include "EventDefine.h"

static const EventType EVENT_SYSTEM_STARTUP = "system startup";

static const EventType EVENT_SYSTEM_TIME_1 = "system timer one second";

static const EventType EVENT_TEST_TARGET_1 = "test target 1";

static const EventType EVENT_TEST_TARGET_2 = "test target 2";

static const EventType EVENT_TEST_THREAD_1 = "test thread 1";

static const EventType EVENT_TEST_THREAD_2 = "test thread 2";


//enum EventType
//{
//	MSG_NONE,
//	MSG_SYSTEM_STARTUP,  // 系统启动消息
//	MSG_SYSTEM_TIME_1,   // 系统每秒一次的定时消息
//	MSG_TEST_TARGET_1,
//	MSG_TEST_TARGET_2,
//	MSG_TEST_THREAD_1,
//	MSG_TEST_THREAD_2,
//};


#endif /* UNITTEST_EVENTTYPE_H_ */
