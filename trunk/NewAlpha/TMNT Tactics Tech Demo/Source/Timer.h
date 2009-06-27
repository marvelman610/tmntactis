////////////////////////////////////////////////////////
//   File Name	:	"Base.h"
//
//   Author		:	Ramon Johannessen (RJ)
//
//   Purpose	:	To provide an easy way to mark the passing of time
////////////////////////////////////////////////////////

#ifndef TIMER_H
#define TIMER_H

class CTimer
{
	bool  m_bIsOn;
	float m_fTotalTime;
	float m_fEndTime;
	float m_fPauseDur;
	float m_fPauseTimer;

public:

	CTimer()
	{
		ResetTimer();
	}

	void ResetTimer(bool bStop = true)
	{
		m_fTotalTime = m_fEndTime = m_fPauseDur = 0.0f;
		m_bIsOn = !bStop;
	}

	//////////////////////////////////////////////////////////////////////////
	// if you pass in an endtime, the update will return true when that endtime is reached
	void StartTimer(float endTime = 0.0f, bool reset = true)
	{
		if (reset)
			ResetTimer();
		m_bIsOn = true;
		if (endTime > 0.0f)
			m_fEndTime = endTime;
	}

	//////////////////////////////////////////////////////////////////////////
	// does not update the timer for a specified amount of time
	void PauseTimer(float pauseDuration = 0.0f)
	{
		m_bIsOn = false;
	}

	//////////////////////////////////////////////////////////////////////////
	// returns true if the user specified an end time and that end time was reached
	bool Update(float fElapsedTime)
	{
		if (m_bIsOn)
		{
			m_fTotalTime += fElapsedTime;
			if (m_fEndTime > 0.0f && m_fTotalTime >= m_fEndTime)
			{ m_fEndTime = m_fTotalTime = 0.0f; return true; }
		}
		if (m_fPauseDur > 0.0f)
		{
			m_fPauseTimer += fElapsedTime;
			if (m_fPauseTimer > m_fPauseDur)
			{m_bIsOn = true; m_fPauseTimer = m_fPauseDur = 0.0f;}
		}
		return false;
	}

	float GetElapsed() const {return m_fTotalTime;}
	bool  IsTimerRunning() const  {return m_bIsOn;}
};

#endif