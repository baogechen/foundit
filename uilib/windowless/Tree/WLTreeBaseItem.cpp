#include "StdAfx.h"
#include "WLTreeBaseItem.h"
#include "common/MemoryLeakChecker.h"

WLTreeBaseItem::WLTreeBaseItem(void)
:
m_hParentItem(NULL) 
, m_hPrevSibling(NULL) 
, m_hNextSibling(NULL) 
, m_hFirstChild(NULL) 
, m_dwStatus(TIS_NORMAL) 
{
}

WLTreeBaseItem::~WLTreeBaseItem(void)
{
	HWLTREEITEM hChild = m_hFirstChild ;
	while (hChild != NULL)
	{
		HWLTREEITEM hTemp = hChild ;
		hChild = hChild->m_hNextSibling ;
		delete hTemp ;
	}
}

HWLTREEITEM WLTreeBaseItem::GetParentItem() const 
{
	return m_hParentItem ;
}

HWLTREEITEM WLTreeBaseItem::GetPrevSibling() const 
{
	return m_hPrevSibling ;
}

HWLTREEITEM WLTreeBaseItem::GetNextSibling() const 
{
	return m_hNextSibling ;
}

HWLTREEITEM WLTreeBaseItem::GetFirstChild() const 
{
	return m_hFirstChild ;
}

void WLTreeBaseItem::SetParentItem(HWLTREEITEM hItem) 
{
	m_hParentItem = hItem ;
}

void WLTreeBaseItem::SetPrevSibling(HWLTREEITEM hItem) 
{
	m_hPrevSibling = hItem ;
}

void WLTreeBaseItem::SetNextSibling(HWLTREEITEM hItem) 
{
	m_hNextSibling = hItem ;
}

void WLTreeBaseItem::SetFirstChild(HWLTREEITEM hItem) 
{
	m_hFirstChild = hItem ;
}

// �жϵ�ǰ�ڵ��Ƿ���ָ���ڵ��������
BOOL WLTreeBaseItem::HasChild(
							  HWLTREEITEM hItem,
							  BOOL bRecursive// = TRUE
							  ) const 
{
	// �ж�hItem�Ƿ�Ϊ�գ����򷵻�FALSE
	// �����ƶ�ȷ�еľ��
	if (hItem == TI_FIRST || hItem == TI_LAST || hItem == TI_SORT)
		//return TRUE ;
		return FALSE ;

	if (hItem == NULL)
		return FALSE ;

	// ����hItem����Ԥ֪�����Բ��ܴ�hItemȥѰ�Ҹ��ڵ�����ж�
	HWLTREEITEM hNext = m_hFirstChild ;
	while (hNext != NULL)
	{
		if (hNext == hItem)
			return TRUE ;

		if (bRecursive && hNext->HasChild(hItem))
			return TRUE ;

		hNext = hNext->m_hNextSibling ;
	}

	return FALSE ;
}

// ������һ������
HWLTREEITEM WLTreeBaseItem::GetLastChild() 
{
	HWLTREEITEM hRes = m_hFirstChild ;
	if (hRes != NULL)
	{
		while (hRes->m_hNextSibling != NULL)
		{
			hRes = hRes->m_hNextSibling ;
		}
	}

	return hRes ;
}

// �Ƿ�չ��
BOOL WLTreeBaseItem::IsExpand() const 
{
	return m_dwStatus & TIS_EXPAND ;
}

// �Ƿ�ɼ�
BOOL WLTreeBaseItem::IsVisible() const 
{
	return (!(m_dwStatus & TIS_HIDE)) && (!(m_dwStatus & TIS_FILTERED)) ;
}

// �Ƿ���ʾ
BOOL WLTreeBaseItem::IsShow() const 
{
	return !(m_dwStatus & TIS_HIDE) ;
}

// ��������ڵ�
HWLTREEITEM WLTreeBaseItem::GetRoot() 
{
	HWLTREEITEM hRoot = this ;
	HWLTREEITEM hTemp = NULL ;

	while (hRoot != NULL)
	{
		hTemp = hRoot->GetParentItem() ;
		if (hTemp == NULL)
			return hRoot ;

		hRoot = hTemp ;
	}

	return hRoot ;
}

// �����
HWL WLTreeBaseItem::GetTree() 
{
	HWL hRoot = GetRoot() ;
	if (hRoot != NULL)
	{
		return hRoot->GetParent() ;
	}

	return NULL ;
}

//////////////////////////////////////////////////////////////////////////
// TreeItem Implement

// ���뺢��
BOOL WLTreeBaseItem::InsertChild(HWLTREEITEM hItem, HWLTREEITEM hInsertAfter) 
{
	// �����ڵ��Ƿ��Ѵ���
	if (hItem == NULL)
		return FALSE ;

	// ��������ṹ�ϵĹ�ϵ
	DWORD dwStyle = 0 ;
	if (IsExpand() && hItem->IsVisible())
		dwStyle = WS_VISIBLE ;

	if (!hItem->Create(dwStyle, 0, 0, 0, 0, this, (UINT)-1))
		return FALSE ;

	// �������ṹ�ϵĹ�ϵ
	if (hInsertAfter == TI_FIRST)
	{
		hInsertAfter = NULL ;
	}
	else if (hInsertAfter == TI_LAST)
	{
		hInsertAfter = GetLastChild() ;
	}

	hItem->m_hParentItem = this ;
	hItem->m_hPrevSibling = hInsertAfter ;
	if (hInsertAfter != NULL)
	{
		hItem->m_hNextSibling = hInsertAfter->m_hNextSibling ;
		hInsertAfter->m_hNextSibling = hItem ;
		if (hItem->m_hNextSibling != NULL)
		{
			hItem->m_hNextSibling->m_hPrevSibling = hItem ;
		}
	}
	else
	{
		hItem->m_hNextSibling = m_hFirstChild ;
		m_hFirstChild = hItem ;
		if (hItem->m_hNextSibling != NULL)
		{
			hItem->m_hNextSibling->m_hPrevSibling = hItem ;
		}
	}

	return TRUE ;
}

BOOL WLTreeBaseItem::DeleteChild(HWLTREEITEM hChild) 
{
	if (hChild == NULL)
		return FALSE ;

	// ������ϵĹ�ϵ
	HWLTREEITEM hPrevSibling = hChild->GetPrevSibling() ;
	HWLTREEITEM hNextSibling = hChild->GetNextSibling() ;

	if (hPrevSibling == NULL)
	{
		m_hFirstChild = hNextSibling ;
	}
	else
	{
		hPrevSibling->m_hNextSibling = hNextSibling ;
	}

	if (hNextSibling != NULL)
	{
		hNextSibling->m_hPrevSibling = hPrevSibling ;
	}

	// �������ṹ�ϵĹ�ϵ
	hChild->Destroy() ;
	delete hChild ;

	return TRUE ;
}

void WLTreeBaseItem::DeleteAllChild() 
{
	HWLTREEITEM hChild = m_hFirstChild ;
	while (hChild != NULL)
	{
		HWLTREEITEM hTemp = hChild ;
		hChild = hChild->GetNextSibling() ;

		hTemp->Destroy() ;
		delete hTemp ;
	}

	m_hFirstChild = NULL ;
}

BOOL WLTreeBaseItem::Expand(UINT /*uCode*/) 
{
	return FALSE ;
}

// ��ʾ/����
BOOL WLTreeBaseItem::Show(BOOL bShow) 
{
	if (bShow)
	{
		if (m_dwStatus & TIS_HIDE)
		{
			m_dwStatus &= ~TIS_HIDE ;
			return TRUE ;
		}
	}
	else
	{
		if (!(m_dwStatus & TIS_HIDE))
		{
			m_dwStatus |= TIS_HIDE ;
			return TRUE ;
		}
	}

	return FALSE ;
}

// ���ˣ���ʾ/����
BOOL WLTreeBaseItem::Filter(BOOL bShow) 
{
	return FALSE ;
}

//////////////////////////////////////////////////////////////////////////
// IWndLess Implement

void WLTreeBaseItem::OnDraw(HDC hDC, RECT const &/*rcUpdate*/) 
{
	CRect rc ;
	GetRectInHost(&rc) ;

	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 255)) ;
	HGDIOBJ hOldPen = ::SelectObject(hDC, hPen) ;

	::MoveToEx(hDC, rc.left, rc.top, NULL) ;
	::LineTo(hDC, rc.right - 1, rc.top) ;
	::LineTo(hDC, rc.right - 1, rc.bottom - 1) ;
	::LineTo(hDC, rc.left, rc.bottom - 1) ;
	::LineTo(hDC, rc.left, rc.top) ;

	::SelectObject(hDC, hOldPen) ;
	::DeleteObject(hPen) ;

	HFONT hFont = (HFONT)::SendMessage(GetCenter()->GetHost(), WM_GETFONT, 0, 0) ;
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont) ;
	int nOldBkMode = ::SetBkMode(hDC, TRANSPARENT) ;

	::SetBkMode(hDC, nOldBkMode) ;
	::SelectObject(hDC, hOldFont) ;
}
