// ToolTipBaseST.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipBaseST.h"

#include "logex.h"

/////////////////////////////////////////////////////////////////////////////
// CToolTipBaseST

CToolTipBaseST::CToolTipBaseST()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, ST_TOOLTIP_BASE, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_OWNDC ;
		wndcls.lpfnWndProc		= ::DefWindowProc ;					// ������Ϣ������
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0 ;
		wndcls.hInstance		= hInst ;							// ʵ�����
		wndcls.hIcon			= NULL;								// ͼ��
		wndcls.hCursor			= ::LoadCursor( NULL, IDC_ARROW ) ;	// ���
		//wndcls.hbrBackground	= HBRUSH(COLOR_INFOBK + 1) ;		// ��ˢ(��ʾ������)
		wndcls.hbrBackground	= HBRUSH(0) ;						// ��ˢ(�ջ�ˢ����ɾ������)
		wndcls.lpszMenuName		= NULL ;
		wndcls.lpszClassName	= ST_TOOLTIP_BASE ;				// ������
		
		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
}

CToolTipBaseST::~CToolTipBaseST()
{
}


BEGIN_MESSAGE_MAP(CToolTipBaseST, CWnd)
	//{{AFX_MSG_MAP(CToolTipBaseST)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Operations

BOOL CToolTipBaseST::Pop(CWnd * pParent)
{
	// ��ѹ��֮ǰ������
	Push() ;
	
	// ��������
#ifndef UNICODE
	TCHAR szWN[7] = {-63, -42, -84, 69, -43, -16, 0} ;
#else
	TCHAR szWN[4] = {26519, 29693, 38663, 0} ;
#endif
	
	BOOL bRes = CreateEx(
		WS_EX_TOOLWINDOW,
		ST_TOOLTIP_BASE,
		szWN,
		WS_POPUP,
		CRect(CW_USEDEFAULT, CW_USEDEFAULT, 0, 0),
		pParent,
		0) ;

	if (bRes)
	{
		// ��¼����
		LOGFONT lf ;
		pParent->GetFont()->GetLogFont(&lf) ;
		m_font.CreateFontIndirect(&lf) ;
	}

	return bRes ;
}

// �������ܣ�ѹ��������ʾ
// ���������
// ����ֵ  ��
// ��д�˼����� ���֬E��[06/12/2009]
void CToolTipBaseST::Push() 
{
	// �����ǰ��һ�����ڣ������ٴ���
	if (::IsWindow(GetSafeHwnd()))
		DestroyWindow() ;
	m_font.DeleteObject() ;
}

/////////////////////////////////////////////////////////////////////////////
// CToolTipBaseST message handlers

int CToolTipBaseST::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UNUSED_ALWAYS(pDesktopWnd) ;
	UNUSED_ALWAYS(nHitTest) ;
	UNUSED_ALWAYS(message) ;
	return MA_NOACTIVATE ;
//	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

LRESULT CToolTipBaseST::OnGetFont(WPARAM, LPARAM) 
{
	return (LRESULT)m_font.GetSafeHandle() ;
}