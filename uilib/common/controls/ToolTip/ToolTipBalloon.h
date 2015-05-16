//////////////////////////////////////////////////////////////////////////
//
//	������	������ʾ�ؼ�
//	
//	���ڣ�	06/10/2009
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPBALLOON_H__9E1D0B54_B238_4DA3_8402_D029CEF7244A__INCLUDED_)
#define AFX_TOOLTIPBALLOON_H__9E1D0B54_B238_4DA3_8402_D029CEF7244A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipBallon.h : header file
//

#include "ToolTipBaseST.h"

/////////////////////////////////////////////////////////////////////////////
// CToolTipBalloon window

class CToolTipBalloon : public CToolTipBaseST
{
// Construction
public:
	CToolTipBalloon();
	virtual ~CToolTipBalloon();

// Attributes
public:
	// ���á���ȡ���
	void SetWidth(int nWidth) ;
	int GetWidth() const ;

	// ���á���ȡ��ͳ��ȡ���ȡ�λ��
	void SetMouthLength(int nLength) ;
	int GetMouthLength() const ;
	void SetMouthWidth(int nWidth) ;
	int GetMouthWidth() const ;
	void SetMouthBeginOffset(int nOffset) ;
	int GetMouthBeginOffset() const ;
	void SetMouthEndOffset(int nOffset) ;
	int GetMouthEndOffset() const ;
	
	// ���á���ȡ�رհ�ť��С
	void SetCloseBtnSize(int cx, int cy) ;
	CSize GetCloseBtnSize() const ;

	// ���á���ȡԲ��ֱ��
	void SetRoundDia(int nRoundDia) ;
	int GetRoundDia() const ;

	// ���ñ�Ե�հ׿��
	void SetMargin(int nMargin) ;
	int GetMargin() const ;
	
	// ���á���ȡ�߿���ɫ
	void SetFrameColor(COLORREF crFrame) ;
	COLORREF GetFrameColor() const ;
	
	// ���á���ȡ�����Ƿ��ö�
	void SetTopMost(BOOL bTopMost = TRUE) ;
	BOOL IsTopMost() const ;
	
	// ���á���ȡ�Ƿ�����
	void SetFriable(BOOL bFriable = TRUE) ;
	BOOL GetFriable() const ;
	
	// ����ð�ݷ������ȼ�
	void SetDirPRI(UINT n0, UINT n1 = 0, UINT n2 = 0, UINT n3 = 0) ;

protected:
	CPoint	m_ptBalloon ;		// ����λ��
	CRgn	m_rgnBolloon ;		// ��������

	CPoint	m_ptMouthPos ;		// �������λ��

	int		m_nMouthBeginOffset ;	// ������λ��
	int		m_nMouthEndOffset ;		// ����յ�λ��
	int		m_nMouthLength ;		// ��ͳ���
	int		m_nMouthWidth ;			// ��Ϳ��

	int		m_nRoundDia ;		// Բ��ֱ��

	int		m_nMargin ;			// ��Ե���

	CPoint	m_ptText ;			// ����λ��
	CSize	m_szText ;			// ���ִ�С

	int		m_nBalloonHeight ;	// ���ݸ߶�(���ı��仯)
	int		m_nBalloonWidth ;	// ���ݿ��(�̶�)

	int		m_nTotalHeight ;	// �ܸ߶�
	int		m_nTotalWidth ;		// �ܿ��

	int		m_nBtnMargin ;		// �رհ�ť�ͱ�Ե֮��Ŀ�϶
	CPoint	m_ptCloseBtn ;		// �رհ�ť��λ��
	CSize	m_szCloseBtn ;		// �رհ�ť�Ĵ�С(�̶�)
	
	COLORREF m_crFrame ;		// �߿���ɫ

	BOOL	m_bTopMost ;		// �Ƿ��ö�
	BOOL	m_bFriable ;		// �Ƿ�����

	CString m_strText ;			// ��ʾ���ı�
	
	enum CLOSEBTN_MODE
	{
		CBM_NORMAL = 0,
		CBM_HOVER,
		CBM_PUSH
	} m_nCloseBtnMode ;			// �رհ�ť��ǰģʽ
	BOOL	m_bCloseBtnDown	;	// �رհ�ť�Ƿ���

	UINT	m_uDuration ;		// ����ʱ��
	UINT	m_uLastTime ;		// ʣ��ʱ��
	UINT	m_nTimePiece ;		// ʱ��Ƭ
	
	UINT	m_arnDirPRI[4] ;	// �������ȼ�����

// Operations
public:
	// ����������ʾ
	BOOL Pop(CPoint ptPos, CString const &strText, CWnd * pParent, UINT nDuration = ~0) ;
	BOOL Pop(CPoint ptPos, CString const &strText, HWND hParent, UINT nDuration = ~0) ;
	// ѹ��������ʾ
	void Push() ;

protected:
	// ������������
	void CalcPostion() ;

	// ���ƹرհ�ť
	void DrawCloseBtn(CDC * pDC, CRect rect, CLOSEBTN_MODE nMode) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipBalloon)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipBalloon)
	afx_msg HFONT OnGetFont(WPARAM, LPARAM) ;
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPBALLON_H__9E1D0B54_B238_4DA3_8402_D029CEF7244A__INCLUDED_)
