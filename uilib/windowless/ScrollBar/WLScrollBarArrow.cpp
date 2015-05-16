#include "StdAfx.h"
#include "WLScrollBarArrow.h"
#include "common/MemoryLeakChecker.h"
WLScrollBarArrowImp::WLScrollBarArrowImp(void)
:
m_uFirstLDownTimer(TIMERID_SCROLLBAR_ARROW_FIRSTDOWN)
, m_uKeepLDownTimer(TIMERID_SCROLLBAR_ARROW_KEEPDOWN)
{
}

WLScrollBarArrowImp::~WLScrollBarArrowImp(void)
{
}

//////////////////////////////////////////////////////////////////////////
// IWndLess Implement

BOOL WLScrollBarArrowImp::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) 
{
	BOOL bRes = __super::OnWndMsg(message, wParam, lParam, pResult) ;

	switch (message)
	{
	case WM_LBUTTONDOWN :
	case WM_LBUTTONDBLCLK :
		{
			// ���Ͱ��²���
			NMWLHDR info ;
			info.hWlFrom = this ;
			info.idFrom = GetID() ;
			info.code = NM_LDOWN ;
			GetParent()->SendMessage(WM_NOTIFY, GetID(), (LPARAM)&info) ;

			// ������ʱ��
			m_uFirstLDownTimer = SetTimer(m_uFirstLDownTimer, 400, NULL) ;
			if (m_uFirstLDownTimer == -1)
				TRACE(_T("WLScrollBarArrow SetTimer Error\n")) ;
		}
		break ;

	case WM_CAPTURECHANGED :
		{
			// ��ֹ��ʱ��
			if (m_uFirstLDownTimer != -1)
			{
				KillTimer(m_uFirstLDownTimer) ;
			}
			if (m_uKeepLDownTimer != -1)
			{
				KillTimer(m_uKeepLDownTimer) ;
			}
		}
		break ;

	case WM_TIMER :
		{
			if (wParam == m_uFirstLDownTimer)
			{
				KillTimer(m_uFirstLDownTimer) ;
				// �����һ�ΰ��º�һ��ʱ���ڻ��ǰ��µģ���ʼ�������Ͱ����¼�
				if (m_bLButtonDown)
				{
					m_uKeepLDownTimer = SetTimer(m_uKeepLDownTimer, 40, NULL) ;
					if (m_uKeepLDownTimer == -1)
						TRACE(_T("SetTimer Error\n")) ;
				}
			}
			else if (wParam == m_uKeepLDownTimer)
			{
				if (m_dwStatus & BS_SELECTED)
				{
					// ���Ͱ��²���
					NMWLHDR info ;
					info.hWlFrom = this ;
					info.idFrom = GetID() ;
					info.code = NM_LDOWN ;
					GetParent()->SendMessage(WM_NOTIFY, GetID(), (LPARAM)&info) ;
				}
			}
		}
		break ;
	}

	return bRes ;
}


template<>
WLScrollBarArrowImp *CreateDefaultImp< WLScrollBarArrowImp >(IWLButton * wrapper)

{
	WLScrollBarArrowImp *ret = new WLScrollBarArrowImp();
	ret->SetWrapper(wrapper);
	return ret;
};
