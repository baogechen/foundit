//////////////////////////////////////////////////////////////////////////
//
//	������	�Զ���CTabCtrl
//			Ŀǰ��֧�ֶ���
//	
//	���ڣ�	06/23/2009
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WL_TAB_CTRL__INCLUDED_)
#define AFX_WL_TAB_CTRL__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCtrlST.h : header file
//

#include <vector>
#include "common/utils/ImageEx/Shared_ImageEx.h"
#include "common/utils/CanvasDC.h"
#include "IWLTabCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// WLTabCtrl window
class WLTabItem;

template <class IT = IWLTabCtrl>
class WLTabCtrlImp : public CWndLessImpBase< CWndLessShell<IT> > 
{

// Construction
public:
	WLTabCtrlImp( BOOL bCanDrag );
	virtual ~WLTabCtrlImp();
// Attributes
public:
	// ���ñ���ͼƬ
	virtual void SetImage(Image * pImgBkgnd, BOOL bRedraw) ;

	// ����������
	virtual void SetSpace(int nNearSpace, int nItemSpace, int nFarSpace) ;

	// ����������ɫ
	virtual void SetTextColor(COLORREF crText, BOOL bRedraw) ;

	// ����ѡ��������ɫ
	virtual void SetSelectTextColor( COLORREF crText, BOOL bRedraw  );

	// ���ñ�����ɫ
	virtual void SetBkgndColor(COLORREF crBkgnd, BOOL bRedraw) ;

	// �����Ƿ���Ի�ȡ����
	virtual void SetFocusable(BOOL bFocusable) ;

	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnDraw( HDC hDC, RECT const &rcUpdate );
	virtual void OnSized(SIZE const &szOld, SIZE const &szNew);
	virtual void OnDestroy();

protected:

	// �Ƿ��ý���
	BOOL m_bFocusable ;

	// ��¼��ǰѡ�к���꾭������������
	//int m_nHotItem ;
	//int m_nSelectedItem ;
	//int m_nLBDownItem ;
	WLTabItem* m_pSelItem;

	// ���
	int m_nNearSpace ;	// ��һ�����������ļ��
	int	m_nItemSpace ;	// ����֮��ļ��
	int m_nFarSpace ;	// ���һ���������յ�ļ��

	// ��¼����ͼƬ�򱳾�dc
	Image * m_pImgBkgnd ;

	// ��¼��������
	std::vector<WLTabItem*> m_vecItems;
	
	// ������ɫ
	COLORREF	m_crText ;
	COLORREF m_crTextSelected;

	// ������ɫ
	COLORREF	m_crBkgnd ;

	// ��ʾ�ؼ�
	//CToolTipCtrl	m_tooltip ;
	//UINT			m_uLastTipID ;

	//BOOL m_bIsTrackingMouse;

	BOOL m_bCanDrag;

	int m_nMaxItemWidth; // item�����

	BOOL m_bDelayCalItemSize;

// Operations
protected:
	// ���¼��㲼��
	virtual void RecalcLayout() ;

	// ��������
	//BOOL Track(int nIndex) ;

	// ��������
	// BOOL DrawItem( HDC hDC, int nIndex) ;

	virtual BOOL GetDrawItemRect( int nIndex, CRect& rcItem );

	virtual WLTabItem* CreateTabItem( LPCTSTR lpszText, 
		UINT uTextStyle,
		CPoint	ptText,
		LPCTSTR sTipText,
		Shared_ImageEx pImgIcon,
		Shared_ImageEx pImgBkgnd,
		BOOL bDrag,
		int nTabStateCount );

	virtual void RepositionAfterDeleteItem( BOOL bResize, int nIndex, CSize &delItemSize );

public:
	// ��������
	virtual int InsertItem(
		int nIndex,
		Shared_ImageEx pImgIcon,
		Shared_ImageEx pImgBkgnd,
		int nTabStateCount,
		LPCTSTR lpszText,
		UINT uTextStyle,
		int nTextX,
		int nTextY,
		LPCTSTR lpszTip,
		int maxWidth
		) ;

	virtual void ResetIndexAndZorder();

	// ���������ı�
	virtual BOOL SetItemText(int nIndex, LPCTSTR lpszText) ;

	// ������������
	virtual BOOL SetItemData(int nIndex, DWORD_PTR dwData) ;

	// ��ȡ��������
	virtual DWORD_PTR GetItemData(int nIndex) const ;

	// ɾ������
	virtual BOOL DeleteItem(int nIndex, BOOL bResize) ;

	// ѡ������
	virtual BOOL SelectItem(int nIndex) ;

	virtual BOOL HightLightItem( int nIndex );

	// �����������
	virtual BOOL GetItemRect(int nIndex, CRect &rcItem) ;

	virtual BOOL SetItemIconSize( int nIndex, CSize& size ); 

	virtual BOOL SetItemIcon( int nIndex, Shared_ImageEx icon );

	// �������λ��
	// int HitTest(CPoint point) ;
	
	// ��õ�ǰѡ�����������
	virtual int GetCurSel() const ;
	
	// �����������
	virtual int GetItemCount() const ;

	virtual void OnItemRBtnClicked( LPNMWLHDR lpNMWLHDR );

protected:

	//virtual HWL HitTestChild(POINT const &pt) ;
	//virtual void OnDrawChild(HDC hDC, RECT const &rcUpdate) ;

	virtual void OnLDownItem( LPNMWLHDR lpNMWLHDR );

	virtual void OnDragItemMove( LPNMWLHDR lpNMWLHDR  );

	
public:
	void OnRButtonUp(UINT nFlags, CPoint point);
};

#include "WLTabCtrl.hpp"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRLST_H__504C243B_F516_40F2_89EB_B1E0EC8C7C23__INCLUDED_)
