
// Base64And3DesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Base64And3Des.h"
#include "Base64And3DesDlg.h"
#include "afxdialogex.h"
#include "CommonInterface.h"
#include "esb_des.h"
#include "CBase64Code.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBase64And3DesDlg �Ի���



CBase64And3DesDlg::CBase64And3DesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBase64And3DesDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBase64And3DesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_KeysEdit);
	DDX_Control(pDX, IDC_EDIT2, m_DecryptionTextEdit);
	DDX_Control(pDX, IDC_EDIT3, m_EncryptionTextEdit);
}

BEGIN_MESSAGE_MAP(CBase64And3DesDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CBase64And3DesDlg::OnBnClickedEncrypt)
	ON_BN_CLICKED(IDC_BUTTON2, &CBase64And3DesDlg::OnBnClickedDecrypt)
END_MESSAGE_MAP()


// CBase64And3DesDlg ��Ϣ�������

BOOL CBase64And3DesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBase64And3DesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBase64And3DesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBase64And3DesDlg::OnBnClickedEncrypt()
{
	CCommonInterface Comter;
	TCHAR szDecryptText[1024] = { 0 };
	m_DecryptionTextEdit.GetWindowTextW(szDecryptText, 1024);
	TCHAR szKeysText[1024] = { 0 };
	m_KeysEdit.GetWindowTextW(szKeysText, 1024);
	std::string strKeys = Comter.UtoUtf8(szKeysText);
	if (lstrlen(szDecryptText) > 0)
	{
		std::string strDecryptPasswordTemp = Comter.UtoUtf8(szDecryptText);

		char bufout[1024] = { 0 };
		long ibuflength = 0;
		encrypt3des(bufout, 
			&ibuflength, 
			strDecryptPasswordTemp.c_str(), 
			strDecryptPasswordTemp.length(), 
			strKeys.c_str());
		char EncryptOut[1024] = { 0 };
		size_t encryptSize = Base64_Encode(EncryptOut, bufout, ibuflength);
		if (strlen(EncryptOut) > 0)
		{
			std::wstring wstrEncryptstr = Comter.Utf8toU(EncryptOut);
			m_EncryptionTextEdit.SetWindowTextW(wstrEncryptstr.c_str());
		}
	}
}


void CBase64And3DesDlg::OnBnClickedDecrypt()
{
	CCommonInterface Comter;
	TCHAR szEncryptPassword[1024] = { 0 };
	m_EncryptionTextEdit.GetWindowTextW(szEncryptPassword, 1024);
	TCHAR szKeysText[MAX_PATH] = { 0 };
	m_KeysEdit.GetWindowTextW(szKeysText, MAX_PATH);
	std::string strKeys = Comter.UtoUtf8(szKeysText);
	if (lstrlen(szEncryptPassword) > 0)
	{
		std::string strEncryptPasswordTemp = Comter.UtoUtf8(szEncryptPassword);
		unsigned char bufout[1024] = { 0 };
		unsigned long ibuflength = 0;
		char DstOut[1024] = { 0 };
		memset(DstOut, 0x0, sizeof(DstOut));
		size_t encryptSize = Base64_Decode(DstOut, strEncryptPasswordTemp.c_str(), strEncryptPasswordTemp.length());
		char ByteDst[1024] = { 0 };
		long tlen = 0;
		decrypt3des(ByteDst, &tlen, DstOut, encryptSize, strKeys.c_str());
		if (strlen(ByteDst) > 0)
		{
			std::wstring wstrDecryptStr = Comter.Utf8toU(ByteDst);
			if (wstrDecryptStr.length() > 0)
			{
				m_DecryptionTextEdit.SetWindowTextW(wstrDecryptStr.c_str());
			}
		}
	}
}
