#pragma once
#include <SapphireDef.h>
#include <fstream>


namespace Sapphire
{

	class SAPPHIRE_CLASS LogUtil
	{
	public:
#define LOGUTIL_MAX_CHARBUFFER_SIZE  512
		static LogUtil* getInstancePtr()
		{
			if (s_pLogUtil == NULL)
				s_pLogUtil = new LogUtil();
			return s_pLogUtil;
		}

		static void Release()
		{
			if (s_pLogUtil != NULL)
			{
				delete s_pLogUtil;
				s_pLogUtil = NULL;
			}
		}

		void Flush();
		void Init(TCHAR* pszFilePath, int nCacheNum = 0);
		void Log(TCHAR* pszLog);
		void Log(TString logStr);
		static void LogMsg(TCHAR* pszLog);
		static void LogMsg(TString logStr);
		static void LogMsgLn(TString& logStr);
		static void LogMsgLn(TCHAR* pszLog);

	private:


		std::vector<TString> m_logCache;
		TString m_logFilePath;
		FILE* m_pLogFile;
		TFStream m_fstream;
		int   m_nCacheNum;
		TCHAR szBuf[LOGUTIL_MAX_CHARBUFFER_SIZE];

		LogUtil();
		~LogUtil();
		static LogUtil* s_pLogUtil;

	};

#define  SAPPHIRE_LOG(info) Sapphire::LogUtil::LogMsgLn(info)
#define  SAPPHIRE_LOGWARNING(info) Sapphire::LogUtil::LogMsgLn(info)
#define  SAPPHIRE_LOGERROR(info) Sapphire::LogUtil::LogMsgLn(info)
}