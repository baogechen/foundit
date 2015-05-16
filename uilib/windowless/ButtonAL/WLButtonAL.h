//////////////////////////////////////////////////////////////////////////
//
//	����:   ��ť
//			֧��Push, Check, Radio, Link���ַ��
//			ǰ��ͼƬ�ɸ���״̬ͼƬˮƽƴװ���ɣ�����״̬���Զ�ƥ��
//			Push���Ͱ�ť��״̬ƴװ��ʽһ��������3�����(���s_arPushStateMap)��
//			a. 5 states - normal, hot, pushed, focus/default, disable
//			b. 4 states - normal, hot, pushed, disable
//			c. 3 states - normal, hot, pushed
//			Check��Radio��״̬ƴװ��ʽһ��������2�����(���s_arCheckStateMap)��
//			a. 8 states - normal, normal_hot, normal_pushed, normal_disable,
//						  selected, selected_hot, selected_pushed, selected_disable
//			b. 3 states - normal, hot, selected
//
//	ȱ��:	��ťλ�øı�ʱδˢ��
//	
//	����:   2011/02/16
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "IWLButtonAL.h"
#include "common/utils/CanvasDC.h"

class WLButtonALImp :
	public WLButtonALImpBase
{
public:
	WLButtonALImp(void);
	virtual ~WLButtonALImp(void);

public:
	// ��ť����
	enum BTN_TYPE
	{
		PUSH		= 0x0000,
		CHECK		= 0x0001,
		RADIO		= 0x0002,
		LINK		= 0x0003,		// ��ť����ʱ���ƶ��ı����������ȥʱ��Ϊ��״
	} ;

	// ��ť���
	enum BTN_STYLE
	{
		NORMAL			= 0x0000,
		PUSHLIKE		= 0x0001,		// Check��Radio���ʱ��Ч
		FITFRGNDSIZE	= 0x0002,		// ����ǰ���Զ�������С
		FITTEXTSIZE		= 0x0004,		// ���������Զ�������С
	} ;

	// �������
	enum BKGND_STYLE
	{
		NOBKGND		= 0x0000,
		HANDPAINT	= 0x0001,		// �ֻ汳������ָ��������ɫ�������
		TRANSLUCENT	= 0x0002,		// ����͸����ʹ�ø����ڱ�����Ϊ������
	} ;

	// ǰ�����
	enum FRGND_STYLE
	{
		NOFRGND		= 0x0000,		// ��ǰ��
		FIXED		= 0x0001,		// �̶�����ǰ��
		FITX		= 0x0002,		// �̶�ǰ���߶ȣ�x�������01...12��ֻ���
		FITY		= 0x0004,		// �̶�ǰ����ȣ�y�������01...12��ֻ���
		FITXY		= 0x0006,		// x��y���������01...12����(δʵ��)
	} ;

	// �ı�����ı�����icon��text
	// BTN_STYLE���Զ�������Сʱ��CENTER��LEFT��RIGHT��TOP��BOTTOM��Ч
	// FITTEXTSIZE
	//	FITFRGNDSIZE + FITX: TOP, BOTTOM ��Ч
	//	FITFRGNDSIZE + FITY: LEFT, RIGHT ��Ч
	enum TEXT_STYLE
	{
// 		TS_NEAR		= 0x0001,
// 		TS_CENTER	= 0x0002,
// 		TS_FAR		= 0x0004,
// 		TS_VERTICAL	= 0x0008,		// ��ֱ����(δʵ��)

		TS_LEFT		= 0x0001,
		TS_HCENTER	= 0x0000,
		TS_RIGHT	= 0x0002,
		TS_TOP		= 0x0010,
		TS_VCENTER	= 0x0000,
		TS_BOTTOM	= 0x0020,

		TS_CENTER	= 0x0000,
	} ;

	enum BKGND_COLOR_INDEX
	{
		BCI_NORMAL				= 0x0001,
		BCI_HOT					= 0x0002,
		BCI_PUSHED				= 0x0004,
		BCI_DEFAULT				= 0x0008,
		BCI_DISABLE				= 0x0010,

		BCI_UNCHECKED			= 0x0001,
		BCI_UNCHECKED_HOT		= 0x0002,
		BCI_UNCHECKED_PUSHED	= 0x0004,
		BCI_UNCHECKED_DISABLE	= 0x0008,
		BCI_CHECKED				= 0x0010,
		BCI_CHECKED_HOT			= 0x0020,
		BCI_CHECKED_PUSHED		= 0x0040,
		BCI_CHECKED_DISABLE		= 0x0080,

		BCI_ALL					= 0xFFFF,
	};

protected:
	DWORD m_dwBtnType ;
	DWORD m_dwBtnStyle ;
	DWORD m_dwBkgndStyle ;
	DWORD m_dwFrgndStyle ;
	DWORD m_dwTextStyle ;

	COLORREF m_crBkgnd[8] ;
	COLORREF m_crText[8] ;

	Image * m_pImgFrgnd ;
	//CCanvasDC m_cvFrgnd ;

	BYTE m_arStateMap[8] ;
	int m_nFrgndStateCount ;
	Image * m_pImgCheckSign ;		// checkbox��radiobox��ť��ʹ�ø�ͼΪ����ͼ

	static BYTE s_arPushStateMap[5][5] ;
	static BYTE s_arCheckStateMap[8][8] ;

	Image * m_pImgIcon ;
	//CCanvasDC m_cvIcon ;

	// ������FITTEXTSIZE���ʱ����
	int m_nLeftMargin ;		// ��Ե
	int m_nRightMargin ;
	int m_nTopMargin ;
	int m_nBottomMargin ;

	int m_nSignTextSpace ;	// ���ź��ı�(�ı�����ͼ�������)֮��ļ�϶
	int m_nIconTextSpace ;	// ͼ�������֮��ļ�϶

	CPoint m_ptSign ;
	CPoint m_ptIcon ;
	CPoint m_ptText ;

	// CHECK/RADIO���Ͱ�ť�ڲ�����
	int m_nCheck ;

	// LINK���Ͱ�ť�ڲ�����
	CString m_sUrl ;

	//CDC * m_pDCParentBkgnd ;

	// ������뱳����ɫ��������ɫ
	COLORREF m_crGlowText ;
	COLORREF m_crGlowTextHalo ;
	COLORREF m_crGlowTextBkgnd ;

	BOOL m_bIconAciton;

public:
	// ����ǰ��
	void SetFrgnd(
		Image * pImgFrgnd,
		int nStateCount,
		DWORD dwFrgndStyle,
		BOOL bDefer = FALSE
		) ;

	// ���÷���
	void SetSign(
		Image * pImgCheckSign,
		BOOL bDefer = FALSE
		) ;

	// ����ͼ��
	void SetIcon(
		Image * pImgIcon,
		BOOL bDefer = FALSE,
		BOOL bAction = TRUE
		) ;

	// ��������
	void SetType(
		DWORD dwBtnType,
		BOOL bDefer = FALSE
		) ;

	// ���÷��
	void SetStyle(
		DWORD dwBtnStyle,
		BOOL bDefer = FALSE
		) ;

	// ���ñ������
	void SetBkgndStyle(
		DWORD dwBkgndStyle,
		BOOL bUpdate = FALSE
		) ;

	// �����ı����
	void SetTextStyle(
		DWORD dwTextStyle,
		BOOL bDefer = FALSE
		) ;

	void SetMargin(int nLeft, int nTop, int nRight, int nBottom, BOOL bDefer = FALSE) ;
	void SetSignTextSpace(int nSignTextSpace, BOOL bDefer = FALSE) ;
	void SetIconTextSpace(int nIconTextSpace, BOOL bDefer = FALSE) ;

	void SetBkgndColor(DWORD dwMask, ...) ;
	void SetTextColor(DWORD dwMask, ...) ;

	void EndDeferRecalcLayout() ;

	void SetUrl(CString const & sUrl) 
	{
		m_sUrl = sUrl ;
	}

protected:
	void RecalcLayout() ;
	void CalcSignIconText(CSize &szSignIconText, CPoint &ptSign, CPoint &ptIcon, CPoint &ptText) ;

	void Draw(HDC hDC, CRect const &rc, DWORD dwState) ;
	void DrawPush(HDC hDC, CRect const &rc, DWORD dwState, BOOL bNormalOrCheck) ;
	void DrawCheckRadio(HDC hDC, CRect const &rc, DWORD dwState) ;
	void DrawLink(HDC hDC, CRect const &rc, DWORD dwState) ;

	void GuessStateMap(int nStateCount) ;

protected:
	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;

public:
	virtual BOOL SetWindowText(LPCTSTR lpszText) ;

	//////////////////////////////////////////////////////////////////////////
	// WLButton Implement

protected:
	virtual BOOL DrawBySelf(HDC hDC, RECT const &rcUpdate, DWORD dwStatus) ;

	//////////////////////////////////////////////////////////////////////////
	// Message Handler
	BOOL OnSetCursor() ;

protected:
	afx_msg BOOL OnBnClicked();
};
