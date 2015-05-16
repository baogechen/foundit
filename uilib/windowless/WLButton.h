//////////////////////////////////////////////////////////////////////////
//
//	����:   һ���򵥵��޾����ťʵ��
//
//	
//	����:   2010/12/24
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "wndless.h"
#include "IWLButton.h"

class WLButtonImp : public WLButtonImpBase
{
public:
	WLButtonImp();
	virtual ~WLButtonImp(void);

public:

	int GetCheck() ;
	void SetCheck(int nCheck) ;
	BOOL DrawBySelf(HDC hDC, RECT const &rcUpdate, DWORD dwStatus) ;

protected:
	BOOL CustomDraw(HDC hDC, RECT const &rcUpdate, DWORD dwStatus) ;

public:
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;

protected:
	DWORD m_dwStatus ;
	BOOL m_bLButtonDown ;
};


