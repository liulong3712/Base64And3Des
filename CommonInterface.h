/*
* Copyright (c) 2015,���������ý����Ƽ����޹�˾C++����
* All rights reserved.
*
* �ļ����ƣ�CommonInterface.h
* ժ Ҫ�����ýӿ��࣬���й��õĽӿڶ��ŵ����������ʵ��
*
* �� �ߣ�liulong
* ������ڣ�2015-03-09
*
*/
#pragma once
#include <vector>
#include <string>
#include <time.h>
#include <atlstr.h>

class CCommonInterface
{
public:
	CCommonInterface();
	~CCommonInterface();

	//��unicodeתΪutf-8��ʽ
	std::string UtoUtf8(std::wstring arg);
	//��utf-8תΪunicode��ʽ
	std::wstring Utf8toU(std::string arg);
	std::string UtoAnsi(std::wstring arg);
	TCHAR * CCommonInterface::AnsiToU(const char* str);
	//�ж��ַ������Ƿ��к���
	bool HaveChineseChar(std::wstring arg);
	//�ж�Ŀ¼�Ƿ����
	BOOL FolderExists(std::wstring s);
	//����Ŀ¼
	BOOL CreateDir(std::wstring P);
	//��ȡ�ļ��Ĵ�С
	long GetFileSize(std::wstring strFilePath);
	std::wstring substr_replace(const std::wstring&src, const std::wstring& search, const std::wstring& replace, bool global = true);
	//��ȡ��Ŀ¼������޸ĵ��ļ�Ŀ¼
	FILETIME GetLastTimeDir(LPCTSTR lpDstDir, std::wstring &lpLastTimeFileName, LPCTSTR lpFileSuffix, DWORD dwFlag = 0);
	//��ȡĿ¼·�����ļ���
	BOOL GetFileNameFromPath(LPCTSTR lpPath, LPTSTR lpFileName);
	// ��ȫ·���л�ȡ��·��
	BOOL GetPathFromPath(std::wstring strAllPath, std::wstring & strPath);
	//���ļ��������ļ�·���л�ȡ�ļ���׺
	BOOL GetSuffixFromPath(LPCTSTR lpPath, LPTSTR Suffix);
	// һ���ļ���ȥ����׺��
	BOOL GetNameFromName(std::wstring strFileName, std::wstring & strName);
	//ɾ��һ��Ŀ¼
	DWORD DeleteDirectoryFile(std::wstring strPath);
	//������Ļ����
	void CenterWindowByScreen(HWND hwnd);
	// �ж��ļ��ǲ��Ǵ���
	bool isFileExist(const char * strFileName);
	//���������С�����ַ����ĳ���
	DWORD GetStringDClength(HDC hDc, LPCTSTR lpString, LPCTSTR lpFontString, int nSize, bool bBold = false, bool bUnderline = false, bool bItalic = false);
	// �ж�����״̬
	BOOL GetNetWorkState(const char *Url);
	//����ĳ������idɱ��һ������
	DWORD ForceCloseProc(DWORD ProcId);
	//��������ɱ��ĳ������
	bool KillProcessByName(std::wstring szProcessName);
	//��ѹĳ���ļ���ָ���ļ�����
	bool Unzipfile(std::wstring wsSourceFilePath, std::wstring wsDstFilePath);
	//add by shixiong.du for get instance path
	std::wstring GetInstancePath();
	void	GetLocalIPs(std::vector<std::string> &ips);
	//ɨ��ĳ����Ƶ�ĵ�N��Ľ�ͼ
	bool GetVideoPic(std::wstring lsVideoPath, std::wstring &lsPicPath, int iTimes, std::wstring strCachePath);
	//add end

	//moved from CourseManagerSystem utils
	HGLOBAL GlobalAllocByString(const CHAR * lpszText, long nLen = -1);
	HGLOBAL GlobalAllocByString(const WCHAR * lpszText, long nLen = -1);

	BOOL EnumSysFont(std::vector<std::wstring> * arrSysFont);	// ö��ϵͳ����
	BOOL FlashWindowEx(HWND hWnd, int nCount);				// ��˸���ڱ�����
	BOOL GetTrayWndRect(RECT * lpRect);						// ��ȡϵͳ����������
	//��time_t��ʽ��ʱ��ת��Ϊָ�����ַ�����ʽ
	const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);
	//��_T("%Y-%m-%d %H:%M:%S")���¸�ʽת��Ϊtime_t��ʽ
	time_t TimeStringToTimet(LPCTSTR lpTimeString);

	TCHAR* GetCurLocalTime();

	BOOL IsDigit(const WCHAR * lpStr);
	void Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr);
	WCHAR ToLower(WCHAR c);
	void ToLower(WCHAR * lpText);
	void ToLower(std::wstring& strText);
	WCHAR ToUpper(WCHAR c);
	void ToUpper(WCHAR * lpText);
	void ToUpper(std::wstring& strText);
	void EncodeHtmlSpecialChars(std::wstring& strText);		// ����Html�����ַ�
	void DecodeHtmlSpecialChars(std::wstring& strText);		// ����Html�����ַ�

	std::wstring GetBetweenString(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd);
	int GetBetweenInt(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd, int nDefValue = 0);

	std::wstring GetBetweenString(const WCHAR * pStr, const WCHAR * pStart, const WCHAR * pEnd);
	int GetBetweenInt(const WCHAR * pStr, const WCHAR * pStart,
		const WCHAR * pEnd, int nDefValue = 0);

	BOOL DllRegisterServer(LPCTSTR lpszFileName);
	BOOL DllUnregisterServer(LPCTSTR lpszFileName);

	static int CALLBACK EnumSysFontProc(const LOGFONT *lpelf, const TEXTMETRIC *lpntm, DWORD dwFontType, LPARAM lParam);
	BOOL DllRegisterServer(LPCTSTR lpszFileName, BOOL bUnregister);
	//end

private:
	BOOL canconnect(std::string strhost);
	BOOL ConnectTest(char * m_server, int m_port);
};

