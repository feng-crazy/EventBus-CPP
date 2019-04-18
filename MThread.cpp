/******************************************************************************
作者：	何登锋
功能说明：
	线程封装，该类会创建一个消息客户系统，并提供一组控制接口。
******************************************************************************/
#include "MThread.h"
#include "EventClient.h"


/////////////////////////////////// MThread ///////////////////////////////////

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void MThread::task_begin(void)
{
	_is_run = true;
}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
void MThread::task_stop(void)
{
	_is_run = false;
}

/******************************************************************************
作者: 何登锋
功能描述:
	另外一个提供线程处理消息的接口。这样在线程的_thread_run函数中当
	内部调用的thread_run是一个循环过程时，利用此函数提供给该线程在循环
	的过程中仍然能够进行消息的响应。(打印中的循环取打印数据过程用到了此功能)
参数说明:
返回值:
******************************************************************************/
void MThread::handle_message(void)
{
	if(_thread_client == NULL)
	{
		return;
	}

	_thread_client->handle_event();
}

/******************************************************************************
作者：何登锋
功能描述：
	线程入口方法。
参数说明：
返回值：
******************************************************************************/
void *MThread::_thread_run(void *arg)
{
	if(arg == NULL)
	{
		return NULL;
	}

	MThread *mthread = reinterpret_cast<MThread*>(arg);
	EventClient *client = new EventClient();

	mthread->_thread_client = client;

	mthread->set_client(*client);

	// 调用子类的设置方法。
	mthread->setup_thread();

	while(!mthread->_is_exit)
	{
		//Mdebug("_thread_run pthread id:%d\n",pthread_self());
		client->handle_event();
		if(!mthread->_is_run)
		{
			this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}
		mthread->thread_run();
	}

	return NULL;
}

/******************************************************************************
作者：何登锋
功能描述：
	创建线程，该方法被子类的构造函数调用，在这个方法中才真正创建线程。
参数说明：
返回值：
******************************************************************************/
void MThread::init(void)
{
	_self_thread = new thread(_thread_run, this);
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
MThread::MThread() : EventTarget(), _self_thread(NULL)
{
	_is_exit = false;
	_is_run = false;
	_thread_client = NULL;
}

/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
MThread::~MThread()
{
	delete _thread_client;
}

void MThread::task_exit(void)
{
	_is_exit = true;
}
