
// Base64And3DesDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CBase64And3DesDlg 对话框
class CBase64And3DesDlg : public CDialogEx
{
// 构造
public:
	CBase64And3DesDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BASE64AND3DES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 密钥，需24位
	CEdit m_KeysEdit;
	// 明文内容
	CEdit m_DecryptionTextEdit;
	// 密文内容
	CEdit m_EncryptionTextEdit;
	afx_msg void OnBnClickedEncrypt();
	afx_msg void OnBnClickedDecrypt();
};
