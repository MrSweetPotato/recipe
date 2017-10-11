#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "nocopyable.h"
#ifndef BOUNDBLOCKINGQUEUE_H_
#define BOUNDBLOCKINGQUEUE_H_
template<typename T>
class CBoundBlockingQueue : public CNocopyable
{
public:
	CBoundBlockingQueue(size_t pSize) 
		:m_MaxSize(pSize)
		, m_StopFlag(false)
	{}

	virtual ~CBoundBlockingQueue()
	{ 
		Stop();
	}

	void Put(const T& t)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotFull.wait(lock,[this](){return !IsFull() || IsStop(); });
		if (!IsStop()){
			m_Queue.push_back(t);
			m_NotEmpty.notify_one();
		}
	}

	void Put(T &&t)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotFull.wait(lock,[this](){return !IsFull() || IsStop(); });
		if (!IsStop()){
			m_Queue.push_back(std::forward<T>(t));
			m_NotEmpty.notify_one();
		}
	}

	void Take(T &t)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotEmpty.wait(lock,[this](){return !IsEmpty() || IsStop(); });
		if (!IsStop()){
			t = m_Queue.front();
			m_Queue.pop_front();
			m_NotFull.notify_one();
		}
	}

	std::shared_ptr<T> Take(void)
	{
		std::shared_ptr<T> tValPtr;
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotEmpty.wait(lock,[this](){return !IsEmpty() || IsStop(); });
		if (!IsStop()){
			tValPtr = std::make_shared<T>(m_Queue.front());
			m_Queue.pop_front();
			m_NotFull.notify_one();
		}
		return tValPtr;
	}

	void Stop(void)
	{
		std::call_once(m_StopOnceFlag, [this](){StopOnce(); });
	}

	size_t Size(void)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_Queue.size();
	}

	bool IsStop(void) const{ return m_StopFlag; }

	bool IsFull(void) const{ return m_MaxSize == m_Queue.size(); }

	bool IsEmpty(void) const{ return m_Queue.empty(); }

private:

	void StopOnce(void)
	{
		m_StopFlag = true;
		m_NotEmpty.notify_all();
		m_NotFull.notify_all();
	}
	std::mutex					m_Mutex;
	std::condition_variable		m_NotEmpty;
	std::condition_variable		m_NotFull;
	std::deque<T>				m_Queue;
	size_t						m_MaxSize;
	bool						m_StopFlag;
	std::once_flag				m_StopOnceFlag;
};
#endif//BOUNDBLOCKINGQUEUE_H_