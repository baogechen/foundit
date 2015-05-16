//////////////////////////////////////////////////////////////////////////
//
//	����:   �����ؼ������һ�μ̳У�ʹ֮�߱����ϵ����������нڵ�
//			������
//
//	
//	����:   2011/01/27
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "..\Tree\WLTree.h"
#include "WLTreeItemAL.h"
#include "..\ScrollBar\WLScrollBar.h"
#include "common\wnds\DragImage2.h"
#include "CanvasDC.h"

typedef struct _tagNMTREEDRAGINFO
{
	NMWLHDR hdr ;
	WLTreeItemAL * pItemBeDragging ;	// ����ק�Ľڵ�
	WLTreeItemAL * pItemBeDragOver ;	// ��ק�����Ľڵ�
}NMTREEDRAGINFO, * PNMTREEDRAGINFO ;

#define TN_CAN_I_DRAG	(NM_WLFIRST - 500U)
#define TN_BEGINDRAG	(NM_WLFIRST - 501U)
// #define TN_DRAGENTER	(NM_WLFIRST - 502U)
// #define TN_DRAGLEAVE	(NM_WLFIRST - 503U)
#define TN_DRAGOVER		(NM_WLFIRST - 502U)
#define TN_DRAGDROP		(NM_WLFIRST - 503U)
#define TN_ENDDRAG		(NM_WLFIRST - 504U)
#define TN_RCLICK		(NM_WLFIRST - 505U)

class WLTreeAL :
	public WLTree
{
public:
	WLTreeAL(void);
	virtual ~WLTreeAL(void);

public:
	enum DropEffect
	{
		NONE, MOVE, COPY
	};

	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement

	virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
	virtual void OnDestroy() ;

	virtual void _InternalOnDraw(HDC hDC, RECT const &rcUpdate) ;
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;
	virtual void OnDrawChild(HDC hDC, RECT const &rcUpdate) ;

	virtual HWL HitTestChild(POINT const &pt) ;
	WLTreeItemAL * HitTestTreeItem(POINT const &pt) ;

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;

	//////////////////////////////////////////////////////////////////////////
	// Message Handler
	void OnVScroll(LPSCROLLMSGINFO pSmi, WLScrollBar* pScrollBar) ;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point) ;
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;

	void OnMouseMove(UINT nFlags, CPoint point) ;
	void OnLButtonUp(UINT nFlags, CPoint point) ;
	void OnCaptureChanged(HWL hWL) ;

	void OnNotifyChildItemHeightChanged(LPNMWLHDR pNMWLHDR, LRESULT * pResult) ;
	void OnNotifyChildItemSelected(LPNMWLHDR pNMWLHDR, LRESULT * pResult) ;
	void OnNotifyTreeItemBeginDrag(LPNMWLHDR pNMWLHDR, LRESULT * pResult) ;

	void OnRButtonClick() ;

	//////////////////////////////////////////////////////////////////////////
	// WLTree Implement

// 	HWLTREEITEM InsertItem(
// 		LPCTSTR lpszText,
// 		HWLTREEITEM hParent = TI_ROOT,
// 		HWLTREEITEM hInsertAfter = TI_LAST,
// 		BOOL bDeferInsert = FALSE
// 		) ;

	virtual BOOL DeleteItem(
		HWLTREEITEM hItem
		) ;

	virtual void DeleteAllItem() ;

	//////////////////////////////////////////////////////////////////////////
	// 
	WLTreeItemAL * GetSelectedItem() const 
	{
		return m_pSelectedItem ;
	}
	void SelectItem(HWLTREEITEM hItem) ;
	BOOL EnsureVisible(HWLTREEITEM hItem) ;

	void GetItemRectInTree(HWLTREEITEM hItem, CRect &rcItem) ;
	void GetItemRectInRoot(HWLTREEITEM hItem, CRect &rcItem) ;

	// ��ȡǰ����ʾ�ڵ�
	HWLTREEITEM GetPrevVisibleItemInRoot(HWLTREEITEM hItem) const ;
	HWLTREEITEM GetNextVisibleItemInRoot(HWLTREEITEM hItem) const ;

protected:
	//////////////////////////////////////////////////////////////////////////
	// Inner Function

	void UpdateBkgndDeviceContent(int cx, int cy) ;
	void RedrawBkgnd(int cx, int cy) ;

	// ��ק
	void BeginDrag(HWLTREEITEM hItem, CPoint const &ptDragPointInItem) ;
	void DoDragging(CPoint const &ptDragging) ;
	void CancelDrag() ;		// ����ȡ����ק
	void EndDrag() ;

protected:
	CCanvasDC m_dcBkgnd ;
	//CBitmap m_bmpBkgnd ;
	//CBitmap * m_pBkgndOldBmp ;

	WLScrollBar m_sbVert ;
	int m_nVScrollWidth ;

	CDragImage2 m_DragImage ;

	WLTreeItemAL * m_pSelectedItem ;		// ��ѡ��Ľڵ�

	BOOL m_bDraggingItem ;					// �Ƿ�������ק���ڵ�
	WLTreeItemAL * m_pDraggingItem ;		// ������ק�����ڵ�
	WLTreeItemAL * m_pDragOverItem ;		// ��ק���������ڵ�
	DropEffect m_enDropEffect ;

	static HCURSOR s_curNone ;
	static HCURSOR s_curMove ;
	static HCURSOR s_curCopy ;

};
