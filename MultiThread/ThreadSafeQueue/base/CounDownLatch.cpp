#include "CounDownLatch.h"
CCoundownLatch::CCoundownLatch(size_t pCount)
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
	m_FinishCond.wait(lock, [this](){return GetCount() == 0 || IsNeedStop(); });
}

void CCoundownLatch::CoundDown(void)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Count--;
	if (m_Count == 0){
		m_FinishCond.notify_all();
	}
}

size_t CCoundownLatch::GetCount(void)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	return m_Count;
}