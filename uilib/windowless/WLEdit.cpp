#include "StdAfx.h"
#include <locale.h>
#include "WLEdit.h"
#include "common/wnds/Menu/hdmenu.h"
#include "common/utils/Graphics.h"
#include "common/utils/UIUtils.h"
#include "common/MemoryLeakChecker.h"

WLEditImp::WLEditImp(void)
{
	m_bFocus = FALSE ;
	m_nCaretPos = 0 ;
	m_bShowCaret = FALSE ;
	m_bInputting = FALSE ;
	m_nLeftMargin = 3 ;
	m_nTopMargin = 3 ;
	m_nRightMargin = 3 ;
	m_nBottomMargin = 3 ;
	m_nFixSel = 0 ;
	m_nLeftSel = 0 ;
	m_nRightSel = 0 ;

	m_bLDown = FALSE ;
	m_bRDown = FALSE ;
	m_bBeginSel = FALSE ;

	m_bPopMenu = FALSE ;

	m_nLineHeight = 12 ;

	//m_nOriginPosX = m_nLeftMargin ;
	m_vctCaretPos.push_back(0) ;

	m_nMaxChars = 0 ;		// 0��ʾ������

	m_uFrameWidth = 1 ;
	m_crFrameColor = RGB(0, 0, 0) ;

	m_crTextNormalColor = hdutils::crTextColor ;
	m_crTextHighlightColor = hdutils::crTextColor ;
	m_crTextHighlightBkColor = Color::MakeARGB(255, 174, 196, 232) ;
	m_crBkgndColor = Color::MakeARGB(0, 0, 0, 0) ;

	m_sCueBanner = _T("") ;
	m_crCurBannerColor = RGB(172, 168, 153) ;
	m_cPasswordChar = 0x25cf ;

	m_bCanAutoCompleteWord = FALSE ;
}

WLEditImp::~WLEditImp(void)
{
}

UINT WLEditImp::SetFrameWidth(UINT uNewFrameWidth) 
{
	if (m_uFrameWidth != uNewFrameWidth)
	{
		m_uFrameWidth = uNewFrameWidth ;

		CSize szClient;
		GetSize(&szClient);

		m_rcTextBound.SetRect(
			m_uFrameWidth + m_nLeftMargin, m_uFrameWidth + m_nTopMargin,
			szClient.cx + 1 - m_uFrameWidth - m_nRightMargin,
			szClient.cy + 1 - m_uFrameWidth - m_nBottomMargin
			) ;
		m_nOriginPosX = m_rcTextBound.left ;
	}

	return m_uFrameWidth ;
}

void WLEditImp::SetMargin(
					   int nLeftMargin,
					   int nTopMargin,
					   int nRightMargin,
					   int nBottomMargin
					   ) 
{
	nLeftMargin != -1 ? (m_nLeftMargin = nLeftMargin) : 0 ;
	nTopMargin != -1 ? (m_nTopMargin = nTopMargin) : 0 ;
	nRightMargin != -1 ? (m_nRightMargin = nRightMargin) : 0 ;
	nBottomMargin != -1 ? (m_nBottomMargin = nBottomMargin) : 0 ;


	CSize szClient;
	GetSize(&szClient);

	m_rcTextBound.SetRect(
		m_uFrameWidth + m_nLeftMargin, m_uFrameWidth + m_nTopMargin,
		szClient.cx - m_uFrameWidth - m_nRightMargin,
		szClient.cy - m_uFrameWidth - m_nBottomMargin
		) ;
	m_nOriginPosX = m_rcTextBound.left ;
}

void WLEditImp::SetCueBanner(LPCTSTR lpszText) 
{
	if (lpszText == NULL)
	{
		ASSERT(FALSE) ;
		return ;
	}

	m_sCueBanner = lpszText ;
}

CString WLEditImp::GetCueBanner() const 
{
	return m_sCueBanner ;
}

BOOL WLEditImp::Create(DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent, UINT uID) 
{
	BOOL bRes = __super::Create(dwStyle, x, y, cx, cy, pParent, uID) ;

	if (bRes)
	{
		m_rcTextBound.SetRect(
			m_uFrameWidth + m_nLeftMargin, m_uFrameWidth + m_nTopMargin,
			cx - m_uFrameWidth - m_nRightMargin,
			cy - m_uFrameWidth - m_nBottomMargin
			) ;
		m_nOriginPosX = m_rcTextBound.left ;
// 		GetRectInParent(m_rcTextBound) ;
// 		m_rcTextBound.OffsetRect(- m_rcTextBound.TopLeft()) ;
// 		m_rcTextBound.DeflateRect(m_nLeftMargin, m_nTopMargin, m_nRightMargin, m_nBottomMargin) ;

		m_ttb.SetFrameColor(RGB(90, 127, 167)) ;
		m_ttb.SetCloseBtnSize(0, 0) ;
		m_ttb.SetWidth(130) ;
		//m_ttb.SetFriable() ;
		// �������ݵ����������ȼ�
		m_ttb.SetDirPRI(3) ;
	}

	return bRes ;
}

void WLEditImp::OnDraw(HDC hDC, RECT const &rcUpdate) 
{
	CRect rcInHost ;
	GetRectInHost(rcInHost) ;

    HFONT font = GetFont() ;
    HGDIOBJ old_font = NULL ;
    if (font != NULL)
        old_font = ::SelectObject(hDC, font) ;

	// ���Ʊ���
	OnDrawBkgnd(hDC, rcInHost, rcUpdate) ;

	HRGN hOldClipRgn = CreateRectRgn(0, 0, 0, 0) ;
	BOOL bNoClipNow = (0 == ::GetClipRgn(hDC, hOldClipRgn)) ;

	// �������ֻ�������
	CRect rcClip(m_rcTextBound) ;
	rcClip.OffsetRect(rcInHost.TopLeft()) ;
	//rcClip.right += 1 ;		// �����Ĺ������λ��
	HRGN hClipRgn = ::CreateRectRgnIndirect(&rcClip) ;
	::ExtSelectClipRgn(hDC, hClipRgn, RGN_AND) ;
	::DeleteObject(hClipRgn) ;
	//::SelectClipRgn(hDC, hClipRgn) ;

	int nOldBkMode = ::SetBkMode(hDC, TRANSPARENT) ;

	CRect rcText(m_rcTextBound) ;
	int i = 0 ;

	// û������ʱ��ʾ��ʾ����
	if (IsEmpty())
	{
		if (!m_sCueBanner.IsEmpty())
		{
			COLORREF crOldText = ::SetTextColor(hDC, m_crCurBannerColor) ;
			CRect rcDraw(rcText) ;
			rcDraw.OffsetRect(rcInHost.TopLeft()) ;
			::DrawText(hDC, m_sCueBanner, m_sCueBanner.GetLength(), rcDraw, DT_VCENTER | DT_SINGLELINE| DT_NOPREFIX) ;
			::SetTextColor(hDC, crOldText) ;
		}
	}
	else
	{
		// �༭���ý���ʱ����ʾѡ��״̬
		BOOL bShowSel = m_bFocus || (GetStyle() & WLES_NOHIDESEL) ;

		// ����������ɫ
		COLORREF crOldText = ::SetTextColor(hDC, m_crTextNormalColor) ;

		
		CString &text = GetWindowTextRef();

		int nTextLen = text.GetLength() ;
		LPCTSTR lpszText = text ;
		for (i = GetFirstVisiblePos() ; i < nTextLen ; ++ i)
		{
			rcText.left = m_vctCaretPos[i] + m_nOriginPosX ;
			if (rcText.left >= m_rcTextBound.right)
				break ;

			rcText.right = m_vctCaretPos[i + 1] + m_nOriginPosX ;

			// ת����dc����
			CRect rcDraw(rcText) ;
			rcDraw.OffsetRect(rcInHost.TopLeft()) ;

			TCHAR s[2] ;
			if (GetStyle() & ES_PASSWORD)
				s[0] = 0x25cf ;
			else
				s[0] = *lpszText ;
			s[1] = 0 ;

			if (i >= m_nLeftSel && i < m_nRightSel && bShowSel)
			{
				// ���Ƹ������ֱ���
				//HBRUSH hBrush = ::GetSysColorBrush(COLOR_HIGHLIGHT) ;
				//::FillRect(hDC, rcDraw, hBrush) ;
				DrawHighlightTextBkgnd(hDC, rcDraw) ;

				// ���Ƹ�������
				COLORREF crOld = ::SetTextColor(hDC, m_crTextHighlightColor) ;
				::DrawText(hDC, s, 1, rcDraw, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX) ;
				::SetTextColor(hDC, crOld) ;
			}
			else
			{
				::DrawText(hDC, s, 1, rcDraw, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX) ;
			}

			// ���ƹ��
			// �����ݱ�ѡ��ʱ����ʾ���
			if (m_bShowCaret && i == m_nCaretPos && m_nLeftSel == m_nRightSel)
			{
				CRect rcCaret(rcDraw.left, rcDraw.top, rcDraw.left + 1, rcDraw.bottom) ;
				//dcMem.InvertRect(rcCaret) ;
				::InvertRect(hDC, &rcCaret) ;
			}

			lpszText += 1 ;
		}

		// �ָ�������ɫ
		::SetTextColor(hDC, crOldText) ;
	}

	// �������λ�ù��
	if (m_bShowCaret && i == m_nCaretPos && m_nLeftSel == m_nRightSel)
	{
		int nLastPos = rcInHost.left + m_vctCaretPos[i] + m_nOriginPosX ;
		if (i != 0)
			-- nLastPos ;
		CRect rcCaret(
			nLastPos,
			rcInHost.top + m_rcTextBound.top,
			nLastPos + 1,
			rcInHost.top + m_rcTextBound.bottom
			) ;
		::InvertRect(hDC, &rcCaret) ;
	}

	// �ָ�DC
	::SetBkMode(hDC, nOldBkMode) ;

	// ȡ�����Ʒ�Χ
	if (bNoClipNow)
	{
		::SelectClipRgn(hDC, NULL) ;
	}
	else
	{
		::SelectClipRgn(hDC, hOldClipRgn) ;
	}
	::DeleteObject(hOldClipRgn) ;

    if (font != NULL)
        ::SelectObject(hDC, old_font) ;
}

void WLEditImp::OnDrawBkgnd(HDC hDC, RECT const &rcInHost, RECT const &rcUpdate) 
{
	// ���Ʊ߿�
	if (m_uFrameWidth != 0)
	{
		CRect rcTop(rcInHost.left, rcInHost.top, rcInHost.right - m_uFrameWidth, rcInHost.top + m_uFrameWidth) ;
		CRect rcRight(rcInHost.right - m_uFrameWidth, rcInHost.top, rcInHost.right, rcInHost.bottom - m_uFrameWidth) ;
		CRect rcBottom(rcInHost.left + m_uFrameWidth, rcInHost.bottom - m_uFrameWidth, rcInHost.right, rcInHost.bottom) ;
		CRect rcLeft(rcInHost.left, rcInHost.top + m_uFrameWidth, rcInHost.left + m_uFrameWidth, rcInHost.bottom) ;

		COLORREF crOld = ::SetBkColor(hDC, m_crFrameColor) ;
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcTop, NULL, 0, NULL) ;
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcRight, NULL, 0, NULL) ;
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcBottom, NULL, 0, NULL) ;
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcLeft, NULL, 0, NULL) ;
		SetBkColor(hDC, crOld) ;
	}

	// ���Ʊ���
	CRect rcBkgnd(rcInHost) ;
	rcBkgnd.DeflateRect(m_uFrameWidth, m_uFrameWidth) ;
	BYTE byA = m_crBkgndColor.GetA() ;
	if (byA != 0)
	{
		if (byA == 255)
		{
			HBRUSH hBrush = ::CreateSolidBrush(m_crBkgndColor.ToCOLORREF()) ;
			::FillRect(hDC, &rcBkgnd, hBrush) ;
			::DeleteObject(hBrush) ;
		}
		else		// �Ȳ����ѣ�����Gdiplus
		{
			Graphics g(hDC) ;
			SolidBrush sb(m_crBkgndColor) ;
			g.FillRectangle(&sb, rcBkgnd.left, rcBkgnd.top, rcBkgnd.Width(), rcBkgnd.Height()) ;
		}
	}
}

void WLEditImp::DrawHighlightTextBkgnd(HDC hDC, RECT const &rcBkgnd) 
{
	BYTE byA = m_crTextHighlightBkColor.GetA() ;
	if (byA != 0)
	{
		if (byA == 255)
		{
			HBRUSH hBrush = ::CreateSolidBrush(m_crTextHighlightBkColor.ToCOLORREF()) ;
			::FillRect(hDC, &rcBkgnd, hBrush) ;
			::DeleteObject(hBrush) ;
		}
		else		// �Ȳ����ѣ�����Gdiplus
		{
			Graphics g(hDC) ;
			SolidBrush sb(m_crTextHighlightBkColor) ;
			g.FillRectangle(&sb, rcBkgnd.left, rcBkgnd.top, rcBkgnd.right - rcBkgnd.left, rcBkgnd.bottom - rcBkgnd.top) ;
		}
	}
}

void WLEditImp::SendChangeCommand() 
{
    if (GetParent() != NULL)
    	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetID(), EN_CHANGE), (LPARAM)m_wrapper) ;
}

// ��ù���¶�λ��(�ͻ�������ϵ)
void WLEditImp::GetCaretPos(LPPOINT lpptPos) const 
{
	if (lpptPos == NULL)
	{
		ASSERT(FALSE) ;
		return ;
	}

	lpptPos->x = m_nOriginPosX + m_vctCaretPos[m_nCaretPos] ;
	lpptPos->y = m_rcTextBound.top ;
}

// �������뷨����λ��
void WLEditImp::UpdateImeWndPosByCaret() const 
{
	POINT ptCaret ;
	GetCaretPos(&ptCaret) ;
	ptCaret.y += (m_rcTextBound.Height() - m_nLineHeight) / 2 ;
	UpdateImeCompositionWindowPos(ptCaret.x, ptCaret.y) ;
}

// ������д��ʾ����
void WLEditImp::PopCapitalBallonTip() 
{
	CRect rcInHost ;
	GetRectInScreen(rcInHost) ;
	int offset = rcInHost.Height() / 2 ;
	CPoint point(rcInHost.left + offset, rcInHost.top + offset) ;

	// ��ʾ30�룬Ȼ���Զ���ʧ
	m_ttb.Pop(point, _T("��д�����Ѵ�"), GetCenter()->GetHost(), 30000) ;
}

UINT WLEditImp::OnGetDlgCode()
{
	//TRACE(_T("[%d]WLEdit::OnGetDlgCode\n"), GetTickCount()) ;
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT uRes = 0 ;
	uRes |= DLGC_WANTARROWS ;
	uRes |= DLGC_HASSETSEL ;

	if (!IsReadOnly())
		uRes |= DLGC_WANTCHARS ;

	return uRes ;
}

BOOL WLEditImp::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) 
{
	BOOL bRes = FALSE ;
	*pResult = 0 ;

	switch (message)
	{
	case WM_LBUTTONDOWN :
		{
			CPoint point(lParam) ;
			HostToClient(&point) ;
			OnLButtonDown(wParam, point) ;
			bRes = TRUE ;
		}
		break ;

	case WM_LBUTTONUP :
		{
			CPoint point(lParam) ;
			HostToClient(&point) ;
			OnLButtonUp(wParam, point) ;
			bRes = TRUE ;
		}
		break ;

	case WM_LBUTTONDBLCLK :
		{
			CPoint point(lParam) ;
			HostToClient(&point) ;
			OnLButtonDblClk(wParam, point) ;
			bRes = TRUE ;
		}
		break ;

	case WM_RBUTTONDOWN :
		{
			CPoint point(lParam) ;
			HostToClient(&point) ;
			OnRButtonDown(wParam, point) ;
			bRes = TRUE ;
		}
		break ;

	case WM_RBUTTONUP :
		{
			CPoint point(lParam) ;
			HostToClient(&point) ;
			OnRButtonUp(wParam, point) ;
			bRes = TRUE ;
		}
		break ;

	case WM_CHAR :
		{
			OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam)) ;
			bRes = TRUE ;
		}
		break ;

	case WM_TIMER :
		{
			OnTimer(wParam) ;
			bRes = TRUE ;
		}
		break ;

	case WM_SETFOCUS :
		{
			// �õõ�������Ϣ����ȥ
			OnSetFocus(reinterpret_cast<HWL>(wParam)) ;
			//bRes = TRUE ;
		}
		break ;

	case WM_KILLFOCUS :
		{
			// ��ʧȥ������Ϣ����ȥ
			OnKillFocus(reinterpret_cast<HWL>(wParam)) ;
		}
		break ;

	case WM_KEYDOWN :
		{
			if (wParam == VK_TAB)
			{
				bRes = FALSE ;
			}
			else
			{
				bRes = OnKeyDown(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam)) ;
			}
		}
		break ;

	case WM_SETCURSOR :
		{
			bRes = OnSetCursor(lParam) ;
		}
		break ;

	case WM_MOUSEMOVE :
		{
			CPoint point(lParam);
			HostToClient(&point) ;
			OnMouseMove(static_cast<UINT>(wParam), point);
			bRes = TRUE ;
		}
		break ;

	case WM_GETDLGCODE :
		{
			*pResult = OnGetDlgCode() ;
			bRes = TRUE ;
		}
		break ;

	case WM_CAPTURECHANGED :
		{
			OnCaptureChanged((HWL)lParam) ;
			bRes = TRUE ;
		}
		break ;

	case WM_LDOWNHOST :
		{
			//m_ttb.Push() ;
		}
		break ;
	}

	return bRes ;
}

void WLEditImp::OnSized(SIZE const &szOld, SIZE const &szNew) 
{
	m_rcTextBound.right = szNew.cx - m_nRightMargin - m_uFrameWidth ;
	m_rcTextBound.bottom = szNew.cy - m_nBottomMargin - m_uFrameWidth ;

	if (m_nOriginPosX < m_rcTextBound.left)
	{
		// ����ԭʼ��X����
		int nLastPos = m_vctCaretPos.back() ;
		if (m_nOriginPosX + nLastPos < m_rcTextBound.right)
		{
			m_nOriginPosX = m_rcTextBound.right - nLastPos ;
			if (m_nOriginPosX > m_rcTextBound.left)
				m_nOriginPosX = m_rcTextBound.left ;
		}
	}

	UpdateView() ;
}

BOOL WLEditImp::SetWindowText(LPCTSTR lpszText) 
{
	if (lpszText == NULL)
	{
		ASSERT(FALSE) ;
		return FALSE ;
	}

	SetSel(0, -1) ;
	_ReplaceSel(lpszText, TRUE) ;
	return TRUE ;
}

void WLEditImp::OnLButtonDown(UINT nFlags, CPoint point)
{
	// δ��ý���ʱ������ֻ���ý���
	if (!m_bFocus)
	{
		SetFocus() ;
		return ;
	}

	m_nCaretPos = HitTestCaretPos(point) ;
	m_bLDown = TRUE ;
	m_bBeginSel = TRUE ;

	// �ж��Ƿ���shift��
	if (nFlags & MK_SHIFT)
	{
		if (m_nFixSel < m_nCaretPos)
		{
			m_nLeftSel = m_nFixSel ;
			m_nRightSel = m_nCaretPos ;
		}
		else
		{
			m_nLeftSel = m_nCaretPos ;
			m_nRightSel = m_nFixSel ;
		}
	}
	else
	{
		m_nLeftSel = m_nRightSel = m_nCaretPos ;
		m_nFixSel = m_nCaretPos ;
	}

	if (GetCapture() != m_wrapper)
		SetCapture() ;

	UpdateView() ;

	UpdateImeWndPosByCaret() ;
}

void WLEditImp::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_bRDown = TRUE ;
	if (m_bLDown)
		m_bLDown = FALSE ;

	int nCaretPos = HitTestCaretPos(point) ;
	if (nCaretPos < m_nLeftSel || nCaretPos > m_nRightSel)
	{
		m_nFixSel = m_nLeftSel = m_nRightSel = m_nCaretPos = nCaretPos ;
	}

	if (GetCapture() != m_wrapper)
		SetCapture() ;

	SetFocus() ;
	UpdateView() ;
}

void WLEditImp::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bRDown)
	{
		m_bRDown = FALSE ;

		if (GetCapture() == m_wrapper)
			ReleaseCapture() ;

		if (!m_rcTextBound.PtInRect(point))
			return ;

// 		if (!PtInWndLess(point))
// 			return ;

		// �����Ҽ��˵�
// 		CMenu menu ;
// 		menu.CreatePopupMenu() ;
// 		menu.AppendMenu(MF_BYCOMMAND | MF_STRING, 15, _T("����")) ;

		BOOL bSelected = m_nRightSel > m_nLeftSel ;
		BOOL bReadOnly = IsReadOnly() ;
		BOOL bCanRead = !(GetStyle() & ES_PASSWORD) ;
		BOOL bHasData = IsClipboardFormatAvailable(CF_UNICODETEXT) ;
		BOOL bEmpty = IsEmpty() ;

		HHDMENU hMenu = hdui::CreatePopupMenu() ;
		//HMENU hMenu = ::CreatePopupMenu() ;

		UINT uFlags = MF_BYCOMMAND | MF_STRING ;

		hdui::AppendMenu(hMenu, (bSelected && bCanRead && !bReadOnly) ? uFlags : (uFlags | MF_GRAYED), IDM_CUT, _T("����")) ;

		hdui::AppendMenu(hMenu, (bSelected && bCanRead) ? uFlags : (uFlags | MF_GRAYED), IDM_COPY, _T("����")) ;

		hdui::AppendMenu(hMenu, (bHasData && !bReadOnly) ? uFlags : (uFlags | MF_GRAYED), IDM_PASTE, _T("ճ��")) ;

		hdui::AppendMenu(hMenu, (bSelected && !bReadOnly) ? uFlags : (uFlags | MF_GRAYED), IDM_DELETE, _T("ɾ��")) ;

		hdui::AppendMenu(hMenu, MF_BYCOMMAND | MF_SEPARATOR, 0, NULL) ;

		hdui::AppendMenu(hMenu, !bEmpty ? uFlags : (uFlags | MF_GRAYED), IDM_SELECTALL, _T("ȫѡ")) ;

		CPoint ptScreen(point) ;
		ClientToScreen(&ptScreen) ;

		m_bPopMenu = TRUE ;
		int nMenuID = hdui::TrackPopupMenu(
			hMenu,
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
			ptScreen.x, ptScreen.y, 
			//0,
			GetCenter()->GetHost(),
			NULL
			) ;
		m_bPopMenu = FALSE ;

		hdui::DestroyMenu(hMenu) ;

		// ����˵�ѡ��
		ProcessMenuCommand(nMenuID) ;
	}
}

void WLEditImp::ProcessMenuCommand(int nMenuID) 
{
	switch (nMenuID)
	{
	case 0 :
		break ;

	case IDM_UNDO :
		{
		}
		break ;

	case IDM_CUT :
		{
			Cut() ;
		}
		break ;

	case IDM_COPY :
		{
			Copy() ;
		}
		break ;

	case IDM_PASTE :
		{
			Paste() ;
		}
		break ;

	case IDM_DELETE :
		{
			Clear() ;
		}
		break ;

	case IDM_SELECTALL :
		{
			SetSel(0, -1) ;
		}
		break ;
	}
}

void WLEditImp::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetCapture() == m_wrapper)
	{
		return ;
	}

	if (m_bBeginSel)
		m_bBeginSel = FALSE ;

	// ���㵱ǰ�ı����ı�λ��
	CDC dc ;
	HDC hDC = ::GetWindowDC(GetCenter()->GetHost()) ;
	dc.Attach(hDC) ;

	CFont * pFont = CFont::FromHandle((HFONT)::SendMessage(GetCenter()->GetHost(), WM_GETFONT, 0, 0)) ;
	CFont * pOldFont = dc.SelectObject(pFont) ;

	if (nChar == VK_BACK)
	{
		if (IsReadOnly())
		{
			MessageBeep(0xffffffff) ;
		}
		else if (!(m_nLeftSel == m_nRightSel && m_nCaretPos == 0))
		{
			CString sDel ;
			int nDelTextExtendX ;
			if (m_nLeftSel == m_nRightSel)
			{
				nDelTextExtendX = m_vctCaretPos[m_nCaretPos] - m_vctCaretPos[m_nCaretPos - 1] ;

				CString &text = GetWindowTextRef();
				sDel += text[m_nCaretPos - 1] ;
				text.Delete(m_nCaretPos - 1) ;

				int i, nLen = m_vctCaretPos.size() ;
				for (i = m_nCaretPos ; i < nLen ; ++ i)
				{
					m_vctCaretPos[i] -= nDelTextExtendX ;
				}
				m_vctCaretPos.erase(m_vctCaretPos.begin() + m_nCaretPos - 1) ;

				-- m_nCaretPos ;
				m_nLeftSel = m_nRightSel = m_nCaretPos ;
			}
			else
			{
				nDelTextExtendX = m_vctCaretPos[m_nRightSel] - m_vctCaretPos[m_nLeftSel] ;
				DeleteSelNoChangeOriginX() ;
			}

			// ����ԭʼ��X����
			//int nCurPos = m_vctCaretPos[m_nCaretPos] ;
			if (m_nOriginPosX < m_rcTextBound.left && m_vctCaretPos.back() + m_nOriginPosX < m_rcTextBound.right)
			{
				m_nOriginPosX += nDelTextExtendX ;
				if (m_nOriginPosX > m_rcTextBound.left)
				{
					m_nOriginPosX = m_rcTextBound.left ;
				}
			}

			m_bCanAutoCompleteWord = FALSE ;
			SendChangeCommand() ;
		}
	}
	else if (nChar == 1)	// Ctrl + A
	{
		// ȫѡ
		SetSel(0, -1) ;
	}
	else if (nChar == 3)	// Ctrl + C
	{
		// ����
		Copy() ;
	}
	else if (nChar == 22)	// Ctrl + V
	{
		// ճ��
		if (IsReadOnly())
			MessageBeep(0xffffffff) ;
		else
			Paste() ;
	}
	else if (nChar == 24)	// Ctrl + X
	{
		// ����
		if (IsReadOnly())
			MessageBeep(0xffffffff) ;
		else
			Cut() ;
	}
	else if (nChar == 26)	// Ctrl + Z
	{
		// ����
		if (IsReadOnly())
			MessageBeep(0xffffffff) ;
		// TODO: �ݲ�ʵ��
	}
	else if (nChar == 9)	// Tab
	{
		TRACE(_T("")) ;
	}
	else
	{
		// �ж��Ƿ�������ַ�
		if (IsReadOnly())
			MessageBeep(0xffffffff) ;
		//else if (/*nChar >= 128 || */iswprint((wint_t)nChar))
		else if (hdutils::IsEditPrint(nChar))
		{
			// ������ʾ����
			m_ttb.Push() ;

			TCHAR s[2] = {0} ;
			s[0] = (wint_t)nChar ;
			ReplaceSel(s) ;

			m_bCanAutoCompleteWord = TRUE ;
			SendChangeCommand() ;
		}
	}

	m_bInputting = TRUE ;
	m_bShowCaret = TRUE ;
	UpdateView() ;

	dc.SelectObject(pOldFont) ;
	dc.Detach() ;
	::ReleaseDC(GetCenter()->GetHost(), hDC) ;
}

void WLEditImp::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMERID_EDIT_CARET)
	{
		if (m_bInputting)
		{
			m_bInputting = FALSE ;
		}
		else
		{
			m_bShowCaret = !m_bShowCaret ;
			UpdateView() ;
		}
	}
}

void WLEditImp::OnSetFocus(HWL pOldWL)
{
	m_bFocus = TRUE ;
	if (!(GetStyle() & WLES_NOHIDESEL))
		SetSel(0, -1) ;

	// �����񣬽������뷨
	if (IsPassword())
	{
		GetCenter()->DisableIme() ;

		// ����ʱ�Ƿ���Caps Lock��
		if (GetKeyState(VK_CAPITAL) & 0x01)
		{
			TRACE(_T("[%d]WM_SETFOCUS: Caps Lock\n"), GetTickCount()) ;

			// ������ʾ����
			PopCapitalBallonTip() ;
		}
	}

	if (!IsReadOnly())
	{
		SetTimer(TIMERID_EDIT_CARET, 500, NULL) ;
		m_bShowCaret = TRUE ;
	}

	UpdateView() ;

	UpdateImeWndPosByCaret() ;

    if (GetParent() != NULL)
        GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetID(), EN_SETFOCUS), (LPARAM)m_wrapper) ;
}

void WLEditImp::OnKillFocus(HWL pNewWL)
{
	m_bFocus = FALSE ;

	KillTimer(TIMERID_EDIT_CARET) ;
	m_bShowCaret = FALSE ;
	UpdateView() ;

	// ����������ʾ
	m_ttb.Push() ;

    if (GetParent() != NULL)
        GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetID(), EN_KILLFOCUS), (LPARAM)m_wrapper) ;
}

BOOL WLEditImp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetCapture() == m_wrapper)
	{
		return FALSE ;
	}

	// ���㵱ǰ�ı����ı�λ��
	BOOL bRes = FALSE ;
	if (nChar == VK_LEFT)
	{
		if (m_nCaretPos > 0)
		{
			int nOldCaretPos = m_nCaretPos ;
			-- m_nCaretPos ;

			// ����ԭʼ��X����
			int nCurPos = m_vctCaretPos[m_nCaretPos] ;
			if (nCurPos + m_nOriginPosX < m_rcTextBound.left)
			{
				if (nCurPos < m_rcTextBound.Width() / 3)
				{
					m_nOriginPosX = m_rcTextBound.left ;
				}
				else
				{
					m_nOriginPosX += m_rcTextBound.Width() / 3 ;
				}
			}

			if (GetKeyState(VK_SHIFT) & 0x80)
			{
				if (!m_bBeginSel)
				{
					m_bBeginSel = TRUE ;
					m_nFixSel = nOldCaretPos ;
				}
				if (m_nCaretPos < m_nFixSel)
				{
					m_nLeftSel = m_nCaretPos ;
					m_nRightSel = m_nFixSel ;
				}
				else
				{
					m_nRightSel = m_nCaretPos ;
					m_nLeftSel = m_nFixSel ;
				}
			}
			else
			{
				if (m_bBeginSel)
					m_bBeginSel = FALSE ;
				m_nLeftSel = m_nRightSel = m_nCaretPos ;
			}

			bRes = TRUE ;
		}
	}
	else if (nChar == VK_RIGHT)
	{
		CString &text = GetWindowTextRef();
		if (m_nCaretPos < text.GetLength())
		{
			int nOldCaretPos = m_nCaretPos ;
			m_nCaretPos ++ ;

			// ����ԭʼ��X����
			int nCurPos = m_vctCaretPos[m_nCaretPos] ;
			if (nCurPos + m_nOriginPosX >= m_rcTextBound.right)
			{
				int nLastPos = m_vctCaretPos.back() ;
				if (nLastPos + m_nOriginPosX - m_rcTextBound.right < m_rcTextBound.Width() / 3)
				{
					m_nOriginPosX -= (nLastPos + m_nOriginPosX - m_rcTextBound.right) ;
				}
				else
				{
					m_nOriginPosX -= m_rcTextBound.Width() / 3 ;
				}
			}

			if (GetKeyState(VK_SHIFT) & 0x80)
			{
				if (!m_bBeginSel)
				{
					m_bBeginSel = TRUE ;
					m_nFixSel = nOldCaretPos ;
				}
				if (m_nCaretPos < m_nFixSel)
				{
					m_nLeftSel = m_nCaretPos ;
					m_nRightSel = m_nFixSel ;
				}
				else
				{
					m_nRightSel = m_nCaretPos ;
					m_nLeftSel = m_nFixSel ;
				}
			}
			else
			{
				if (m_bBeginSel)
					m_bBeginSel = FALSE ;
				m_nLeftSel = m_nRightSel = m_nCaretPos ;
			}

			bRes = TRUE ;
		}
	}
	else if (nChar == VK_DELETE)
	{
		if (IsReadOnly())
		{
			bRes = FALSE ;
		}
		else if (!(m_nLeftSel == m_nRightSel && m_nCaretPos == int(m_vctCaretPos.size()) - 1))
		{
			CString sDel ;
			int nDelTextExtendX ;
			if (m_nLeftSel == m_nRightSel)
			{
				nDelTextExtendX = m_vctCaretPos[m_nCaretPos + 1] - m_vctCaretPos[m_nCaretPos] ;

				CString &text = GetWindowTextRef();
				sDel += text[m_nCaretPos] ;
				text.Delete(m_nCaretPos) ;

				int i, nLen = m_vctCaretPos.size() ;
				for (i = m_nCaretPos + 1 ; i < nLen ; ++ i)
				{
					m_vctCaretPos[i] -= nDelTextExtendX ;
				}
				m_vctCaretPos.erase(m_vctCaretPos.begin() + m_nCaretPos) ;
			}
			else
			{
				nDelTextExtendX = m_vctCaretPos[m_nRightSel] - m_vctCaretPos[m_nLeftSel] ;
				DeleteSelNoChangeOriginX() ;
			}

			// ����ԭʼ��X����
			//int nCurPos = m_vctCaretPos[m_nCaretPos] ;
			if (m_nOriginPosX < m_rcTextBound.left && m_vctCaretPos.back() + m_nOriginPosX < m_rcTextBound.right)
			{
				m_nOriginPosX += nDelTextExtendX ;
				if (m_nOriginPosX > m_rcTextBound.left)
				{
					m_nOriginPosX = m_rcTextBound.left ;
				}
			}

			m_bCanAutoCompleteWord = FALSE ;
			SendChangeCommand() ;

			bRes = TRUE ;
		}
	}
	else if (nChar == VK_HOME)
	{
		if (m_nCaretPos != 0 || m_nLeftSel != 0 || m_nRightSel != 0)
		{
			int nOldCaretPos = m_nCaretPos ;
			m_nCaretPos = 0 ;
			m_nLeftSel = m_nRightSel = m_nCaretPos ;
			m_nOriginPosX = m_rcTextBound.left ;

			if (GetKeyState(VK_SHIFT) & 0x80)
			{
				if (!m_bBeginSel)
				{
					m_bBeginSel = TRUE ;
					m_nFixSel = nOldCaretPos ;
				}
				if (m_nCaretPos < m_nFixSel)
				{
					m_nLeftSel = m_nCaretPos ;
					m_nRightSel = m_nFixSel ;
				}
				else
				{
					m_nRightSel = m_nCaretPos ;
					m_nLeftSel = m_nFixSel ;
				}
			}
			else
			{
				if (m_bBeginSel)
					m_bBeginSel = FALSE ;
				m_nLeftSel = m_nRightSel = m_nCaretPos ;
			}

			bRes = TRUE ;
		}
	}
	else if (nChar == VK_END)
	{
		int nLastIndex = m_vctCaretPos.size() - 1 ;
		if (m_nCaretPos != nLastIndex || m_nLeftSel != nLastIndex || m_nRightSel != nLastIndex)
		{
			int nOldCaretPos = m_nCaretPos ;
			m_nCaretPos = m_vctCaretPos.size() - 1 ;
			m_nLeftSel = m_nRightSel = m_nCaretPos ;
			m_nOriginPosX = m_rcTextBound.right - m_vctCaretPos.back() ;
			if (m_nOriginPosX > m_rcTextBound.left)
			{
				m_nOriginPosX = m_rcTextBound.left ;
			}

			// δʵ�֣���Ҫm_nFixSel
			if (GetKeyState(VK_SHIFT) & 0x80)
			{
				if (!m_bBeginSel)
				{
					m_bBeginSel = TRUE ;
					m_nFixSel = nOldCaretPos ;
				}
				if (m_nCaretPos < m_nFixSel)
				{
					m_nLeftSel = m_nCaretPos ;
					m_nRightSel = m_nFixSel ;
				}
				else
				{
					m_nRightSel = m_nCaretPos ;
					m_nLeftSel = m_nFixSel ;
				}
			}
			else
			{
				if (m_bBeginSel)
					m_bBeginSel = FALSE ;
				m_nLeftSel = m_nRightSel = m_nCaretPos ;
			}

			bRes = TRUE ;
		}
	}
	else if (nChar == VK_CAPITAL)
	{
		if (IsPassword())
		{
			if (GetKeyState(VK_CAPITAL) & 0x01)
			{
				TRACE(_T("[%d]WM_KEYDOWN: Caps Lock\n"), GetTickCount()) ;
				// ������ʾ����
				PopCapitalBallonTip() ;
			}
			else
			{
				m_ttb.Push() ;
			}
			bRes = TRUE ;
		}
	}
// 	else if (nChar == VK_RETURN || nChar == VK_ESCAPE)
// 	{
// 		bRes = FALSE ;
// 	}
	m_bInputting = TRUE ;
	m_bShowCaret = TRUE ;
	UpdateView() ;

	UpdateImeWndPosByCaret() ;

// 	dc.SelectObject(pOldFont) ;
	return bRes ;
}

int WLEditImp::HitTestCaretPos(CPoint point) 
{
// 	CRect rcClient ;
// 	GetRectInParent(rcClient) ;
// 	point.x -= rcClient.left ;
// 	point.y -= rcClient.top ;

	VCT_INT::iterator pos ;
	int i = 0 ;
	int nPrevPos = -999999 ;
	for (pos = m_vctCaretPos.begin() ; pos != m_vctCaretPos.end() ; ++ pos, ++ i)
	{
		int nTruePos = *pos + m_nOriginPosX ;
		if (point.x < nTruePos)
		{
			if (point.x < (nPrevPos + nTruePos) / 2)
			{
				return i - 1 ;
			}
			else
				return i ;
		}

		nPrevPos = nTruePos ;
	}
	return i - 1 ;
}

BOOL WLEditImp::OnSetCursor(UINT message)
{
	//TRACE(_T("[%d]WLEdit::OnSetCursor\n"), GetTickCount()) ;
	if (m_bPopMenu)
	{
		// �����˵�ʱ����ɸ���������ʵ����Ҳ��̫��������ɲ˵��������ھ���
		// �����˴���
		//TRACE(_T("[%d]WLEdit::OnSetCursor\n"), GetTickCount()) ;
		return FALSE ;
	}
	else
		::SetCursor(::LoadCursor(NULL, IDC_IBEAM)) ;
	return TRUE ;
}

int WLEditImp::GetFirstVisiblePos() const 
{
	int i, nLen = m_vctCaretPos.size() ;
	for (i = 0 ; i < nLen ; ++ i)
	{
		if (m_vctCaretPos[i] > 0)
		{
			ASSERT(i > 0) ;
			return i - 1 ;
		}
		else if (m_vctCaretPos[i] == 0)
		{
			return i ;
		}
	}

	return 0 ;
}

void WLEditImp::ReplaceSel(LPCTSTR lpszNewText) 
{
	_ReplaceSel(lpszNewText, FALSE) ;
}

void WLEditImp::_ReplaceSel(LPCTSTR lpszNewText, BOOL bForce) 
{
	// �������ʾ�ַ�
	int nLength = _tcslen(lpszNewText) ;
	TCHAR * pszNewText = new TCHAR[nLength + 1] ;
	int i = 0 ;
	TCHAR c ;
	while ((c = *lpszNewText) != 0)
	{
		//if (_iswprint_l(c, _get_current_locale()))
		if (hdutils::IsEditPrint(c))
		{
			pszNewText[i] = c ;
			++ i ;
		}
		++ lpszNewText ;
	}
	pszNewText[i] = 0 ;

	// ����������ַ����ᳬ����������������˳�
	if (!bForce && m_nMaxChars > 0)
	{
		CString &text = GetWindowTextRef();
		if (text.GetLength() - (m_nRightSel - m_nLeftSel) + i > m_nMaxChars)
		{
			delete [] pszNewText ;
			MessageBeep(0xFFFFFFFF) ;
			return ;
		}
	}

	WLDC dc(this) ;

	// ɾ��ѡ���ı�
	DeleteSelNoChangeOriginX() ;

	// �����ı�������λ��
	CString &text = GetWindowTextRef();
	text.Insert(m_nCaretPos, pszNewText) ;
	int nInsertTextLen = _tcslen(pszNewText) ;

	VCT_INT vctAddText(nInsertTextLen) ;
	int nPrevPos = m_vctCaretPos[m_nCaretPos] ;
	int nAllAddTextExtendX = 0 ;
	for (i = 0 ; i < nInsertTextLen ; ++ i)
	{
		vctAddText[i] = nPrevPos ;

		TCHAR s[2] ;
		if (GetStyle() & ES_PASSWORD)
			s[0] = 0x25cf ;
		else
			s[0] = pszNewText[i] ;
		s[1] = 0 ;

		CSize szTmp = dc.GetTextExtent(s, 1) ;
		nPrevPos += szTmp.cx ;
		nAllAddTextExtendX += szTmp.cx ;
	}
	for (i = m_vctCaretPos.size() - 1 ; i >= m_nCaretPos ; -- i)
	{
		m_vctCaretPos[i] += nAllAddTextExtendX ;
	}
	m_vctCaretPos.insert(m_vctCaretPos.begin() + m_nCaretPos, vctAddText.begin(), vctAddText.end()) ;

	m_nCaretPos += nInsertTextLen ;
	m_nLeftSel = m_nCaretPos ;
	m_nRightSel = m_nCaretPos ;
	m_nFixSel = m_nRightSel ;

/*	// ����ԭʼ��X����
	int nCurPos = m_vctCaretPos[m_nCaretPos] ;
	if (nCurPos + m_nOriginPosX >= m_rcTextBound.right)
	{
		int nLastPos = m_vctCaretPos.back() ;
		if (nLastPos + m_nOriginPosX - m_rcTextBound.right < m_rcTextBound.Width() / 3)
		{
			m_nOriginPosX -= (nLastPos + m_nOriginPosX - m_rcTextBound.right) ;
		}
		else
		{
			m_nOriginPosX -= m_rcTextBound.Width() / 3 ;
		}
	}*/

	// ����ԭʼ��X����
	// ȷ���ı�����޶ȵ���ʾ����ʾ���
	int nCurPos = m_vctCaretPos[m_nCaretPos] ;
	int nLastPos = m_vctCaretPos.back() ;
	if (m_nOriginPosX + nLastPos < m_rcTextBound.right)
	{
		m_nOriginPosX = m_rcTextBound.right - nLastPos ;
		if (m_nOriginPosX > m_rcTextBound.left)
			m_nOriginPosX = m_rcTextBound.left ;
	}
	if (nCurPos + m_nOriginPosX < m_rcTextBound.left)
	{
		m_nOriginPosX = m_rcTextBound.left - nCurPos + m_rcTextBound.Width() / 3 ;
	}
	else if (nCurPos + m_nOriginPosX >= m_rcTextBound.right)
	{
		m_nOriginPosX = m_rcTextBound.right - nCurPos ;
	}
	if (m_nOriginPosX > m_rcTextBound.left)
		m_nOriginPosX = m_rcTextBound.left ;

	delete [] pszNewText ;
	UpdateView() ;
}

void WLEditImp::DeleteSelNoChangeOriginX() 
{
	if (m_nLeftSel == m_nRightSel)
		return ;

	CString &text = GetWindowTextRef();
	text.Delete(m_nLeftSel, m_nRightSel - m_nLeftSel) ;

	int nDelTextExtendY = m_vctCaretPos[m_nRightSel] - m_vctCaretPos[m_nLeftSel] ;
	int i, nPosLen = m_vctCaretPos.size() ;
	for (i = m_nRightSel ; i < nPosLen ; ++ i)
	{
		m_vctCaretPos[i] -= nDelTextExtendY ;
	}
	m_vctCaretPos.erase(m_vctCaretPos.begin() + m_nLeftSel, m_vctCaretPos.begin() + m_nRightSel) ;

	m_nCaretPos = m_nLeftSel ;
	m_nRightSel = m_nLeftSel ;

	// ������ԭʼ��X����
}

void WLEditImp::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if (m_bBeginSel)
	if (m_bLDown)
	{
		//m_bBeginSel = FALSE ;
		m_bLDown = FALSE ;

		if (GetCapture() == m_wrapper)
			ReleaseCapture() ;
	}
}

void WLEditImp::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if (m_bBeginSel)
	if (m_bLDown)
	{
		m_nCaretPos = HitTestCaretPos(point) ;
		if (m_nCaretPos < m_nFixSel)
		{
			m_nLeftSel = m_nCaretPos ;
			m_nRightSel = m_nFixSel ;
		}
		else
		{
			m_nLeftSel = m_nFixSel ;
			m_nRightSel = m_nCaretPos ;
		}

		// ����ԭʼ��X����
		int nCurPos = m_vctCaretPos[m_nCaretPos] ;
		if (m_nOriginPosX + nCurPos < m_rcTextBound.left)
		{
			m_nOriginPosX = m_rcTextBound.left - nCurPos ;
		}
		else if (m_nOriginPosX + nCurPos > m_rcTextBound.right)
		{
			m_nOriginPosX = m_rcTextBound.right - nCurPos - m_rcTextBound.Width() / 3 ;
			if (m_nOriginPosX < m_rcTextBound.right - m_vctCaretPos.back())
			{
				m_nOriginPosX = m_rcTextBound.right - m_vctCaretPos.back() ;
			}
		}

		UpdateView() ;
	}
}

void WLEditImp::SetSel(int nStartChar, int nEndChar) 
{
	if (nStartChar < -1 || nEndChar < -1)
		return ;

	if (nStartChar == -1)
		nStartChar = m_vctCaretPos.size() - 1 ;

	if (nEndChar == -1)
		nEndChar = m_vctCaretPos.size() - 1 ;

	if (nStartChar > nEndChar)
		return ;

	m_nLeftSel = nStartChar ;
	m_nRightSel = nEndChar ;
	m_nCaretPos = nEndChar ;

	// ����ԭʼ��X����
	int nCurPos = m_vctCaretPos[m_nCaretPos] ;
	if (nCurPos + m_nOriginPosX < m_rcTextBound.left)
	{
		m_nOriginPosX = m_rcTextBound.left - nCurPos + m_rcTextBound.Width() / 3 ;
	}
	if (m_nOriginPosX > m_rcTextBound.left)
		m_nOriginPosX = m_rcTextBound.left ;

	UpdateView() ;
}

void WLEditImp::LimitText(int nChars) 
{
	if (nChars >= 0)
		m_nMaxChars = nChars ;
}

COLORREF WLEditImp::SetFrameColor(COLORREF crFrame) 
{
	COLORREF crOldFrame = m_crFrameColor ;
	m_crFrameColor = crFrame ;
	return crOldFrame ;
}

COLORREF WLEditImp::SetTextNormalColor(COLORREF crTextNormal) 
{
	COLORREF crOldTextNormalColor = m_crTextNormalColor ;
	m_crTextNormalColor = crTextNormal ;
	return crOldTextNormalColor ;
}

Color WLEditImp::SetBkgndColor(Color crBkgndColor) 
{
	Color crOldBkCOlor = m_crBkgndColor ;
	m_crBkgndColor = crBkgndColor ;
	return crOldBkCOlor ;
}

void WLEditImp::Undo() 
{

}

void WLEditImp::Cut() 
{
	if (IsReadOnly() || IsPassword())
		return ;

	Copy() ;
	Clear() ;
}

void WLEditImp::Copy() 
{
	if (IsPassword())
		return ;

	if (m_nLeftSel >= m_nRightSel)
		return ;

	if (!OpenClipboard(GetCenter()->GetHost()))
		return ;
	EmptyClipboard() ;

	int nLength = m_nRightSel - m_nLeftSel ;
	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, (nLength + 1) * sizeof(wchar_t)) ;
	if (hData == NULL)
		return ;

	wchar_t * pszText = (wchar_t *)GlobalLock(hData) ;
	CString &text = GetWindowTextRef();
	memcpy(pszText, text.GetString() + m_nLeftSel, nLength * sizeof(wchar_t)) ;
	pszText[nLength] = 0 ;
	GlobalUnlock(hData) ;

	SetClipboardData(CF_UNICODETEXT, hData) ;
	CloseClipboard() ;
}

void WLEditImp::Paste() 
{
	if (IsReadOnly())
		return ;

	// ��ü��а�����
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
		return ;

	if (!OpenClipboard(GetCenter()->GetHost()))
		return ;

	wchar_t * pszText = NULL ;
	HGLOBAL hData = GetClipboardData(CF_UNICODETEXT) ;
	if (hData != NULL)
	{
		pszText = (wchar_t*)GlobalLock(hData) ;
		if (pszText != NULL)
		{
			ReplaceSel(pszText) ;
		}

		GlobalUnlock(hData) ;
	}

	CloseClipboard() ;
	UpdateView() ;

	m_bCanAutoCompleteWord = TRUE ;
	SendChangeCommand() ;
}

void WLEditImp::Clear() 
{
	if (IsReadOnly())
		return ;

	if (m_nLeftSel >= m_nRightSel)
		return ;

	int nDelTextExtendX ;
	nDelTextExtendX = m_vctCaretPos[m_nRightSel] - m_vctCaretPos[m_nLeftSel] ;
	DeleteSelNoChangeOriginX() ;

	// ����ԭʼ��X����
	//int nCurPos = m_vctCaretPos[m_nCaretPos] ;
	if (m_nOriginPosX < m_rcTextBound.left && m_vctCaretPos.back() + m_nOriginPosX < m_rcTextBound.right)
	{
		m_nOriginPosX += nDelTextExtendX ;
		if (m_nOriginPosX > m_rcTextBound.left)
		{
			m_nOriginPosX = m_rcTextBound.left ;
		}
	}

	UpdateView() ;

	m_bCanAutoCompleteWord = FALSE ;
	SendChangeCommand() ;
}

// ��øպô������ʱ�Ĵ�С
void WLEditImp::CalcWrapContentSize(LPSIZE lpSize) 
{
	if (lpSize == NULL)
	{
		ASSERT(FALSE) ;
		return ;
	}

	WLDC dc(this) ;
	CString &text = GetWindowTextRef();
	CSize sz = dc.GetTextExtent(text) ;
	lpSize->cx = m_uFrameWidth + m_nLeftMargin + sz.cx + m_nRightMargin + m_uFrameWidth ;
	lpSize->cy = m_uFrameWidth + m_nTopMargin + sz.cy + m_nBottomMargin + m_uFrameWidth ;
}

void WLEditImp::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	SetSel(0, -1) ;
}

void WLEditImp::OnCaptureChanged(HWL hWl) 
{
	m_bLDown = FALSE ;
	//m_bBeginSel = FALSE ;
}


template<>
UILIB_EXPORT WLEditImpBase *CreateDefaultImp< WLEditImpBase >(IWLEdit * wrapper)

{
	WLEditImp *ret = new WLEditImp();
	ret->SetWrapper(wrapper);
	return ret;
};
