
// testMqtt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestMqttApp: 
// �йش����ʵ�֣������ testMqtt.cpp
//

class CtestMqttApp : public CWinApp
{
public:
	CtestMqttApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestMqttApp theApp;