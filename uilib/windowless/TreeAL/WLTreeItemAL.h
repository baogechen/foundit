//////////////////////////////////////////////////////////////////////////
//
//	����:   ��ӦWLTreeAL�����ڵ�
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
#include "..\tree\wltreebaseitem.h"

#define TIN_HEIGHT_CHANGED		(NM_WLFIRST - 100U)
typedef struct _tagNMTIHEIGHTINFO
{
	NMWLHDR hdr ;
	HWLTREEITEM hItem ;
	long lOldHeight ;
	long lNewHeight ;
	BOOL bNoScroll ;
}NMTIHEIGHTINFO, *LPNMTIHEIGHTINFO ;

#define TIN_SELECTED			(NM_WLFIRST - 101U)
typedef struct _tagNMTREEITEM
{
	NMWLHDR hdr ;
	HWLTREEITEM hItem ;
}NMTREEITEM, *LPNMTREEITEM ;

#define TIN_BEGIN_DRAG			(NM_WLFIRST - 102U)
typedef struct _tagNMTIDRAGINFO
{
	NMWLHDR hdr ;
	POINT ptInItem ;
}NMTIDRAGINFO, *LPNMTIDRAGINFO ;

#define WM_CAN_YOU_DRAG		WM_USER + 1986		// ѯ�ʽڵ��ܷ���ק
#define	WM_KILLSELECTED		WM_USER + 1989		// ���ѡ��״̬

// tree item type
#define TIT_AIRLAM			_T("TreeAirlamItem")

class WLTreeItemAL :
	public WLTreeBaseItem
{
public:
	WLTreeItemAL(void);
	virtual ~WLTreeItemAL(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// TreeItem Implement

	// ���뺢��
	virtual BOOL InsertChild(HWLTREEITEM hItem, HWLTREEITEM hInsertAfter) ;
	// ɾ������
	virtual BOOL DeleteChild(HWLTREEITEM hChild) ;
	virtual void DeleteAllChild() ;
	virtual void Type(CString &sType) const { sType = TIT_AIRLAM ; }

	virtual BOOL Expand(UINT uCode) ;
	virtual BOOL Show(BOOL bShow) ;
	virtual BOOL Filter(BOOL bShow) ;
	virtual BOOL Select(BOOL bSelect) ;
	virtual BOOL DragOver(BOOL bEnter) ;

	virtual int CalcBranchHeight() ;					// �����֧�߶�(�����ڵ�߶�)

	WLTreeItemAL* GetFirstVisibleChild(int nTreeTopInThis) ;

	// �Զ���Ļ��ƺ���
	// �������Ϻܶ�ڵ��ǲ���ʾ�ģ����Ҷ����й��ɵĶ������У�
	// ����ʵ�ʻ����п��Բ�������Щ����ʾ�Ľڵ㣬����߻���Ч��
	virtual void _CustomInternalDraw(HDC hDC, RECT const &rcUpdate, RECT const &rcViewInThis) ;
	virtual void CustomOnDrawChild(HDC hDC, RECT const &rcUpdate, RECT const &rcViewInThis) ;

	// �Զ���Ĳ��Ժ���
	// ����ͬ��
	virtual HWL _CustomInternalHitTest(POINT const &pt, RECT const &rcViewInThis) ;
	virtual HWL CustomHitTestChild(POINT const &pt, RECT const &rcViewInThis) ;

	virtual WLTreeItemAL * HitTestTreeItem(POINT const &pt, RECT const &rcViewInThis) ;

	void GetItemRectInHost(CRect &rcItem) ;
	void UpdateItemView() ;

	// λ�ơ�ָ���������������еĺ���
	static void OffsetChildItemsY(
		HWLTREEITEM hFirstBackChild,
		HWLTREEITEM hLastBackChild,
		long lOffsetY
		) ;

	int GetItemHeight() const 
	{
		return m_nItemHeight ;
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement

	//virtual void GetRectInHost(LPRECT lpRect) ;

protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;

	//////////////////////////////////////////////////////////////////////////
	// Message Handler

	void OnNotifyChildItemHeightChanged(LPNMWLHDR pNMWLHDR, LRESULT * pResult) ;
	void OnLButtonDown(UINT nFlags, CPoint point) ;
	void OnLButtonUp(UINT nFlags, CPoint point) ;

	void OnRButtonDown(UINT nFlags, CPoint point) ;
	void OnRButtonUp(UINT nFlags, CPoint point) ;

	void OnMouseHover() ;
	void OnMouseMove(UINT nFlags, CPoint point) ;
	void OnMouseLeave() ;

	void OnCaptureChanged(HWL hWndLess) ;

protected:
	//////////////////////////////////////////////////////////////////////////
	// �ڲ�����

	// ����
	BOOL DoCollapse() ;

	// չ��
	BOOL DoExpand() ;

public:
	// ֪ͨ�����Լ��ĸ߶ȸı�
	void NotifyParentHeightChanged(long lOldHeight, long lNewHeight, BOOL bNoScroll) ;

protected:
	int m_nItemHeight ;				// �ڵ�߶ȣ�Ҳ���ӽڵ��Y������ʼֵ
	//BOOL m_bLButtonDown ;			// �������Ƿ���
	//BOOL m_bRButtonDown ;			// ����Ҽ��Ƿ���

	// ��קʹ�õ�������
	BOOL m_bPrepareDrag ;			// �Ƿ��Ѿ�׼����ק
	DWORD m_dwPrepareDragTime ;		// ׼����ʼ��ק��ʱ��
	CPoint m_ptPrepareDragPoint ;	// ׼����ʼ��קʱ����ڽڵ��е�λ��
	//BOOL m_bEnterDragging ;			// �Ƿ�������ק

};
