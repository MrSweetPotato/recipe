#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include "nocopyable.h"

#ifndef THREADSAFE_QUEUE_H_
#define THREADSAFE_QUEUE_H_

template<typename T>
class CBlockingQueue : public CNocopyable
{
public:
	CBlockingQueue() :m_StopFlag(false){}
	virtual ~CBlockingQueue(){ Stop(); }

	void Put(const T &t)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.push_back(t);
		m_NotEmpty.notify_one();
	}

	void Put(T &&t)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.push_back(std::forward<T>(t));
		m_NotEmpty.notify_one();
	}

	void Take(T &t)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotEmpty.wait(lock, [this](){return !m_Queue.empty() || IsNeedStop(); });
		if (!IsNeedStop()){
			t = m_Queue.front();
			m_Queue.pop_front();
		}
	}

	std::shared_ptr<T> Take(void)
	{
		std::shared_ptr<T> tElementPtr;
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotEmpty.wait(lock, [this](){return !m_Queue.empty() || IsNeedStop(); });
		if (!IsNeedStop()){
			tElementPtr = std::make_shared<T>(m_Queue.front());
			m_Queue.pop_front();
		}
		return tElementPtr;
	}
	
	void Stop(void)
	{
		if (!IsNeedStop()){
			StopOnce();
		}
	}

	size_t Size(void)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_Queue.size();
	}

private:
	bool IsNeedStop(void) const { return m_StopFlag; }

	void StopOnce(void)
	{
		m_StopFlag = true;
		m_NotEmpty.notify_all();
	}

	std::condition_variable m_NotEmpty;
	std::mutex				m_Mutex;
	std::deque<T>			m_Queue;
	bool					m_StopFlag;
};

#endif//THREADSAFE_QUEUE_H_