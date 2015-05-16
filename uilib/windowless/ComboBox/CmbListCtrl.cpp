// CmbListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CmbListCtrl.h"
#include "common/utils/Graphics.h"
#include "common/MemoryLeakChecker.h"

// CCmbListCtrl

IMPLEMENT_DYNAMIC(CCmbListCtrl, CWnd)

CCmbListCtrl::CCmbListCtrl() : m_pImgDelBtn(NULL)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, ST_CMBLISTCTRL, &wndcls)))
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
		wndcls.lpszClassName	= ST_CMBLISTCTRL ;					// ������

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}

	m_pResData = NULL ;

	// ��ʼ������
	m_crFrame = RGB(0, 147, 209) ;

	m_ftFontNormal.CreateFont(
		16,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH,				// nPitchAndFamily
		_T("MS Reference sans serif")
		) ;

	m_crTextColorNormal = hdutils::crTextColor ;
	m_crBackColorNormal = RGB(255, 255, 255) ;
	m_crTextColorHovered = RGB(255, 255, 255) ;
	m_crBackColorHovered = RGB(108, 198, 255) ;

	// ���ڱ߿�ռ1������
	m_nItemPosOrigin	= 1 ;
	m_nTrackItemIndex	= -1 ;
	m_nTextOffsetLeft	= 4 ;
	m_nBtnOffsetRight	= 6 ;
	m_nBtnWidth			= 0 ;
	m_nBtnHeight		= 0 ;
	m_nBtnPushedItem	= -1 ;
	m_bBtnPushed		= FALSE ;

	m_bBeginHandleLBtnUp = FALSE ;

	m_nItemHeightNormal = 32 ;
}

CCmbListCtrl::~CCmbListCtrl()
{
}

int CCmbListCtrl::Popup(
						int x, int y, int cx, int cy,
						HWND hParent,
						RESDATA &res
						) 
{
	BOOL bRes = CWnd::CreateEx(
		WS_EX_TOPMOST, 
		ST_CMBLISTCTRL,
		NULL,
		WS_POPUP,
		x, y, cx, cy,
		hParent,
		NULL,
		0
		) ;
	m_rcClient.SetRect(0, 0, cx, cy) ;

	m_pResData = &res ;

	if (!bRes)
	{
		return -1 ;
	}

	AutoAdjustSize() ;
	SetWindowPos(
		NULL,
		0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW
		) ;

	if (GetCapture() != this)
		SetCapture() ;

	m_nFlags |= (WF_MODALLOOP|WF_CONTINUEMODAL) ;
	MSG msg ;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_NULL)
		{
			if (!ContinueModal())
				break ;
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE || msg.wParam == VK_RETURN)
			{
				break ;
			}
		}

		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}

	DestroyWindow() ;

	return 0 ;
}

void CCmbListCtrl::LoadImage(
							Image * pImgDelBtn
							) 
{
	if (pImgDelBtn == NULL || pImgDelBtn->GetLastStatus() != Ok)
		return ;

	m_pImgDelBtn = pImgDelBtn ;
	m_nBtnWidth = m_pImgDelBtn->GetWidth() / 3 ;
	m_nBtnHeight = m_pImgDelBtn->GetHeight() ;
}

// ����ַ���
int CCmbListCtrl::AddString(LPCTSTR lpszString) 
{
	// �������
	LCITEM item(lpszString) ;
	if (m_vecItems.size() > 0)
	{
		item.m_nPosition = m_vecItems.back().m_nPosition + m_vecItems.back().m_nCurHeight ;
	}
	else
	{
		item.m_nPosition = 0 ;
	}
	item.m_nCurHeight = m_nItemHeightNormal ;
	item.m_nHeight = m_nItemHeightNormal ;

	m_vecItems.push_back(item) ;

	// ������ھ����Ч����ˢ�½���
	if (::IsWindow(m_hWnd))
	{
		InvalidateRect(NULL, FALSE) ;
	}

	return m_vecItems.size() - 1 ;
}

// �������ܣ��������
// ���������
// �� �� ֵ��
// ��д�˼����� ���֬E��[08/22/2009]
int CCmbListCtrl::HitTest(CPoint point) 
{
	if (point.x < m_rcClient.left || point.x >= m_rcClient.right
		|| point.y < m_rcClient.top || point.y >= m_rcClient.bottom)
		return -1 ;

	VEC_LCITEM::iterator pos ;
	for (pos = m_vecItems.begin() ; pos != m_vecItems.end() ; ++ pos)
	{
		if (IsItemVisible(*pos))
			break ;
	}

	for (pos = m_vecItems.begin() ; pos != m_vecItems.end() ; ++ pos)
	{
		if (pos->m_nPosition + m_nItemPosOrigin <= point.y 
			&& pos->m_nPosition + m_nItemPosOrigin + pos->m_nCurHeight > point.y)
			return pos - m_vecItems.begin() ;
	}

	return -1 ;
}

// �������ܣ��ж������Ƿ�ɼ�
// ���������
// �� �� ֵ��
// ��д�˼����� ���֬E��[08/22/2009]
BOOL CCmbListCtrl::IsItemVisible(UINT nIndex) 
{
	if (nIndex < 0 || nIndex >= m_vecItems.size())
		return FALSE ;

	return IsItemVisible(m_vecItems[nIndex]) ;
}

// �������ܣ��ж������Ƿ�ɼ�
// ���������
// �� �� ֵ��
// ��д�˼����� ���֬E��[08/22/2009]
BOOL CCmbListCtrl::IsItemVisible(LCITEM const & item) 
{
	CRect rcClient ;
	GetClientRect(rcClient) ;

	if (item.m_nPosition + m_nItemPosOrigin >= rcClient.bottom
		|| item.m_nPosition + m_nItemPosOrigin + item.m_nCurHeight <= rcClient.top)
		return FALSE ;

	return TRUE ;
}

// �������ܣ���������
// ���������nIndexΪ-1ʱ��ʾ���ѡ��
// �� �� ֵ��
// ��д�˼����� ���֬E��[08/22/2009]
int CCmbListCtrl::SetCurSel(int nIndex) 
{
	if (nIndex < -1 || nIndex >= (int)m_vecItems.size())
		return CB_ERR ;
	//	if (nIndex == -1)
	//		return FALSE ;

	if (nIndex != m_nTrackItemIndex)
	{
		if (m_nTrackItemIndex != -1)
		{
			m_vecItems[m_nTrackItemIndex].m_uState = LCIS_NORMAL ;
		}

		if (nIndex != -1)
		{
			m_vecItems[nIndex].m_uState = LCIS_PRIHOVERED ;
		}

		m_nTrackItemIndex = nIndex ;
	}

	// ���½���
	if (::IsWindow(GetSafeHwnd()))
		InvalidateRect(NULL, FALSE) ;

	return nIndex ;
}

// ��������߶�
void CCmbListCtrl::SetItemHeight(int nHeight) 
{
	m_nItemHeightNormal = nHeight ;
}

// �������ܣ�����Ӧ��С
// ���������
// �� �� ֵ��
// ��д�˼����� ���֬E��[08/23/2009]
void CCmbListCtrl::AutoAdjustSize() 
{
	if (!IsWindow(m_hWnd))
		return ;

	if (!m_vecItems.empty())
	{
		CRect rcClient ;
		GetClientRect(rcClient) ;

		CClientDC dc(this) ;
		CFont * pOldFont = dc.SelectObject(&m_ftFontNormal) ;

		int nHeight = 2 ;	// ��������߿�߶�
		int nWidth = rcClient.Width() ;	// ��¼��С���
		CSize szTemp ;
		VEC_LCITEM::iterator pos ;
		for (pos = m_vecItems.begin() ; pos != m_vecItems.end() ; ++ pos)
		{
			nHeight += pos->m_nCurHeight ;
			//			nHeight += m_nItemHeightHovered ;

			szTemp = dc.GetTextExtent(pos->m_strText) ;
			int item_width = szTemp.cx + m_nTextOffsetLeft + m_nBtnOffsetRight + m_nBtnWidth + m_nBtnOffsetRight + 2 ;
			if (nWidth < item_width)
				nWidth = item_width ;
		}

		dc.SelectObject(pOldFont) ;

		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE) ;
	}
}


BEGIN_MESSAGE_MAP(CCmbListCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()



// CCmbListCtrl ��Ϣ�������

void CCmbListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	m_rcClient.SetRect(0, 0, cx, cy) ;
}

void CCmbListCtrl::OnCaptureChanged(CWnd *pWnd)
{
	if (m_nFlags & (WF_MODALLOOP|WF_CONTINUEMODAL))
		EndModalLoop(0);
	CWnd::OnCaptureChanged(pWnd);
}

// �������ܣ���������
// ���������
// �� �� ֵ��
// ��д�˼����� ���֬E��[08/22/2009]
void CCmbListCtrl::DrawItem(CDC * pDC, LCITEM const & item) 
{
	CRect rcItem(
		m_rcClient.left + 1, m_nItemPosOrigin + item.m_nPosition,
		m_rcClient.right - 1, m_nItemPosOrigin + item.m_nPosition + item.m_nCurHeight) ;
	
	COLORREF crText, crBack ;

//	TRACE(_T("%s: %d\n"), item.m_strText, item.m_uState) ;
	if (item.m_uState == LCIS_PRIHOVERED)
	{
//		TRACE(_T("CurSel : %s\n"), item.m_strText) ;
		crText = m_crTextColorHovered ;
		crBack = m_crBackColorHovered ;
	}
	else
	{
		crText = m_crTextColorNormal ;
		crBack = m_crBackColorNormal ;
	}
	
	// ���Ʊ�����
	CRect rcBack(rcItem) ;
	rcBack.DeflateRect(1, 1) ;
	CBrush brTemp(crBack) ;
	pDC->FillRect(rcBack, &brTemp) ;
	
	int nOldMode = pDC->SetBkMode(TRANSPARENT) ;
	
	// ��������
	CRect rcText(rcItem) ;
	rcText.DeflateRect(1, 1) ;
	rcText.left += m_nTextOffsetLeft ;
	COLORREF crOldText = pDC->SetTextColor(crText) ;
	pDC->DrawText(item.m_strText, rcText, DT_VCENTER | DT_SINGLELINE) ;
	pDC->SetTextColor(crOldText) ;

	// ���ư�ť
	if (m_pImgDelBtn != NULL && item.m_uState == LCIS_PRIHOVERED)
	{
		CPoint ptBtn ;
		ptBtn.x = rcItem.right - m_nBtnOffsetRight - m_nBtnWidth ;
		ptBtn.y = (rcItem.top + rcItem.bottom - m_nBtnHeight) / 2 ;

		int nXOffset = 0 ;
		switch(item.m_uBtnState)
		{
		case LCIBS_NORMAL :
			break ;

		case LCIBS_HOVERED :
			nXOffset = m_nBtnWidth ;
			break ;

		case LCIBS_PUSHED :
			nXOffset = m_nBtnWidth + m_nBtnWidth ;
			break ;

		default :
			break ;
		}

		Graphics gph(pDC->GetSafeHdc()) ;
		gph.DrawImage(
			m_pImgDelBtn,
			Rect(ptBtn.x, ptBtn.y, m_nBtnWidth, m_nBtnHeight),
			nXOffset, 0, m_nBtnWidth, m_nBtnHeight,
			UnitPixel,
			NULL
			) ;
	}

	pDC->SetBkMode(nOldMode) ;
}

void CCmbListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// ������������
	CDC dcMem ;
	dcMem.CreateCompatibleDC(&dc) ;
	CBitmap bmpMem ;
	bmpMem.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height()) ;
	
	CBitmap * pOldBmp = dcMem.SelectObject(&bmpMem) ;
	CFont * pOldFont = dcMem.SelectObject(&m_ftFontNormal) ;
	
	// ����ɫ����
	dcMem.FillSolidRect(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, RGB(255, 255, 255)) ;

	// ���Ʊ߿�
	CPen penFrame(PS_SOLID, 1, m_crFrame) ;
	CPen * pOldPen = dcMem.SelectObject(&penFrame) ;

	dcMem.MoveTo(m_rcClient.TopLeft()) ;
	dcMem.LineTo(m_rcClient.right - 1, m_rcClient.top) ;
	dcMem.LineTo(m_rcClient.right - 1, m_rcClient.bottom - 1) ;
	dcMem.LineTo(m_rcClient.left, m_rcClient.bottom - 1) ;
	dcMem.LineTo(m_rcClient.TopLeft()) ;

	dcMem.SelectObject(pOldPen) ;

	// ��������
	VEC_LCITEM::iterator pos ;
	BOOL bBeginDraw = FALSE ;
	for (pos = m_vecItems.begin() ; pos != m_vecItems.end() ; ++ pos)
	{
		// �������ɼ���������
		if (IsItemVisible(*pos))
		{
			bBeginDraw = TRUE ;

			// ��������
			DrawItem(&dcMem, *pos) ;
		}
		else if (bBeginDraw)		// ����ѿ�ʼ���ƺ�������ɼ������Ժ��ٻ���
			break ;
	}

	// ����
	dc.BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(),
		&dcMem, 0, 0, SRCCOPY) ;

	// ѡ�ؾ�����
	dcMem.SelectObject(pOldFont) ;	
	dcMem.SelectObject(pOldBmp) ;
}

void CCmbListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bBeginHandleLBtnUp)
	{
		if (m_rcClient.PtInRect(point))
			m_bBeginHandleLBtnUp = TRUE ;

		CWnd::OnMouseMove(nFlags, point);
		return ;
	}

	if (!m_bBtnPushed)
	{
		// ��õ�ǰ������ڵ�����
		int nIndex = HitTest(point) ;

//		if (nIndex == -1 && !(nFlags & MK_LBUTTON))
//			return ;
// 		if (nIndex == -1)
// 		{
// 			if (!(nFlags & MK_LBUTTON))
// 				return ;
// 
// 			// ������£��������-1�����ʧȥ����
// 		}
		
		if (nIndex != m_nTrackItemIndex)
		{
			m_bBeginHandleLBtnUp = TRUE ;

			// ���û���£���m_nTrackItemIndex�������-1
			if (!(nFlags & MK_LBUTTON))
			{
				if (nIndex != -1)
				{
					if (m_nTrackItemIndex != -1)
					{
						m_vecItems[m_nTrackItemIndex].m_uState = LCIS_NORMAL ;
						m_vecItems[m_nTrackItemIndex].m_uBtnState = LCIBS_NORMAL ;
						m_vecItems[m_nTrackItemIndex].m_nHeight = m_nItemHeightNormal ;
					}
					m_nTrackItemIndex = nIndex ;
					if (m_nTrackItemIndex != -1)
					{
						m_vecItems[m_nTrackItemIndex].m_uState = LCIS_PRIHOVERED ;
					}
				}
				else
				{
					if (m_nTrackItemIndex != -1)
					{
						m_vecItems[m_nTrackItemIndex].m_uBtnState = LCIBS_NORMAL ;
					}
				}
			}
			else
			{
				if (m_nTrackItemIndex != -1)
				{
					m_vecItems[m_nTrackItemIndex].m_uState = LCIS_NORMAL ;
					m_vecItems[m_nTrackItemIndex].m_uBtnState = LCIBS_NORMAL ;
					m_vecItems[m_nTrackItemIndex].m_nHeight = m_nItemHeightNormal ;
				}
				m_nTrackItemIndex = nIndex ;
				if (m_nTrackItemIndex != -1)
				{
					m_vecItems[m_nTrackItemIndex].m_uState = LCIS_PRIHOVERED ;
				}
			}

			// ���½���
			InvalidateRect(NULL, FALSE) ;
		}
		
		// �ж��Ƿ���ɾ����ť��
		if (m_nTrackItemIndex != -1)
		{
			CRect rcBtn ;
			rcBtn.left = m_rcClient.right - 1 - m_nBtnOffsetRight - m_nBtnWidth ;
			rcBtn.top = m_vecItems[m_nTrackItemIndex].m_nPosition + m_nItemPosOrigin
				+ (m_vecItems[m_nTrackItemIndex].m_nCurHeight - m_nBtnHeight) / 2 ;
			rcBtn.right = rcBtn.left + m_nBtnWidth ;
			rcBtn.bottom = rcBtn.top + m_nBtnHeight ;

			if (rcBtn.PtInRect(point))
			{
				if (m_vecItems[m_nTrackItemIndex].m_uBtnState != LCIBS_HOVERED)
				{
					m_vecItems[m_nTrackItemIndex].m_uBtnState = LCIBS_HOVERED ;
					InvalidateRect(rcBtn, FALSE) ;
				}
			}
			else
			{
				if (m_vecItems[m_nTrackItemIndex].m_uBtnState == LCIBS_HOVERED)
				{
					m_vecItems[m_nTrackItemIndex].m_uBtnState = LCIBS_NORMAL ;
					InvalidateRect(rcBtn, FALSE) ;
				}
			}
		}
	}
	else
	{
		LCITEM &item = m_vecItems[m_nBtnPushedItem] ;
		if (m_rcCurrentBtn.PtInRect(point))
		{
			if (item.m_uBtnState != LCIBS_PUSHED)
			{
				item.m_uBtnState = LCIBS_PUSHED ;
				InvalidateRect(m_rcCurrentBtn, FALSE) ;
			}
		}
		else
		{
			if (item.m_uBtnState == LCIBS_PUSHED)
			{
				item.m_uBtnState = LCIBS_NORMAL ;
				InvalidateRect(m_rcCurrentBtn, FALSE) ;
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CCmbListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ��õ�ǰ�����������������
	int nIndex = HitTest(point) ;
	if (nIndex != -1)
	{
		// �ж�����Ƿ���ɾ����ť��
		LCITEM &item = m_vecItems[nIndex] ;
		
		CRect rcBtn ;
		rcBtn.left = m_rcClient.right - 1 - m_nBtnOffsetRight - m_nBtnWidth ;
		rcBtn.top = item.m_nPosition + m_nItemPosOrigin
			+ (item.m_nCurHeight - m_nBtnHeight) / 2 ;
		rcBtn.right = rcBtn.left + m_nBtnWidth ;
		rcBtn.bottom = rcBtn.top + m_nBtnHeight ;
		
		if (rcBtn.PtInRect(point))
		{
			item.m_uBtnState = LCIBS_PUSHED ;
			InvalidateRect(rcBtn, FALSE) ;
			
			// ���ɾ����ť�ѱ�����
			m_nBtnPushedItem = nIndex ;
			m_bBtnPushed = TRUE ;
			m_rcCurrentBtn = rcBtn ;
			
			// ��ȡ������Ȩ
			if (GetCapture() != this)
			{
				SetCapture() ;
			}
		}
	}
	else
	{
		// ����ѡ�������Ϣ
		
// 		CPoint ptScreen(point) ;
// 		ClientToScreen(&ptScreen) ;
// 		CWnd * pWnd = GetOwner() ;
		//pWnd->SendMessage(WM_SELOVER, nIndex, MAKELPARAM(ptScreen.x, ptScreen.y)) ;
		
		// ���ٴ���
// 		DestroyWindow() ;

		// ����������
		if (GetCapture() == this)
			ReleaseCapture() ;
		ASSERT(m_pResData != NULL) ;
		if (m_pResData != NULL)
		{
			m_pResData->ot = EXITSEL ;
			m_pResData->nData = -1 ;
		}
	}

/*	CRect rcClient ;
	GetClientRect(rcClient) ;

	if (GetCapture() == this)
		ReleaseCapture() ;

	if (!rcClient.PtInRect(point))
	{
		// ����������
		ASSERT(m_pResData != NULL) ;
		if (m_pResData != NULL)
		{
			m_pResData->ot = EXITSEL ;
			m_pResData->nData = -1 ;
		}
	}
	else
	{

	}*/
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCmbListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bBeginHandleLBtnUp)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return ;
	}

	if (m_bBtnPushed)
	{
		m_bBtnPushed = FALSE ;

		// �ж�����Ƿ��ڵ�ǰɾ����ť��
		if (m_rcCurrentBtn.PtInRect(point))
		{
			//ReleaseCapture() ;
			//DestroyWindow() ;

			// ����ɾ���¼�
			//GetOwner()->SendMessage(WM_DELITEM, m_nBtnPushedItem) ;

			if (GetCapture() == this)
				ReleaseCapture() ;

			ASSERT(m_pResData != NULL) ;
			if (m_pResData == NULL)
				return ;
			m_pResData->ot = DELITEM ;
			m_pResData->nData = m_nBtnPushedItem ;
		}
		else
		{

		}
	}
	else
	{
		if (GetCapture() == this)
			ReleaseCapture() ;

		int nIndex = HitTest(point) ;

		ASSERT(m_pResData != NULL) ;
		if (m_pResData == NULL)
			return ;
		m_pResData->ot = SELITEM ;
		m_pResData->nData = nIndex ;

// 		CPoint ptScreen(point) ;
// 		ClientToScreen(&ptScreen) ;
// 		CWnd * pWnd = GetOwner() ;
// 		pWnd->SendMessage(WM_SELOVER, nIndex, MAKELPARAM(ptScreen.x, ptScreen.y)) ;
// 
// 		DestroyWindow() ;
	}

	CWnd::OnLButtonUp(nFlags, point);
}
