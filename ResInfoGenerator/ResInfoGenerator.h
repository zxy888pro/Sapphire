
// ResInfoGenerator.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CResInfoGeneratorApp: 
// �йش����ʵ�֣������ ResInfoGenerator.cpp
//

class CResInfoGeneratorApp : public CWinApp
{
public:
	CResInfoGeneratorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CResInfoGeneratorApp theApp;