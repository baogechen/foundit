//////////////////////////////////////////////////////////////////////////
//
//	����:   �޾����������ͷ��ť
//
//	
//	����:   2011/02/10
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "..\wlbutton.h"

class WLScrollBarArrowImp :
	public WLButtonImp
{
public:
	WLScrollBarArrowImp(void);
	virtual ~WLScrollBarArrowImp(void);

	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;

protected:
	UINT_PTR m_uFirstLDownTimer ;
	UINT_PTR m_uKeepLDownTimer ;
};


typedef CWLButtonShell<IWLButton, WLScrollBarArrowImp> WLScrollBarArrow;