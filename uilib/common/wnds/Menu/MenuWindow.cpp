// MenuWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MenuWindow.h"
#include "MenuWindowMannger.h"
#include "common/utils/Graphics.h"

#include "common/utils/ImageEx/ImageResource.h"
#include "common/utils/ImageEx/ImageEx.h"

// CMenuWindow

IMPLEMENT_DYNAMIC(CMenuWindow, CWnd)

CMenuWindow::CMenuWindow(CMenuWindowMannger * pMgr, BOOL bRootMenu)
:
m_pMannger(pMgr)
, m_pMenuCore(NULL)
, m_bRootMenu(bRootMenu)
, m_uFlags(0)
, m_nParentMenuItemHeight(0)
// , m_cvBkgnd(TRUE)
, m_hFont(NULL)
, m_crTextNormal(RGB(56, 144, 208))
, m_crTextHot(RGB(251, 251, 253))
, m_crBkgndNormal(RGB(251, 251, 253))
, m_crBkgndHot(RGB(56, 144, 208))
, m_pHotItem(NULL)
, m_pPopItem(NULL)
, m_bPop(FALSE)
, m_pPopMenuItem(NULL)
, m_hPopMenuWindow(NULL)
, m_uTimerShowSubMenu(1)
, m_pPreparePopItem(NULL)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, ST_MENUWINDOW, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS ;
		wndcls.lpfnWndProc		= ::DefWindowProc ;					// ������Ϣ������
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0 ;
		wndcls.hInstance		= hInst ;							// ʵ�����
		wndcls.hIcon			= (HICON)0 ;						// ͼ��
		wndcls.hCursor			= ::LoadCursor( NULL, IDC_ARROW ) ;	// ���
		wndcls.hbrBackground	= (HBRUSH)(0) ;						// ��ˢ(�ջ�ˢ����ɾ������)
		wndcls.lpszMenuName		= NULL ;
		wndcls.lpszClassName	= ST_MENUWINDOW ;					// ������

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}

	::SetRect(&m_rcExclude, 0, 0, 0, 0) ;
	::SetRect(&m_rcMargin, 6, 6, 6, 6) ;
	//::SetRect(&m_rcItemMargin, 5, 5, 5, 5) ;

	m_nItemHeight = 22 ;			// ��ͨ�˵���߶�
	m_nSeparatorItemHeight = 5 ;	// �ָ����˵���߶�

	m_nIconAreaWidth = 24 ;			// ͼ��������
	m_nIconSide = 16 ;				// ͼ��߳�
	m_nTextLeftMargin = 10 ;		// �������Ե
	m_nTextRightMargin = 29 ;		// �����ұ�Ե
	m_nPopArrowRightMargin = 7 ;	// ������ͷ�ұ�Ե
	m_nMenuWindowInterval = -5 ;	// �˵����ڼ��
}

CMenuWindow::~CMenuWindow()
{
	std::vector<ITEMINFO*>::iterator pos ;
	for (pos = m_vctItems.begin() ; pos != m_vctItems.end() ; ++ pos)
	{
		delete (*pos) ;
	}
}

void CMenuWindow::SetImage(
						   CCanvasDC * pBkgnd,
						   CCanvasDC * pItemHotBkgnd
						   ) 
{
	//m_pImgBkgnd = pImgBkgnd ;
	m_pBkgnd = pBkgnd ;
	m_pItemHotBkgnd = pItemHotBkgnd ;
}

BOOL CMenuWindow::Create(hdui::PMENUCORE pMenuCore, int x, int y, HWND hParent) 
{
	if (pMenuCore == NULL)
		return FALSE ;

	if (!CWnd::CreateEx(
		WS_EX_LAYERED | WS_EX_TOPMOST, 
		ST_MENUWINDOW,
		NULL,
		WS_POPUP,
		0, 0, 0, 0,
		hParent,
		NULL,
		0
		))
		return FALSE ;

	m_pMenuCore = pMenuCore ;

	// ����������MenuWindowMannger����
	//m_hFont = (HFONT)::SendMessage(hParent, WM_GETFONT, 0, 0) ;

	SIZE sz = {0} ;
	CalcLayout(sz, m_pMenuCore) ;

	POINT pos = {x, y} ;
	CalcPos(pos, m_uFlags, sz, m_rcExclude) ;

	SetWindowPos(0, pos.x, pos.y, sz.cx, sz.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW) ;

	HDC hWndDC = ::GetDC(GetSafeHwnd()) ;
	m_cvBkgnd.Create32BitDC(hWndDC, sz.cx, sz.cy) ;
	::ReleaseDC(GetSafeHwnd(), hWndDC) ;
	CRect rcBkgnd(0, 0, sz.cx, sz.cy) ;
	DrawBkgnd(m_cvBkgnd, rcBkgnd) ;

	UpdateView() ;

	return TRUE ;
}

void CMenuWindow::CalcLayout(SIZE &sz, hdui::PMENUCORE pMenuCore) 
{
	CClientDC dc(this) ;
	HGDIOBJ hOldFont = ::SelectObject(dc.GetSafeHdc(), m_hFont) ;

	sz.cx = sz.cy = 0 ;

	int nItemLeft = m_rcMargin.left ;
	int nItemTop = m_rcMargin.top ;
	int nItemWidth = 0 ;				// ��С���Ϊ0

	hdui::PMENUITEMCORE pItem = pMenuCore->pFirstItem ;
	for ( ; pItem != NULL ; pItem = pItem->pNextItem)
	{
		ITEMINFO * pII = new ITEMINFO ;
		pII->pCore = pItem ;
		pII->dwStatus = ITEMINFO::IS_NORMAL ;

		if (pItem->uStyle & MF_CHECKED)
		{
			pII->imgIcon = Shared_ImageEx::FromId(IDI_MENU_CHECK_PNG) ;
		}
		else if (pItem->uStyle & MF_RADIO)
		{
			pII->imgIcon = Shared_ImageEx::FromId(IDI_MENU_RADIO_PNG) ;
		}
		else
		{
			if (pItem->uIconId != 0)
			{
				pII->imgIcon = Shared_ImageEx::FromId(pItem->uIconId) ;
			}
		}

		RECT &rcItem = pII->rcItem ;
		rcItem.left = rcItem.right = nItemLeft ;
		rcItem.top = nItemTop ;

		int nItemHeight = 0 ;
		if (pItem->uStyle & MF_SEPARATOR)
		{
			nItemHeight = m_nSeparatorItemHeight ;
		}
		else
		{
			CSize szText = dc.GetTextExtent(pItem->szText, _tcslen(pItem->szText)) ;
			szText.cx += m_nIconAreaWidth + m_nTextLeftMargin + m_nTextRightMargin ;

			if (nItemWidth < szText.cx)
				nItemWidth = szText.cx ;

			nItemHeight = m_nItemHeight ;
		}

		nItemTop += nItemHeight ;
		rcItem.bottom = nItemTop ;

		m_vctItems.push_back(pII) ;
	}

	VCT_PITEM::iterator pos ;
	for (pos = m_vctItems.begin() ; pos != m_vctItems.end() ; ++ pos)
	{
		(*pos)->rcItem.right += nItemWidth ;
	}

	sz.cx = (m_rcMargin.left + nItemWidth + m_rcMargin.right) ;
	sz.cy = nItemTop + m_rcMargin.bottom ;

	::SelectObject(dc.GetSafeHdc(), hOldFont) ;
}

// ���ݴ�С�ͱ��������λ��
void CMenuWindow::CalcPos(
						  POINT &pos,
						  //POPTREND &enPopTrend,
						  unsigned int &uFlags,
						  SIZE const &sz,
						  RECT const &rcExclude
						  ) 
{
	// ����Ǹ��˵�����λ����Ҫ�ͳ�������룬���򣬱����m_rcMargin�ڵ�item�������
	// 1. �ȸ��ݵ�����ǡ������㡢��С����Ļ��Сȷ��λ��

	// �����Ļ��С
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN) ;
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN) ;

	int x = pos.x, y = pos.y ;
	if (uFlags & TPM_BOTTOMALIGN)
	{
		y = pos.y - (sz.cy - m_nParentMenuItemHeight) ;
		m_bRootMenu ? 0 : y += m_rcMargin.bottom ;
		if (y < 0)		// ����ռ䲻��
		{
			// �ж�����ռ��Ƿ񹻷ţ�������ţ��������
			y = pos.y ;
			m_bRootMenu ? 0 : y -= m_rcMargin.top ;
			if (y + sz.cy <= nScreenHeight)
			{
			}
			else		// ���򣬻��Ƿ�����
			{
				y = 0 ;
			}
		}
	}
	else
	{
		y = pos.y ;
		m_bRootMenu ? 0 : y -= m_rcMargin.top ;

		if (y + sz.cy > nScreenHeight)	// ����ռ䲻��
		{
			// �����ϵ�
			y = pos.y - (sz.cy - m_nParentMenuItemHeight) ;
			m_bRootMenu ? 0 : y += m_rcMargin.bottom ;
			if (y >= 0)
			{
			}
			else
			{
				y = nScreenHeight - sz.cy ;
			}
		}
	}

	if (uFlags & TPM_RIGHTALIGN)
	{
		x = pos.x - sz.cx ;
		if (x < 0)		// ��߿ռ䲻��
		{
			// �ж��ұߵĿռ��Ƿ񹻷ţ�������ţ�����ұ�
			if (pos.x + sz.cx <= nScreenWidth)
			{
				uFlags &= ~TPM_RIGHTALIGN ;
				x = pos.x ;
			}
			else		// ���򣬻��Ƿ����
			{
				x = 0 ;
			}
		}
	}
	else
	{
		x = pos.x ;
		if (x + sz.cx > nScreenWidth)
		{
			if (x >= sz.cx)
			{
				uFlags |= TPM_RIGHTALIGN ;
				x -= sz.cx ;
			}
			else
			{
				x = nScreenWidth - sz.cx ;
			}
		}
	}

	// 2. ����rcExclude����λ��
	RECT rcMenu ;
	::SetRect(&rcMenu, x, y, x + sz.cx, y + sz.cy) ;
	RECT rcInterset ;
	if (::IntersectRect(&rcInterset, &rcMenu, &rcExclude))
	{
		// �������֮ǰ��xy
		int x0 = x, y0 = y ;

		BOOL bOk = FALSE ;
		// ����ˮƽ�����ϵ���
		// ���֮ǰ���Ҷ��룬���ȵ�������ߣ��ٵ������ұߣ������෴
		if (uFlags & TPM_RIGHTALIGN)
		{
			x = rcExclude.left - sz.cx ;
			if (x >= 0)
			{
				// �����ɹ�
				bOk = TRUE ;
				uFlags |= TPM_RIGHTALIGN ;
			}
			else
			{
				x = rcExclude.right ;
				if (x + sz.cx <= nScreenWidth)
				{
					// �����ɹ�
					bOk = TRUE ;
					uFlags &= ~TPM_RIGHTALIGN ;
				}
			}
		}
		else
		{
			x = rcExclude.right ;
			if (x + sz.cx <= nScreenWidth)
			{
				// �����ɹ�
				bOk = TRUE ;
				uFlags &= ~TPM_RIGHTALIGN ;
			}
			else
			{
				// ���������
				x = rcExclude.left - sz.cx ;
				if (x >= 0)
				{
					// �����ɹ�
					bOk = TRUE ;
					uFlags |= TPM_RIGHTALIGN ;
				}
			}
		}

		// ���ҵ��������У����ڴ�ֱ�����ϵ���
		if (!bOk)
		{
			x = x0 ;
			// �������±�
			y = rcExclude.bottom ;
			if (y + sz.cy <= nScreenHeight)
			{
				// �����ɹ�
				bOk = TRUE ;
			}
			else
			{
				// �������ϱ�
				y = rcExclude.top - sz.cy ;
				if (y >= 0)
				{
					// �����ɹ�
					bOk = TRUE ;
				}
			}

			// ��������У��ָ�ԭλ��
			if (!bOk)
				y = y0 ;
		}
	}

	pos.x = x ;
	pos.y = y ;
}

void CMenuWindow::PopSubMenuWindow(ITEMINFO * pMenuItem) 
{
	PushSubMenuWindow() ;

	m_pPopItem = pMenuItem ;
	if (pMenuItem->pCore->hSubMenu == NULL)
		return ;

	pMenuItem->dwStatus |= ITEMINFO::IS_POP ;

	// �����˵�����
	CMenuWindow * pSubMenuWindow = new CMenuWindow(m_pMannger, FALSE) ;
	pSubMenuWindow->SetImage(m_pBkgnd, m_pItemHotBkgnd) ;
	pSubMenuWindow->SetFont(m_hFont) ;

	// ��(TPM_LEFTALIGN | TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON)���ݸ��¼��Ӳ˵�
	// �����ݴ�ֱ�����ϵĶ��뷽ʽ
	unsigned int uInheritance = m_uFlags & (TPM_LEFTALIGN | TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON) ;
	
	pSubMenuWindow->SetFlags(uInheritance) ;

	CRect rcWnd ;
	GetWindowRect(&rcWnd) ;
	rcWnd.InflateRect(m_nMenuWindowInterval, m_nMenuWindowInterval) ;
	pSubMenuWindow->SetExcludeRect(&rcWnd) ;
	pSubMenuWindow->SetParentMenuItemHeight(m_nItemHeight) ;

	//pSubMenuWindow->SetPopTrend(m_enPopTrend) ;

	CRect rcClient ;
	GetClientRect(&rcClient) ;
	CPoint ptBorn(rcClient.right + m_nMenuWindowInterval, m_pHotItem->rcItem.top) ;
	ClientToScreen(&ptBorn) ;

	if (!pSubMenuWindow->Create(m_pHotItem->pCore->hSubMenu, ptBorn.x, ptBorn.y, GetSafeHwnd()))
	{
		delete pSubMenuWindow ;
		pSubMenuWindow = NULL ;
		return ;
	}

	m_bPop = TRUE ;
	m_pPopMenuItem = pMenuItem ;
	m_hPopMenuWindow = pSubMenuWindow->GetSafeHwnd() ;

	m_pMannger->AppendMenuWindow(pSubMenuWindow->GetSafeHwnd()) ;
}

void CMenuWindow::PushSubMenuWindow() 
{
	if (m_bPop)
	{
		m_bPop = FALSE ;
		if (m_pPopMenuItem != NULL)
		{
			m_pPopMenuItem->dwStatus &= ~ITEMINFO::IS_POP ;
			//InvalidateRect(&m_pPopMenuItem->rcItem, FALSE) ;
			UpdateView() ;
			m_pPopMenuItem = NULL ;
		}
		::DestroyWindow(m_hPopMenuWindow) ;

		m_pPopItem = NULL ;

		m_pMannger->RemoveMenuWindow(m_hPopMenuWindow) ;
	}
}

void CMenuWindow::PreparePopSubMenu(ITEMINFO * pItem) 
{
	PushSubMenuWindow() ;
	StopPopSubMenu() ;

	if (!(pItem->pCore->uStyle & MF_GRAYED))
	{
		// ��¼׼���������Ӳ˵���Ϣ
		m_pPreparePopItem = pItem ;

		SetTimer(m_uTimerShowSubMenu, 300, NULL) ;
	}
}

void CMenuWindow::StopPopSubMenu() 
{
	KillTimer(m_uTimerShowSubMenu) ;
	m_pPreparePopItem = NULL ;
}

// ���Ʋ˵�
void CMenuWindow::DrawMenu(HDC hDC, RECT const &rcMenu) 
{
	//DrawBkgnd(hDC, rcMenu) ;
	::BitBlt(
		hDC,
		rcMenu.left, rcMenu.top, rcMenu.right - rcMenu.left, rcMenu.bottom - rcMenu.top,
		m_cvBkgnd,
		0, 0,
		SRCCOPY
		) ;

	Graphics gph(hDC) ;

	VCT_PITEM::iterator pos ;
	for (pos = m_vctItems.begin() ; pos != m_vctItems.end() ; ++ pos)
	{
		DrawItem(hDC, *pos) ;
	}
}

// ���Ʊ���
void CMenuWindow::DrawBkgnd(HDC hdcBkgnd, RECT const &rcBkgnd) 
{
// 	hdutils::SurfaceStretch(
// 		hdcBkgnd,
// 		rcBkgnd.left, rcBkgnd.top, rcBkgnd.right - rcBkgnd.left, rcBkgnd.bottom - rcBkgnd.top,
// 		m_pImgBkgnd,
// 		0, 0, m_pImgBkgnd->GetWidth(), m_pImgBkgnd->GetHeight()
// 		) ;
	hdutils::AlphaSurfaceTile(
		hdcBkgnd,
		rcBkgnd.left, rcBkgnd.top, rcBkgnd.right - rcBkgnd.left, rcBkgnd.bottom - rcBkgnd.top,
		*m_pBkgnd,
		0, 0, m_pBkgnd->GetWidth(), m_pBkgnd->GetHeight()
		) ;

	// ����ͼ������
	Graphics gph(hdcBkgnd) ;
	//SolidBrush brIcon(Color(254, 227, 233, 235)) ;
	//SolidBrush brIcon(Color(254, 209, 234, 238)) ;
	//SolidBrush brIcon(Color(254, 0xd1, 0xea, 0xee)) ;
	SolidBrush brIcon(Color(254, 0xe4, 0xf0, 0xf5)) ;
	gph.FillRectangle(&brIcon, m_rcMargin.left, m_rcMargin.top, m_nIconAreaWidth, rcBkgnd.bottom - m_rcMargin.top - m_rcMargin.bottom) ;
}

// ���Ʋ˵���
void CMenuWindow::DrawItem(HDC hDC, ITEMINFO * pItem) 
{
	RECT &rcItem = pItem->rcItem ;

	if (pItem->pCore->uStyle & MF_SEPARATOR)
	{
		Graphics gph(hDC) ;
		Pen pen(Color(254, 183, 195, 204)) ;
		gph.DrawLine(
			&pen,
			rcItem.left + m_nIconAreaWidth, (rcItem.top + rcItem.bottom) / 2, rcItem.right - 4, (rcItem.top + rcItem.bottom) / 2
			) ;
	}
	else
	{
		if ((pItem->dwStatus & (ITEMINFO::IS_HOT | ITEMINFO::IS_POP)) && !(pItem->pCore->uStyle & MF_GRAYED))
		{
			hdutils::AlphaLineTile(
				hDC,
				rcItem.left, rcItem.top, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top,
				*m_pItemHotBkgnd,
				0, 0, m_pItemHotBkgnd->GetWidth(), m_pItemHotBkgnd->GetWidth()
				) ;
		}

		Graphics gph(hDC) ;

		// ����ͼ��
// 		Rect rcIcon(
// 			rcItem.left + (m_nIconAreaWidth - m_nIconSide) / 2,
// 			(rcItem.bottom + rcItem.top - m_nIconSide) / 2,
// 			m_nIconSide,
// 			m_nIconSide
// 			) ;
		CRect rcIcon ;
		rcIcon.left = rcItem.left + (m_nIconAreaWidth - m_nIconSide) / 2 ;
		rcIcon.top = (rcItem.bottom + rcItem.top - m_nIconSide) / 2 ;
		rcIcon.right = rcIcon.left + m_nIconSide ;
		rcIcon.bottom = rcIcon.top + m_nIconSide ;

		if (pItem->imgIcon != NULL)
		{
// 			gph.DrawImage(
// 				pItem->imgIcon,
// 				rcIcon,
// 				0, 0, pItem->imgIcon->GetWidth(), pItem->imgIcon->GetHeight(), UnitPixel, NULL
// 				) ;
			hdutils::DrawImageIndeformable(
				gph,
				pItem->imgIcon,
				rcIcon
				) ;
		}

		// ��������
		Gdiplus::Font ft(hDC, m_hFont) ;
		Gdiplus::SolidBrush brText(Color(254, 0, 20, 35));
		Gdiplus::SolidBrush brTextGray(Color(254, 128, 138, 145));

		PointF pt ;
		pt.X = Gdiplus::REAL(rcItem.left + m_nIconAreaWidth + m_nTextLeftMargin) ;
		pt.Y = Gdiplus::REAL(rcItem.top + 5) ;

		if (pItem->pCore->uStyle & MF_GRAYED)
			gph.DrawString(pItem->pCore->szText, _tcslen(pItem->pCore->szText), &ft, pt, &brTextGray) ;
		else
			gph.DrawString(pItem->pCore->szText, _tcslen(pItem->pCore->szText), &ft, pt, &brText) ;

		// ���Ƽ�ͷ
		if (pItem->pCore->hSubMenu != NULL)
		{
			SolidBrush br(Color(254, 128, 140, 151)) ;
			Point arpt[3] ;
			int nTop = 6 ;
			arpt[0].X = rcItem.right - m_nPopArrowRightMargin - 4 ;
			arpt[0].Y = rcItem.top + nTop ;
			arpt[1].X = rcItem.right - m_nPopArrowRightMargin ;
			arpt[1].Y = rcItem.top + nTop + 4 ;
			arpt[2].X = rcItem.right - m_nPopArrowRightMargin - 4 ;
			arpt[2].Y = rcItem.top + nTop + 8 ;
			gph.FillPolygon(&br, arpt, 3) ;
		}
	}
}

// ������ͼ
void CMenuWindow::UpdateView() 
{
	HDC hWndDC = ::GetDC(GetSafeHwnd()) ;

	CRect rcWnd ;
	GetWindowRect(rcWnd) ;
	HDC hMemDC = ::CreateCompatibleDC(hWndDC) ;

	//HBITMAP hMemBmp = ::CreateCompatibleBitmap(hWndDC, rcWnd.Width(), rcWnd.Height()) ;
	HBITMAP hMemBmp = hdutils::CreateCompatible32Bitmap(hWndDC, rcWnd.Width(), rcWnd.Height()) ;
	HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp) ;

	CRect rcMenu(0, 0, rcWnd.Width(), rcWnd.Height()) ;

	DrawMenu(hMemDC, rcMenu) ;

	CPoint ptLeftTop = rcWnd.TopLeft();   // ���÷ֲ㴰�����꣬��ΪNULL
	CPoint ptDC(0, 0);                    // ��hMemDC�����￪ʼ
	CSize size(rcWnd.Width(), rcWnd.Height());// ���÷ֲ㴰�ڵĴ�С����ΪNULL
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER ;
	bf.AlphaFormat = AC_SRC_ALPHA ;
	bf.BlendFlags = 0 ;
	bf.SourceConstantAlpha = 255 ;   // 0~255 ͸����
	if( 0 == ::UpdateLayeredWindow(m_hWnd, hWndDC, &ptLeftTop, &size,
		hMemDC, &ptDC, 0, &bf, ULW_ALPHA ) )   // ����������������˼�������
	{
		DWORD dwErr = ::GetLastError();
	}

	::SelectObject(hMemDC, hOldMemBmp) ;
	::DeleteDC(hMemDC) ;
	::DeleteObject(hMemBmp) ;

	::ReleaseDC(GetSafeHwnd(), hWndDC) ;
}

BEGIN_MESSAGE_MAP(CMenuWindow, CWnd)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_MOUSELEAVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMenuWindow ��Ϣ�������

LRESULT CMenuWindow::OnGetFont(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_hFont ;
}

void CMenuWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nChar == VK_ESCAPE)
	{
		//TRACE(_T("%#08x Post WM_NULL : 0\n"), GetSafeHwnd()) ;
		//PostMessage(WM_NULL) ;
		m_pMannger->Exit(0) ;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CMenuWindow::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return MA_NOACTIVATE ;
}

void CMenuWindow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	CRect rcClient ;
	GetClientRect(&rcClient) ;
	DrawBkgnd(dc.GetSafeHdc(), rcClient) ;

// 	COLORREF crOld = dc.SetBkColor(m_crBkgndNormal) ;
// 	dc.ExtTextOut(0, 0, ETO_OPAQUE, rcClient, NULL, 0, NULL) ;
// 	dc.SetBkColor(crOld) ;

	HGDIOBJ hOldFont = ::SelectObject(dc.GetSafeHdc(), m_hFont) ;

	std::vector<ITEMINFO*>::iterator pos ;
	for (pos = m_vctItems.begin() ; pos != m_vctItems.end() ; ++ pos)
	{
		ITEMINFO * pItem = (*pos) ;
		RECT &rcItem = pItem->rcItem ;

		COLORREF crOldBkgnd, crBkgnd ;
		COLORREF crOldText, crText ;
		if (pItem->dwStatus & (ITEMINFO::IS_HOT | ITEMINFO::IS_POP))
		{
			crText = m_crTextHot ;
			crBkgnd = m_crBkgndHot ;
		}
		else
		{
			crText = m_crTextNormal ;
			crBkgnd = m_crBkgndNormal ;
		}

		crOldText = dc.SetTextColor(crText) ;
		crOldBkgnd = dc.SetBkColor(crBkgnd) ;

		dc.ExtTextOut(
			rcItem.left + m_nIconAreaWidth + m_nTextLeftMargin, rcItem.top + 4,
			ETO_OPAQUE,
			&rcItem,
			pItem->pCore->szText,
			_tcslen(pItem->pCore->szText),
			NULL
			) ;

		dc.SetTextColor(crOldText) ;
		dc.SetBkColor(crOldBkgnd) ;
	}

	::SelectObject(dc.GetSafeHdc(), hOldFont) ;
}

void CMenuWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	//TRACE(_T("CMenuWindow::OnMouseMove %#08x: %d, %d\n"), GetSafeHwnd(), point.x, point.y) ;
	VCT_PITEM::iterator pos ;
	for (pos = m_vctItems.begin() ; pos != m_vctItems.end() ; ++ pos)
	{
		ITEMINFO * pItem = (*pos) ;
		RECT &rcItem = pItem->rcItem ;
		DWORD &dwStatus = pItem->dwStatus ;

		if (::PtInRect(&rcItem, point))
		{
			if (m_pHotItem != pItem)
			{
				if (m_pHotItem != NULL)
				{
					m_pHotItem->dwStatus &= ~ITEMINFO::IS_HOT ;
					//InvalidateRect(&(m_pHotItem->rcItem), FALSE) ;
				}

				dwStatus |= ITEMINFO::IS_HOT ;
				//InvalidateRect(&rcItem, FALSE) ;
				UpdateView() ;

				m_pHotItem = pItem ;
			}
			if (m_pPopItem != pItem)
			{
				PreparePopSubMenu(pItem) ;
			}
			break ;
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CMenuWindow::OnMouseLeave()
{
	if (m_pHotItem != NULL)
	{
		m_pHotItem->dwStatus &= ~ITEMINFO::IS_HOT ;
		//InvalidateRect(&(m_pHotItem->rcItem), FALSE) ;
		UpdateView() ;

		m_pHotItem = NULL ;

		StopPopSubMenu() ;
	}
	CWnd::OnMouseLeave();
}

void CMenuWindow::OnCancelMode()
{
	//	__super::OnCancelMode() ;
}

void CMenuWindow::OnCaptureChanged(CWnd *pWnd)
{
	// ���˵��������к���
	// ���߳��е�������ģ̬����ʱ���������ٴ���
	DestroyWindow() ;
	m_pMannger->Exit(0) ;

	CWnd::OnCaptureChanged(pWnd);
}

void CMenuWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pHotItem != NULL)
	{
		unsigned int uStyle = m_pHotItem->pCore->uStyle ;
		if (uStyle & MF_POPUP)
		{
			if (!(m_pHotItem->dwStatus & ITEMINFO::IS_POP))
			{
				PopSubMenuWindow(m_pHotItem) ;
			}
			//TRACE(_T("Pop submenu: %#08x\n"), m_pSubMenuWindow->GetSafeHwnd()) ;
		}
		else if (uStyle & (MF_SEPARATOR | MF_GRAYED))
		{
			// �ָ���ɶ����Ҳ����
			// ��������ɶ����Ҳ����
		}
		else
		{
			//TRACE(_T("%#08x Post WM_NULL : %u\n"), GetSafeHwnd(), m_pHotItem->pCore->uCmd) ;
			//PostMessage(WM_NULL, m_pHotItem->pCore->uCmd) ;
			m_pMannger->Exit(m_pHotItem->pCore->uCmd) ;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CMenuWindow::OnRButtonUp(UINT nFlags, CPoint point)
{
	if ((m_uFlags & TPM_RIGHTBUTTON))
	{
		OnLButtonUp(nFlags, point) ;
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CMenuWindow::PostNcDestroy()
{
	// �Ǹ������Զ�����
	if (!m_bRootMenu)
		delete this ;
	CWnd::PostNcDestroy();
}

void CMenuWindow::OnDestroy()
{
	CWnd::OnDestroy();

	//m_pMannger->RemoveMenuWindow(GetSafeHwnd()) ;
	// TODO: �ڴ˴������Ϣ����������
}

void CMenuWindow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == m_uTimerShowSubMenu)
	{
		if (m_pHotItem == m_pPreparePopItem)
		{
			if (!(m_pHotItem->dwStatus & ITEMINFO::IS_POP))
			{
				PopSubMenuWindow(m_pHotItem) ;
			}
		}

		KillTimer(m_uTimerShowSubMenu) ;
	}
	CWnd::OnTimer(nIDEvent);
}
