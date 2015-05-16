#include "stdafx.h"
#include <shlobj.h>
#include <vector>


#include "common/utils/UIUtils.h"
#include "Ch2PyConvertor.h"

#include "utils/strutils.h"
#include "utils/Passport.h"

#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>



#include "logex.h"
#include "common/MemoryLeakChecker.h"




namespace hdutils
{

bool CheckUserID(std::wstring & wstrID)
{
	boost::trim(wstrID);
	boost::to_lower(wstrID);
	std::string passport = WTOU8(wstrID);
	bool blnRet = validatePassport(passport);
	return blnRet;
}

bool CheckPwd(const std::wstring & wstrPwd)
{
	const int nMinPWDLen = 6;
	if ( wstrPwd.length() < nMinPWDLen)
	{
		return false;
	}	
	return true;
}

// ���һ���ַ��Ƿ���ڱ༭���б���ʾ
bool IsEditPrint(TCHAR c) 
{
	return ((c >= 256) || (iswprint(c))) ;
}

void OpenURL(const std::wstring & url)
{

	CString args;
	args.Format(_T("url.dll,FileProtocolHandler %s"), url.c_str());
	HINSTANCE result = ShellExecute(NULL, _T("open"), _T("rundll32.exe"), args, NULL, SW_SHOWNORMAL);
	if ((UINT)result <= HINSTANCE_ERROR)
	{
		LOG_WARN("open url " << url << " ret: " << result);
		ShellExecute( NULL, NULL, url.c_str(), NULL, NULL, SW_SHOW);
	}
	return;

}
void OpenFolder(const std::wstring &path)
{
	ShellExecute(NULL, _T("open"), path.c_str(), NULL, NULL, SW_SHOWNORMAL);

};


bool GenDictionaryString(const std::wstring& name, std::wstring &fixedStr)
{
	bool isFirstChinese = false;
	Ch2PyConvertor::GetInstance()->Convert( name, fixedStr);
	if (name.size() && fixedStr.size() && name[0] != fixedStr[0]) 
	{
		isFirstChinese = true;
	}
	boost::to_lower(fixedStr);
	return isFirstChinese;

}

bool DictionarySorter::operator () ( const std::wstring& lc, const std::wstring& rc)
{
	std::wstring lcfix, rcfix;
	bool is_lc_chinese = GenDictionaryString(lc, lcfix);
	bool is_rc_chinese = GenDictionaryString(rc, rcfix);
	if (is_lc_chinese && !is_rc_chinese)
	{
		return false;
	}
	else if (!is_lc_chinese && is_rc_chinese)
	{
		return true;
	}
	else
	{
		return lcfix < rcfix;
	}
}

void CopyToClipBoard(const std::wstring &wstrID)
{
	if(OpenClipboard(NULL))   
	{   
		HGLOBAL   clipbuffer = NULL;   
		char*	  buffer = NULL;   
		int nSize = sizeof( TCHAR)*(wstrID.length() + 1);
		EmptyClipboard();   
		clipbuffer   =   GlobalAlloc( GMEM_MOVEABLE, nSize); 
		buffer		 =   (char*)GlobalLock(clipbuffer);   
		memcpy( buffer, (char*)U2M(wstrID).c_str(), nSize);
		GlobalUnlock(clipbuffer);   
		SetClipboardData(CF_TEXT,clipbuffer);   
		GlobalFree( clipbuffer);
		CloseClipboard();   
	} 

}

bool IsStringMarch( const std::wstring& marStr, const std::wstring& str )
{
	boost::iterator_range<std::wstring::const_iterator> ir;
	ir = boost::ifind_first(str, marStr);
	if (ir) return true;

	std::wstring pyStr, abbreviation;
	Ch2PyConvertor::GetInstance()->Convert( str, pyStr, abbreviation );

	ir = boost::ifind_first(pyStr, marStr);
	if (ir) return true;

	ir = boost::ifind_first(abbreviation, marStr);
	if (ir) return true;

	return false;
}

HRESULT GetShellFolderViewDual(ITEMIDLIST* pidl, IShellFolderViewDual** ppIShellFolderViewDual)   
{   
	IWebBrowserApp* pIWebBrowserApp;   
	IDispatch* pDoc;   
	HWND hWnd;   
	HRESULT hr;   
	HINSTANCE ghSHDOCVW;   


	HRESULT (WINAPI*gpfSHGetIDispatchForFolder)(ITEMIDLIST* pidl, IWebBrowserApp** ppIWebBrowserApp);   

	*ppIShellFolderViewDual = NULL;   

	ghSHDOCVW = LoadLibrary(_T("SHDOCVW.DLL"));   
	if (ghSHDOCVW == NULL)   
		return FALSE;   

	pIWebBrowserApp=NULL;   
	gpfSHGetIDispatchForFolder = (HRESULT (WINAPI*)(ITEMIDLIST*, IWebBrowserApp**)) GetProcAddress(ghSHDOCVW, "SHGetIDispatchForFolder");   
	if (gpfSHGetIDispatchForFolder == NULL)
	{
		FreeLibrary(ghSHDOCVW) ;
		return FALSE ;
	}

	// ����δ������API���� SHGetIDispatchForFolder
	if (SUCCEEDED(gpfSHGetIDispatchForFolder(pidl, &pIWebBrowserApp)))    
	{   
		if (SUCCEEDED(pIWebBrowserApp->get_HWND((long*)&hWnd)))    
		{   
			SetForegroundWindow(hWnd);   
			ShowWindow(hWnd, SW_SHOWNORMAL);   
		}   

		if (SUCCEEDED(hr = pIWebBrowserApp->get_Document(&pDoc)))   
		{   
			pDoc->QueryInterface(IID_IShellFolderViewDual, (void**) ppIShellFolderViewDual);   
			pDoc->Release();   
		}   

		pIWebBrowserApp->Release();   
	}
	FreeLibrary(ghSHDOCVW) ;

	return TRUE;   
}

BOOL CustomOpenFolderAndSelectItems(ITEMIDLIST *pidlFolder) 
{
	ITEMIDLIST *pidl, *pidl2;   
	ITEMIDLIST* pIdlFile;   
	USHORT cb;   
	IShellFolderViewDual* pIShellFolderViewDual;   

	HRESULT (WINAPI *gpfSHOpenFolderAndSelectItems)(LPCITEMIDLIST *pidlFolder, UINT cidl, LPCITEMIDLIST *apidl, DWORD dwFlags);   
	HINSTANCE ghShell32;

	// ֻ��WinXp�����ϼ�ϵͳ��֧��SHOpenFolderAndSelectItems() API
	// ������ϵͳ����ôʵ����������أ�ֻ�ܲ��������ķ���������
	// ������XP���ٵ�SHOpenFolderAndSelectItems()API�У���������δ���ģ�����ͬ���ķ���ȥʵ��
	// ����ϵͳ���������ʹ�ù��� VC6 .net 2003 MSDN Ollydbg v1.10���İ�   
	ghShell32 = LoadLibrary(_T("Shell32.DLL"));   
	if (ghShell32 == NULL)   
		return FALSE;   

	gpfSHOpenFolderAndSelectItems = (HRESULT (WINAPI*)(LPCITEMIDLIST*, UINT, LPCITEMIDLIST*, DWORD)) GetProcAddress(ghShell32, "SHOpenFolderAndSelectItems");   
	if (gpfSHOpenFolderAndSelectItems != NULL)   
	{   
		// ���Ի��SHOpenFolderAndSelectItems()������API��ַ
		if (SUCCEEDED(gpfSHOpenFolderAndSelectItems((LPCITEMIDLIST*)pidlFolder,0,(LPCITEMIDLIST*)NULL,0)))    
		{   
			// ֱ�ӵ���ϵͳ�Ĺ���
			FreeLibrary(ghShell32);   
			return TRUE;   
		}   
		FreeLibrary(ghShell32);   
		return FALSE;   
	}
	FreeLibrary(ghShell32);   

	// ������ϵͳ��֧��SHOpenFolderAndSelectItems()������APIʱ�Ĵ���
	// ���Ѷ���дһ����ϵͳ������ͬ�Ĵ���
	pidl = pidlFolder;   
	pIdlFile = pidl;

	// �ҳ�Ŀ���ļ���ƫ����
	while (cb = pIdlFile->mkid.cb)    
	{   
		pidl2 = pIdlFile;   
		pIdlFile = (ITEMIDLIST*)((BYTE*)pIdlFile + cb);   
	}   

	cb = pidl2->mkid.cb;   
	pidl2->mkid.cb = 0;   

	// ��Ŀ���ļ����ڵ��ļ���
	if (SUCCEEDED(GetShellFolderViewDual(pidl, &pIShellFolderViewDual)))    
	{   
		pidl2->mkid.cb = cb;   
		// 0 Deselect the item.     
		// 1 Select the item.     
		// 3 Put the item in edit mode.     
		// 4 Deselect all but the specified item.     
		// 8 Ensure the item is displayed in the view.     
		// 0x10 Give the item the focus.     
		COleVariant bszFile(pidl2);   

		if(pIShellFolderViewDual != NULL)   
		{   
			// ѡ����Ӧ��ѡ��
			pIShellFolderViewDual->SelectItem(bszFile, 0x1d);   
			pIShellFolderViewDual->Release();   
		}   
		return TRUE;   
	}   

	return FALSE;
}

void SearchTarget(LPCWSTR lpszFilePath)
{
	HRESULT hRes ;
	IShellLink * psl ;
	ITEMIDLIST * pidl ;

	CoInitialize(NULL) ;

	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
		IID_IShellLink, (LPVOID*)&psl) ;
	if (SUCCEEDED(hRes))
	{
		hRes = psl->SetPath(lpszFilePath) ;
		if (!FAILED(hRes))
		{
			hRes = psl->GetIDList(&pidl) ;
			if (!FAILED(hRes))
			{
				//SHOpenFolderAndSelectItems(pidl, 0, NULL, 0) ;
				CustomOpenFolderAndSelectItems(pidl) ;
			}
		}

		psl->Release() ;
	}

	CoUninitialize() ;
}

void SearchTarget2(LPCWSTR lpszFilePath)
{
	HRESULT hRes ;
	IShellLink * psl ;
	ITEMIDLIST * pidl ;

	CoInitialize(NULL) ;

	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
		IID_IShellLink, (LPVOID*)&psl) ;
	if (SUCCEEDED(hRes))
	{
		hRes = psl->SetPath(lpszFilePath) ;
		if (!FAILED(hRes))
		{
			hRes = psl->GetIDList(&pidl) ;
			if (!FAILED(hRes))
			{
				SHOpenFolderAndSelectItems(pidl, 0, NULL, 0) ;
			}
		}

		psl->Release() ;
	}

	CoUninitialize() ;
}


bool GetWindowsDir(int dirid,std::wstring &wstrPath)
{
	WCHAR Path[MAX_PATH]={ 0 };
	LPITEMIDLIST lpItemIDList;
	SHGetSpecialFolderLocation(0,dirid, &lpItemIDList);
	SHGetPathFromIDList(lpItemIDList,Path);
	if (Path[0] == '\0')
	{
		return false;
	}
	wstrPath = Path;
	return true;
}
/*
std::wstring GetStatusStr(LibIM::ContactStatus status)
{
	switch (status)
	{
		case LibIM::STATUS_AVAILABLE :
				return _T("����");
		case LibIM::STATUS_BUSY :
				return _T("æµ");
		case LibIM::STATUS_AWAY :
				return _T("�뿪");
		case LibIM::STATUS_DONTDISTURB :
				return _T("�������");
		default:
				return _T("����");
	}
}
*/


std::wstring GetTimeString(time_t t)
{
	std::wstring time_str;
	try
	{
		boost::posix_time::ptime utcTime = boost::posix_time::from_time_t( t );
		typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;
		boost::posix_time::ptime localTime = local_adj::utc_to_local( utcTime );
		time_str = boost::posix_time::to_iso_extended_wstring( localTime ).c_str();

		boost::replace_all(time_str, _T("T"), _T(" "));
	}
	catch (std::exception& )
	{

	}


	return time_str;
};


CPUPerformance::CPUPerformance(LPCTSTR lpszTag)
:
m_sTag(lpszTag)
{
	QueryPerformanceFrequency(&m_liFrequency) ;
	Start() ;
}

CPUPerformance::~CPUPerformance()
{
	Tick() ;
}

BOOL CPUPerformance::Start() 
{
	return QueryPerformanceCounter(&m_liStart) ;
}

BOOL CPUPerformance::Tick() 
{
	LARGE_INTEGER liNow ;
	if (!QueryPerformanceCounter(&liNow))
		return FALSE ;

	DWORD dwInstruction = liNow.LowPart - m_liStart.LowPart ;
	DWORD dwTime = dwInstruction * 1000 / m_liFrequency.LowPart ;
	TRACE(_T("%s cost %u ms [%u]\n"), m_sTag, dwTime, dwInstruction) ;

	m_liStart = liNow ;

	return TRUE ;
}


bool GetModuleFileName( std::wstring& fileName )
{
	TCHAR tcFilePath[MAX_PATH*2] = { 0 };
	if( ::GetModuleFileName( NULL, tcFilePath, MAX_PATH*2))
	{
		fileName = tcFilePath;
		return true;
	}
	return false;
}

bool GetAppDirectory( std::wstring& moudleDir )
{
	// �˴���ʹ��GetCurrentDirectory, �����ڵ�ǰĿ¼�����ڴ洢�ļ����Ʋ����޸ģ� ����ȫ
	TCHAR tcFilePath[MAX_PATH*2] = { 0 };
	if( ::GetModuleFileName( NULL, tcFilePath, MAX_PATH*2))
	{
		moudleDir = tcFilePath;
		int nPos = moudleDir.find_last_of(L"\\/");
		if ( nPos > 0 )
		{
			moudleDir = moudleDir.substr(0, nPos);
			return true;
		}
	}
	return false;
}

std::map<std::string , unsigned int> g_ui_counters;

void TrackUIOperation(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
	{
		WORD wCode = HIWORD(wParam) ;
		WORD wID = LOWORD(wParam) ;
		if (lParam == 0)
		{
			// record Menu command
			if (wCode == 0)
			{
				TCHAR szName[512];
				::GetWindowText(wnd, szName, 512);
				std::string key = "ui/menu_click/" + WTOU8(szName) + ":" + u32_to_string(wID);
				if (!g_ui_counters.count(key))
				{
					g_ui_counters[key] = 1;
				}
				else
				{
					g_ui_counters[key]++;
				}
			}

		}
		else	// process control command
		{
			if (wCode == BN_CLICKED)
			{
				TCHAR szName[512];
				::GetWindowText(wnd, szName, 512);
				std::string key = "ui/btn_click/" + WTOU8(szName) + ":" + u32_to_string(wID);
				if (!g_ui_counters.count(key))
				{
					g_ui_counters[key] = 1;
				}
				else
				{
					g_ui_counters[key]++;
				}
			}
		}
	}
};

void GetUIOperationCounter( std::map<std::string , unsigned int> &counts)
{
	counts = g_ui_counters;
}

};
