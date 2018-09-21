/*
 * main.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: hdf_123
 */


#include <signal.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>

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

//		subscribe(EVENT_TEST_TARGET_1,*this);
//		subscribe(EVENT_TEST_TARGET_2,*this);
//		subscribe(EVENT_TEST_THREAD_1,*this);
//		subscribe(EVENT_TEST_THREAD_2,*this);


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
				printf("TestMain... publish_event :%s\n", input_str.c_str());
				type = EVENT_SYSTEM_STARTUP;
			}
			else if (input_str == "time")
			{
				printf("TestMain... publish_event :%s\n", input_str.c_str());
				type = EVENT_SYSTEM_TIME_1;
			}
			else if (input_str == "target1")
			{
				printf("TestMain... publish_event :%s\n", input_str.c_str());
				type = EVENT_TEST_TARGET_1;
			}
			else if (input_str == "target2")
			{
				printf("TestMain... publish_event :%s\n", input_str.c_str());
				type = EVENT_TEST_TARGET_2;
			}
			else if (input_str == "thread1")
			{
				printf("TestMain... publish_event :%s\n", input_str.c_str());
				type = EVENT_TEST_THREAD_1;
			}
			else if (input_str == "thread2")
			{
				printf("TestMain... publish_event :%s\n", input_str.c_str());
				type = EVENT_TEST_THREAD_2;
			}
			else
			{
				break;
			}

			publish_event(type, content);

			_client->handle_event();
		}

//        unsubscribe(EVENT_TEST_TARGET_1,*this);
//        unsubscribe(EVENT_TEST_TARGET_2,*this);
//        unsubscribe(EVENT_TEST_THREAD_1,*this);
//        unsubscribe(EVENT_TEST_THREAD_2,*this);

//		delete eventbus;
	}

	~TestMain()
	{

	}

	int event_handle(EventType type, EventContent content)
	{
		printf("TestMain message_handle msg id: %s\n", type.c_str());
		return 0;
	}
};

void stack_trace(const char *ptr)
{
    const int max_depth = 32;
    void *trace[max_depth];
    int stack_depth = backtrace(trace, max_depth);
    char log_str[100];   // 每条错误信息不能超过这个长度

    flockfile(stdout);

    ////////////////////////////////////////////////////////
    // 开始搜集错误信息
    ////////////////////////////////////////////////////////
    time_t t = time(NULL);
    strftime(log_str, sizeof(log_str), "\n\n%Y-%m-%d %H:%M:%S\n", localtime(&t));
    printf("%s", log_str);

    for(int i=0; i<stack_depth; i++)
    {
        Dl_info dlinfo;
        if(!dladdr(trace[i], &dlinfo))
        {
            snprintf(log_str, sizeof(log_str), "@ %p\n", trace[i]);
            printf("%s", log_str);
            continue;
        }

        const char *symname = dlinfo.dli_sname;

        int status;
        char *demangled = __cxxabiv1::__cxa_demangle(symname, NULL, NULL, &status);
        if((status == 0) && demangled)
        {
            symname = demangled;
        }

        if(symname)
        {
            snprintf(log_str, sizeof(log_str), "@ %s\n", symname);
            printf("%s", log_str);
        }
        else
        {
            snprintf(log_str, sizeof(log_str), "@ %p\n", trace[i]);
            printf("%s", log_str);
        }

        if(demangled)
        {
            free(demangled);
        }
    }

    printf("%s", ptr);

    funlockfile(stdout);
}

void error_handler(int sig)
{


    string s("signal is = ");
    s += to_string(sig);

    stack_trace(s.c_str());

    signal(sig, SIG_DFL);
    raise(sig);

}

int main()
{
    signal(SIGINT, error_handler);
    signal(SIGQUIT, error_handler);
    signal(SIGILL, error_handler);
    signal(SIGTRAP, error_handler);
    signal(SIGABRT, error_handler);
    signal(SIGIOT, error_handler);
    signal(SIGBUS, error_handler);
    signal(SIGFPE, error_handler);
    signal(SIGKILL, error_handler);
    signal(SIGSEGV, error_handler);
    signal(SIGTERM, error_handler);
    signal(SIGSTKFLT, error_handler);
    signal(SIGPIPE, SIG_IGN);

	TestMain test;

	return 0;
}
