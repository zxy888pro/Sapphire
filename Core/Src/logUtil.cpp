#include "Sapphire.h"
#include "Charset.h"
#include "LogUtil.h"
#include <cassert>

namespace Sapphire
{
	LogUtil* LogUtil::s_pLogUtil = NULL;

	void LogUtil::Flush()
	{
		if (m_fstream.is_open())
		{

			for (int i = 0; i < m_logCache.size(); ++i)
			{
				m_fstream.write(m_logCache[i].c_str(), m_logCache[i].size());
			}
			m_logCache.clear();
		}
	}

	void LogUtil::Init(TCHAR* pszFilePath, int nCacheNum)
	{
		memset(szBuf, '\0', LOGUTIL_MAX_CHARBUFFER_SIZE*sizeof(TCHAR));
		m_logFilePath = pszFilePath;
//#ifdef UNICODE
//		m_pLogFile = FileOpen(pszFilePath, _T("wb+"));
//		byte pheader[2] = { 0xff, 0xfe };
//		fwrite(pheader, 2, 1, m_pLogFile);
//#else
//		m_pLogFile = FileOpen(pszFilePath, _T("at+"));
//		
//#endif
		m_fstream.open(pszFilePath, std::ios_base::app | std::ios_base::out);
	}

	void LogUtil::Log(TCHAR* szLog)
	{
		if (m_nCacheNum > 0)
		{
			m_logCache.push_back(szLog);
			if (m_logCache.size() > m_nCacheNum)
				Flush();
		}
		else
		{
			int len = _tcslen(szLog);
			if (m_fstream.is_open())
			{
				m_fstream.write(szLog, len);
				m_fstream.flush();
			}
		}


	}

	void LogUtil::Log(TString logStr)
	{
		if (m_nCacheNum > 0)
		{
			m_logCache.push_back(logStr);
			if (m_logCache.size() > m_nCacheNum)
				Flush();
		}
		else
		{
			if (m_fstream.is_open())
			{
				m_fstream.write(logStr.c_str(), logStr.size());
				m_fstream.flush();
			}
		}
	}

	void LogUtil::LogMsg(TCHAR* pszLog)
	{
		assert(s_pLogUtil);
		s_pLogUtil->Log(pszLog);
		std::cout << pszLog << std::endl;
	}


	void LogUtil::LogMsg(TString logStr)
	{
		assert(s_pLogUtil);
		s_pLogUtil->Log(logStr);
		std::cout << logStr.c_str() << std::endl;
	}

	void LogUtil::LogMsgLn(TString& logStr)
	{
		logStr += _T("\n");
		assert(s_pLogUtil);
		s_pLogUtil->Log(logStr);
		std::cout << logStr.c_str() << std::endl;
	}

	void LogUtil::LogMsgLn(TCHAR* pszLog)
	{
		TString logStr = pszLog;
		logStr += '\n';
		assert(s_pLogUtil);
		s_pLogUtil->Log(logStr);
		std::cout << logStr.c_str() << std::endl;
	}


	LogUtil::LogUtil()
	{
		m_logFilePath = _T("log.txt");
		m_pLogFile = NULL;
		m_nCacheNum = 0;

	}

	LogUtil::~LogUtil()
	{
		/*if (m_pLogFile != NULL)
		{
		assert(fclose(m_pLogFile) < 0);
		m_pLogFile = NULL;
		}*/
		if (m_fstream.is_open())
		{
			m_fstream.close();
		}
	}
}
