
// Base64And3DesDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CBase64And3DesDlg �Ի���
class CBase64And3DesDlg : public CDialogEx
{
// ����
public:
	CBase64And3DesDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BASE64AND3DES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ��Կ����24λ
	CEdit m_KeysEdit;
	// ��������
	CEdit m_DecryptionTextEdit;
	// ��������
	CEdit m_EncryptionTextEdit;
	afx_msg void OnBnClickedEncrypt();
	afx_msg void OnBnClickedDecrypt();
};
