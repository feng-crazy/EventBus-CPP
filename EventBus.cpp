/******************************************************************************
作者：		何登锋
功能说明:

******************************************************************************/

#include <signal.h>
#include <thread>

#include "EventBus.h"

#include "zmq/zmq.h"

const char* EventBus::XSUB_ADDR_PORT = "inproc://XsubEndpoint";
const char* EventBus::XPUB_ADDR_PORT = "inproc://XpubEndpoint";
void* EventBus::ZmqContext = zmq_ctx_new();

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventBus::run(void *arg)
{
	if(arg == NULL)
	{
		return;
	}

	EventBus *self = reinterpret_cast<EventBus*>(arg);
	zmq_proxy(self->_xsub_socket, self->_xpub_socket, NULL);
}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void EventBus::begin(void)
{
	_self_thread = new thread(run, this);
	if(_self_thread == NULL)
	{
		MLOG_DEBUGF("Create thread failure and error number is :%d\n",errno);
		return;
	}
	_self_thread->detach();

	this_id = _self_thread->get_id();
}
/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventBus::EventBus(): _self_thread(NULL)
{
	_xpub_socket = zmq_socket(ZmqContext, ZMQ_XPUB);
	assert(_xpub_socket);
	int rc = zmq_bind(_xpub_socket, XPUB_ADDR_PORT);
	assert(rc==0);

	_xsub_socket = zmq_socket(ZmqContext, ZMQ_XSUB);
	assert(_xsub_socket);
	rc = zmq_bind(_xsub_socket, XSUB_ADDR_PORT);
	assert(rc==0);

}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
EventBus::~EventBus()
{
	zmq_disconnect(_xpub_socket, XPUB_ADDR_PORT);
	zmq_close(_xpub_socket);

	zmq_disconnect(_xsub_socket, XSUB_ADDR_PORT);
	zmq_close(_xsub_socket);

	zmq_ctx_destroy(ZmqContext);

	delete _self_thread;
}


//EventBus *eventbus = NULL;
//
//static void signal_handler(int sig)
//{
//
//	string s("signal is = ");
//	s += to_string(sig);
//
//	stack_trace(s.c_str());
//
//	signal(sig, SIG_DFL);
//	raise(sig);
//
//	if(eventbus)
//	{
//		delete eventbus;
//		eventbus = NULL;
//	}
//}
//
//static void registerSignal()
//{
//	signal(SIGINT, signal_handler);
//	signal(SIGQUIT, signal_handler);
//	signal(SIGILL, signal_handler);
//	signal(SIGTRAP, signal_handler);
//	signal(SIGABRT, signal_handler);
//	signal(SIGIOT, signal_handler);
//	signal(SIGBUS, signal_handler);
//	signal(SIGFPE, signal_handler);
//	signal(SIGKILL, signal_handler);
//	signal(SIGSEGV, signal_handler);
//	signal(SIGTERM, signal_handler);
//	signal(SIGSTKFLT, signal_handler);
//	signal(SIGPIPE, SIG_IGN);
//}
//
//int main(int argc, char *argv[])
//{
//	registerSignal();
//
//	eventbus = new EventBus();
//	eventbus->run();
//
//	mdebug("eventbus except exit()\n");
//	delete eventbus;
//
//	exit(0);
//}
