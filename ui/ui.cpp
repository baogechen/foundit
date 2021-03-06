// ui.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "include/ui/ui.h"
#include "uilib/uilib.h"
#include "uilib/common/utils/UIUtils.h"
#include "utils/IProfile.h"
#include "mainwnd/MainWindow.h"
#include "logex.h"

// CuiApp

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CuiApp
// 有关此类实现的信息，请参阅 ui.cpp

#if defined(_WIN32) && defined(_WINDLL)
LOG_IMPL_GLOBAL();
#endif


class CuiApp : public CWinApp
{
public:
	CuiApp();

	// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


BEGIN_MESSAGE_MAP(CuiApp, CWinApp)
END_MESSAGE_MAP()


// CuiApp 构造

CuiApp::CuiApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CuiApp 对象

CuiApp theApp;

// CuiApp 初始化

BOOL CuiApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void RegisterHDBaseWindow()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, ST_HDBASEWINDOW, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS ;
		wndcls.lpfnWndProc		= ::DefWindowProc ;					
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0 ;
		wndcls.hInstance		= hInst ;							
		wndcls.hIcon			= (HICON)0 ;					
		wndcls.hCursor			= ::LoadCursor( NULL, IDC_ARROW ) ;	
		wndcls.hbrBackground	= (HBRUSH)(0) ;						
		wndcls.lpszMenuName		= NULL ;
		wndcls.lpszClassName	= ST_HDBASEWINDOW ;					

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
}

CMainWindow * pMainWindow = NULL;

void UIInit()
{

#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif


	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	InitUILib();
	AfxInitRichEdit2();
//	AfxOleInit();
	OleInitialize(NULL);

	RegisterHDBaseWindow();
	pMainWindow = new CMainWindow();	
}

void RunMsgLoop()
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	if (pMainWindow)
	{
		pMainWindow->Create();
		pMainWindow->RunModalLoop();
		pMainWindow->DestroyWindow();
		delete pMainWindow;
		pMainWindow = NULL;
	}

	OleUninitialize();
	UnInitUILib();


	//save ui statistic
	std::map<std::string , unsigned int> counters;
	std::map<std::string , unsigned int>::iterator it;
	hdutils::GetUIOperationCounter(counters);
	if (counters.size())
	{

		/*PropertyTreePtr pt = GetRoamProfile().createView("ui_stat");
		for (it = counters.begin(); it != counters.end(); it ++)
		{
			unsigned int old_val = pt->getUInt(it->first);
			pt->set(it->first, old_val + it->second);
		}
		GetRoamProfile().Save();
	*/}
	
};

void StopMsgLoop()
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	if (::IsWindow(pMainWindow->GetSafeHwnd()))
	{
		pMainWindow->EndModalLoop(0);
	}
};

