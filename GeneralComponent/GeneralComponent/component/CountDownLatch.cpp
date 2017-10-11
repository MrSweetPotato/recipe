#include "CountDownLatch.h"
CCoundownLatch::CCoundownLatch(int pCount)
:m_Count(pCount)
, m_StopFlag(false)
{
}

CCoundownLatch::~CCoundownLatch()
{
	Stop();
}

void CCoundownLatch::Wait(void)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_FinishCond.wait(lock, [this](){return m_Count == 0 || IsStop(); });
}

void CCoundownLatch::CountDown(void)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Count--;
	if (m_Count == 0){
		m_FinishCond.notify_all();
	}
}

int CCoundownLatch::GetCount(void)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	return m_Count;
}