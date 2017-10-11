#include "Threadpool.h"

CThreadPool::CThreadPool(uInt pSize, Task pInitialTask)
:m_ThreadSize(pSize)
, m_InitialTask(pInitialTask)
, m_bRunning(true)
{
}

CThreadPool::~CThreadPool()
{
	Stop();
}

void CThreadPool::Start(void)
{
	std::call_once(m_StartFlag, [this](){StartOnce(); });
}

void CThreadPool::Stop(void)
{
	std::call_once(m_StopFlag, [this](){StopOnce(); });
}

void CThreadPool::Run(Task &&t)
{
	m_TaskBlockQueue.Put(std::move(t));
}

void CThreadPool::Run(Task &t)
{
	m_TaskBlockQueue.Put(t);
}

void CThreadPool::StartOnce(void)
{
	for (uInt num = 0; num < m_ThreadSize; num++){
		ThreadPtr thread(new std::thread(std::bind(&CThreadPool::RunInThread, this)));
		m_ThreadIdVec.push_back(thread->get_id());
		m_ThreadVec.push_back(std::move(thread));
	}
}

void CThreadPool::StopOnce(void)
{
	WeakUpAndStop();
	auto f = [](ThreadPtr& threadptr){
		if (threadptr->joinable()){ 
			threadptr->join(); 
		} };
	std::for_each(m_ThreadVec.begin(), m_ThreadVec.end(), f);
	m_ThreadIdVec.clear();
	m_ThreadVec.clear();
}

void CThreadPool::RunInThread(void)
{
	if (m_InitialTask){
		m_InitialTask();
	}
	while (true){
		Task task;
		m_TaskBlockQueue.Take(task);
		if (IsStop()){
			break;
		}
		task();
	}
}