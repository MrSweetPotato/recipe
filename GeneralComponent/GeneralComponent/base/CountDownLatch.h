#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_
#include <mutex>
#include <condition_variable>
#include "nocopyable.h"
class CCoundownLatch : public CNocopyable
{
public:
	explicit CCoundownLatch(int pCount);
	
	virtual ~CCoundownLatch();

	void Wait(void);

	void CountDown(void);

	void CCoundownLatch::Stop(void)
	{
		m_StopFlag = true;
	}

	int GetCount(void);

	bool IsStop(void) const { return m_StopFlag; }

private:

	std::condition_variable m_FinishCond;
	std::mutex				m_Mutex;
	int						m_Count;
	bool					m_StopFlag;
};

#endif//COUNTDOWNLATCH_H_