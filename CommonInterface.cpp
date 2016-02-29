#include "stdafx.h"

#include <string>

#include <io.h>
#include <strsafe.h>
#include <WinSock.h>
#include <TlHelp32.h>
#include <ShellAPI.h>
#include "XUnzip.h"
#pragma comment(lib, "wsock32.lib")

#include "CommonInterface.h"
using namespace std;

#pragma warning(disable:4996)
CCommonInterface::CCommonInterface()
{
}

CCommonInterface::~CCommonInterface()
{
}

std::string CCommonInterface::UtoUtf8(std::wstring arg)
{
	wchar_t* strAnswer = (wchar_t*)arg.c_str();
	int sizeneed = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)strAnswer, -1, 0, 0, 0, 0);
	char *Utf8bufAnSwerTrue = new char[sizeneed + 1];
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)strAnswer, -1, Utf8bufAnSwerTrue, sizeneed, 0, 0);
	std::string ret = Utf8bufAnSwerTrue;
	delete[]Utf8bufAnSwerTrue;
	Utf8bufAnSwerTrue = NULL;
	return ret;
}

TCHAR * CCommonInterface::AnsiToU(const char* str)
{
	if (!str)
		return NULL;
	int wcslen = ::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), NULL, 0);
	TCHAR *wstr = new TCHAR[wcslen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), wstr, wcslen);
	wstr[wcslen] = '\0';
	return wstr;
}

std::string CCommonInterface::UtoAnsi(std::wstring arg)
{
	wchar_t* strAnswer = (wchar_t*)arg.c_str();
	int sizeneed = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)strAnswer, -1, 0, 0, 0, 0);
	char *Utf8bufAnSwerTrue = new char[sizeneed + 1];
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)strAnswer, -1, Utf8bufAnSwerTrue, sizeneed, 0, 0);
	std::string ret = Utf8bufAnSwerTrue;
	delete Utf8bufAnSwerTrue;
	Utf8bufAnSwerTrue = NULL;
	return ret;
}

std::wstring CCommonInterface::Utf8toU(std::string arg)
{
	char* strAnswer = (char*)arg.c_str();
	int sizeneed = MultiByteToWideChar(CP_UTF8, 0, strAnswer, -1, 0, 0);
	wchar_t *Utf8bufAnSwerTrue = new wchar_t[sizeneed + 1];
	MultiByteToWideChar(CP_UTF8, 0, strAnswer, -1, Utf8bufAnSwerTrue, sizeneed);
	std::wstring ret = Utf8bufAnSwerTrue;
	delete[]Utf8bufAnSwerTrue;
	Utf8bufAnSwerTrue = NULL;
	return ret;
}
//判断字符串中是否有汉字
bool CCommonInterface::HaveChineseChar(std::wstring arg)
{
	bool bHaveChineseChar = FALSE;
	for (size_t i = 0; i < arg.length(); i++)
	{
		if (arg.at(i) > 255)
		{
			bHaveChineseChar = TRUE;
			break;
		}
	}
	return bHaveChineseChar;
}
// 判断目录是否存在
BOOL CCommonInterface::FolderExists(std::wstring s)
{
	DWORD attr;
	attr = GetFileAttributes(s.data());
	return (attr != (DWORD)(-1)) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}
//窗口靠屏幕居中
void CCommonInterface::CenterWindowByScreen(HWND hwnd)
{
	if (hwnd == NULL)
	{
		return;
	}
	//Center the window
	//Add By lch 2014-6-8
	RECT rc, rc1, rctomove;
	GetClientRect(hwnd, &rc1);

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	rctomove.left = (rc.right - rc.left) / 2 - (rc1.right - rc1.left) / 2;
	rctomove.right = (rc.right - rc.left) / 2 + (rc1.right - rc1.left) / 2;
	rctomove.top = (rc.bottom - rc.top) / 2 - (rc1.bottom - rc1.top) / 2;
	rctomove.bottom = (rc.bottom - rc.top) / 2 + (rc1.bottom - rc1.top) / 2;
	MoveWindow(hwnd, rctomove.left, rctomove.top, rc1.right - rc1.left, rc1.bottom - rc1.top, TRUE);
}

bool IsRoot(LPCTSTR lpPath)
{
	std::wstring Root;
	Root = lpPath[0] + _T("://");
	if (Root == lpPath)
		return true;
	else
		return false;
}
DWORD CCommonInterface::GetStringDClength(HDC hDc, LPCTSTR lpString, LPCTSTR lpFontString, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
	DWORD dwStringDCLength = 0;

	{
		SIZE size = { 0 };
		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		_tcsncpy(lf.lfFaceName, lpFontString, LF_FACESIZE);
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfHeight = -nSize;
		if (bBold) lf.lfWeight += FW_BOLD;
		if (bUnderline) lf.lfUnderline = TRUE;
		if (bItalic) lf.lfItalic = TRUE;
		HFONT hFont = ::CreateFontIndirect(&lf);

		SelectObject(hDc, hFont);
		GetTextExtentPoint32(hDc,
			lpString,
			lstrlen(lpString),
			&size);
		dwStringDCLength = size.cx;
	}

	return dwStringDCLength;
}
//************************************
// Method:    GetLastTimeDir
// Access:    获取该目录中最后修改的文件路径
// Returns:   LPFILETIME 返回修改时间
// Qualifier:
// Parameter: LPCTSTR lpDstDir ：要遍历的目录 LPTSTR lpLastTimeFileName：返回的文件路径,dwFlag=0取最晚的文件，dwFlag=1取最早的文件。
// Note:	  
//************************************
FILETIME CCommonInterface::GetLastTimeDir(LPCTSTR lpDstDir, std::wstring &lpLastTimeFileName, LPCTSTR lpFileSuffix, DWORD dwFlag)
{
	FILETIME LastfileTime = { 0 };
	if (lpDstDir == NULL || lstrlen(lpDstDir) == 0)
	{
		return LastfileTime;
	}
	std::wstring strDstDir = lpDstDir;
	if (!IsRoot(lpDstDir))
	{
		strDstDir += _T("//");
	}
	strDstDir += _T("*.*");
	//开始遍历目录
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = FindFirstFile(strDstDir.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return LastfileTime;
	}
	do 
	{
		if (FindFileData.cFileName[0] == L'.')
		{
			continue;
		}
		//如果是目录
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::wstring strDestFilePath = _T("");
			if (IsRoot(lpDstDir))
			{
				strDestFilePath = lpDstDir;
				strDestFilePath += FindFileData.cFileName;
			}
			else
			{
				strDestFilePath = lpDstDir;
				strDestFilePath += _T("\\");
				strDestFilePath += FindFileData.cFileName;
			}
			LastfileTime = GetLastTimeDir(strDestFilePath.c_str(), lpLastTimeFileName, lpFileSuffix);
		}
		//如果是文件
		else
		{
			if (lpFileSuffix != NULL)
			{
				//先获取文件后缀
				TCHAR szFileSuffix[MAX_PATH] = { 0 };
				TCHAR *pCLastPos = _tcsrchr(FindFileData.cFileName, L'.');
				if (pCLastPos != NULL)
				{
					lstrcpy(szFileSuffix, pCLastPos);
					//如果文件类型不匹配，不需要比较
					if (0 != lstrcmpi(lpFileSuffix, szFileSuffix))
					{
						continue;
					}
				}
			}
			std::wstring strDestFilePath = _T("");
			if (IsRoot(lpDstDir))
			{
				strDestFilePath = lpDstDir;
				strDestFilePath += FindFileData.cFileName;
			}
			else
			{
				strDestFilePath = lpDstDir;
				strDestFilePath += _T("\\");
				strDestFilePath += FindFileData.cFileName;
			}
			//比较时间大小，如果LastfileTime比FindFileData.ftLastWriteTime更大的话
			if (dwFlag == 0)
			{
				if (-1 == CompareFileTime(&LastfileTime, &(FindFileData.ftLastWriteTime)) ||
					(LastfileTime.dwHighDateTime == 0 && LastfileTime.dwLowDateTime == 0))
				{
					LastfileTime = FindFileData.ftLastWriteTime;
					lpLastTimeFileName = strDestFilePath;
				}
			}
			else
			{
				if (1 == CompareFileTime(&LastfileTime, &(FindFileData.ftLastWriteTime)) ||
					(LastfileTime.dwHighDateTime == 0 && LastfileTime.dwLowDateTime == 0))
				{
					LastfileTime = FindFileData.ftLastWriteTime;
					lpLastTimeFileName = strDestFilePath;
				}
			}
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	
	return LastfileTime;
}
BOOL CCommonInterface::GetSuffixFromPath(LPCTSTR lpPath, LPTSTR lpFileName)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };
	StringCchPrintf(szFilePath, MAX_PATH, _T("%s"), lpPath);
	wchar_t *pFileNamePos = _tcsrchr(szFilePath, L'.');
	if (pFileNamePos != NULL)
	{
		StringCchPrintf(lpFileName, MAX_PATH, _T("%s"), pFileNamePos + 1);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CCommonInterface::GetNameFromName(std::wstring strFileName, std::wstring & strName)
{
	//int index = strFileName.find('.');
	int index = strFileName.find_last_of('.');
	if (index > 0)
	{
		strName = strFileName.substr(0, index);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CCommonInterface::GetFileNameFromPath(LPCTSTR lpPath, LPTSTR Suffix)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };
	StringCchPrintf(szFilePath, MAX_PATH, _T("%s"), lpPath);
	wchar_t *pFileNamePos = _tcsrchr(szFilePath, L'\\');
	if (pFileNamePos != NULL)
	{
		StringCchPrintf(Suffix, MAX_PATH, _T("%s"), pFileNamePos + 1);
	}
	else
	{
		pFileNamePos = _tcsrchr(szFilePath, L'/');
		if (pFileNamePos != NULL)
		{
			StringCchPrintf(Suffix, MAX_PATH, _T("%s"), pFileNamePos + 1);
		}
	}
	return TRUE;
}

BOOL CCommonInterface::GetPathFromPath(std::wstring strAllPath, std::wstring & strPath)
{
	int nPos = strAllPath.find_last_of('\\');
	if (nPos > 0)
	{
		strPath = strAllPath.substr(0, nPos);
	}
	else
	{
		nPos = strAllPath.find_last_of('/');
		if (nPos > 0)
		{
			strPath = strAllPath.substr(0, nPos);
		}
	}
	return TRUE;
}
std::wstring CCommonInterface::substr_replace(const std::wstring&src, const std::wstring& search, const std::wstring& replace, bool global)
{
	//typedef std::string::npos NPOS;
	//typedef std::wstring::npos NPOS;

	size_t i = 0, inc = (replace.length()) ? replace.length() : 1;
	std::wstring res = src;
	while (true)
	{
		if ((i = res.find(search, i)) == std::wstring::npos)
		{
			break;
		}
		res.replace(i, search.length(), replace);
		if (!global)
			break;
		i += inc;
	}
	return res;
}
// 创建目录
BOOL CCommonInterface::CreateDir(std::wstring P)
{
	int len = P.length();
	if (len < 2)
	{
		return FALSE;
	}

	if ('\\' == P[len - 1])
	{
		P = P.substr(0, len - 1);
		len = P.length();
	}

	if (len <= 0)
	{
		return FALSE;
	}

	if (len <= 3)
	{
		if (FolderExists(P))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	if (FolderExists(P))
	{
		return TRUE;
	}

	std::wstring Parent;
	Parent = P.substr(0, P.rfind('\\'));

	if (Parent.length() <= 0)
	{
		return FALSE;
	}

	BOOL Ret = CreateDir(Parent);

	if (Ret)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = 0;
		Ret = (CreateDirectory(P.data(), &sa) == TRUE);
		return Ret;
	}
	else
	{
		return FALSE;
	}
}

//获取文件的大小
long CCommonInterface::GetFileSize(wstring strFilePath)
{
	long lFileSize = 0;
	FILE *pFileHandle = NULL;
	errno_t  errorId = _wfopen_s(&pFileHandle, strFilePath.c_str(), _T("r"));
	//如果打开成功
	if (errorId == 0 && pFileHandle != NULL)
	{
		fseek(pFileHandle, 0, SEEK_END); //定位到文件末 
		lFileSize = ftell(pFileHandle); //文件长度
		fclose(pFileHandle);
	}
	//打开失败返回0
	else
	{
		return lFileSize;
	}
	return lFileSize;
}
DWORD CCommonInterface::DeleteDirectoryFile(std::wstring strPath)
{
	DWORD dwResult = 0;
	WIN32_FIND_DATA FindFileData;
	std::wstring strFindFolder = strPath;
	strFindFolder += _T("\\*.*");
	HANDLE hFind = ::FindFirstFile(strFindFolder.data(), &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return 1;
	}
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes
			&FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strFindFolder = strPath;
				strFindFolder += _T("\\");
				strFindFolder += FindFileData.cFileName;
				DeleteDirectoryFile(strFindFolder);
			}
		}
		else
		{
			strFindFolder = strPath;
			strFindFolder += _T("\\");
			strFindFolder += FindFileData.cFileName;
			if (!DeleteFile(strFindFolder.data()))
			{
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
		{
			break;
		}
	}
	FindClose(hFind);

	return 0;
}

bool CCommonInterface::isFileExist(const char * strFileName)
{
	if (_access(strFileName, 0) == 0)
	{
		return true;
	}
	return false;
}
//杀死某个进程
DWORD CCommonInterface::ForceCloseProc(DWORD ProcId)
{

	if (ProcId <= 0)
	{
		return ERROR_INVALID_PARAMETER;
	}

	DWORD dwProcId = 0;
	HANDLE hProcess = NULL;
	DWORD dwRlt = ERROR_SUCCESS;

	__try
	{
		// 获取进程ID

		dwProcId = ProcId;

		// 打开进程

		SetLastError(ERROR_SUCCESS);

		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcId);

		if (hProcess == NULL)
		{
			dwRlt = GetLastError();

			return ERROR_INVALID_PARAMETER == dwRlt ? ERROR_SUCCESS : dwRlt;
		}

		// 结束进程

		if (!TerminateProcess(hProcess, -1))
		{
			dwRlt = GetLastError();
			return dwRlt;
		}

	}
	__finally
	{
		CloseHandle(hProcess); hProcess = NULL;
	}

	return ERROR_SUCCESS;
}
bool CCommonInterface::Unzipfile(std::wstring wsSourceFilePath, std::wstring wsDstFilePath)
{
	std::wstring strFilePath = wsSourceFilePath;
	std::wstring dbgstr;
	CCommonInterface comInterface;
	dbgstr = _T("filename:") + strFilePath;
	OutputDebugString(dbgstr.c_str());
	HZIP hz = NULL;
	hz = OpenZip((void*)strFilePath.c_str(), 0, 2);
	if (0 == hz)
	{
		OutputDebugString(_T("htzs压缩包已损坏!"));
		return false;
	}
	ZIPENTRY ze;
	//如果不是ZR_OK则直接返回
	if (0 != GetZipItem(hz, -1, &ze))
	{
		OutputDebugString(_T("取zip信息失败!"));
		return false;
	}
	int count = ze.index;
	if (count <= 0)
	{
		return false;
	}
	///////////////////////////////////
	wchar_t buf[10] = { 0 };
	_itow(count, buf, 10);
	dbgstr = _T("");
	dbgstr += _T("文件数:");
	dbgstr += buf;
	if (hz == NULL)
	{
		dbgstr += _T("打开zip失败");
	}
	else
	{
		dbgstr += _T("打开zip成功");
	}
	OutputDebugString(dbgstr.c_str());
	////////////////////////////////////
	std::wstring path = wsDstFilePath;
	if (!PathFileExists(path.c_str()))
	{
		comInterface.CreateDir(path);
	}
	//CString dir,file;
	std::wstring dir, file;
	file = strFilePath.c_str();
	for (int i = 0; i < count; i++)
	{
		GetZipItem(hz, i, &ze);
		DWORD dwSize = ze.unc_size;
		BYTE* pByte = new BYTE[dwSize];
		int res = UnzipItem(hz, i, pByte, dwSize, 3);

		file = ze.name;
		int ret = 0;
		HANDLE hFile = NULL;
		bool isfile = true;

		//if (-1 != (ret=file.Find(_T("/"))) && ret == file.GetLength())
		if (-1 != (ret = file.find(_T("/"))) && ret == file.length())
		{
			isfile = false;
		}
		ret = 0;
		while (-1 != (ret = file.find(_T("/"), ret)))
			//ret = file.find(_T("/"), ret);
			//while (-1 != ret)
		{
			//if (ret != -1)
			//{
			dir = path;
			//CString strtmp = file.Left(ret);
			std::wstring strtmp = file.substr(0, ret);
			dir += strtmp;

			std::wstring tmp = comInterface.substr_replace(dir, L"/", L"\\");
			if (!PathFileExists(tmp.c_str()))
			{
				comInterface.CreateDir(tmp);
			}
			ret += 1;
			//}
			//ret = file.find(_T("/"), ret);
		}
		if (isfile)
		{

			std::wstring now;
			std::wstring str1 = L"\\";
			std::wstring str2 = L"/";
			now = comInterface.substr_replace(file, str2, str1);
			//file.replace(file.begin(), file.end(), L"\\");
			now = path + now;
			dbgstr = now;
			OutputDebugString(dbgstr.c_str());
			hFile = ::CreateFile(now.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			_itow((int)hFile, buf, 10);
			dbgstr += _T("文件句柄:");
			dbgstr += buf;
			OutputDebugString(dbgstr.c_str());
			if (hFile != INVALID_HANDLE_VALUE)
			{
				dbgstr = _T("");
				DWORD writecnt = 0;
				BOOL ret = ::WriteFile(hFile, pByte, dwSize, &writecnt, 0);
				if (ret)
				{
					dbgstr += _T("写文件成功");
				}
				else
				{
					dbgstr += _T("写文件失败");
				}
				OutputDebugString(dbgstr.c_str());
				if (hFile != INVALID_HANDLE_VALUE)
					::CloseHandle(hFile);
			}
			//::WriteFile(hFile, pByte, dwSize, 0, 0);
			//MessageBox(NULL, L"asd",L"",MB_OK);
		}
		delete[] pByte;
		pByte = NULL;
		//if (hFile != INVALID_HANDLE_VALUE)
		//::CloseHandle(hFile);
	}
	CloseZip(hz);
	return true;
}
bool CCommonInterface::KillProcessByName(std::wstring szProcessName)
{
	PROCESSENTRY32 pe;
	BOOL bResult = TRUE;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
	{
		bResult = FALSE;
		return bResult;
	}
	DWORD dwProcessID = GetCurrentProcessId();
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (lstrcmp(pe.szExeFile, szProcessName.c_str()) == 0)
		{
			//add by liulong 2015/09/07
			//排除自己，不必杀死自己
			if (dwProcessID == pe.th32ProcessID)
			{
				continue;
			}
			//end add
			if (!ForceCloseProc(pe.th32ProcessID))
			{
				bResult = FALSE;
				break;
			}
		}
	}
	CloseHandle(hSnapshot);
	return bResult;
}
BOOL CCommonInterface::GetNetWorkState(const char *Url)
{
	BOOL bCanConnet = FALSE;
	char *pTempUrl = NULL;
	if (Url == NULL)
	{
		return FALSE;
	}
	int iUrlLength = strlen(Url);
	if (iUrlLength > 0)
	{
		pTempUrl = new char[iUrlLength + 1];
		strcpy(pTempUrl, Url);
		if (pTempUrl[iUrlLength - 1] == '/')
		{
			pTempUrl[iUrlLength - 1] = '\0';
		}
		char *pChildUrl = NULL;
		pChildUrl = strstr(pTempUrl, "http://");
		if (pChildUrl != NULL)
		{
			bCanConnet = canconnect(pChildUrl + 7);
		}
		else
		{
			bCanConnet = canconnect(pTempUrl);
		}
		if (pTempUrl != NULL)
		{
			delete[]pTempUrl;
			pTempUrl = NULL;
		}
		return bCanConnet;
	}
	else
	{
		return FALSE;
	}

	//return InternetGetConnectedState(&flag,0);
}

std::wstring CCommonInterface::GetInstancePath()
{
	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetModuleFileName(NULL, tszModule, MAX_PATH);
	std::wstring ret = tszModule;

	size_t pos = ret.rfind(_T('\\'));
	if (pos > 0) ret = ret.substr(0, pos);
	return ret;
}

void CCommonInterface::GetLocalIPs(std::vector<std::string> &ips)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(1, 1);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	if (gethostname(name, sizeof(name)) == 0)
	{
		printf("Host name: %s\n", name);

		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			int nCount = 0;
			while (hostinfo->h_addr_list[nCount])
			{
				ips.push_back(inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[nCount]));
				nCount++;
			}
		}
	}
}

BOOL CCommonInterface::canconnect(std::string strhost)
{
	BOOL bRet;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	bRet = ConnectTest((char*)strhost.c_str(), 80);
	return bRet;
}

BOOL CCommonInterface::ConnectTest(char * m_server, int m_port)
{

	struct hostent* host = NULL;
	struct sockaddr_in saddr;
	unsigned int s = 0;
	BOOL  ret;
	char error[MAX_PATH];
	memset(error, 0, sizeof(error));
	host = gethostbyname(m_server);
	if (host == NULL)return  FALSE;

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(m_port);
	saddr.sin_addr = *((struct in_addr*)host->h_addr);


	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		return FALSE;
	}

	u_long imode = 1;
	ioctlsocket(s, FIONBIO, &imode);

	if (connect(s, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
		//if (errno == EINPROGRESS){// it is in the connect process   
		struct timeval tv;
		fd_set writefds;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO(&writefds);
		FD_SET(s, &writefds);
		if (select(s + 1, NULL, &writefds, NULL, &tv) > 0){
			int len = sizeof(int);
			//下面的一句一定要，主要针对防火墙   
			getsockopt(s, SOL_SOCKET, SO_ERROR, error, &len);

			return TRUE;
			// 				if (error == 0) ret = TRUE;
			// 				else ret = FALSE;
		}
		else   ret = FALSE;//timeout or error happen   
		//}
		//else ret = FALSE;
	}
	else    ret = TRUE;

	closesocket(s);

	return ret;
}

//moved from CourseManagerSystem utils
HGLOBAL CCommonInterface::GlobalAllocByString(const CHAR * lpszText, long nLen/* = -1*/)
{
	if (NULL == lpszText)
		return NULL;

	if (-1 == nLen)
		nLen = strlen(lpszText);

	HGLOBAL hGlobal = ::GlobalAlloc(GHND, nLen + 4);
	if (hGlobal != NULL)
	{
		CHAR * lpData = (CHAR *)::GlobalLock(hGlobal);
		if (lpData != NULL)
		{
			memset(lpData, 0, nLen + 4);
			memcpy(lpData, lpszText, nLen);
			::GlobalUnlock(hGlobal);
		}
	}
	return hGlobal;
}

HGLOBAL CCommonInterface::GlobalAllocByString(const WCHAR * lpszText, long nLen/* = -1*/)
{
	if (NULL == lpszText)
		return NULL;

	if (-1 == nLen)
		nLen = wcslen(lpszText);

	return GlobalAllocByString((const CHAR *)lpszText, nLen * sizeof(TCHAR));
}

// 枚举系统字体回调函数
int CALLBACK CCommonInterface::EnumSysFontProc(const LOGFONT *lpelf, const TEXTMETRIC *lpntm, DWORD dwFontType, LPARAM lParam)
{
	if (dwFontType & TRUETYPE_FONTTYPE)
	{
		std::vector<std::wstring> * arrSysFont = (std::vector<std::wstring> *)lParam;
		if (arrSysFont != NULL)
		{
			for (int i = 0; i < (int)arrSysFont->size(); i++)
			{
				if ((*arrSysFont)[i] == lpelf->lfFaceName)
					return TRUE;
			}
			arrSysFont->push_back(lpelf->lfFaceName);
		}
	}

	return TRUE;
}

// 枚举系统字体
BOOL CCommonInterface::EnumSysFont(std::vector<std::wstring> * arrSysFont)
{
	if (NULL == arrSysFont)
		return FALSE;

	HDC hDC = ::GetDC(NULL);
	::EnumFontFamiliesEx(hDC, NULL, CCommonInterface::EnumSysFontProc, (LPARAM)arrSysFont, 0);
	::ReleaseDC(NULL, hDC);

	return TRUE;
}

// 闪烁窗口标题栏
BOOL CCommonInterface::FlashWindowEx(HWND hWnd, int nCount)
{
	FLASHWINFO stFlashInfo = { 0 };
	stFlashInfo.cbSize = sizeof(FLASHWINFO);
	stFlashInfo.hwnd = hWnd;
	stFlashInfo.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
	stFlashInfo.uCount = nCount;
	stFlashInfo.dwTimeout = 0;
	return ::FlashWindowEx(&stFlashInfo);
}

// 获取系统任务栏区域
BOOL CCommonInterface::GetTrayWndRect(RECT * lpRect)
{
	if (NULL == lpRect)
		return FALSE;

	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if (hWnd != NULL)
		return ::GetWindowRect(hWnd, lpRect);
	else
		return FALSE;
}
//将_T("%Y-%m-%d %H:%M:%S")如下格式转换为time_t格式
time_t CCommonInterface::TimeStringToTimet(LPCTSTR lpTimeString)
{
	tm tm_;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	swscanf(lpTimeString, _T("%d-%d-%d %d:%d:%d"), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;

	time_t t_ = mktime(&tm_); //已经减了8个时区  
	return t_; //秒时间 
}
// _T("%Y-%m-%d %H:%M:%S")
const TCHAR * CCommonInterface::FormatTime(time_t lTime, LPCTSTR lpFmt)
{
	struct tm * lpTimeInfo;
	static TCHAR cTime[32];

	memset(cTime, 0, sizeof(cTime));

	lpTimeInfo = localtime(&lTime);
	if (lpTimeInfo != NULL)
		_tcsftime(cTime, sizeof(cTime) / sizeof(TCHAR), lpFmt, lpTimeInfo);
	return cTime;
}

TCHAR* CCommonInterface::GetCurLocalTime()
{
	SYSTEMTIME stime = { 0 };
	GetLocalTime(&stime);
	static TCHAR szCurrentTime[MAX_PATH] = { 0 };
	//这里只记录到天数，不记录具体的时间点
	_stprintf(szCurrentTime, _T("%02d-%02d-%02d %02d:%02d:%02d"),
		stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);

	return szCurrentTime;
}


BOOL CCommonInterface::IsDigit(const WCHAR * lpStr)
{
	for (const WCHAR * p = lpStr; *p != _T('\0'); p++)
	{
		if (!isdigit(*p))
			return FALSE;
	}
	return TRUE;
}

void CCommonInterface::Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr)
{
	if (NULL == lpOldStr || NULL == lpNewStr)
		return;

	int nOldStrLen = wcslen(lpOldStr);
	int nNewStrLen = wcslen(lpNewStr);

	std::wstring::size_type nPos = 0;
	while ((nPos = strText.find(lpOldStr, nPos)) != std::wstring::npos)
	{
		strText.replace(nPos, nOldStrLen, lpNewStr);
		nPos += nNewStrLen;
	}
}

inline WCHAR CCommonInterface::ToLower(WCHAR c)
{
	return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

void CCommonInterface::ToLower(WCHAR * lpText)
{
	for (WCHAR * p = lpText; *p != _T('\0'); p++)
		*p = ToLower(*p);
}

void CCommonInterface::ToLower(std::wstring& strText)
{
	for (std::wstring::iterator i = strText.begin(); i != strText.end(); ++i)
		*i = ToLower(*i);
}

inline WCHAR CCommonInterface::ToUpper(WCHAR c)
{
	return (c >= 'a' && c <= 'z') ? (c + ('A' - 'a')) : c;
}

void CCommonInterface::ToUpper(WCHAR * lpText)
{
	for (WCHAR * p = lpText; *p != _T('\0'); p++)
		*p = ToUpper(*p);
}

void CCommonInterface::ToUpper(std::wstring& strText)
{
	for (std::wstring::iterator i = strText.begin(); i != strText.end(); ++i)
		*i = ToUpper(*i);
}

// 编码Html特殊字符
void CCommonInterface::EncodeHtmlSpecialChars(std::wstring& strText)
{
	Replace(strText, _T("&"), _T("&amp;"));
	Replace(strText, _T("'"), _T("&#39;"));
	Replace(strText, _T("\""), _T("&quot;"));
	Replace(strText, _T("<"), _T("&lt;"));
	Replace(strText, _T(">"), _T("&gt;"));
	Replace(strText, _T(" "), _T("&nbsp;"));
}

// 解码Html特殊字符
void CCommonInterface::DecodeHtmlSpecialChars(std::wstring& strText)
{
	Replace(strText, _T("&#39;"), _T("'"));
	Replace(strText, _T("&quot;"), _T("\""));
	Replace(strText, _T("&lt;"), _T("<"));
	Replace(strText, _T("&gt;"), _T(">"));
	Replace(strText, _T("&nbsp;"), _T(" "));
	Replace(strText, _T("&amp;"), _T("&"));
}

std::wstring CCommonInterface::GetBetweenString(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd)
{
	std::wstring strText;

	if (NULL == pStr)
		return _T("");

	const TCHAR * p1 = _tcschr(pStr, cStart);
	if (NULL == p1)
		return _T("");

	const TCHAR * p2 = _tcschr(p1 + 1, cEnd);
	if (NULL == p2)
		return _T("");

	int nLen = p2 - (p1 + 1);
	if (nLen <= 0)
		return _T("");

	TCHAR * lpText = new TCHAR[nLen + 1];
	if (NULL == lpText)
		return _T("");

	memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
	_tcsncpy(lpText, p1 + 1, nLen);
	strText = lpText;
	delete[]lpText;

	return strText;
}

int CCommonInterface::GetBetweenInt(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd, int nDefValue/* = 0*/)
{
	std::wstring strText = GetBetweenString(pStr, cStart, cEnd);
	if (!strText.empty() && IsDigit(strText.c_str()))
		return _tcstol(strText.c_str(), NULL, 10);
	else
		return nDefValue;
}

std::wstring CCommonInterface::GetBetweenString(const WCHAR * pStr, const WCHAR * pStart, const WCHAR * pEnd)
{
	std::wstring strText;

	if (NULL == pStr || NULL == pStart || NULL == pEnd)
		return _T("");

	int nStartLen = _tcslen(pStart);

	const TCHAR * p1 = _tcsstr(pStr, pStart);
	if (NULL == p1)
		return _T("");

	const TCHAR * p2 = _tcsstr(p1 + nStartLen, pEnd);
	if (NULL == p2)
		return _T("");

	int nLen = p2 - (p1 + nStartLen);
	if (nLen <= 0)
		return _T("");

	TCHAR * lpText = new TCHAR[nLen + 1];
	if (NULL == lpText)
		return _T("");

	memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
	_tcsncpy(lpText, p1 + nStartLen, nLen);
	strText = lpText;
	delete[]lpText;

	return strText;
}

int CCommonInterface::GetBetweenInt(const WCHAR * pStr, const WCHAR * pStart,
	const WCHAR * pEnd, int nDefValue/* = 0*/)
{
	std::wstring strText = GetBetweenString(pStr, pStart, pEnd);
	if (!strText.empty() && IsDigit(strText.c_str()))
		return _tcstol(strText.c_str(), NULL, 10);
	else
		return nDefValue;
}

BOOL CCommonInterface::DllRegisterServer(LPCTSTR lpszFileName, BOOL bUnregister)
{
	typedef HRESULT(WINAPI * FREG)();

	BOOL bRet = FALSE;

	HMODULE hDLL = ::LoadLibrary(lpszFileName);
	if (NULL == hDLL)
		return FALSE;

	CHAR * lpszFuncName;
	if (!bUnregister)
		lpszFuncName = "DllRegisterServer";
	else
		lpszFuncName = "DllUnregisterServer";

	FREG lpfunc = (FREG)::GetProcAddress(hDLL, lpszFuncName);
	if (lpfunc != NULL)
	{
		lpfunc();
		bRet = TRUE;
	}

	::FreeLibrary(hDLL);

	return bRet;
}

BOOL CCommonInterface::DllRegisterServer(LPCTSTR lpszFileName)
{
	return DllRegisterServer(lpszFileName, FALSE);
}

BOOL CCommonInterface::DllUnregisterServer(LPCTSTR lpszFileName)
{
	return DllRegisterServer(lpszFileName, TRUE);
}

//获取视频第5秒的截图
bool CCommonInterface::GetVideoPic(std::wstring lsVideoPath, std::wstring &lsPicPath, int iTimes, std::wstring strCachePath)
{
	//ffmpeg -ss 5 -i test.mp4 -f image2 -y  -s 350x240 test1.jpg
	//如果没有指定本地路径，先要获取一个临时路径用于存储生成的图片
	if (lsPicPath.length() <= 0)
	{
		wstring strFilePath = strCachePath;
		strFilePath += _T("\\TempPic\\");
		if (!PathFileExists(strFilePath.c_str()))
		{
			CCommonInterface comter;
			comter.CreateDir(strFilePath);
		}
		SYSTEMTIME stime = { 0 };
		GetLocalTime(&stime);
		TCHAR szCurrentTime[MAX_PATH] = { 0 };
		_stprintf(szCurrentTime, _T("%s%02d%02d%02d%02d%02d%04d.jpg"),
			_T("Pic"), stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds);
		strFilePath += szCurrentTime;
		lsPicPath = strFilePath;
	}
	CCommonInterface ComInterface;
	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = _T("open");
	sei.nShow = SW_NORMAL;
	TCHAR szCacheUpdateTempPath[MAX_PATH] = { 0 };
	wstring strFfmpegPath = strCachePath;
	strFfmpegPath += _T("\\Ffmpeg\\ffmpeg.exe");
	TCHAR szParames[MAX_PATH * 2] = { 0 };
	StringCchPrintf(szParames, MAX_PATH * 2, _T("-ss %d -i \"%s\" -f image2 -y -s 854x480 \"%s\""),
		iTimes,
		lsVideoPath.c_str(),
		lsPicPath.c_str());
	sei.lpFile = strFfmpegPath.c_str();
	sei.lpParameters = szParames;
	sei.nShow = SW_HIDE;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	//GetLogFun(Info)(_T("....ffmpeg的参数%s"), sei.lpParameters);
	if (ShellExecuteEx(&sei))
	{
		//等待进程结束
		WaitForSingleObject(sei.hProcess, INFINITE);
		return true;
	}
	return false;
}
//GUID GetFileTypeGuidByExtension(const WCHAR * lpExtension)
//{
//	GUID guid = GUID_NULL;
//
//	if (NULL == lpExtension)
//		return guid;
//
//	UINT numEncoders = 0, size = 0;
//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
//	if (status != Gdiplus::Ok)
//		return guid;
//
//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
//	if (NULL == lpEncoders)
//		return guid;
//
//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
//	if (Gdiplus::Ok == status)
//	{
//		for (UINT i = 0; i < numEncoders; i++)
//		{
//			BOOL bFind = FALSE;
//			const WCHAR * pStart = lpEncoders[i].FilenameExtension;
//			const WCHAR * pEnd = wcschr(pStart, L';');
//			do
//			{
//				if (NULL == pEnd)
//				{
//					LPCWSTR lpExt = ::wcsrchr(pStart, L'.');
//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
//					{
//						guid = lpEncoders[i].FormatID;
//						bFind = TRUE;
//					}
//					break;
//				}
//
//				int nLen = pEnd - pStart;
//				if (nLen < MAX_PATH)
//				{
//					WCHAR cBuf[MAX_PATH] = { 0 };
//					wcsncpy(cBuf, pStart, nLen);
//					LPCWSTR lpExt = ::wcsrchr(cBuf, L'.');
//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
//					{
//						guid = lpEncoders[i].FormatID;
//						bFind = TRUE;
//						break;
//					}
//				}
//				pStart = pEnd + 1;
//				if (L'\0' == *pStart)
//					break;
//				pEnd = wcschr(pStart, L';');
//			} while (1);
//			if (bFind)
//				break;
//		}
//	}
//
//	free(lpEncoders);
//
//	return guid;
//}

//CLSID GetEncoderClsidByExtension(const WCHAR * lpExtension)
//{
//	CLSID clsid = CLSID_NULL;
//
//	if (NULL == lpExtension)
//		return clsid;
//
//	UINT numEncoders = 0, size = 0;
//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
//	if (status != Gdiplus::Ok)
//		return clsid;
//
//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
//	if (NULL == lpEncoders)
//		return clsid;
//
//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
//	if (Gdiplus::Ok == status)
//	{
//		for (UINT i = 0; i < numEncoders; i++)
//		{
//			BOOL bFind = FALSE;
//			const WCHAR * pStart = lpEncoders[i].FilenameExtension;
//			const WCHAR * pEnd = wcschr(pStart, L';');
//			do
//			{
//				if (NULL == pEnd)
//				{
//					LPCWSTR lpExt = ::wcsrchr(pStart, L'.');
//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
//					{
//						clsid = lpEncoders[i].Clsid;
//						bFind = TRUE;
//					}
//					break;
//				}
//
//				int nLen = pEnd - pStart;
//				if (nLen < MAX_PATH)
//				{
//					WCHAR cBuf[MAX_PATH] = { 0 };
//					wcsncpy(cBuf, pStart, nLen);
//					LPCWSTR lpExt = ::wcsrchr(cBuf, L'.');
//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
//					{
//						clsid = lpEncoders[i].Clsid;
//						bFind = TRUE;
//						break;
//					}
//				}
//				pStart = pEnd + 1;
//				if (L'\0' == *pStart)
//					break;
//				pEnd = wcschr(pStart, L';');
//			} while (1);
//			if (bFind)
//				break;
//		}
//	}
//
//	free(lpEncoders);
//
//	return clsid;
//}
//
//// ImageFormatBMP, ImageFormatJPEG, ImageFormatPNG, ImageFormatGIF, ImageFormatTIFF
//CLSID GetEncoderClsidByFileType(REFGUID guidFileType)
//{
//	CLSID clsid = CLSID_NULL;
//
//	UINT numEncoders = 0, size = 0;
//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
//	if (status != Gdiplus::Ok)
//		return clsid;
//
//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
//	if (NULL == lpEncoders)
//		return clsid;
//
//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
//	if (Gdiplus::Ok == status)
//	{
//		for (UINT i = 0; i < numEncoders; i++)
//		{
//			if (lpEncoders[i].FormatID == guidFileType)
//				clsid = lpEncoders[i].Clsid;
//		}
//	}
//
//	free(lpEncoders);
//
//	return clsid;
//}
//
//// image/bmp, image/jpeg, image/gif, image/tiff, image/png
//CLSID GetEncoderClsidByMimeType(const WCHAR * lpMineType)
//{
//	CLSID clsid = CLSID_NULL;
//
//	if (NULL == lpMineType)
//		return clsid;
//
//	UINT numEncoders = 0, size = 0;
//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
//	if (status != Gdiplus::Ok)
//		return clsid;
//
//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
//	if (NULL == lpEncoders)
//		return clsid;
//
//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
//	if (Gdiplus::Ok == status)
//	{
//		for (UINT i = 0; i < numEncoders; i++)
//		{
//			if (wcsicmp(lpEncoders[i].MimeType, lpMineType) == 0)
//				clsid = lpEncoders[i].Clsid;
//		}
//	}
//
//	free(lpEncoders);
//
//	return clsid;
//}
//utils move end

