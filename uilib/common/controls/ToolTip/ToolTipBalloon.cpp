// ToolTipBallon.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipBalloon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipBalloon

CToolTipBalloon::CToolTipBalloon()
{
	m_nMouthBeginOffset	= 15 ;	// ������λ��
	m_nMouthEndOffset	= 15 ;	// ����յ�λ��
	m_nMouthLength	= 15 ;	// ��ͳ���
	m_nMouthWidth	= 15 ;	// ��Ϳ��

	m_nRoundDia		= 13 ;	// Բ��ֱ��

	m_nMargin		= 10 ;	// ��Ե���

	m_nBalloonWidth	= 120 ;	// ���ݿ��

	m_nBtnMargin	= 5 ;	// �رհ�ť�ͱ�Ե֮��Ŀ�϶
	m_szCloseBtn.cx = 16 ;	// �رհ�ť�Ŀ��
	m_szCloseBtn.cy = 16 ;	// �رհ�ť�ĸ߶�

	m_crFrame		= RGB(0, 0, 0) ;	// �߿���ɫ
	m_bTopMost		= FALSE ;			// û���ö�
	m_bFriable		= FALSE ;			// ������

	m_nCloseBtnMode = CBM_NORMAL ;	// �رհ�ť��ǰģʽ
	m_bCloseBtnDown = FALSE ;		// �رհ�ť�Ƿ���

	m_uDuration		= (UINT)~0 ;			// ����ʱ��
	m_uLastTime		= (UINT)~0 ;			// ʣ��ʱ��
	m_nTimePiece	= 100 ;			// ʱ��Ƭ

	m_arnDirPRI[0]	= 0 ;
	m_arnDirPRI[1]	= 1 ;
	m_arnDirPRI[2]	= 2 ;
	m_arnDirPRI[3]	= 3 ;
}

CToolTipBalloon::~CToolTipBalloon()
{
}


BEGIN_MESSAGE_MAP(CToolTipBalloon, CToolTipBaseST)
	//{{AFX_MSG_MAP(CToolTipBalloon)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Operations

// �������ܣ�����������ʾ
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/10/2009]
BOOL CToolTipBalloon::Pop(CPoint ptPos, CString const &strText, HWND hParent, UINT nDuration /*= ~0*/) 
{
	CWnd * pParent = CWnd::FromHandle(hParent) ;
	return Pop(ptPos, strText, pParent, nDuration) ;
}

BOOL CToolTipBalloon::Pop(CPoint ptPos, CString const &strText, CWnd * pParent, UINT nDuration /* = ~0 */)
{
	if (!CToolTipBaseST::Pop(pParent))
		return FALSE ;

	// ��¼λ�ú���ʾ�ı�
	m_ptMouthPos = ptPos ;
	m_strText = strText ;

	// ������ʾ�ؼ���λ�úʹ�С
	CalcPostion() ;

	// �Ǽ�����ʾ����
	SetWindowPos(
		m_bTopMost ? &CWnd::wndTopMost : NULL,
		m_ptBalloon.x,
		m_ptBalloon.y,
		m_nTotalWidth,
		m_nTotalHeight,
		SWP_NOACTIVATE | SWP_SHOWWINDOW) ;

//	AnimateWindow(GetSafeHwnd(), 500, AW_BLEND) ;

	m_uDuration = nDuration ;
	m_uLastTime = nDuration ;
	if (m_uDuration != ~0)
	{
		int n = SetTimer(1, m_nTimePiece, NULL) ;
	}

	// ���������ģ���Ҫ��׽���
	if (m_bFriable && GetCapture()->GetSafeHwnd() != GetSafeHwnd())
	{
		SetCapture() ;
	}

	return TRUE ;
}

// �������ܣ�������ʾ�ؼ���λ�úʹ�С
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/10/2009]
void CToolTipBalloon::CalcPostion()
{
	CClientDC dc(this) ;
	CFont * pOldFont = dc.SelectObject(GetFont()) ;
	// 1. ����߶�
	// 1.1. ��0
	m_nBalloonHeight = 0 ;
	m_nTotalHeight = 0 ;

	// 1.2. �ۼ���ͳ���
	m_nTotalHeight += m_nMouthLength ;

	// 1.3. �ۼӱ�Ե���
	m_nBalloonHeight += 2 * m_nMargin ;

	// 1.4. �ۼ����ָ߶�
	// 1.4.1 ������ֿ��
	m_ptText.x = 0 ;
	m_ptText.y = 0 ;
	m_szText.cx = m_nBalloonWidth - m_nMargin - m_szCloseBtn.cx - m_nBtnMargin - m_nBtnMargin ;
	m_szText.cy = 0 ;
	CRect rcText(m_ptText, m_szText) ;

	// 1.4.2 ������ָ߶�
	dc.DrawText(m_strText, rcText, DT_WORDBREAK | DT_CALCRECT) ;

	m_szText.cx = rcText.Width() ;
	m_szText.cy = rcText.Height() ;
	m_nBalloonHeight += rcText.Height() ;
	m_nTotalHeight += m_nBalloonHeight ;

	// 2. ȷ��ð�ݷ���(��ʱȷ��������λ�á��رհ�ťλ�úʹ�������)
	// 2.1. �����Ļ���صĴ�С
	int nCXScreen = GetSystemMetrics(SM_CXSCREEN) ;
	int nCYScreen = GetSystemMetrics(SM_CYSCREEN) ;

	// 2.2. ����ϡ��¡������ĸ������Ƿ����㹻�ռ�
	BOOL bLeftEnough = FALSE, bTopEnough = FALSE, bRightEnough = FALSE, bBottomEnough = FALSE ;
	bLeftEnough = (m_ptMouthPos.x >= (m_nBalloonWidth - m_nMouthBeginOffset)) ;
	bRightEnough = ((nCXScreen - m_ptMouthPos.x) >= (m_nBalloonWidth - m_nMouthBeginOffset)) ;
	bTopEnough = (m_ptMouthPos.y >= m_nTotalHeight) ;
	bBottomEnough = ((nCYScreen - m_ptMouthPos.y) >= m_nTotalHeight) ;

	// 2.2. �������ð�ݷ���
/*	int nDirection = 0 ;	// ����Ĭ������
	if (bLeftEnough && bTopEnough)
	{
		nDirection = 0 ;	// ����
	}
	else if (bLeftEnough && bBottomEnough)
	{
		nDirection = 1 ;	// ����
	}
	else if (bRightEnough && bTopEnough)
	{
		nDirection = 2 ;	// ����
	}
	else if (bRightEnough && bBottomEnough)
	{
		nDirection = 3 ;	// ����
	}*/

	BOOL b[4] ;
	b[0] = bLeftEnough && bTopEnough ;
	b[1] = bLeftEnough && bBottomEnough ;
	b[2] = bRightEnough && bTopEnough ;
	b[3] = bRightEnough && bBottomEnough ;

	int nDirection = m_arnDirPRI[0] ;
	int i ;
	for (i = 0 ; i < 4 ; ++ i)
	{
//		int nPRI = m_arnDirPRI[i] ;
		if (b[m_arnDirPRI[i] & 0x3])
		{
			nDirection = m_arnDirPRI[i] ;
			break ;
		}
	}

	// 2.3. ����ð�ݷ����������λ�á��رհ�ťλ�úʹ�������
	m_ptBalloon = m_ptMouthPos ;
	m_nTotalWidth = m_nBalloonWidth + (m_nMouthBeginOffset > 0 ? 0 : -m_nMouthBeginOffset ) ;
	CRgn rgnAll, rgnMain, rgnMouth ;
	switch(nDirection)
	{
	case 0 :	// ����
		m_ptText.x = m_nMargin ;
		m_ptText.y = m_nMargin ;
		m_ptCloseBtn.x = m_nBalloonWidth - m_nBtnMargin - m_szCloseBtn.cx ;
		m_ptCloseBtn.y = m_nBtnMargin ;
		m_ptBalloon.Offset(-(m_nBalloonWidth - m_nMouthBeginOffset), -m_nTotalHeight) ;
		rgnMain.CreateRoundRectRgn(
			0, 0,
			m_nBalloonWidth + 1, m_nBalloonHeight + 1,
			m_nRoundDia, m_nRoundDia) ;

		dc.BeginPath() ;
		dc.MoveTo(m_nBalloonWidth - m_nMouthBeginOffset, m_nTotalHeight) ;
		dc.LineTo(m_nBalloonWidth - m_nMouthEndOffset, m_nTotalHeight - m_nMouthLength) ;
		dc.LineTo(m_nBalloonWidth - m_nMouthEndOffset - m_nMouthWidth, m_nTotalHeight - m_nMouthLength) ;
		dc.LineTo(m_nBalloonWidth - m_nMouthBeginOffset, m_nTotalHeight) ;
		dc.EndPath() ;
		rgnMouth.CreateFromPath(&dc) ;
		break;

	case 1 :	// ����
		m_ptText.x = m_nMargin ;
		m_ptText.y = m_nMargin + m_nMouthLength ;
		m_ptCloseBtn.x = m_nBalloonWidth - m_nBtnMargin - m_szCloseBtn.cx ;
		m_ptCloseBtn.y = m_nBtnMargin + m_nMouthLength ;
		m_ptBalloon.Offset(-(m_nBalloonWidth - m_nMouthBeginOffset), 0) ;
		rgnMain.CreateRoundRectRgn(
			0, m_nMouthLength,
			m_nBalloonWidth + 1, m_nTotalHeight + 1,
			m_nRoundDia, m_nRoundDia) ;
		
		dc.BeginPath() ;
		dc.MoveTo(m_nBalloonWidth - m_nMouthBeginOffset, 0) ;
		dc.LineTo(m_nBalloonWidth - m_nMouthEndOffset, m_nMouthLength) ;
		dc.LineTo(m_nBalloonWidth - m_nMouthEndOffset - m_nMouthWidth, m_nMouthLength) ;
		dc.LineTo(m_nBalloonWidth - m_nMouthBeginOffset, 0) ;
		dc.EndPath() ;
		rgnMouth.CreateFromPath(&dc) ;
		break;

	case 2 :	// ����
//		m_ptText.x = m_nMouthBeginOffset - m_nMouthEndOffset + m_nMargin ;
		m_ptText.y = m_nMargin ;
		m_ptCloseBtn.x = m_nTotalWidth - m_nBtnMargin - m_szCloseBtn.cx ;
		m_ptCloseBtn.y = m_nBtnMargin ;
		if (m_nMouthBeginOffset < 0)
		{
			m_ptText.x = -m_nMouthBeginOffset + m_nMargin ;
			m_ptBalloon.Offset(0, -m_nTotalHeight) ;
			rgnMain.CreateRoundRectRgn(
				-m_nMouthBeginOffset, 0,
				m_nTotalWidth + 1, m_nBalloonHeight + 1,
				m_nRoundDia, m_nRoundDia) ;
			dc.BeginPath() ;
			dc.MoveTo(0, m_nTotalHeight) ;
			dc.LineTo(-m_nMouthBeginOffset + m_nMouthEndOffset, m_nBalloonHeight) ;
			dc.LineTo(-m_nMouthBeginOffset + m_nMouthEndOffset + m_nMouthWidth, m_nBalloonHeight) ;
			dc.LineTo(0, m_nTotalHeight) ;
			dc.EndPath() ;
		}
		else
		{
			m_ptText.x = m_nMargin ;
			m_ptBalloon.Offset(-m_nMouthBeginOffset, -m_nTotalHeight) ;
			rgnMain.CreateRoundRectRgn(
				0, 0,
				m_nBalloonWidth + 1, m_nBalloonHeight + 1,
				m_nRoundDia, m_nRoundDia) ;
			dc.BeginPath() ;
			dc.MoveTo(m_nMouthBeginOffset, m_nTotalHeight) ;
			dc.LineTo(m_nMouthEndOffset, m_nBalloonHeight) ;
			dc.LineTo(m_nMouthEndOffset + m_nMouthWidth, m_nBalloonHeight) ;
			dc.LineTo(m_nMouthBeginOffset, m_nTotalHeight) ;
			dc.EndPath() ;
		}

		rgnMouth.CreateFromPath(&dc) ;
	    break;

	case 3 :	// ����
		m_ptText.y = m_nMargin + m_nMouthLength ;
		m_ptCloseBtn.x = m_nTotalWidth - m_nBtnMargin - m_szCloseBtn.cx ;
		m_ptCloseBtn.y = m_nBtnMargin + m_nMouthLength ;
		if (m_nMouthBeginOffset < 0)
		{
			m_ptText.x = -m_nMouthBeginOffset + m_nMargin ;
			m_ptBalloon.Offset(0, 0) ;
			rgnMain.CreateRoundRectRgn(
				-m_nMouthBeginOffset, m_nMouthLength,
				m_nTotalWidth + 1, m_nTotalHeight + 1,
				m_nRoundDia, m_nRoundDia) ;
			
			dc.BeginPath() ;
			dc.MoveTo(0, 0) ;
			dc.LineTo(-m_nMouthBeginOffset + m_nMouthEndOffset, m_nMouthLength) ;
			dc.LineTo(-m_nMouthBeginOffset + m_nMouthEndOffset + m_nMouthWidth, m_nMouthLength) ;
			dc.LineTo(0, 0) ;
			dc.EndPath() ;
		}
		else
		{
			m_ptText.x = m_nMargin ;
			m_ptBalloon.Offset(-m_nMouthBeginOffset, 0) ;
			rgnMain.CreateRoundRectRgn(
				0, m_nMouthLength,
				m_nTotalWidth + 1, m_nTotalHeight + 1,
				m_nRoundDia, m_nRoundDia) ;
			
			dc.BeginPath() ;
			dc.MoveTo(m_nMouthBeginOffset, 0) ;
			dc.LineTo(m_nMouthEndOffset, m_nMouthLength) ;
			dc.LineTo(m_nMouthEndOffset + m_nMouthWidth, m_nMouthLength) ;
			dc.LineTo(m_nMouthBeginOffset, 0) ;
			dc.EndPath() ;
		}
		rgnMouth.CreateFromPath(&dc) ;
	    break;

	default:
		ASSERT(FALSE) ;
	    break;
	}
	rgnAll.CreateRectRgn(0, 0, 0, 0) ;
	rgnAll.CombineRgn(&rgnMain, &rgnMouth, RGN_OR) ;
	m_rgnBolloon.CreateRectRgn(0, 0, 0, 0) ;
	m_rgnBolloon.CopyRgn(&rgnAll) ;
	SetWindowRgn(rgnAll, TRUE) ;

	dc.SelectObject(pOldFont) ;
}

// �������ܣ�ѹ��������ʾ
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/12/2009]
void CToolTipBalloon::Push() 
{
	CToolTipBaseST::Push() ;

	m_rgnBolloon.DeleteObject() ;
	m_nCloseBtnMode = CBM_NORMAL ;
	m_bCloseBtnDown = FALSE ;
}

// �������ܣ����ƹرհ�ť
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/11/2009]
void CToolTipBalloon::DrawCloseBtn(CDC * pDC, CRect rect, CLOSEBTN_MODE nMode) 
{
	if (rect.Width() == 0 || rect.Height() == 0)
		return ;

	CPen pen(PS_SOLID, 1, RGB(128, 128, 128)) ;
	CPen * pOldPen = pDC->SelectObject(&pen) ;
	CBrush brush ;//(GetSysColor(COLOR_BTNFACE)) ;
	if (nMode == CBM_NORMAL)
	{
		brush.CreateSolidBrush(RGB(212, 208, 200)) ;
	}
	else
	{
		brush.CreateSolidBrush(RGB(232, 228, 220)) ;
	}
	CBrush * pOldBrush = pDC->SelectObject(&brush) ;

	// ���Ʊ߿�
	pDC->Rectangle(rect) ;

	// ����x
	CPen penX(PS_SOLID, 2, RGB(0, 0, 0)) ;
	pDC->SelectObject(&penX) ;

	int nWidth = rect.Width() / 4, nHeight = rect.Height() / 4 ;
	CPoint ptTL(rect.left + nWidth, rect.top + nHeight) ;
	CPoint ptBL(rect.left + nWidth, rect.bottom - nHeight - 1) ;
	CPoint ptTR(rect.right - nWidth - 1, rect.top + nHeight) ;
	CPoint ptBR(rect.right - nWidth - 1, rect.bottom - nHeight - 1) ;
	if (nMode == CBM_PUSH)
	{
		ptTL.Offset(1, 1) ;
		ptBL.Offset(1, 1) ;
		ptTR.Offset(1, 1) ;
		ptBR.Offset(1, 1) ;
	}
	pDC->MoveTo(ptTL) ;
	pDC->LineTo(ptBR) ;
	pDC->MoveTo(ptTR) ;
	pDC->LineTo(ptBL) ;

	pDC->SelectObject(pOldBrush) ;
	pDC->SelectObject(pOldPen) ;
}

// �������ܣ����á���ȡ���
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/11/2009]
void CToolTipBalloon::SetWidth(int nWidth) 
{
	m_nBalloonWidth = nWidth ;
}

int CToolTipBalloon::GetWidth() const
{
	return m_nBalloonWidth ;
}

// �������ܣ����á���ȡ��ͳ��ȡ���ȡ�λ��
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/11/2009]
void CToolTipBalloon::SetMouthLength(int nLength) 
{
	m_nMouthLength = nLength ;
}

int CToolTipBalloon::GetMouthLength() const 
{
	return m_nMouthLength ;
}

void CToolTipBalloon::SetMouthWidth(int nWidth) 
{
	m_nMouthWidth = nWidth ;
}

int CToolTipBalloon::GetMouthWidth() const 
{
	return m_nMouthWidth ;
}

void CToolTipBalloon::SetMouthBeginOffset(int nOffset) 
{
	if (nOffset <= m_nMouthEndOffset)
	{
		m_nMouthBeginOffset = nOffset ;
	}
	else
	{
		m_nMouthBeginOffset = m_nMouthEndOffset ;
	}
}

int CToolTipBalloon::GetMouthBeginOffset() const 
{
	return m_nMouthBeginOffset ;
}

void CToolTipBalloon::SetMouthEndOffset(int nOffset) 
{
	m_nMouthEndOffset = nOffset ;
}

int CToolTipBalloon::GetMouthEndOffset() const 
{
	return m_nMouthEndOffset ;
}

// �������ܣ����á���ȡ�رհ�ť���ȡ����
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/18/2009]
void CToolTipBalloon::SetCloseBtnSize(int cx, int cy)
{
	m_szCloseBtn.cx = cx ;
	m_szCloseBtn.cy = cy ;
}

CSize CToolTipBalloon::GetCloseBtnSize() const
{
	return m_szCloseBtn ;
}

// �������ܣ����á���ȡԲ��ֱ��
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/11/2009]
void CToolTipBalloon::SetRoundDia(int nRoundDia) 
{
	m_nRoundDia = nRoundDia ;
}

int CToolTipBalloon::GetRoundDia() const 
{
	return m_nRoundDia ;
}

// �������ܣ����ñ�Ե���
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/11/2009]
void CToolTipBalloon::SetMargin(int nMargin) 
{
	m_nMargin = nMargin ;
}

int CToolTipBalloon::GetMargin() const 
{
	return m_nMargin ;
}

// �������ܣ����á���ȡ�߿���ɫ
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/18/2009]
void CToolTipBalloon::SetFrameColor(COLORREF crFrame) 
{
	m_crFrame = crFrame ;
}

COLORREF CToolTipBalloon::GetFrameColor() const 
{
	return m_crFrame ;
}

// �������ܣ����á���ȡ�����Ƿ��ö�
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/18/2009]
void CToolTipBalloon::SetTopMost(BOOL bTopMost/* = TRUE*/) 
{
	m_bTopMost = bTopMost ;
}

BOOL CToolTipBalloon::IsTopMost() const 
{
	return m_bTopMost ;
}

// �������ܣ����á���ȡ�Ƿ�����
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/18/2009]
void CToolTipBalloon::SetFriable(BOOL bFriable/* = TRUE*/) 
{
	m_bFriable = bFriable ;
}

BOOL CToolTipBalloon::GetFriable() const 
{
	return m_bFriable ;
}

// �������ܣ�����ð�ݷ������ȼ�
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/18/2009]
void CToolTipBalloon::SetDirPRI(UINT n0, UINT n1, UINT n2, UINT n3) 
{
	m_arnDirPRI[0] = (n0 & 0x3) ;
	m_arnDirPRI[1] = (n1 & 0x3) ;
	m_arnDirPRI[2] = (n2 & 0x3) ;
	m_arnDirPRI[3] = (n3 & 0x3) ;
}

/////////////////////////////////////////////////////////////////////////////
// CToolTipBalloon message handlers

void CToolTipBalloon::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient ;
	GetClientRect(&rcClient) ;
	CDC dcMem ;
	dcMem.CreateCompatibleDC(&dc) ;
	CBitmap bmpMem ;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height()) ;
	CBitmap * pOldMemBmp = dcMem.SelectObject(&bmpMem) ;

	CBrush brBkgnd(GetSysColor(COLOR_INFOBK)) ;
	dcMem.FillRect(rcClient, &brBkgnd) ;

	// ���Ʊ߿�
	CBrush brTemp(m_crFrame) ;
	BOOL b = dcMem.FrameRgn(&m_rgnBolloon, &brTemp, 1, 1) ;

	// ��������
	CFont * pOldFont = dcMem.SelectObject(GetFont()) ;
	int nOldMode = dcMem.SetBkMode(TRANSPARENT) ;

	CRect rcText(m_ptText, m_szText) ;
	dcMem.DrawText(m_strText, rcText, DT_WORDBREAK) ;

	dcMem.SetBkMode(nOldMode) ;
	dcMem.SelectObject(pOldFont) ;

	// ���ƹرհ�ť
	CRect rcCloseBtn(m_ptCloseBtn, m_szCloseBtn) ;
	DrawCloseBtn(&dcMem, rcCloseBtn, m_nCloseBtnMode) ;

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY) ;

	dcMem.SelectObject(pOldMemBmp) ;
}

void CToolTipBalloon::OnMouseMove(UINT nFlags, CPoint point) 
{
	// ��׽���
	if (GetCapture() != this)
	{
		SetCapture() ;
	}
	// ֹͣ��ʱ
	TRACE(_T("KillTimer\n")) ;
	KillTimer(1) ;

	CRect rcCloseBtn(m_ptCloseBtn, m_szCloseBtn) ;
	CLOSEBTN_MODE nMode ;
	if (rcCloseBtn.PtInRect(point))
	{
		if (m_bCloseBtnDown)
		{
			nMode = CBM_PUSH ;
		}
		else
		{
			nMode = CBM_HOVER ;
		}
	}
	else
	{
		nMode = CBM_NORMAL ;
		if (!m_rgnBolloon.PtInRegion(point) && !m_bCloseBtnDown)
		{
			// �ͷ����(���������������Ҫ�ͷ����)
			if (!m_bFriable)
			{
				ReleaseCapture() ;
			}
			// ��ʼ��ʱ
			SetTimer(1, m_nTimePiece, NULL) ;
		}
	}
	if (m_nCloseBtnMode != nMode)
	{
		m_nCloseBtnMode = nMode ;
		InvalidateRect(rcCloseBtn, FALSE) ;
	}

	CToolTipBaseST::OnMouseMove(nFlags, point);
}

void CToolTipBalloon::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcCloseBtn(m_ptCloseBtn, m_szCloseBtn) ;
	if (rcCloseBtn.PtInRect(point))
	{
		m_bCloseBtnDown = TRUE ;
		m_nCloseBtnMode = CBM_PUSH ;
		InvalidateRect(rcCloseBtn, FALSE) ;
	}
	else
	{
		// �������Ϣ��������Ĵ���
		CPoint ptScreen(point) ;
		ClientToScreen(&ptScreen) ;
		CWnd *pWnd = CWnd::WindowFromPoint(ptScreen) ;
		if (pWnd != NULL && pWnd->GetSafeHwnd() != GetSafeHwnd())
		{
			LRESULT lres = pWnd->SendMessage(WM_NCHITTEST, 0, MAKELPARAM(ptScreen.x, ptScreen.y)) ;
			if (lres == HTCLIENT)
			{
				pWnd->ScreenToClient(&ptScreen) ;
				pWnd->SendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(ptScreen.x, ptScreen.y)) ;
			}
			else
			{
				lres = pWnd->SendMessage(WM_NCLBUTTONDOWN, lres, MAKELPARAM(ptScreen.x, ptScreen.y)) ;
			}
		}
		// ���ٴ���
		DestroyWindow() ;
	}
	CToolTipBaseST::OnLButtonDown(nFlags, point);
}

void CToolTipBalloon::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// �ͷ����
	if (GetCapture() == this)
	{
		ReleaseCapture() ;
		// ��ʼ��ʱ
		TRACE(_T("OnLButtonUp\n")) ;
		int n = SetTimer(1, m_nTimePiece, NULL) ;
	}

	CRect rcCloseBtn(m_ptCloseBtn, m_szCloseBtn) ;
	if (m_bCloseBtnDown)
	{
		m_bCloseBtnDown = FALSE ;
		if (rcCloseBtn.PtInRect(point))
		{
			// �رմ���
			DestroyWindow() ;
		}
	}
	else
		CToolTipBaseST::OnLButtonUp(nFlags, point) ;
}

void CToolTipBalloon::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		m_uLastTime -= m_nTimePiece ;
		if (m_uLastTime <= 0)
		{
			// ����ʱ����������ٴ���
			DestroyWindow() ;
		}
	}
	CToolTipBaseST::OnTimer(nIDEvent);
}

void CToolTipBalloon::OnDestroy() 
{
	CToolTipBaseST::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(1) ;
}
