


/******************************************************************************
作者：	何登锋
功能说明：
	线程封装，MCThread简单封装线程的创建与销毁，这样可以减少应用程序创建线程的繁琐步
	骤。MThread类会创建一个消息客户系统，并提供一组控制接口。这样当需要创建一个线程
	时，可以简化很多与消息相关设置的工作，MThread将这些都封装起来，可以使应用程序更
	简洁。而不用关注消息系统的细节，而可以正常地使用消息系统。
******************************************************************************/
#ifndef M_THREAD_H
#define M_THREAD_H

#include "EventTarget.h"
#include "StandardHead.h"


// MThread封装了线程创建
// 创建一个可以接收其他线程消息的线程。
class MThread : public EventTarget
{
	public:
		// 派生类需要在其构造方法中调用它。
		void init(void);
		
		// 启动该线程。
		void task_begin(void);
		
		// 停止该线程的运行。
		void task_stop(void);

		void task_exit(void);

		// 处理消息队列，该方法在线程中比较消耗时间的地方调用，这样可以处理消息队列。比如，在
		// 某个地方需要等待某事件发生，这时可以调用该方法处理消息队列。
		void handle_message(void);

		thread::id this_id;

		MThread();
		virtual ~MThread();

	protected:
		// 派生类必须重写该方法，必须将需要接收消息的对象在该方法中构造！
		virtual void setup_thread(void) = 0;
		
		// 线程的主体代码，在该方法中写线程的主循环代码，主要这个函数不要写while(1)的死循环，
		// 只需做好休眠即可。
		virtual void thread_run(void) = 0;

	private:
		static void *_thread_run(void *arg);
		EventClient *_thread_client;//属于这个线程的消息客户系统对象指针

		// 该成员会被不同的线程访问，由于一个读，一个写，而且操作也不频繁，因此未加保护。
		bool _is_run;

		bool _is_exit;

		thread *_self_thread;
};


#endif // M_THREAD_H
