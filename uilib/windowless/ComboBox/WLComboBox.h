//////////////////////////////////////////////////////////////////////////
//
//	����:   �޾����Ͽ�
//
//	
//	����:   2011/02/17
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "IWLComboBox.h"
#include "WLCmbEdit.h"
#include <vector>



class WLComboBoxImp :
	public WLComboBoxImpBase
{
public:
	WLComboBoxImp(void);
	virtual ~WLComboBoxImp(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// Operation

	void LoadImage(
		Image * pImgFrame,
		Image * pImgDropBtn,
		Image * pImgListDelBtn
		) ;

	COLORREF SetFrameColor(COLORREF crFrame) 
	{
		COLORREF crOld = m_crFrame ;
		if (m_crFrame != crFrame)
		{
			m_crFrame = crFrame ;
		}
		return crOld ;
	}

	// ���á���ȡ��ʾ����
	void SetCueBanner(LPCTSTR lpszText) ;
	CString GetCurBanner() const ;

	// ����ַ���
	int AddString(LPCTSTR lpszString, DWORD dwData = 0) ;

	// �����ַ���
	int InsertString(UINT nIndex, LPCTSTR lpszString, DWORD dwData = 0) ;

	// ɾ���ڵ�
	int	DeleteString(UINT nIndex) ;
	void ResetContent();
	int GetCount()			{ return m_vecItems.size() ; }
	HWL SetFocus()			{ return m_wlEdit.SetFocus() ; }
	// ��ȡ�����ı�
	void GetText(int nIndex, CString& rString) const ;
	// ��õ�ǰѡ��
	int GetCurSel() ;

	// ���õ�ǰѡ��
	int SetCurSel(int nSelect) ;

	// ���á���ȡ��������
	int SetItemData(int nIndex, DWORD dwItemData) ;
	DWORD GetItemData(int nIndex) const ;

	// Ѱ���ַ���
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const ;

	// ���Ʊ༭����������
	BOOL LimitText(int nMaxChars) ;

	// ���ñ༭��ѡ���ı�
	void SetEditSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE) ;

	BOOL CanAutoCompleteWord() const ;

	void SetEditMargin(
		int nLeftMargin,
		int nTopMargin,
		int nRightMargin,
		int nBottomMargin
		) 
	{
		(nLeftMargin != -1) ? (m_nEditLeftMargin = nLeftMargin) : 0 ;
		(nTopMargin != -1) ? (m_nEditTopMargin = nTopMargin) : 0 ;
		(nRightMargin != -1) ? (m_nEditBtnSpace = nRightMargin) : 0 ;
		(nBottomMargin != -1) ? (m_nEditBottomMargin = nBottomMargin) : 0 ;
	}


	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;
	virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;


	virtual BOOL SetWindowText(LPCTSTR lpszText) ;
	virtual void GetWindowText(CString &sText) ;

protected:
	//////////////////////////////////////////////////////////////////////////
	// Message Handler
	BOOL OnCommand(WPARAM wParam, LPARAM lParam) ;
	void OnLButtonDown(UINT nFlags, CPoint point) ;
	void OnMouseMove(UINT nFlags, CPoint point) ;
	void OnMouseLeave() ;
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point) ;
	void OnEnChanged() ;

	//////////////////////////////////////////////////////////////////////////
	// Inner Function
	void DrawDropBtn(HDC hDC) ;
	void UpdateDropBtn() ;
	void SelPrev() ;
	void SelNext() ;
	
protected:
	enum
	{
		IDC_EDIT	= 47,
//		IDC_DROPBTN	= 48,
	};

	typedef struct _tagCBITEM
	{
		CString m_strText ;
		DWORD	m_dwData ;

		BOOL operator==(CString const & rhs)
		{
			return (rhs.Compare(m_strText) == 0) ;
		};
	}CBITEM ;
	std::vector<CBITEM> m_vecItems ;

	BOOL m_bMouseInner ;	// ����ڴ��ڡ�Edit��
	BOOL m_bListVisible ;	// DropList����ʾ��
	enum BTN_STATE {
		BS_NORMAL		= 0x00,			// ����
		BS_HOT			= 0x01,			// ��꾭��
		BS_SELECTED		= 0x02,			// ��갴��
	} ;
	DWORD m_dwBtnState ;

	WLCmbEdit m_wlEdit ;
	CRect m_rcDropBtn ;

	Image * m_pImgFrame ;
	Image * m_pImgDropBtn ;
	Image * m_pImgListDelBtn ;

	int m_nEditLeftMargin ;
	int m_nEditTopMargin ;
	int m_nEditBottomMargin ;

	int m_nEditBtnSpace ;

	int m_nBtnWidth ;
	int m_nBtnTopMargin ;
	int m_nBtnBottomMargin ;
	int m_nBtnRightMargin ;

	// ��ǰѡ��������
	int m_nCurSel ;

	// ��ɫ
	COLORREF m_crFrame ;

};
