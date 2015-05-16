#include "StdAfx.h"

#include "WLTreeAL.h"
#include "common/utils/ImageEx/ImageEx.h"
#include "common/utils/UIUtils.h"

HCURSOR WLTreeAL::s_curNone = NULL ;
HCURSOR WLTreeAL::s_curMove = NULL ;
HCURSOR WLTreeAL::s_curCopy = NULL ;

WLTreeAL::WLTreeAL(void)
:
m_nVScrollWidth(16)
, m_pSelectedItem(NULL)
, m_bDraggingItem(FALSE)
, m_pDraggingItem(NULL)
, m_pDragOverItem(NULL)
, m_enDropEffect(NONE)
{
	m_pRoot = new WLTreeItemAL ;

	if (s_curNone == NULL)
		s_curNone = AfxGetApp()->LoadCursor(IDC_CUR_NONE) ;

	if (s_curMove == NULL)
		s_curMove = AfxGetApp()->LoadCursor(IDC_CUR_MOVE) ;

	if (s_curCopy == NULL)
		s_curCopy = AfxGetApp()->LoadCursor(IDC_CUR_COPY) ;
}

WLTreeAL::~WLTreeAL(void)
{
	delete m_pRoot ;
	m_pRoot = NULL ;
}

BOOL WLTreeAL::OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) 
{
// 	WLDC dc(this) ;
// 	m_dcBkgnd.CreateCompatibleDC(&dc) ;
// 	m_bmpBkgnd.CreateCompatibleBitmap(&dc, cx, cy) ;
// 	m_pBkgndOldBmp = m_dcBkgnd.SelectObject(&m_bmpBkgnd) ;
// 	m_dcBkgnd.FillSolidRect(0, 0, cx, cy, RGB(255, 255, 255)) ;

	UpdateBkgndDeviceContent(cx, cy) ;

	m_sbVert.SetImage(IDI_SCROLLBAR_PNG) ;
	// ����������
	if (!m_sbVert.Create(
		WS_VISIBLE | WLScrollBar::STYLE_RIGHTALIGN | WLScrollBar::STYLE_VERT,
		0, 0, cx, cy,
		this, (UINT)-1))
	{
		return FALSE ;
	}
	m_nVScrollWidth = m_sbVert.GetScrollWidth() ;

	SCROLLINFO si ;
	si.cbSize = sizeof(si) ;
	si.fMask = SIF_ALL ;
	si.nMin = 0 ;
	si.nMax = 0 ;
	si.nPage = cy ;
	si.nPos = 0 ;
	m_sbVert.SetScrollInfo(&si) ;

	// ������а�������
	if (!__super::OnCreate(dwStyle, x, y, cx, cy, pParent))
	{
// 		delete m_pRoot ;
// 		m_pRoot = NULL ;
		return FALSE ;
	}

	return TRUE ;
}

void WLTreeAL::OnDestroy() 
{
}

// ��Ĭ�ϻ������������ƻ���������
void WLTreeAL::_InternalOnDraw(HDC hDC, RECT const &rcUpdate) 
{
	CRect rcInHost ;
	GetRectInHost(rcInHost) ;
	RECT rcInsterset ;
	if (!IsWndLessVisible() || !::IntersectRect(&rcInsterset, &rcInHost, &rcUpdate))
		return ;

// 	TRACE(_T("--> %S::_InternalOnDraw [%p]\n"), typeid(*this).name(), this) ;
// 	CString s ;
// 	s.Format(_T("<-- %S::_InternalOnDraw [%p]"), typeid(*this).name(), this) ;
// 	hdutils::CPUPerformance cp(s) ;

	// ���ƻ�������
	HRGN hClipRgn = ::CreateRectRgnIndirect(&rcInHost) ;
	::SelectClipRgn(hDC, hClipRgn) ;

	// �����Լ�
	OnDraw(hDC, rcInsterset) ;

	// ���ƺ���
	OnDrawChild(hDC, rcInsterset) ;

	::SelectClipRgn(hDC, NULL) ;
	::DeleteObject(hClipRgn) ;
}

void WLTreeAL::OnDraw(HDC hDC, RECT const &rcUpdate) 
{
	CRect rc ;
	GetRectInHost(&rc) ;

/*	{
		Graphics g(hDC) ;
		SolidBrush sb(Color(180, 255, 255, 255)) ;
		g.FillRectangle(&sb, rc.left, rc.top, rc.Width(), rc.Height()) ;
	}*/
	//COLORREF crOldBk = ::SetBkColor(hDC, RGB(255, 0, 0)) ;
	//::ExtTextOutW(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL) ;
	//::SetBkColor(hDC, crOldBk) ;

	BLENDFUNCTION ftn ;
	ftn.BlendOp = AC_SRC_OVER ;
	ftn.BlendFlags = 0 ;
	ftn.SourceConstantAlpha = 220 ;
	ftn.AlphaFormat = 0 ;
	::AlphaBlend(
		hDC,
		rcUpdate.left, rcUpdate.top, rcUpdate.right - rcUpdate.left, rcUpdate.bottom - rcUpdate.top,
		m_dcBkgnd,
		rcUpdate.left - rc.left, rcUpdate.top - rc.top, rcUpdate.right - rcUpdate.left, rcUpdate.bottom - rcUpdate.top,
		ftn
		) ;

/*	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 255)) ;
	HGDIOBJ hOldPen = ::SelectObject(hDC, hPen) ;

	::MoveToEx(hDC, rc.left, rc.top, NULL) ;
	::LineTo(hDC, rc.right - 1, rc.top) ;
	::LineTo(hDC, rc.right - 1, rc.bottom - 1) ;
	::LineTo(hDC, rc.left, rc.bottom - 1) ;
	::LineTo(hDC, rc.left, rc.top) ;

	::SelectObject(hDC, hOldPen) ;
	::DeleteObject(hPen) ;*/

	// 	int nOldBkMode = ::SetBkMode(hDC, TRANSPARENT) ;
	// 	::TextOut(hDC, rc.left + 1, rc.top + 1, _T("WLBaseTree"), 10) ;
	// 	::SetBkMode(hDC, nOldBkMode) ;
}

void WLTreeAL::OnDrawChild(HDC hDC, RECT const &rcUpdate) 
{
//	DWORD dwBegin = GetTickCount() ;

	CRect rcRoot ;
	m_pRoot->GetRectInParent(rcRoot) ;

	RECT rcViewInRoot ;
	rcViewInRoot.left = 0 ;
	rcViewInRoot.top = -rcRoot.top ;
	rcViewInRoot.right = m_szClient.cx ;
	rcViewInRoot.bottom = rcViewInRoot.top + m_szClient.cy ;

	((WLTreeItemAL*)m_pRoot)->_CustomInternalDraw(hDC, rcUpdate, rcViewInRoot) ;

	m_sbVert._InternalOnDraw(hDC, rcUpdate) ;

// 	TRACE(
// 		_T("[%u] WLTreeAL::OnDrawChild{top=%d bottom=%d left=%d right=%d} cost %u\n"),
// 		GetTickCount(),
// 		rcUpdate.top, rcUpdate.bottom, rcUpdate.left, rcUpdate.right,
// 		GetTickCount() - dwBegin
// 		) ;
}

HWL WLTreeAL::HitTestChild(POINT const &pt) 
{
	HWL hRes = NULL ;

	CRect rcRoot ;
	m_pRoot->GetRectInParent(rcRoot) ;

	RECT rcViewInRoot ;
	rcViewInRoot.left = 0 ;
	rcViewInRoot.top = -rcRoot.top ;
	rcViewInRoot.right = m_szClient.cx ;
	rcViewInRoot.bottom = rcViewInRoot.top + m_szClient.cy ;

	hRes = ((WLTreeItemAL*)m_pRoot)->_CustomInternalHitTest(pt, rcViewInRoot) ;
	if (hRes != NULL)
		return hRes ;

	hRes = m_sbVert._InternalHitTest(pt) ;
	return hRes ;
}

WLTreeItemAL * WLTreeAL::HitTestTreeItem(POINT const &pt) 
{
	WLTreeItemAL * hRes = NULL ;

	CRect rcRoot ;
	m_pRoot->GetRectInParent(rcRoot) ;

	RECT rcViewInRoot ;
	rcViewInRoot.left = 0 ;
	rcViewInRoot.top = -rcRoot.top ;
	rcViewInRoot.right = m_szClient.cx ;
	rcViewInRoot.bottom = rcViewInRoot.top + m_szClient.cy ;

	hRes = ((WLTreeItemAL*)m_pRoot)->HitTestTreeItem(pt, rcViewInRoot) ;
	if (hRes == m_pRoot)
		return NULL ;

	return hRes ;
}

BOOL WLTreeAL::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) 
{
	__super::OnWndMsg(message, wParam, lParam, pResult) ;

	BOOL bRes = FALSE ;
	*pResult = 0 ;

	switch (message)
	{
	case WM_MOUSEMOVE :
		{
			OnMouseMove(wParam, CPoint(lParam)) ;
			bRes = TRUE ;
		}
		break ;

	case WM_LBUTTONDOWN :
		{
			SetFocus() ;
		}
		break ;

	case WM_NOTIFY :
		{
			switch (wParam)
			{
			case ID_ROOT :
				{
					LPNMWLHDR lpNMWLHDR = (LPNMWLHDR)lParam ;
					switch (lpNMWLHDR->code)
					{
					case TIN_HEIGHT_CHANGED :
						OnNotifyChildItemHeightChanged(lpNMWLHDR, pResult) ;
						bRes = TRUE ;
						break ;
					}
				}
				break ;

			case (UINT)-1 :
				{
					LPNMWLHDR lpNMWLHDR = (LPNMWLHDR)lParam ;
					switch (lpNMWLHDR->code)
					{
					case TIN_BEGIN_DRAG :
						OnNotifyTreeItemBeginDrag(lpNMWLHDR, pResult) ;
						bRes = TRUE ;
						break ;

					case TIN_SELECTED :
						OnNotifyChildItemSelected(lpNMWLHDR, pResult) ;
						bRes = TRUE ;
						break ;
					}
				}
				break ;
			}
		}
		break ;

	case WM_VSCROLL :
		{
//			OnVScroll(LOWORD(wParam), HIWORD(wParam), (WLScrollBar*)lParam) ;
			OnVScroll((LPSCROLLMSGINFO)wParam, (WLScrollBar*)lParam) ;
			bRes = TRUE ;
		}
		break ;

	case WM_MOUSEWHEEL :
		{
			OnMouseWheel(LOWORD(wParam), HIWORD(wParam), CPoint(lParam)) ;
			bRes = TRUE ;
		}
		break ;

	case WM_LBUTTONUP :
		{
			OnLButtonUp(wParam, CPoint(lParam)) ;
			bRes = TRUE ;
		}
		break ;

	case WM_CAPTURECHANGED :
		{
			OnCaptureChanged(HWL(lParam)) ;
			bRes = TRUE ;
		}
		break ;

	case WM_KEYDOWN :
		{
			bRes = OnKeyDown(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam)) ;
		}
		break ;

	case WM_SCROLLBAR_SHOW :
		{
			if (lParam == (LPARAM)&m_sbVert)
			{
				if (wParam == TRUE)
					m_pRoot->SetWndLessPos(0, 0, m_szClient.cx - m_nVScrollWidth, 0, SWLP_CX) ;
				else
					m_pRoot->SetWndLessPos(0, 0, m_szClient.cx, 0, SWLP_CX) ;
			}
		}
		break ;

	case WM_RBUTTONCLICK :
		{
			OnRButtonClick() ;
		}
		break ;
	}

	return bRes ;
}

void WLTreeAL::OnSized(SIZE const &szOld, SIZE const &szNew) 
{
	UpdateBkgndDeviceContent(szNew.cx, szNew.cy) ;

	m_sbVert.SetWndLessPos(szNew.cx - m_nVScrollWidth, 0, 0, szNew.cy, SWLP_X | SWLP_CY) ;

	if (szNew.cx != szOld.cx)
	{
		int cx = szNew.cx ;
		if (m_sbVert.IsWndLessVisible())
			cx -= m_nVScrollWidth ;

		m_pRoot->SetWndLessPos(0, 0, cx, 0, SWLP_CX) ;
	}

	if (szNew.cy != szOld.cy)
	{

		SCROLLINFO si ;
		si.cbSize = sizeof(si) ;
		si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE ;
		m_sbVert.GetScrollInfo(&si) ;

		si.nPage = szNew.cy ;

		int nOldPos = si.nPos ;
		if (si.nPos + int(si.nPage) > si.nMax + 1)
		{
			si.nPos = si.nMax + 1 - si.nPage ;
			if (si.nPos < 0)
			{
				si.nPos = 0 ;
			}

			m_pRoot->SetWndLessPos(0, nOldPos - si.nPos, 0, 0, SWLP_Y | SWLP_Y_OFFSET) ;
		}

		m_sbVert.SetScrollInfo(&si) ;
	}
}

//////////////////////////////////////////////////////////////////////////
// Message Handler

void WLTreeAL::OnNotifyChildItemHeightChanged(LPNMWLHDR pNMWLHDR, LRESULT * pResult) 
{
	LPNMTIHEIGHTINFO pHeightChangedInfo = (LPNMTIHEIGHTINFO)pNMWLHDR ;
	*pResult = 1 ;

	if (pHeightChangedInfo->lOldHeight == pHeightChangedInfo->lNewHeight)
		return ;
	if (pHeightChangedInfo->bNoScroll)
		return ;

	SCROLLINFO si ;
	si.cbSize = sizeof(si) ;

	m_sbVert.GetScrollInfo(&si, SIF_POS) ;
	int nOldPos = si.nPos ;

	si.fMask = SIF_RANGE | SIF_PAGE ;
	si.nMin = 0 ;
	si.nMax = pHeightChangedInfo->lNewHeight - 1 ;
	si.nPage = m_szClient.cy ;

	// ��������λ�ü����
	if (pHeightChangedInfo->lOldHeight > m_szClient.cy
		&& pHeightChangedInfo->lNewHeight <= m_szClient.cy)
	{
		m_pRoot->SetWndLessPos(0, 0, m_szClient.cx, 0, SWLP_Y | SWLP_CX) ;
		si.fMask |= SIF_POS ;
		si.nPos = 0 ;
	}
	else if(pHeightChangedInfo->lOldHeight <= m_szClient.cy
		&& pHeightChangedInfo->lNewHeight > m_szClient.cy)
	{
		m_pRoot->SetWndLessPos(0, 0, m_szClient.cx - m_nVScrollWidth, 0, SWLP_CX) ;
	}

	int nNewPos = m_sbVert.SetScrollInfo(&si) ;
	if (nOldPos != nNewPos)
	{
		m_pRoot->SetWndLessPos(0, -nNewPos, 0, 0, SWLP_Y) ;
	}
}

void WLTreeAL::OnNotifyChildItemSelected(LPNMWLHDR pNMWLHDR, LRESULT * pResult) 
{
	LPNMTREEITEM pTreeItem = (LPNMTREEITEM)pNMWLHDR ;
	*pResult = 1 ;

	if (m_pSelectedItem != pTreeItem->hItem)
	{
		if (m_pSelectedItem != NULL)
		{
			m_pSelectedItem->SendMessage(WM_KILLSELECTED, 0, 0) ;
		}

		if (pTreeItem->hItem != NULL)
		{
		}

		m_pSelectedItem = (WLTreeItemAL*)pTreeItem->hItem ;
	}
}

//void WLTreeAL::OnVScroll(UINT nSBCode, UINT nPos, WLScrollBar* pScrollBar)
void WLTreeAL::OnVScroll(LPSCROLLMSGINFO pSmi, WLScrollBar* /*pScrollBar*/) 
{
	if (!m_sbVert.IsWndLessVisible())
		return ;

	SCROLLINFO si ;
	si.cbSize = sizeof(si) ;
	m_sbVert.GetScrollInfo(&si, SIF_ALL) ;

	UINT nSBCode = pSmi->nSBCode ;
	UINT nPos = pSmi->nPos ;
	switch (nSBCode)
	{
	case SB_LINEUP :
		{
			si.fMask = SIF_POS ;
			si.nPos -= 25 ;
			if (si.nPos < si.nMin)
				si.nPos = si.nMin ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_LINEDOWN :
		{
			si.fMask = SIF_POS ;
			si.nPos += 25 ;
			if (si.nPos > si.nMax + 1 - (int)si.nPage)
				si.nPos = si.nMax + 1 - (int)si.nPage ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_PAGEUP :
		{
			si.fMask = SIF_POS ;
			si.nPos -= si.nPage ;
			if (si.nPos < si.nMin)
				si.nPos = si.nMin ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_PAGEDOWN :
		{
			si.fMask = SIF_POS ;
			si.nPos += si.nPage ;
			if (si.nPos > si.nMax + 1 - (int)si.nPage)
				si.nPos = si.nMax + 1 - (int)si.nPage ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_WHEELUP :
		{
			si.fMask = SIF_POS ;
			si.nPos -= (si.nPage * 2 / 5) ;
			if (si.nPos < si.nMin)
				si.nPos = si.nMin ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_WHEELDOWN :
		{
			si.fMask = SIF_POS ;
			si.nPos += (si.nPage * 2 / 5) ;
			if (si.nPos > si.nMax + 1 - (int)si.nPage)
				si.nPos = si.nMax + 1 - (int)si.nPage ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_THUMBPOSITION :
		{
			si.fMask = SIF_POS ;
			si.nPos = nPos ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;

	case SB_THUMBTRACK :
		{
			si.fMask = SIF_POS ;
			si.nPos = nPos ;
			m_sbVert.SetScrollInfo(&si) ;
		}
		break ;
	}

	m_pRoot->SetWndLessPos(0, -si.nPos, 0, 0, SWLP_Y | SWLP_NOREDRAW) ;
	RECT rcView = {0, 0, m_szClient.cx - m_nVScrollWidth, m_szClient.cy} ;
	UpdateView(&rcView) ;
}

BOOL WLTreeAL::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*point*/) 
{
	SCROLLMSGINFO smi ;
	if (zDelta > 0)
		smi.nSBCode = SB_WHEELUP ;
	else
		smi.nSBCode = SB_WHEELDOWN ;

	SendMessage(WM_VSCROLL, (WPARAM)&smi, NULL) ;
	return TRUE ;
}

BOOL WLTreeAL::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
{
	if (nChar == VK_ESCAPE)
	{
		CancelDrag() ;
		return TRUE ;
	}
	else if (nChar == VK_UP)
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		HWLTREEITEM hPrevItem = GetPrevVisibleItemInRoot(hSelectItem) ;
		if (hPrevItem != NULL)
		{
			SelectItem(hPrevItem) ;
			hPrevItem->SetFocus() ;
			EnsureVisible(hPrevItem) ;
		}
		return TRUE ;
	}
	else if (nChar == VK_DOWN)
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		HWLTREEITEM hNextItem = GetNextVisibleItemInRoot(hSelectItem) ;
		if (hNextItem != NULL)
		{
			SelectItem(hNextItem) ;
			hNextItem->SetFocus() ;
			EnsureVisible(hNextItem) ;
		}
		return TRUE ;
	}
	else if (nChar == VK_SPACE)		// չ��/����
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		if (hSelectItem != NULL)
		{
			hSelectItem->Expand(TE_TOGGLE) ;
		}
		return TRUE ;
	}
	else if (nChar == VK_ADD)
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		if (hSelectItem != NULL)
		{
			hSelectItem->Expand(TE_COLLAPSE) ;
		}
		return TRUE ;
	}
	else if (nChar == VK_SUBTRACT)
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		if (hSelectItem != NULL)
		{
			hSelectItem->Expand(TE_EXPAND) ;
		}
		return TRUE ;
	}
	else if (nChar == VK_LEFT)
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		if (hSelectItem != NULL)
		{
			if (!hSelectItem->Expand(TE_COLLAPSE))
			{
				// ����Ѿ��������Ƶ����ڵ���
				HWLTREEITEM hParentItem = hSelectItem->GetParentItem() ;
				if (hParentItem != NULL && hParentItem != m_pRoot)
				{
					SelectItem(hParentItem) ;
					EnsureVisible(hParentItem) ;
				}
			}
		}
		return TRUE ;
	}
	else if (nChar == VK_RIGHT)
	{
		HWLTREEITEM hSelectItem = GetSelectedItem() ;
		if (hSelectItem != NULL)
		{
			if (!hSelectItem->Expand(TE_EXPAND))
			{
				// ����Ѿ�չ�����Ƶ���һ���ӽڵ���
				HWLTREEITEM hFirstChildItem = hSelectItem->GetFirstChild() ;
				while (hFirstChildItem != NULL)
				{
					if (hFirstChildItem->IsVisible())
					{
						SelectItem(hFirstChildItem) ;
						EnsureVisible(hFirstChildItem) ;

						break ;
					}

					hFirstChildItem = hFirstChildItem->GetNextSibling() ;
				}
			}
		}
		return TRUE ;
	}
	else if (nChar == VK_RETURN)
	{
	}

	return FALSE ;
}

HWLTREEITEM WLTreeAL::GetPrevVisibleItemInRoot(HWLTREEITEM hItem) const 
{
	// �������hItem���ڵ���֦�ǵݹ�չ�����ҿɼ���
	if (hItem == NULL)
		return NULL ;

	// ���ǰһ���ɼ����ֵܽڵ㣬���û�У����������ڵ���[���̵ݹ�]
	HWLTREEITEM hRes = NULL ;

	HWLTREEITEM hCur = hItem ;
	HWLTREEITEM hPrev = hCur->GetPrevSibling() ;
	BOOL bParent = FALSE ;		// �����һ���ڵ��Ƿ񸸽ڵ㣬����ǣ��ַ�����ʾ����������������
	while (hPrev != m_pRoot)
	{
		if (hPrev == NULL)
		{
			hPrev = hCur->GetParentItem() ;
			bParent = TRUE ;
		}
		else if (!hPrev->IsVisible()/* || !hPrev->IsWndLessShow()*/)
		{
			hCur = hPrev ;
			hPrev = hPrev->GetPrevSibling() ;
			bParent = FALSE ;
		}
		else
		{
			if (bParent)
				return hPrev ;

			hRes = hPrev ;
			break ;
		}
	}

	// ����ֵܽڵ����һ���ɼ�������ڵ�
	while (hRes != NULL && hRes->IsExpand())
	{
		// ������һ������
		HWLTREEITEM hLastChild = NULL ;
		HWLTREEITEM hTemp = hRes->GetFirstChild() ;
		while (hTemp != NULL)
		{
			if (hTemp->IsVisible()/* && hTemp->IsWndLessShow()*/)
				hLastChild = hTemp ;

			hTemp = hTemp->GetNextSibling() ;
		}
		if (hLastChild == NULL)
			break ;

		hRes = hLastChild ;
	}

	return hRes ;
}

HWLTREEITEM WLTreeAL::GetNextVisibleItemInRoot(HWLTREEITEM hItem) const 
{
    // �������hItem���ڵ���֦�ǵݹ�չ�����ҿɼ���
    if (hItem == NULL)
        return NULL ;

	// �ж�hItem�Ƿ�չ�������չ������һ����ʾ�Ľڵ�������һ����ʾ�ĺ��ӽ��
	if ((hItem->IsExpand()))
	{
		HWLTREEITEM hNvItem = hItem->GetFirstChild() ;
		while (hNvItem != NULL)
		{
			if (hNvItem->IsVisible()/* && hNvItem->IsWndLessShow()*/)
				return hNvItem ;
			hNvItem = hNvItem->GetNextSibling() ;
		}
	}

	// ������һ����ʾ�ڵ�Ϊ����һ���ֵܽڵ�
	// �����һ���ֵܽڵ�Ϊ�գ�����һ����ʾ�ڵ�Ϊ����ڵ�
	HWLTREEITEM hCurItem = hItem ;
	while (hCurItem != NULL)
	{
		HWLTREEITEM hNvItem = hCurItem->GetNextSibling() ;
		while (hNvItem != NULL)
		{
			// �������ڵ�ɼ����򷵻ؽڵ��������򣬷���NULL
			if (hNvItem->IsVisible()/* && hNvItem->IsWndLessShow()*/)
				return hNvItem ;
			else
				// ���ӽڵ���ʾ/����ʾ���Ժ���������
				hNvItem = hNvItem->GetNextSibling() ;
		}

		hCurItem = hCurItem->GetParentItem() ;
	}

	return NULL ;
}

void WLTreeAL::OnNotifyTreeItemBeginDrag(LPNMWLHDR pNMWLHDR, LRESULT * pResult) 
{
	LPNMTIDRAGINFO pInfo = (LPNMTIDRAGINFO)pNMWLHDR ;
	*pResult = 1 ;

	// ѯ�����ؼ����ڴ����Ƿ�������ק
	NMWLHDR hdr ;
	hdr.code = TN_CAN_I_DRAG ;
	hdr.hWlFrom = this ;
	hdr.idFrom = GetID() ;
	LRESULT lResult = GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetID(), (LPARAM)&hdr) ;

	// ���������Ƿ������϶�
	if (lResult == 0)
	{
		return ;
	}
	
	if (GetCapture() != this)
		SetCapture() ;

	m_bDraggingItem = TRUE ;
	m_pDraggingItem = (WLTreeItemAL*)pInfo->hdr.hWlFrom ;
	m_pDragOverItem = NULL ;
	BeginDrag(m_pDraggingItem, pInfo->ptInItem) ;
}

void WLTreeAL::OnRButtonClick() 
{
	// ֪ͨ���ؼ����ڴ���(����Ҫ��һЩԤ������������Ϸ���ڵ�)
	NMWLHDR hdr ;
	hdr.code = TN_RCLICK ;
	hdr.hWlFrom = this ;
	hdr.idFrom = GetID() ;
	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetID(), (LPARAM)&hdr) ;
}

//////////////////////////////////////////////////////////////////////////
// Inner Function

void WLTreeAL::UpdateBkgndDeviceContent(int cx, int cy) 
{
	WLDC dc(this) ;

	if (!m_dcBkgnd.IsAvailable())
	{
		if (!m_dcBkgnd.CreateCompatibleDC(dc, cx, cy))
			return ;
		//m_dcBkgnd.SelectFont(m_hFont) ;
	}
	else
	{
		if (!m_dcBkgnd.ChangeSize(cx, cy))
			return ;
	}

	RedrawBkgnd(cx, cy) ;
}

void WLTreeAL::RedrawBkgnd(int cx, int cy) 
{
/*	COLORREF crOldBkColor = ::SetBkColor(m_dcBkgnd, RGB(255, 255, 255)) ;

	RECT rc = {0, 0, cx, cy} ;
	::ExtTextOut(m_dcBkgnd, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL) ;

	::SetBkColor(m_dcBkgnd, crOldBkColor) ;*/

	Graphics gph(m_dcBkgnd) ;
	SolidBrush sb(Color(254, 255, 255, 255)) ;
	gph.FillRectangle(&sb, 0, 0, m_dcBkgnd.GetWidth(), m_dcBkgnd.GetHeight()) ;
}

void WLTreeAL::BeginDrag(HWLTREEITEM hItem, CPoint const &ptDragPointInItem) 
{
	// ��ʾ��קͼ��
	WLDC dc(this) ;
	CRect rcItem ;
	hItem->GetRectInHost(rcItem) ;
	CPoint ptImage(rcItem.TopLeft()) ;
	::ClientToScreen(GetCenter()->GetHost(), &ptImage) ;
	m_DragImage.Create(&dc, rcItem, GetCenter()->GetHost(), ptImage, ptDragPointInItem) ;

	// ֪ͨ���ؼ����ڴ���(����Ҫ��һЩԤ������������Ϸ���ڵ�)
	NMWLHDR hdr ;
	hdr.code = TN_BEGINDRAG ;
	hdr.hWlFrom = this ;
	hdr.idFrom = GetID() ;
	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetID(), (LPARAM)&hdr) ;

	TRACE(_T("[%u]BeginDrag\n"), GetTickCount()) ;
}

void WLTreeAL::OnMouseMove(UINT /*nFlags*/, CPoint point) 
{
	if (m_bDraggingItem)
	{
// 		CPoint ptInTree(point) ;
// 		HostToClient(&ptInTree) ;
// 		DoDragging(ptInTree) ;

		DoDragging(point) ;

// 		CPoint ptInScreen(point) ;
// 		::ClientToScreen(GetCenter()->GetHost(), &ptInScreen) ;
// 		DoDragging(ptInScreen) ;
	}
}

void WLTreeAL::DoDragging(
						  CPoint const &ptDragging		// ��������ϵ
						  ) 
{
	CPoint ptInScreen(ptDragging) ;
	::ClientToScreen(GetCenter()->GetHost(), &ptInScreen) ;
	m_DragImage.DoDragging(ptInScreen) ;

	// ������ק���������ڵ�
	WLTreeItemAL * hDragOver = HitTestTreeItem(ptDragging) ;

	if (m_pDragOverItem != hDragOver)
	{
		if (m_pDragOverItem != NULL)
		{
			m_pDragOverItem->DragOver(FALSE) ;
		}
		if (hDragOver != NULL)
		{
			hDragOver->DragOver(TRUE) ;
		}

		m_pDragOverItem = (WLTreeItemAL *)hDragOver ;

		NMTREEDRAGINFO di ;
		di.hdr.hWlFrom = this ;
		di.hdr.idFrom = GetID() ;
		di.hdr.code = TN_DRAGOVER ;
		di.pItemBeDragging = m_pDraggingItem ;
		di.pItemBeDragOver = (WLTreeItemAL *)hDragOver ;
		m_enDropEffect = (DropEffect)GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetID(), (LPARAM)&di) ;
		switch (m_enDropEffect)
		{
		case NONE :
			::SetCursor(s_curNone) ;
			break ;

		case MOVE :
			::SetCursor(s_curMove) ;
			break ;

		case COPY :
			::SetCursor(s_curCopy) ;
			break ;

		default:
			break ;
		}
	}

	//TRACE(_T("[%u]DoDragging: %d, %d\n"), GetTickCount(), ptDragging.x, ptDragging.y) ;
}

void WLTreeAL::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/) 
{
	if (m_enDropEffect != NONE)
	{
		NMTREEDRAGINFO di ;
		di.hdr.hWlFrom = this ;
		di.hdr.idFrom = GetID() ;
		di.hdr.code = TN_DRAGDROP ;
		di.pItemBeDragging = m_pDraggingItem ;
		di.pItemBeDragOver = m_pDragOverItem ;
		GetParent()->SendMessage(WM_NOTIFY, WPARAM(GetID()), (LPARAM)&di) ;
	}

	CancelDrag() ;
// 	if (m_bDraggingItem)
// 	{
// 		if (GetCapture() == this)
// 			ReleaseCapture() ;
// 	}
}

void WLTreeAL::OnCaptureChanged(HWL /*hWL*/) 
{
	// �˳���ק
	EndDrag() ;
}

// �����˳���ק
void WLTreeAL::CancelDrag() 
{
	if (m_bDraggingItem)
	{
		if (GetCapture() == this)
			ReleaseCapture() ;

//		EndDrag() ;
	}
}

// ��ק���������β����
void WLTreeAL::EndDrag() 
{
	// �˳���ק
	if (m_bDraggingItem)
	{
		m_bDraggingItem = FALSE ;
		m_pDraggingItem = NULL ;

		if (m_pDragOverItem != NULL)
		{
			m_pDragOverItem->DragOver(FALSE) ;
			m_pDragOverItem = NULL ;
		}
		m_DragImage.EndDrag() ;

		TRACE(_T("[%u]EndDrag\n"), GetTickCount()) ;

		// ֪ͨ���ؼ����ڴ���(����Ҫ��һЩԤ����������չ����קǰչ���ķ���ڵ�)
		NMWLHDR hdr ;
		hdr.code = TN_ENDDRAG ;
		hdr.hWlFrom = this ;
		hdr.idFrom = GetID() ;
		GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetID(), (LPARAM)&hdr) ;

		::SetCursor(::LoadCursor(NULL, IDC_ARROW)) ;
	}
}

//////////////////////////////////////////////////////////////////////////
// WLTree Implement
/*
HWLTREEITEM WLTreeAL::InsertItem(
								 LPCTSTR lpszText,
								 HWLTREEITEM hParent,// = TI_ROOT
								 HWLTREEITEM hInsertAfter,// = TI_LAST
								 BOOL bDeferInsert// = FALSE
								 ) 
{
	WLTreeTextItem * pItem = new WLTreeTextItem ;
	pItem->SetItemText(lpszText) ;

	HWLTREEITEM hRes = __super::InsertItem(pItem, hParent, hInsertAfter, bDeferInsert) ;
	if (NULL == hRes)
	{
		delete pItem ;
		return NULL ;
	}

	return hRes ;
	return NULL ;
}
*/

BOOL WLTreeAL::DeleteItem(
						  HWLTREEITEM hItem
						  ) 
{
	BOOL bRes = __super::DeleteItem(hItem) ;

	// �����ѡ��ڵ�
	if (m_pSelectedItem == hItem)
		m_pSelectedItem = NULL ;

	// �˳���ק
	if (m_pDraggingItem == hItem)
	{
		CancelDrag() ;
	}

	return bRes ;
}

void WLTreeAL::DeleteAllItem() 
{
	__super::DeleteAllItem() ;

	// �����ѡ��ڵ�
	m_pSelectedItem = NULL ;

	// �˳���ק
	CancelDrag() ;
}

void WLTreeAL::SelectItem(HWLTREEITEM hItem) 
{
	if (hItem != NULL)
	{
		WLTreeItemAL * pItem = (WLTreeItemAL*)hItem ;
		pItem->Select(TRUE) ;
	}
}

BOOL WLTreeAL::EnsureVisible(HWLTREEITEM hItem) 
{
	if (hItem == NULL || !hItem->IsVisible())
		return FALSE ;

	// ��ø�λ��
	CRect rcRoot ;
	m_pRoot->GetRectInParent(&rcRoot) ;

	// ��ýڵ�λ��
	CRect rcItemInRoot ;
	GetItemRectInRoot(hItem, rcItemInRoot) ;

	CRect rcItemInTree ;
	GetItemRectInTree(hItem, rcItemInTree) ;

	// �ڵ��Ϸ����ɼ�������ͼ֮��
	//int nItemTopInTree = rcItemInRoot.top + rcRoot.top ;
	int nScrollPos = 0 ;
	BOOL bChangePos = FALSE ;
	if (rcItemInTree.top < 0)
	{
		nScrollPos = rcItemInRoot.top ;
		bChangePos = TRUE ;
	}
	else if (rcItemInTree.top >= 0)
	{
		if (rcItemInTree.bottom > m_szClient.cy)
		{
			// ����ڵ��ܸ߶�С����ͼ�߶ȣ��豣֤����ȫ����ʾ
			if (rcItemInTree.Height() < m_szClient.cy)
			{
				nScrollPos = rcItemInRoot.bottom - m_szClient.cy ;
				bChangePos = TRUE ;
			}
			// ����ڵ��ܸ߶ȴ�����ͼ�߶ȣ��豣֤����ȫ����ʾ
			else
			{
				nScrollPos = rcItemInRoot.top ;
				bChangePos = TRUE ;
			}
		}
		else
		{
			// �Ѿ��ɼ�
		}
	}

	if (bChangePos)
	{
		m_pRoot->SetWndLessPos(0, -nScrollPos, 0, 0, SWLP_Y | SWLP_NOREDRAW) ;
		UpdateView() ;

		SCROLLINFO si ;
		si.cbSize = sizeof(si) ;
		si.fMask = SIF_POS ;
		si.nPos = nScrollPos ;
		m_sbVert.SetScrollInfo(&si) ;
	}

	return FALSE ;
}

void WLTreeAL::GetItemRectInTree(HWLTREEITEM hItem, CRect &rcItem) 
{
	if (hItem == NULL || !GetCenter()->IsWndLess(hItem))
		return ;

	CRect rcTreeInHost ;
	GetRectInHost(&rcTreeInHost) ;

	((WLTreeItemAL*)hItem)->GetItemRectInHost(rcItem) ;

	rcItem.OffsetRect(-rcTreeInHost.TopLeft()) ;
}

void WLTreeAL::GetItemRectInRoot(HWLTREEITEM hItem, CRect &rcItem) 
{
	if (hItem == NULL || !GetCenter()->IsWndLess(hItem))
		return ;

	CRect rcRootInHost ;
	m_pRoot->GetRectInHost(rcRootInHost) ;

	((WLTreeItemAL*)hItem)->GetItemRectInHost(rcItem) ;

	rcItem.OffsetRect(-rcRootInHost.TopLeft()) ;
}
