// uilib.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "uilib.h"

#include "include/uilib/common/utils/UIUtils.h"
#include "include/uilib/common/utils/ImageEx/Shared_ImageEx.h"
#include "include/uilib/common/utils/ImageEx/ImageEx.h"
#include "common/utils/Image2/TimerTaskPump.h"

#include "windowless/BaseWndlessWnd.h"
#include "windowless/ButtonAL/WLButtonALHelper.h"
#include "logex.h"

#if defined(_WIN32) && defined(_WINDLL)
LOG_IMPL_GLOBAL();
#endif



ULONG_PTR gdiplusToken;
bool InitUILib()
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

 	GdiplusStartupInput gdiplusStartupInput;
	if (Ok == GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL))
	{
		TimerTaskPump::Initialize() ;
		WLButtonALHelper::Initialize();
		return true;
	}

	return false;
}
void UnInitUILib()
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
	
	ImageEx::ExitGifManager();
	WLButtonALHelper::UnInitialize() ;
	TimerTaskPump::Uninitialize() ;

	GdiplusShutdown(gdiplusToken);
}



class CUIlibApp : public CWinApp
{
public:
	CUIlibApp();

	// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


BEGIN_MESSAGE_MAP(CUIlibApp, CWinApp)
END_MESSAGE_MAP()


// CuiApp ����

CUIlibApp::CUIlibApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

}

// Ψһ��һ�� CuiApp ����

CUIlibApp theApp;

// CuiApp ��ʼ��

BOOL CUIlibApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


