
// Base64And3Des.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBase64And3DesApp: 
// �йش����ʵ�֣������ Base64And3Des.cpp
//

class CBase64And3DesApp : public CWinApp
{
public:
	CBase64And3DesApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBase64And3DesApp theApp;