//////////////////////////////////////////////////////////////////////////
//
//	������	��ʾ�ؼ�����
//	
//	���ڣ�	06/10/2009
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPBASEST_H__F3E9ECBA_F812_472D_B015_6B4AF41FABD7__INCLUDED_)
#define AFX_TOOLTIPBASEST_H__F3E9ECBA_F812_472D_B015_6B4AF41FABD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipBaseST.h : header file
//

// Class Name
#define ST_TOOLTIP_BASE	_T("St.ToolTipBase")

/////////////////////////////////////////////////////////////////////////////
// CToolTipBaseST window

class CToolTipBaseST : public CWnd
{
// Construction
public:
	CToolTipBaseST();
	virtual ~CToolTipBaseST();

// Attributes
public:
protected:
	CFont	m_font ;			// ����

// Operations
public:
	// ����������ʾ
	BOOL Pop(CWnd * pParent) ;
	// ѹ��������ʾ
	virtual void Push() ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipBaseST)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipBaseST)
	afx_msg LRESULT OnGetFont(WPARAM, LPARAM) ;
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPBASEST_H__F3E9ECBA_F812_472D_B015_6B4AF41FABD7__INCLUDED_)
