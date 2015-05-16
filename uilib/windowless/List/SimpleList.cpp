#include "StdAfx.h"
#include <algorithm>
#include "SimpleList.h"

#include "SimpleListItem.h"

WLSimpleListImp::WLSimpleListImp(void)
:
m_frame_color(RGB(0, 0, 255))
, m_multiple_select(false)
, m_last_selectd_item(NULL)
, m_event_listener(NULL)
{
    ::SetRect(&m_frame_margin, 1, 1, 1, 1) ;
}

WLSimpleListImp::~WLSimpleListImp(void)
{
}

//////////////////////////////////////////////////////////////////////////
// ����ӿ�

// �����ܷ�ֱ����
void WLSimpleListImp::SetCanVertScroll(bool can) 
{
    m_list.SetCanVertScroll(can) ;
}

// �����Ƿ��д�ֱ������
void WLSimpleListImp::SetHasVertScrollBar(bool has) 
{
    m_list.SetHasVertScrollBar(has) ;
}

// �����ܷ�ˮƽ����
void WLSimpleListImp::SetCanHorzScroll(bool can) 
{
    m_list.SetCanHorzScroll(can) ;
}

// �����Ƿ���ˮƽ������
void WLSimpleListImp::SetHasHorzScrollBar(bool has) 
{
    m_list.SetHasHorzScrollBar(has) ;
}

// ���÷���
void WLSimpleListImp::SetVertical(bool vert) 
{
    m_list.SetMainScrollMode(vert ? IWLScrollWnd::Vertical : IWLScrollWnd::Horizontal) ;
    m_plate.SetOrientation(vert ? LinearListPlate::Vertical : LinearListPlate::Horizontal) ;
}

// �б���Ϊ��ֱʱ��widthΪ-1��ʾ�п�����̿�ȱ���һֱ
// �б���Ϊˮƽʱ����ʾԪ��Ĭ���п�
void WLSimpleListImp::SetColumnWidth(int width) 
{
    m_plate.SetColumnWidth(width) ;
}

// �б���Ϊˮƽʱ��heightΪ-1��ʾ�п�����̸߶ȱ���һֱ
// �б���Ϊ��ֱʱ����ʾԪ��Ĭ���и�
void WLSimpleListImp::SetRowHeight(int height) 
{
    m_plate.SetRowHeight(height) ;
}

// ǰ�壬indexΪ-1��ʾ������󣬷���-1��ʾʧ�ܣ�
// ����һ����ͨ�Ĳ���Ԫ�ؽӿڣ����ʺ���������
int WLSimpleListImp::InsertItem(int index, IWLSimpleListItem * item) 
{
    item->SetSelectManager(this) ;
    return m_plate.InsertItem(index, item) ;
}

bool WLSimpleListImp::DeleteItem(int index) 
{
    IWLSelectableItem * item = m_plate.GetItem(index) ;
    if (item == NULL)
        return false ;

    //m_selected_item_list.erase(std::remove(m_selected_item_list.begin(), m_selected_item_list.end(), item), m_selected_item_list.end()) ;
    m_selected_item_list.erase(item) ;
    return m_plate.DeleteItem(index) ;
}

bool WLSimpleListImp::DeleteAllItems()
{
	m_selected_item_list.clear();

	return m_plate.DeleteAllItems() == TRUE;
}

IWLSimpleListItem * WLSimpleListImp::GetItem(int index) 
{
    return dynamic_cast<IWLSimpleListItem*>(m_plate.GetItem(index)) ;
}

//////////////////////////////////////////////////////////////////////////
// ItemSelectManager Implement

IWLSelectableItem * WLSimpleListImp::GetSelectedItem() const 
{
    if (m_selected_item_list.empty())
        return NULL ;

    return (*m_selected_item_list.begin()) ;
}

int WLSimpleListImp::GetMultipleSelectedItem(ITEMLIST & selected_item_list) const 
{
    int size = (int)m_selected_item_list.size() ;
    selected_item_list.insert(m_selected_item_list.begin(), m_selected_item_list.end()) ;
    return size ;
}

// ����
void WLSimpleListImp::Sort(SORTFUN sort_fun) 
{
    m_plate.Sort(sort_fun) ;
    m_list.UpdateView() ;
}

// uFlags��ǵ�ǰ�İ��������״̬
void WLSimpleListImp::SelectItem(IWLSelectableItem * item, UINT uFlags) 
{
    // ���¿����ж���ڵ�ѡ��״̬�ʹ�С�����ı䣬����plate��С�ı䣬Ϊ�����������ָı�
    // ���µ�plateλ�ñ仯����ѡ��״̬�仯֮ǰ������plate��С�ı�����µ�λ�ñ仯
    m_list.LockPlateSizeChange() ;

    if (!IsMultipleSelect())
    {
        // ��ѡ

        // �����ѡԪ��ѡ��״̬
        bool item_has_selected = false ;
        ITEMLIST::iterator pos ;
        for (pos = m_selected_item_list.begin() ; pos != m_selected_item_list.end() ; ++ pos)
        {
            if (!item_has_selected && (*pos) == item)
            {
                item_has_selected = true ;
            }
            else
            {
                (*pos)->Unselect() ;

                if (m_event_listener != NULL)
                    m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(*pos), false) ;
            }
        }
        m_selected_item_list.clear() ;
        m_selected_item_list.insert(item) ;
		
		// ѡ����Ԫ��
		item->Select() ;
        if (!item_has_selected)
        {
            m_selected_item_list.insert(item) ;

            if (m_event_listener != NULL)
                m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), true) ;
        }
    }
    else
    {
        // ��ѡ

        bool press_control = !!(uFlags & MK_CONTROL) ;
        bool press_shift = !!(uFlags & MK_SHIFT) ;
        bool press_right_button = !!(uFlags & MK_RBUTTON) ;

        // shift�����ȼ����
        if (press_shift)
        {
            if (press_right_button)
            {
                // ������µ����Ҽ���ɶ�²���
            }
            else if (m_last_selectd_item == NULL)
            {
                m_last_selectd_item = item ;
                item->Select() ;
                m_selected_item_list.insert(item) ;

                if (m_event_listener != NULL)
                    m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), true) ;
            }
            else
            {
                // �����ѡԪ��
                ITEMLIST old_selected_item_list ;
//                if (!press_control)
                {
                    // �����ѡ��
                    old_selected_item_list.insert(m_selected_item_list.begin(), m_selected_item_list.end()) ;
                    m_selected_item_list.clear() ;
                }

                // ������ڱ�ѡ���Ԫ��
                IWLSimpleListItem * list_item_begin = dynamic_cast<IWLSimpleListItem*>(m_last_selectd_item) ;
                IWLSimpleListItem * list_item_end = dynamic_cast<IWLSimpleListItem*>(item) ;

                int index_begin = list_item_begin->GetIndex() ;
                int index_end = list_item_end->GetIndex() ;
                if (index_begin > index_end) 
                {
                    index_begin ^= index_end ;
                    index_end ^= index_begin ;
                    index_begin ^= index_end ;
                }

                int i ;
                ITEMLIST::iterator pos ;
                for (i = index_begin ; i <= index_end ; ++ i)
                {
                    IWLSelectableItem * selected_item = m_plate.GetItem(i) ;
                    m_selected_item_list.insert(selected_item) ;

                    pos = old_selected_item_list.find(selected_item) ;
                    if (pos != old_selected_item_list.end())
                    {
                        old_selected_item_list.erase(pos) ;
                    }
                    else
                    {
                        selected_item->Select() ;

                        if (m_event_listener != NULL)
                            m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), true) ;
                    }
                }

                // �����ʱ����control���������֮ǰѡ�����ЩԪ��
                if (press_control)
                {
                    m_selected_item_list.insert(old_selected_item_list.begin(), old_selected_item_list.end()) ;

                    m_last_selectd_item = item ;
                }
                else
                {
                    for (pos = old_selected_item_list.begin() ; pos != old_selected_item_list.end() ; ++ pos)
                    {
                        (*pos)->Unselect() ;

                        if (m_event_listener != NULL)
                            m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(*pos), false) ;
                    }
                }
            }
        }
        // �����control
        else if (press_control)
        {
            if (press_right_button)
            {
                // ������µ����Ҽ���ɶ�²���
            }
            else if (item->IsSelected())
            {
                item->Unselect() ;
                m_selected_item_list.erase(
                    std::remove(m_selected_item_list.begin(), m_selected_item_list.end(), item),
                    m_selected_item_list.end()
                    ) ;

                if (m_event_listener != NULL)
                    m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), false) ;
            }
            else
            {
                item->Select() ;
                m_selected_item_list.insert(item) ;

                if (m_event_listener != NULL)
                    m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), true) ;
            }

            m_last_selectd_item = item ;
        }
        else
        {
            // �����ѡԪ��ѡ��״̬
            bool item_has_selected = false ;
            ITEMLIST::iterator pos ;
            for (pos = m_selected_item_list.begin() ; pos != m_selected_item_list.end() ; ++ pos)
            {
                if (!item_has_selected && (*pos) == item)
                {
                    item_has_selected = true ;
                }
                else
                {
                    (*pos)->Unselect() ;

                    if (m_event_listener != NULL)
                        m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), false) ;
                }
            }
            m_selected_item_list.clear() ;

            // ��Ϊ����ɾ���Ĳ����ȽϺ�ʱ��������clear���ٲ�����ѡ�е�Ԫ��
            if (item_has_selected)
            {
                m_selected_item_list.insert(item) ;
            }
            else
            {
                // ѡ����Ԫ��
                item->Select() ;
                m_selected_item_list.insert(item) ;

                if (m_event_listener != NULL)
                    m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), true) ;
            }

            m_last_selectd_item = item ;
        }
    }

    m_list.UnlockPlateSizeChange() ;
}

void WLSimpleListImp::UnselectItem(IWLSelectableItem * item, UINT uFlags) 
{
    item->Unselect() ;
    //m_selected_item_list.erase(std::remove(m_selected_item_list.begin(), m_selected_item_list.end(), item), m_selected_item_list.end()) ;

    m_selected_item_list.erase(item) ;

    if (m_event_listener != NULL)
        m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(item), false) ;
}

void WLSimpleListImp::UnselectAllItem() 
{
    ITEMLIST::iterator pos ;
    for (pos = m_selected_item_list.begin() ; pos != m_selected_item_list.end() ; ++ pos)
    {
        (*pos)->Unselect() ;

        if (m_event_listener != NULL)
            m_event_listener->OnListItemSelectChanged(this, dynamic_cast<IWLSimpleListItem*>(*pos), false) ;
    }

    m_selected_item_list.clear() ;
}

void WLSimpleListImp::LeftClickItem(IWLSelectableItem * item) 
{
    if (m_event_listener != NULL)
        m_event_listener->OnListLeftClick(this, dynamic_cast<IWLSimpleListItem*>(item)) ;
}

void WLSimpleListImp::LeftDoubleClickItem(IWLSelectableItem * item) 
{
    if (m_event_listener != NULL)
        m_event_listener->OnListLeftDoubleClick(this, dynamic_cast<IWLSimpleListItem*>(item)) ;
}

void WLSimpleListImp::RightClickItem(IWLSelectableItem * item) 
{
    if (m_event_listener != NULL)
        m_event_listener->OnListRightClick(this, dynamic_cast<IWLSimpleListItem*>(item)) ;
}

void WLSimpleListImp::RightDoubleClickItem(IWLSelectableItem * item) 
{
    if (m_event_listener != NULL)
        m_event_listener->OnListRightDoubleClick(this, dynamic_cast<IWLSimpleListItem*>(item)) ;
}

// ʹĳԪ�ؿɼ�
void WLSimpleListImp::EnsureVisible(int index) 
{
    m_plate.EnsureVisible(index) ;
}

//////////////////////////////////////////////////////////////////////////
// IWndLess Implement

BOOL WLSimpleListImp::OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) 
{
 	m_plate.SetItemFixedSpan(true) ;
    m_list.SetPlate(&m_plate) ;
    if (!m_list.Create(WS_VISIBLE, m_frame_margin.left, m_frame_margin.top, cx - m_frame_margin.left - m_frame_margin.right, cy - m_frame_margin.top - m_frame_margin.bottom, this, -1))
        return FALSE ;

    return TRUE ;
}

void WLSimpleListImp::OnSized(SIZE const &szOld, SIZE const &szNew) 
{
    m_list.SetWndLessPos(0, 0, szNew.cx - m_frame_margin.left - m_frame_margin.right, szNew.cy - m_frame_margin.top - m_frame_margin.bottom, SWLP_CX | SWLP_CY) ;
}

void WLSimpleListImp::OnDraw(HDC hDC, RECT const &rcUpdate) 
{
// 	CRect bound_in_host ;
//     GetRectInHost(&bound_in_host) ;
// 
//     COLORREF crOld = ::SetBkColor(hDC, m_frame_color) ;
//     if (m_frame_margin.top > 0)
//     {
//         CRect rcTop(bound_in_host.left, bound_in_host.top, bound_in_host.right - m_frame_margin.right, bound_in_host.top + m_frame_margin.top) ;
//         ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcTop, NULL, 0, NULL) ;
//     }
//     if (m_frame_margin.right > 0)
//     {
//         CRect rcRight(bound_in_host.right - m_frame_margin.right, bound_in_host.top, bound_in_host.right, bound_in_host.bottom - m_frame_margin.bottom) ;
//         ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcRight, NULL, 0, NULL) ;
//     }
//     if (m_frame_margin.bottom > 0)
//     {
//         CRect rcBottom(bound_in_host.left + m_frame_margin.left, bound_in_host.bottom - m_frame_margin.bottom, bound_in_host.right, bound_in_host.bottom) ;
//         ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcBottom, NULL, 0, NULL) ;
//     }
//     if (m_frame_margin.left > 0)
//     {
//         CRect rcLeft(bound_in_host.left, bound_in_host.top + m_frame_margin.top, bound_in_host.left + m_frame_margin.left, bound_in_host.bottom) ;
//         ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcLeft, NULL, 0, NULL) ;
//     }
//     SetBkColor(hDC, crOld) ;
}

ItemSelectManager* WLSimpleListImp::GetSelectManager()
{
	return this;
}

template<>
UILIB_EXPORT WLSimpleListImpBase *CreateDefaultImp< WLSimpleListImpBase >(IWLSimpleList * wrapper)

{
	WLSimpleListImp *ret = new WLSimpleListImp();
	ret->SetWrapper(wrapper);
	return ret;
};