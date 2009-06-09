////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CodeProfiler.h"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	Profile functions in order to optimize.
//
////////////////////////////////////////////////////////////////////////////

#ifndef CODEPROFILER_H
#define CODEPROFILER_H

#include<fstream>

class CCodeProfiler
{
private:
	char*			name;
	int				timesRan;
	double			totalTime;
	float			avgTime;
	LARGE_INTEGER   timeStart;
    LARGE_INTEGER   tickFrequency;

public:
	CCodeProfiler(char* functionName)
	{
		name = functionName;
		timesRan = 0;
		avgTime = 0;

		ZeroMemory(&totalTime, sizeof(totalTime));		
		ZeroMemory(&timeStart, sizeof(timeStart));

		QueryPerformanceFrequency( &tickFrequency );

	}
	~CCodeProfiler()
	{
		avgTime = (totalTime / tickFrequency.QuadPart) / timesRan;
		ofstream ofs;
		ofs.open("Resources/code_profile.txt", ios_base::out | ios_base::trunc);

		if(ofs.is_open())
		{
			ofs.write(name,strlen(name)+1);
			ofs<<'\n'<<timesRan<<'\n';
			ofs<<avgTime;

		}
		ofs.close();

	}
	void Start()
	{
		timesRan++;
		
		QueryPerformanceCounter( &timeStart );

		//timeStart = ( float )ticks.QuadPart / ( float )tickFrequency.QuadPart;
	}
	void End()
	{
		LARGE_INTEGER   time;

		QueryPerformanceCounter( &time );
		//time = ( float )ticks.QuadPart / ( float )tickFrequency.QuadPart;
		totalTime += (time.QuadPart - timeStart.QuadPart);
	}
};

#endif