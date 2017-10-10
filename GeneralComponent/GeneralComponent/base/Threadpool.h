#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <thread>
#include <functional>
#include <vector>
#include "BlockingQueue.h"
#include "nocopyable.h"

class CThreadPool : public CNocopyable
{
public:
	typedef std::function<void(void)>		Task;
	typedef unsigned int					uInt;
	typedef std::unique_ptr<std::thread>	ThreadPtr;
	typedef std::thread::id					ThreadId;
	typedef std::vector<ThreadPtr>			ThreadPtrVector;
	typedef std::vector<ThreadId>			ThreadIdVector;
	typedef CBlockingQueue<Task>			TaskQueue;

	CThreadPool(uInt pSize, Task pInitialTask = Task());

	virtual ~CThreadPool();

	void Start(void);

	void Stop(void);

	void Run(Task &&t);

	void Run(Task &t);

	bool IsStop(void) const { return m_bRunning == false; }

private:

	void StartOnce(void);

	void StopOnce(void);

	void RunInThread(void);

	void WeakUpAndStop(void)
	{ 
		m_bRunning = false;
		m_TaskBlockQueue.Stop();
	}

	Task						m_InitialTask;
	TaskQueue					m_TaskBlockQueue;
	ThreadPtrVector				m_ThreadVec;
	ThreadIdVector				m_ThreadIdVec;
	uInt						m_ThreadSize;
	std::once_flag				m_StartFlag;
	std::once_flag				m_StopFlag;
	bool						m_bRunning;
};
#endif//THREADPOOL_H_