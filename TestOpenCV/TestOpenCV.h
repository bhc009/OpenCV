
// TestOpenCV.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTestOpenCVApp:
// �� Ŭ������ ������ ���ؼ��� TestOpenCV.cpp�� �����Ͻʽÿ�.
//

class CTestOpenCVApp : public CWinApp
{
public:
	CTestOpenCVApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CTestOpenCVApp theApp;