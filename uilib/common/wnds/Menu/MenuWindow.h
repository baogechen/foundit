//////////////////////////////////////////////////////////////////////////
//
//	����:   �˵�����
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
#include "hdmenu.h"
#include <vector>
#include "common/utils/CanvasDC.h"

#include "common/utils/ImageEx/Shared_ImageEx.h"

class CMenuWindowMannger ;

// CMenuWindow
#define ST_MENUWINDOW	_T("St.MenuWindow")

class CMenuWindow : public CWnd
{
	DECLARE_DYNAMIC(CMenuWindow)

public:
	CMenuWindow(CMenuWindowMannger * pMgr, BOOL bRootMenu);
	virtual ~CMenuWindow();

public:
	void SetImage(
		CCanvasDC * pBkgnd,
		CCanvasDC * pItemHotBkgnd
		) ;
	void SetFont(HFONT hFont) { m_hFont = hFont ; }
	void SetFlags(unsigned int uFlags) { m_uFlags = uFlags ; }
	void SetExcludeRect(LPRECT lprcExclude)
	{
		if (lprcExclude != NULL)
			::SetRect(&m_rcExclude, lprcExclude->left, lprcExclude->top, lprcExclude->right, lprcExclude->bottom) ;
	}
	void SetParentMenuItemHeight(int nHeight) { m_nParentMenuItemHeight = nHeight ; }

	BOOL Create(hdui::PMENUCORE pMenuCore, int x, int y, HWND hParent) ;

protected:
// 	enum POPTREND ;
// 	void SetPopTrend(POPTREND enPopTrend) { m_enPopTrend = enPopTrend ; }

protected:
	// ���ݲ˵����ݼ��㲼��
	void CalcLayout(SIZE &sz, hdui::PMENUCORE pMenuCore) ;
	// ���ݴ�С�ͱ��������λ��
	void CalcPos(
		POINT &pos,
		//POPTREND &enPopTrend,
		unsigned int &uFlags,
		SIZE const &sz,
		RECT const &rcExclude
		) ;

	struct ITEMINFO ;

	// �����Ӳ˵�
	void PopSubMenuWindow(ITEMINFO * pMenuItem) ;
	// �����Ӳ˵�
	void PushSubMenuWindow() ;

	// ׼�������Ӳ˵�
	void PreparePopSubMenu(ITEMINFO * pItem) ;
	// ֹͣ�����Ӳ˵�����
	void StopPopSubMenu() ;

	// ���Ʋ˵�
	void DrawMenu(HDC hDC, RECT const &rcMenu) ;

	// ���Ʊ���
	void DrawBkgnd(HDC hdcBkgnd, RECT const &rcBkgnd) ;

	// ���Ʋ˵���
	void DrawItem(HDC hDC, ITEMINFO * pItem) ;

	// ������ͼ
	void UpdateView() ;

protected:
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	CMenuWindowMannger * m_pMannger ;

	// ����
	hdui::PMENUCORE m_pMenuCore ;

	// �Ƿ���˵�
	BOOL m_bRootMenu ;

	// ���
	unsigned int m_uFlags ;			// ����λ�á����Ҽ���Ӧ

	// ��������
// 	enum POPTREND
// 	{
// 		LEFT, RIGHT
// 	} ;
// 	POPTREND m_enPopTrend ;

	// ���˵���߶�
	int m_nParentMenuItemHeight ;

	// ���ɵ����ľ�������
	RECT m_rcExclude ;

	// �ߴ�
	RECT m_rcMargin ;

	int m_nItemHeight ;				// ��ͨ�˵���߶�
	int m_nSeparatorItemHeight ;	// �ָ����˵���߶�

	int m_nIconAreaWidth ;			// ͼ��������
	int m_nIconSide ;				// ͼ��߳�
	int m_nTextLeftMargin ;			// �������Ե
	int m_nTextRightMargin ;		// �����ұ�Ե
	int m_nPopArrowRightMargin ;	// ������ͷ�ұ�Ե

	int m_nMenuWindowInterval ;		// �˵����ڼ��

	// ͼƬ
	//Image * m_pImgBkgnd ;
	CCanvasDC * m_pBkgnd ;
	CCanvasDC * m_pItemHotBkgnd ;

	// ����
	HFONT m_hFont ;
	CCanvasDC m_cvBkgnd ;

	COLORREF m_crTextNormal ;
	COLORREF m_crTextHot ;
	COLORREF m_crBkgndNormal ;
	COLORREF m_crBkgndHot ;

	struct ITEMINFO
	{
		enum STATUS
		{
			IS_NORMAL	= 0x00,
			IS_HOT		= 0x01,
			IS_POP		= 0x02,
		};

		hdui::PMENUITEMCORE pCore ;
		RECT rcItem ;
		DWORD dwStatus ;
		Shared_ImageEx imgIcon ;
	};

	typedef std::vector<ITEMINFO*> VCT_PITEM ;
	VCT_PITEM m_vctItems ;

	// ��꾭���Ĳ˵���
	ITEMINFO * m_pHotItem ;

	// �����Ӳ˵��Ĳ˵���
	ITEMINFO * m_pPopItem ;

	// �Ӳ˵�
	BOOL m_bPop ;
	ITEMINFO * m_pPopMenuItem ;
	HWND m_hPopMenuWindow ;

	// ׼�������Ӳ˵�
	UINT_PTR m_uTimerShowSubMenu ;
	ITEMINFO * m_pPreparePopItem ;

};


