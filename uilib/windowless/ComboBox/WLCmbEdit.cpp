#include "StdAfx.h"
#include "WLCmbEdit.h"
#include "common/MemoryLeakChecker.h"
WLCmbEditImp::WLCmbEditImp(void)
{
}

WLCmbEditImp::~WLCmbEditImp(void)
{
}

//////////////////////////////////////////////////////////////////////////
// IWndLess Implement

BOOL WLCmbEditImp::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) 
{
	BOOL bRes = FALSE ;
	*pResult = 0 ;

	bRes = __super::OnWndMsg(message, wParam, lParam, pResult) ;

	// ������ƶ���Ϣ����������
	switch (message)
	{
	case WM_MOUSEMOVE :
		bRes = FALSE ;
		break ;

	case WM_MOUSELEAVE :
		bRes = FALSE ;
		break ;

	case WM_KEYDOWN :
		{
			if (wParam == VK_UP || wParam == VK_DOWN)
				bRes = FALSE ;
		}
		break ;
	}

	return bRes ;
}

template<>
WLCmbEditImp *CreateDefaultImp< WLCmbEditImp >(IWLEdit * wrapper)

{
	WLCmbEditImp *ret = new WLCmbEditImp();
	ret->SetWrapper(wrapper);
	return ret;
};
