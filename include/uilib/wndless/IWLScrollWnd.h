#pragma once
#include "IWndLess.h"
#include "IWLScrollWndPlate.h"

class IWLScrollWnd : public IWndLess
{
public:
	virtual ~IWLScrollWnd(void){};

	enum MainScrollMode
	{
		Horizontal = 0, Vertical,
	};

public:

	virtual void SetPlate(IWLScrollWndPlate * plate) = 0;

	virtual void LockPlateSizeChange() = 0;
	virtual void UnlockPlateSizeChange() = 0;

	// �ڴ���ǰ����
	// �����Ƿ��ܴ�ֱ����
	virtual void SetCanVertScroll(bool can) = 0;

	// �����Ƿ��д�ֱ������
	virtual void SetHasVertScrollBar(bool has) = 0;

	// �����Ƿ���ˮƽ����
	virtual void SetCanHorzScroll(bool can) = 0;

	// �����Ƿ���ˮƽ������
	virtual void SetHasHorzScrollBar(bool has) = 0;

	// ����������ģʽ��
	// ������ģʽΪ��ֱʱ�������¼�ת���ɴ�ֱ��������ҳ�¼���shift+�����¼�ת����ˮƽ��������ҳ�¼���
	// ˮƽʱ�෴��
	virtual void SetMainScrollMode(MainScrollMode msm) = 0;


	// �������ڼ�����
	class ScrollListener
	{
	public:
		virtual void OnContentPositionXChanged(int /*old_x*/, int /*new_x*/) {}
		virtual void OnContentPositionYChanged(int /*old_y*/, int /*new_y*/) {}
		virtual void OnViewportWidthChanged(int /*old_width*/, int /*new_width*/) {}
		virtual void OnViewportHeightChanged(int /*old_height*/, int /*new_height*/) {}
	};
	virtual void SetScrollListener(ScrollListener * listener) = 0 ;


};



typedef CWndLessImpBase< CWndLessShell<IWLScrollWnd> >  WLScrollWndImpBase;

template <class IT = IWLScrollWnd, class IMPT = WLScrollWndImpBase > 
class CWLScrollWndShell : public CWndLessShell<IT, IMPT> 
{
public:
	CWLScrollWndShell() {}
	virtual ~CWLScrollWndShell(){}
	virtual void SetPlate(IWLScrollWndPlate * plate)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetPlate(plate);
		}
		return ;

	}

	virtual void LockPlateSizeChange()
	{
		if (m_wlimp)
		{
			return m_wlimp->LockPlateSizeChange();
		}
		return ;

	}
	virtual void UnlockPlateSizeChange()
	{
		if (m_wlimp)
		{
			return m_wlimp->UnlockPlateSizeChange();
		}
		return ;

	}

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

	virtual void SetMainScrollMode(IWLScrollWnd::MainScrollMode msm)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetMainScrollMode(msm);
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
	
};

typedef CWLScrollWndShell<> WLScrollWnd;