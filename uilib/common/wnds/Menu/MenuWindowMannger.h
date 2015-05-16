//////////////////////////////////////////////////////////////////////////
//
//	����:   �˵����ڹ�����
//			����ά����Ϣ�õ�������������Ϣ�ַ���������˵�����
//
//	
//	����:   2011/03/22
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include "hdmenu.h"
#include "common/utils/CanvasDC.h"

class CMenuWindowMannger
{
public:
	CMenuWindowMannger(void);
	~CMenuWindowMannger(void);

public:
	int Run(HHDMENU hMenu, unsigned int uFlags, int x, int y, HWND hOwner, LPRECT lprcExclude) ;

	void AppendMenuWindow(HWND hMenuWnd) ;
	void RemoveMenuWindow(HWND hMenuWnd) ;

	void Exit(int nResult) ;

protected:
	void PreTranslateMessage(LPMSG pMsg) ;

protected:
	std::list<HWND> m_listMenuWindows ;
	HWND m_hHotMenuWindow ;
	int m_nResult ;

	CCanvasDC m_cvMenuBkgnd ;
	CCanvasDC m_cvMenuItemHotBkgnd ;
	HFONT m_hFont ;

};
