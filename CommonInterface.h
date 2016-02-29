/*
* Copyright (c) 2015,北京绩优堂教育科技有限公司C++部门
* All rights reserved.
*
* 文件名称：CommonInterface.h
* 摘 要：公用接口类，所有公用的接口都放到这个类里面实现
*
* 作 者：liulong
* 完成日期：2015-03-09
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

	//将unicode转为utf-8格式
	std::string UtoUtf8(std::wstring arg);
	//将utf-8转为unicode格式
	std::wstring Utf8toU(std::string arg);
	std::string UtoAnsi(std::wstring arg);
	TCHAR * CCommonInterface::AnsiToU(const char* str);
	//判断字符串中是否有汉字
	bool HaveChineseChar(std::wstring arg);
	//判断目录是否存在
	BOOL FolderExists(std::wstring s);
	//创建目录
	BOOL CreateDir(std::wstring P);
	//获取文件的大小
	long GetFileSize(std::wstring strFilePath);
	std::wstring substr_replace(const std::wstring&src, const std::wstring& search, const std::wstring& replace, bool global = true);
	//获取该目录中最后修改的文件目录
	FILETIME GetLastTimeDir(LPCTSTR lpDstDir, std::wstring &lpLastTimeFileName, LPCTSTR lpFileSuffix, DWORD dwFlag = 0);
	//获取目录路径的文件名
	BOOL GetFileNameFromPath(LPCTSTR lpPath, LPTSTR lpFileName);
	// 从全路径中获取出路径
	BOOL GetPathFromPath(std::wstring strAllPath, std::wstring & strPath);
	//从文件名或者文件路径中获取文件后缀
	BOOL GetSuffixFromPath(LPCTSTR lpPath, LPTSTR Suffix);
	// 一个文件名去掉后缀名
	BOOL GetNameFromName(std::wstring strFileName, std::wstring & strName);
	//删除一个目录
	DWORD DeleteDirectoryFile(std::wstring strPath);
	//窗口屏幕剧中
	void CenterWindowByScreen(HWND hwnd);
	// 判断文件是不是存在
	bool isFileExist(const char * strFileName);
	//根据字体大小计算字符串的长度
	DWORD GetStringDClength(HDC hDc, LPCTSTR lpString, LPCTSTR lpFontString, int nSize, bool bBold = false, bool bUnderline = false, bool bItalic = false);
	// 判断网络状态
	BOOL GetNetWorkState(const char *Url);
	//根据某个进程id杀死一个进程
	DWORD ForceCloseProc(DWORD ProcId);
	//根据名称杀死某个进程
	bool KillProcessByName(std::wstring szProcessName);
	//解压某个文件到指定文件夹内
	bool Unzipfile(std::wstring wsSourceFilePath, std::wstring wsDstFilePath);
	//add by shixiong.du for get instance path
	std::wstring GetInstancePath();
	void	GetLocalIPs(std::vector<std::string> &ips);
	//扫描某个视频的第N秒的截图
	bool GetVideoPic(std::wstring lsVideoPath, std::wstring &lsPicPath, int iTimes, std::wstring strCachePath);
	//add end

	//moved from CourseManagerSystem utils
	HGLOBAL GlobalAllocByString(const CHAR * lpszText, long nLen = -1);
	HGLOBAL GlobalAllocByString(const WCHAR * lpszText, long nLen = -1);

	BOOL EnumSysFont(std::vector<std::wstring> * arrSysFont);	// 枚举系统字体
	BOOL FlashWindowEx(HWND hWnd, int nCount);				// 闪烁窗口标题栏
	BOOL GetTrayWndRect(RECT * lpRect);						// 获取系统任务栏区域
	//将time_t格式的时间转换为指定的字符串格式
	const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);
	//将_T("%Y-%m-%d %H:%M:%S")如下格式转换为time_t格式
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
	void EncodeHtmlSpecialChars(std::wstring& strText);		// 编码Html特殊字符
	void DecodeHtmlSpecialChars(std::wstring& strText);		// 解码Html特殊字符

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

