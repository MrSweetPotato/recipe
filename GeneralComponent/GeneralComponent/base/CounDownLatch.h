#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_
#include <mutex>
#include <condition_variable>
#include "nocopyable.h"
class CCoundownLatch : public CNocopyable
{
public:
	explicit CCoundownLatch(size_t pCount);
	
	virtual ~CCoundownLatch();

	void Wait(void);

	void CoundDown(void);

	void CCoundownLatch::Stop(void)
	{
		m_StopFlag = true;
	}

	size_t GetCount(void);

private:

	bool IsNeedStop(void) const { return m_StopFlag; }

	std::condition_variable m_FinishCond;
	std::mutex				m_Mutex;
	size_t					m_Count;
	bool					m_StopFlag;
};

#endif//COUNTDOWNLATCH_H_