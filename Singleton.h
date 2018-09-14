/******************************************************************************
作者：		何登锋
功能说明：
	单件类封装。如果你需要定义一个单件类，就可以使用该文件提供服务，它可以帮你解决多线程问题，资
	源释放问题，这些代码的实现基本上是标准的单件实现，如果你看不懂，相关资料非常多，请自行找资料
	了解。
******************************************************************************/
#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <memory>
#include<mutex>

using namespace std;

template <typename T>
class Singleton
{
	public:
		// two common methods for getting instance of singleton class.
		static T *instance();
		static T *instance(void *arg);

	private:
		Singleton(){}//do nothing
		~Singleton(){}

		static std::auto_ptr<T> _instance; // 它会自动释放对象。
        static std::recursive_mutex _mutex;
};


/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
template <typename T>
std::auto_ptr<T> Singleton<T>::_instance;



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
template <typename T>
recursive_mutex Singleton<T>::_mutex;



/******************************************************************************
作者：何登锋
功能描述：
	得到一个对象。
参数说明：
返回值：
******************************************************************************/
template <typename T>
inline T *Singleton<T>::instance()
{
	T *object = NULL;
	if(_instance.get() == NULL)
	{
		_mutex.lock();
		if(_instance.get() == NULL)
		{
			//Mdebug("instance.reset(new T)\n");
			_instance.reset(new T);
		}
		_mutex.unlock();

		object = _instance.get();
	}
	else
	{
		object = _instance.get();
	}
	return object;
}

/******************************************************************************
作者：何登锋
功能描述：
	得到一个对象。
参数说明：
返回值：
******************************************************************************/
template <typename T>
inline T *Singleton<T>::instance(void *arg)
{
	T *object = NULL;

	if(_instance.get() == NULL)
	{
		_mutex.lock();
		if(_instance.get() == NULL)
		{
			_instance.reset(new T(arg));
		}
		_mutex.unlock();

		object = _instance.get();
	}
	else
	{
		object = _instance.get();
	}

	return object;
}


#define DECLARE_SINGLETON_CLASS(type)      \
friend class std::auto_ptr<type>;          \
friend class Singleton<type>


#endif

