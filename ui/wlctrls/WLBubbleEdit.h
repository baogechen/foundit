//////////////////////////////////////////////////////////////////////////
//
//	����:   ��ʵ����һ����Ͽؼ������Ǽ̳��Ա༭��ؼ�
//
//	
//	����:   2011/02/21
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "uilib/wndless/IWLEdit.h"
#include "uilib/common/utils/ImageEx/Shared_ImageEx.h"

// message
#define WM_BEGINEDIT	(WM_USER + 1986)

// command
#define BEN_BEGINEDIT		0x0100
#define BEN_ENDEDIT			0x0101

class WLBubbleEdit :
	public CWndLess
{
public:
	WLBubbleEdit(void);
	virtual ~WLBubbleEdit(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// Operation
	void SetBubbleImage(UINT nID) ;
	virtual void BeginEdit() ;
	virtual void EndEdit() ;
	void LimitText(int nChars) ;
	void SetMargin(int left, int top, int right, int bottom) ;
	void SetTextColor(COLORREF crText) { m_crText = crText ; }
//	void SetTextHaloColor(COLORREF crHalo) { m_crTextHalo = crHalo ; }

	// ������ʾ�ı�
	void SetCueBanner(LPCTSTR lpszCueBanner) ;
	void SetCueBannerColor(COLORREF crCueBanner) ;

	// ��øպô������ʱ�Ĵ�С
	void CalcWrapContentSize(LPSIZE lpSize) ;

protected:
	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;
	virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;

	//////////////////////////////////////////////////////////////////////////
	// Message Handler
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;

public:
	virtual BOOL SetWindowText(LPCTSTR lpszText) ;
	virtual int GetWindowText(CString &sText) ;

protected:
	Shared_ImageEx m_imgBubble ;
	WLEdit m_wlEdit ;
	RECT m_rcMargin ;

	CString m_sTextBeforeEdit ;

	enum
	{
		BES_NORMAL		= 0x00,
		BES_HOT			= 0x01,
		BES_PUSHED		= 0x02,
		BES_EDIT		= 0x04,
	};
	DWORD m_dwStatus ;
	enum 
	{
		CTRLID_EDIT		= 47,
	};

	COLORREF m_crText ;
	COLORREF m_crGlowTextHalo ;
	COLORREF m_crGlowTextBkgnd ;

	CString m_sCueBanner ;
	COLORREF m_crCueBanner ;

	CString m_sTooltipText ;

};
