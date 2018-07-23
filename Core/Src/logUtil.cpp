#include "Sapphire.h"
#include "Charset.h"
#include "LogUtil.h"
#include <cassert>

namespace Sapphire
{
	LogUtil* LogUtil::s_pLogUtil = NULL;

	void LogUtil::Flush()
	{
		if (m_pLogFile != NULL)
		{

			for (int i = 0; i < m_logCache.size(); ++i)
			{
				memset(szBuf, '\0', LOGUTIL_MAX_CHARBUFFER_SIZE*sizeof(TCHAR));
				_tcscpy(szBuf, m_logCache[i].c_str());
				fwrite(szBuf, _tcslen(m_logCache[i].c_str()) + 1, 1, m_pLogFile);
			}
			m_logCache.clear();
		}
	}

	void LogUtil::Init(TCHAR* pszFilePath, int nCacheNum)
	{
		memset(szBuf, '\0', LOGUTIL_MAX_CHARBUFFER_SIZE*sizeof(TCHAR));
		m_logFilePath = pszFilePath;
#ifdef UNICODE
		m_pLogFile = FileOpen(pszFilePath, _T("wb+"));
		byte pheader[2] = { 0xff, 0xfe };
		fwrite(pheader, 2, 1, m_pLogFile);
#else
		m_pLogFile = FileOpen(pszFilePath, _T("at+"));
#endif
		assert(m_pLogFile);

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
			memset(szBuf, '\0', LOGUTIL_MAX_CHARBUFFER_SIZE*sizeof(TCHAR));
			_tcscpy(szBuf, szLog);
			int len = _tcslen(szLog);
			fwrite(szBuf, len + 1, 1, m_pLogFile);
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
			memset(szBuf, '\0', LOGUTIL_MAX_CHARBUFFER_SIZE*sizeof(TCHAR));
			_tcscpy(szBuf, logStr.c_str());
			int len = _tcslen(logStr.c_str());
			fwrite(szBuf, len + 1, 1, m_pLogFile);
		}
	}

	void LogUtil::LogMsg(TCHAR* pszLog)
	{
		assert(s_pLogUtil);
		s_pLogUtil->Log(pszLog);
	}


	void LogUtil::LogMsg(TString logStr)
	{
		assert(s_pLogUtil);
		s_pLogUtil->Log(logStr);
	}

	void LogUtil::LogMsgLn(TString& logStr)
	{
		logStr += _T("\n");
		assert(s_pLogUtil);
		s_pLogUtil->Log(logStr);
	}

	void LogUtil::LogMsgLn(TCHAR* pszLog)
	{
		TString logStr = pszLog;
		logStr += '\n';
		assert(s_pLogUtil);
		s_pLogUtil->Log(logStr);
	}


	LogUtil::LogUtil()
	{
		m_logFilePath = _T("log.txt");
		m_pLogFile = NULL;
		m_nCacheNum = 0;

	}

	LogUtil::~LogUtil()
	{
		if (m_pLogFile != NULL)
		{
			assert(fclose(m_pLogFile) < 0);
			m_pLogFile = NULL;
		}
	}
}
