
// test_PLATERECOGNIZE.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctest_PLATERECOGNIZEApp: 
// �йش����ʵ�֣������ test_PLATERECOGNIZE.cpp
//

class Ctest_PLATERECOGNIZEApp : public CWinApp
{
public:
	Ctest_PLATERECOGNIZEApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctest_PLATERECOGNIZEApp theApp;