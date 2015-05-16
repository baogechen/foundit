
#pragma once

#include "uilib/wndless/IWLScrollWnd.h"
#include "uilib/wndless/IWLListItem.h"

class IWLSimpleList :public IWndLess
{
public:
	virtual ~IWLSimpleList(){}

	class EventListener
	{
	public:
		virtual void OnListItemSelectChanged(IWLSimpleList *, IWLSimpleListItem * /*item*/, bool /*gain_or_lose*/) {}
		virtual void OnListLeftClick(IWLSimpleList *, IWLSimpleListItem *) {}
		virtual void OnListLeftDoubleClick(IWLSimpleList *, IWLSimpleListItem *) {}
		virtual void OnListRightClick(IWLSimpleList *, IWLSimpleListItem *) {}
		virtual void OnListRightDoubleClick(IWLSimpleList *, IWLSimpleListItem *) {}
	};

public:
	//////////////////////////////////////////////////////////////////////////
	// ����ӿ�

	// �����ܷ�ֱ����
	virtual void SetCanVertScroll(bool can)  = 0;

	// �����Ƿ��д�ֱ������
	virtual void SetHasVertScrollBar(bool has)  = 0;

	// �����ܷ�ˮƽ����
	virtual void SetCanHorzScroll(bool can) = 0;

	// �����Ƿ���ˮƽ������
	virtual void SetHasHorzScrollBar(bool has) = 0;

	// ���÷���
	virtual void SetVertical(bool vert) = 0;

	// �б���Ϊ��ֱʱ��widthΪ-1��ʾ�п�����̿�ȱ���һֱ
	// �б���Ϊˮƽʱ����ʾԪ��Ĭ���п�
	virtual void SetColumnWidth(int width) = 0;

	// �б���Ϊˮƽʱ��heightΪ-1��ʾ�п�����̸߶ȱ���һֱ
	// �б���Ϊ��ֱʱ����ʾԪ��Ĭ���и�
	virtual void SetRowHeight(int height) = 0;

	virtual void SetItemFixedSpan(bool fix) = 0;

	// ǰ�壬indexΪ-1��ʾ������󣬷���-1��ʾʧ�ܣ�
	// ����һ����ͨ�Ĳ���Ԫ�ؽӿڣ����ʺ���������
	virtual int InsertItem(int index, IWLSimpleListItem * item) = 0;
	virtual bool DeleteItem(int index) = 0;
	virtual bool DeleteAllItems() = 0;
	virtual IWLSimpleListItem * GetItem(int index) = 0;
	virtual int GetItemCount() const = 0;
	virtual void GetSize(LPSIZE lpSize) const = 0;

	// ʹĳԪ�ؿɼ�
	virtual void EnsureVisible(int index) = 0;

	virtual void SetEventListener(EventListener * listener) = 0;

	virtual void SetScrollListener(IWLScrollWnd::ScrollListener * listener) = 0;

	virtual ItemSelectManager* GetSelectManager() = 0;

};

typedef CWndLessImpBase< CWndLessShell<IWLSimpleList> >  WLSimpleListImpBase;

template <class IT = IWLSimpleList, class IMPT = WLSimpleListImpBase > 
class CWLSimpleListShell : public CWndLessShell<IT, IMPT> 
{
public:
	CWLSimpleListShell() {}
	virtual ~CWLSimpleListShell(){}

	virtual void SetCanVertScroll(bool can)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetCanVertScroll(can);
		}
		return ;

	}

	virtual void SetHasVertScrollBar(bool has)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetHasVertScrollBar(has);
		}
		return ;

	}

	virtual void SetCanHorzScroll(bool can)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetCanHorzScroll(can);
		}
		return ;

	}

	virtual void SetHasHorzScrollBar(bool has)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetHasHorzScrollBar(has);
		}
		return ;

	}

	virtual void SetVertical(bool vert)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetVertical(vert);
		}
		return ;

	}

	virtual void SetColumnWidth(int width)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetColumnWidth(width);
		}
		return ;

	}
	virtual void SetRowHeight(int height)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetRowHeight(height);
		}
		return ;

	}

	virtual void SetItemFixedSpan(bool fix)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetItemFixedSpan(fix);
		}
		return ;

	}
	virtual int InsertItem(int index, IWLSimpleListItem * item)
	{
		if (m_wlimp)
		{
			return m_wlimp->InsertItem(index, item);
		}
		return -1;

	}
	virtual bool DeleteItem(int index)
	{
		if (m_wlimp)
		{
			return m_wlimp->DeleteItem(index);
		}
		return false;

	}
	virtual bool DeleteAllItems()
	{
		if (m_wlimp)
		{
			return m_wlimp->DeleteAllItems();
		}
		return false;

	}
	virtual IWLSimpleListItem * GetItem(int index)
	{
		if (m_wlimp)
		{
			return m_wlimp->GetItem(index);
		}
		return NULL ;

	}
	virtual int GetItemCount() const
	{
		if (m_wlimp)
		{
			return m_wlimp->GetItemCount();
		}
		return 0;

	}
	virtual void GetSize(LPSIZE lpSize) const
	{
		if (m_wlimp)
		{
			return m_wlimp->GetSize(lpSize);
		}
		return ;

	}

	virtual void EnsureVisible(int index)
	{
		if (m_wlimp)
		{
			return m_wlimp->EnsureVisible(index);
		}
		return ;

	}

	virtual void SetEventListener(IWLSimpleList::EventListener * listener)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetEventListener(listener);
		}
		return ;

	}
	virtual void SetScrollListener(IWLScrollWnd::ScrollListener * listener)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetScrollListener(listener);
		}
		return ;

	}

	virtual ItemSelectManager* GetSelectManager()
	{
		if (m_wlimp)
		{
			return m_wlimp->GetSelectManager();
		}
		return NULL;

	}

	
};
typedef CWLSimpleListShell<> WLSimpleList;
