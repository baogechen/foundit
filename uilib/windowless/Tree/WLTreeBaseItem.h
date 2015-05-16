//////////////////////////////////////////////////////////////////////////
//
//	����:   ���ؼ��ڵ������
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
#include "..\wndless.h"

class WLTreeBaseItem ;
typedef WLTreeBaseItem * HWLTREEITEM ;

// Special Item Sign
#define TI_ROOT                ((HWLTREEITEM)0xFFFF0000)
#define TI_FIRST               ((HWLTREEITEM)0xFFFF0001)
#define TI_LAST                ((HWLTREEITEM)0xFFFF0002)
#define TI_SORT                ((HWLTREEITEM)0xFFFF0003)

// Status
#define TIS_NORMAL				0x00		// ��ͨ
#define TIS_HOT					0x01		// ��꾭��
#define TIS_SELECTED			0x02		// ��ѡ��
#define TIS_EXPAND				0x04		// ��չ��
#define TIS_HIDE				0x08		// ������
#define TIS_DRAGOVER			0x10		// ��ק����
#define TIS_FILTERED			0x20		// �����˵�(����ʾ)

// Expand Code
#define TE_COLLAPSE				0x01
#define TE_EXPAND				0x02
#define TE_TOGGLE				0x03
#define TE_EXPAND_RECURSIVE		0x04
#define TE_COLLAPSE_RECURSIVE	0x05

// TreeItem Type
#define TIT_BASE				_T("TreeBaseItem")

class WLTreeBaseItem :
	public CWndLess
{
public:
	WLTreeBaseItem(void);
	virtual ~WLTreeBaseItem(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// Attribute
	HWLTREEITEM GetParentItem() const ;
	HWLTREEITEM GetPrevSibling() const ;
	HWLTREEITEM GetNextSibling() const ;
	HWLTREEITEM GetFirstChild() const ;

	void SetParentItem(HWLTREEITEM hItem) ;
	void SetPrevSibling(HWLTREEITEM hItem) ;
	void SetNextSibling(HWLTREEITEM hItem) ;
	void SetFirstChild(HWLTREEITEM hItem) ;

	// �жϵ�ǰ�ڵ��Ƿ���ָ���ڵ��������
	BOOL HasChild(HWLTREEITEM hItem, BOOL bRecursive = TRUE) const ;

	// ������һ������
	HWLTREEITEM GetLastChild() ;

	// �Ƿ�չ��
	BOOL IsExpand() const ;

	// �Ƿ�ɼ�
	BOOL IsVisible() const ;

	// �Ƿ���ʾ
	BOOL IsShow() const ;

	// ��������ڵ�
	HWLTREEITEM GetRoot() ;

	// �����
	HWL GetTree() ;

public:
	//////////////////////////////////////////////////////////////////////////
	// TreeItem Implement

	virtual BOOL InsertChild(HWLTREEITEM hItem, HWLTREEITEM hInsertAfter) ;		// ���뺢��
	virtual BOOL DeleteChild(HWLTREEITEM hChild) ;								// ɾ������
	virtual void DeleteAllChild() ;												// ɾ�����к���
	virtual BOOL Expand(UINT uCode) ;											// չ��/�պ�
	virtual BOOL Show(BOOL bShow) ;												// ��ʾ/����
	virtual BOOL Filter(BOOL bShow) ;											// ���ˣ���ʾ/����
	virtual void Type(CString &sType) const { sType = TIT_BASE ; }

protected:
	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;

protected:
	// Base Data
	//HWLTREEITEM	m_hRoot ;
	HWLTREEITEM	m_hParentItem ;
	HWLTREEITEM	m_hPrevSibling ;
	HWLTREEITEM	m_hNextSibling ;
	HWLTREEITEM	m_hFirstChild ;

	// Data
	DWORD m_dwStatus ;			// �ڵ�״̬

};
