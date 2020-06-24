#pragma once
#include <sys/timeb.h>
#include <time.h>
#include <atlstr.h>
#include <Windows.h>
#include <ShellAPI.h>
#include <iostream>
#include <string>
using namespace std;



class InfoLog
{
private:
	InfoLog()
	{

		InitializeCriticalSection(&m_CriticalFileWrite);
		const int buffSize = 1024;
		char buffFilePath[buffSize] = { 0 };
		GetModuleFileNameA(GetModuleHandleA(NULL), buffFilePath, buffSize);
		//std::cout<<buff<<endl;
		//PathRemoveExtensionA(buff);
		PathRenameExtensionA(buffFilePath, ".log");
		//std::cout<<PathFindFileNameA(buffFilePath)<<endl;

		struct tm newtime = { 0 };
		__int64 ltime = 0;

		_time64(&ltime);
		this->m_logFileName = buffFilePath;;
		// Obtain coordinated universal time:
		errno_t errNum = _gmtime64_s(&newtime, &ltime); // C4996
		if (errNum == 0)
		{
			char buffFileName[buffSize] = { 0 };
			sprintf_s(buffFileName, buffSize, "%d-%d-%d-", 1900 + newtime.tm_year, newtime.tm_mon + 1, newtime.tm_mday);
			std::string tmFileName = buffFileName;
			tmFileName += PathFindFileNameA(buffFilePath);

#ifdef _DEBUG
			this->m_logFileName = tmFileName;
#else
			char OutPath[MAX_PATH];
			int Strlen = GetTempPathA(MAX_PATH, OutPath);
			this->m_logFileName = OutPath + tmFileName;
#endif

		}
		fopen_s(&m_pw, this->m_logFileName.c_str(), "a+");

	}

public:
	static InfoLog& GetSigleInfoLog()   //单例模式最简单代码，需要C++11, GCC > 4.3, VS2015支持该特性
	{
		static InfoLog m_Instance;
		return m_Instance;
	}
	~InfoLog()
	{
		if (m_pw)
		{
			fclose(m_pw);
			m_pw = nullptr;
		}
		DeleteCriticalSection(&m_CriticalFileWrite);


	};
	void WriteLog(int infoText)
	{
		char buff[100] = { 0 };
		sprintf_s(buff, 100, "%d", infoText);
		WriteLog(buff);
	}

	void WriteLog(std::string infoText)
	{
		const int timeInfoBufferSize = 1024;
		struct _timeb timebuffer;
		char timeline[26] = { 0 };
		char logtimeInfo[timeInfoBufferSize] = { 0 };
		errno_t err = 0;


		err = _ftime64_s(&timebuffer);
		if (err != 0)
		{
			sprintf_s(logtimeInfo, timeInfoBufferSize, "%s", "log failed!");
		}
		err = ctime_s(timeline, 26, &(timebuffer.time));
		if (err)
		{
			sprintf_s(logtimeInfo, timeInfoBufferSize, "%s", "log failed!");//printf("Invalid argument to ctime_s. ");
		}

		sprintf_s(logtimeInfo, timeInfoBufferSize, "time : %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20]);
		//std::cout<<logtimeInfo<<endl;

		std::string strWrite = logtimeInfo;
		strWrite[strWrite.length() - 1] = ' ';
		strWrite += "\t\tLogInfo:\t" + infoText + "\n";

		OutputDebugStringA(strWrite.c_str());

		EnterCriticalSection(&this->m_CriticalFileWrite);
		{
			if (m_pw)
			{

				fwrite(strWrite.c_str(), sizeof(char), strWrite.length(), m_pw);
				fflush(m_pw);
			}
			else
			{
				std::cout << "LOG Failed";
				/*MessageBoxA(nullptr, "LOG Failed", "Error", MB_ICONERROR);*/
			}


		}
		LeaveCriticalSection(&this->m_CriticalFileWrite);
	}
	void WriteDetail(std::string infoText, std::string file, int line, std::string function)
	{
		char buff[100] = { 0 };
		sprintf_s(buff, 100, "%d", line);
		string strLine = buff;
		infoText += "\t\t" + file + "\t\t" + strLine + "\t\t" + function + "\t\t";
		WriteLog(infoText);

	}
protected:
private:
	FILE *m_pw = nullptr;
	std::string m_logFileName; //Log文件名	
	CRITICAL_SECTION m_CriticalFileWrite; //保护代码的临界变量
}; static InfoLog*   pDoLog = &InfoLog::GetSigleInfoLog();  //全局变量，执行Log过程

extern InfoLog*   pDoLog;
#define  LOG(x) pDoLog->WriteLog(x)
#define	 LOGDETAIL(x,file,line,function) pDoLog->WriteDetail(x,__FILE__,__LINE__,__FUNCTION__)
