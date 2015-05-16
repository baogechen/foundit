//////////////////////////////////////////////////////////////////////////
//
//	����:   ����б��б������̡�ѡ����������߿�
//
//	
//	����:   2011/08/26
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "uilib/wndless/IWndLess.h"
#include "uilib/wndless/IWLScrollWnd.h"
#include "uilib/wndless/IWLListItem.h"
#include "uilib/wndless/IWLSimpleList.h"

#include "LinearListPlate.h"


class WLSimpleListImp :
    public WLSimpleListImpBase, public ItemSelectManager
{
public:
    WLSimpleListImp(void);
    virtual ~WLSimpleListImp(void);

public:
    //////////////////////////////////////////////////////////////////////////
    // ����ӿ�

    // �����ܷ�ֱ����
    void SetCanVertScroll(bool can) ;

    // �����Ƿ��д�ֱ������
    void SetHasVertScrollBar(bool has) ;

    // �����ܷ�ˮƽ����
    void SetCanHorzScroll(bool can) ;

    // �����Ƿ���ˮƽ������
    void SetHasHorzScrollBar(bool has) ;

    // ���÷���
    void SetVertical(bool vert) ;

    // �б���Ϊ��ֱʱ��widthΪ-1��ʾ�п�����̿�ȱ���һֱ
    // �б���Ϊˮƽʱ����ʾԪ��Ĭ���п�
    void SetColumnWidth(int width) ;

    // �б���Ϊˮƽʱ��heightΪ-1��ʾ�п�����̸߶ȱ���һֱ
    // �б���Ϊ��ֱʱ����ʾԪ��Ĭ���и�
    void SetRowHeight(int height) ;

	void SetItemFixedSpan(bool fix)
	{
		m_plate.SetItemFixedSpan(fix);
	}

    // ǰ�壬indexΪ-1��ʾ������󣬷���-1��ʾʧ�ܣ�
    // ����һ����ͨ�Ĳ���Ԫ�ؽӿڣ����ʺ���������
    int InsertItem(int index, IWLSimpleListItem * item) ;
    bool DeleteItem(int index) ;
	bool DeleteAllItems();
    IWLSimpleListItem * GetItem(int index) ;
	int GetItemCount() const { return m_plate.GetItemCount(); }
	virtual void GetSize(LPSIZE lpSize) const { return m_plate.GetSize(lpSize); }

    // ʹĳԪ�ؿɼ�
    void EnsureVisible(int index) ;

    void SetEventListener(EventListener * listener) { m_event_listener = listener ; }
	void SetScrollListener(IWLScrollWnd::ScrollListener * listener) {m_list.SetScrollListener(listener) ;};

    //////////////////////////////////////////////////////////////////////////
    // ItemSelectManager Implement
    virtual void SetMultipleSelect(bool mutiple) { m_multiple_select = mutiple ; }
    virtual bool IsMultipleSelect() const { return m_multiple_select ; }

    virtual IWLSelectableItem * GetSelectedItem() const ;
    virtual int GetMultipleSelectedItem(ITEMLIST & selected_item_list) const ;

    // ����
    typedef bool (*SORTFUN)(IWLSelectableItem * lhs, IWLSelectableItem * rhs) ;
    void Sort(SORTFUN sort_fun) ;

    template<typename T>
    void Sort(T sort_fun)
    {
        m_plate.Sort<T>(sort_fun) ;
        m_list.UpdateView() ;
    }

	virtual ItemSelectManager* GetSelectManager();

protected:
    // uFlags��ǵ�ǰ�İ��������״̬
    virtual void SelectItem(IWLSelectableItem * item, UINT uFlags) ;
    virtual void UnselectItem(IWLSelectableItem * item, UINT uFlags) ;
    virtual void UnselectAllItem() ;

    virtual void LeftClickItem(IWLSelectableItem * item) ;
    virtual void LeftDoubleClickItem(IWLSelectableItem * item) ;
    virtual void RightClickItem(IWLSelectableItem * item) ;
    virtual void RightDoubleClickItem(IWLSelectableItem * item) ;

protected:
    //////////////////////////////////////////////////////////////////////////
    // IWndLess Implement
    virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
    virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;
    virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;

protected:
    WLScrollWnd m_list ;
    LinearListPlate m_plate ;

    // ��Ե�հ�
    RECT m_frame_margin ;
    COLORREF m_frame_color ;

    bool m_multiple_select ;
    ITEMLIST m_selected_item_list ;
    IWLSelectableItem * m_last_selectd_item ;

    EventListener * m_event_listener ;

};
