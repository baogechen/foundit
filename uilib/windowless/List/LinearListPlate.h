//////////////////////////////////////////////////////////////////////////
//
//	����:   �б����̣���������б���������б����С�仯���������С��
//          �������б�����ʾ
//          һά�б�����ֻ�е��л��ߵ���
//
//	
//	����:   2011/08/18
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "uilib/wndless/IWLScrollWndPlate.h"
#include "uilib/wndless/IWLListItem.h"

class LinearListPlate: public CWndLessShell<IWLScrollWndPlate>, public IWndLess::DisplayListener
{
public:
    LinearListPlate() ;
    virtual ~LinearListPlate() ;

    enum Orientation
    {
        Horizontal = 0, Vertical,
    };

public:
    // ���÷���
    void SetOrientation(Orientation ori) ;

    // �б���Ϊ��ֱʱ��widthΪ-1��ʾ�п�����̿�ȱ���һֱ
    // �б���Ϊˮƽʱ����ʾԪ��Ĭ���п�
    void SetColumnWidth(int width) ;

    // �б���Ϊˮƽʱ��heightΪ-1��ʾ�п�����̸߶ȱ���һֱ
    // �б���Ϊ��ֱʱ����ʾԪ��Ĭ���и�
    void SetRowHeight(int height) ;

    // ����Ԫ�ع̶����
    void SetItemFixedSpan(bool fixed) { m_fixed_span = fixed ; }

    // ǰ�壬indexΪ-1��ʾ������󣬷���-1��ʾʧ�ܣ�
    // ����һ����ͨ�Ĳ���Ԫ�ؽӿڣ����ʺ���������
    int InsertItem(int index, IWLSelectableItem * item) ;
    bool DeleteItem(int index) ;
	BOOL DeleteAllItems();
    IWLSelectableItem * GetItem(int index) ;

	int GetItemCount()const;

    // ʹĳ��Ԫ�ؿɼ�
    void EnsureVisible(int index) ;

    typedef bool (*SORTFUN)(IWLSelectableItem * lhs, IWLSelectableItem * rhs) ;
    void Sort(SORTFUN sort_fun) ;

    template<typename T>
    void Sort(T sort_fun) 
	{
		std::sort(m_item_list.begin(), m_item_list.end(), sort_fun) ;

		RecalcLayout() ;
	}

protected:
    //virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
    //virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;

    //////////////////////////////////////////////////////////////////////////
    // IWndLess Implement
    virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;

    //////////////////////////////////////////////////////////////////////////
    // ListPlate Implement
    virtual void OnViewportWidthChanged(int old_width, int new_width) ;
    virtual void OnViewportHeightChanged(int old_height, int new_height) ;

    //////////////////////////////////////////////////////////////////////////
    // IWndLess::DisplayListener Implement
    virtual void OnWndLessSizeChanged(IWndLess * wnd, SIZE const &old_size, SIZE const &new_size) ;
    virtual void OnWndLessShowHideChanged(IWndLess * wnd, bool old_status, bool new_status) ;

protected:
    //////////////////////////////////////////////////////////////////////////
    // Internal 
    bool CalcItemPositionByIndex(int index, POINT &pos) ;
    void GetItemDefaultSize(SIZE &size) ;
    void RecalcLayout() ;

private:

    typedef std::vector<IWLSelectableItem*> ITEMLIST ;
    ITEMLIST m_item_list ;

    Orientation m_orientation ;
    int m_column_width ;    // �б���Ϊˮƽʱ���п�Ϊÿ��Ԫ�صĳ�ʼĬ�Ͽ�ȣ�Ϊ��ֱʱ���п�Ϊÿ��Ԫ�ص�ͳһ���
    int m_row_height ;      // �б���Ϊˮƽʱ���и�Ϊÿ��Ԫ�ص�ͳһ��ȣ�Ϊ��ֱʱ���п�Ϊÿ��Ԫ�صĳ�ʼĬ�Ͽ��

    bool m_fixed_span ;     // �̶����

    SIZE m_viewport_size ;  // �ӿڴ�С

};
