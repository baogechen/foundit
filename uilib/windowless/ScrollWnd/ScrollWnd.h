//////////////////////////////////////////////////////////////////////////
//
//	����:   �������ڣ����Խ�ͨ�������ӿڿ��ƹ��������Ժ���Ӧ�������¼�
//
//  ����:   Content��ʾScrollWnd��Ҫչ�ֵ����ݣ�Viewport��ʾչ�ֵ��ӿ�
//
//	
//	����:   2011/08/15
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "uilib/wndless/IWLScrollWnd.h"
#include "windowless/ScrollBar/WLScrollBar.h"

class WLScrollWndImp :
    public WLScrollWndImpBase
{
public:
    WLScrollWndImp(void);
    virtual ~WLScrollWndImp(void);

public:
    // �ڴ���ǰ����
    // �����Ƿ��ܴ�ֱ����
    virtual void SetCanVertScroll(bool can) 
    {
        m_can_vert_scroll = can ;
        if (!can)
        {
            m_has_vert_scroll_bar = false ;
        }
    }

    // �����Ƿ��д�ֱ������
    virtual void SetHasVertScrollBar(bool has) { m_has_vert_scroll_bar = has ; }

    // �����Ƿ���ˮƽ����
    virtual void SetCanHorzScroll(bool can) 
    {
        m_can_horz_scroll = can ;
        if (!can)
        {
            m_has_horz_scroll_bar = false ;
        }
    }

    // �����Ƿ���ˮƽ������
    virtual void SetHasHorzScrollBar(bool has) { m_has_horz_scroll_bar = has ; }

    // ����������ģʽ��
    // ������ģʽΪ��ֱʱ�������¼�ת���ɴ�ֱ��������ҳ�¼���shift+�����¼�ת����ˮƽ��������ҳ�¼���
    // ˮƽʱ�෴��
    virtual void SetMainScrollMode(MainScrollMode msm) { m_main_scroll_mode = msm ; }

protected:
    virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
    virtual void OnSized(SIZE const &szOld, SIZE const &szNew) ;

    void OnScrollShow(BOOL show, WLScrollBar* pScrollBar) ;
    void OnVScroll(LPSCROLLMSGINFO pSmi, WLScrollBar* pScrollBar) ;
    void OnHScroll(LPSCROLLMSGINFO pSmi, WLScrollBar* pScrollBar) ;
    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point) ;

public:
//protected:
    void SetContentSize(int width, int height) ;
    SIZE GetContentSize() const { return m_content_size ; }

    SIZE GetViewportSize() const { return m_viewport_size ; }

    // x,y����viewport���Ͻǵ�Ϊԭ�������
    // ͨ������ӿڿ��ƹ�������λ��
    void SetContentPosition(int x, int y) ;
    POINT GetContentPosition() const { return m_content_position ; }

protected:
    ///////////////////////////////////////////////////////////////////////////
    // ��Щ�ӿ����ڱ�����
    virtual void OnContentPositionXChanged(int /*old_x*/, int /*new_x*/) {}
    virtual void OnContentPositionYChanged(int /*old_y*/, int /*new_y*/) {}
    virtual void OnViewportWidthChanged(int /*old_width*/, int /*new_width*/) {}
    virtual void OnViewportHeightChanged(int /*old_height*/, int /*new_height*/) {}

    virtual bool CanVertScroll() const { return m_can_vert_scroll ; }
    virtual bool HasVertScrollBar() const { return m_has_vert_scroll_bar ; }
    virtual bool CanHorzScroll() const { return m_can_horz_scroll ; }
    virtual bool HasHorzScrollBar() const { return m_has_horz_scroll_bar ; }

protected:
    // ������һ�㲻Ҫֱ��ʹ�����³�Ա����
    // ��ֱ������
    WLScrollBar m_vert_scroll_bar ;

    // ˮƽ������
    WLScrollBar m_horz_scroll_bar ;

    int m_scroll_width ;

    POINT m_content_position ;
    SIZE m_content_size ;
    SIZE m_viewport_size ;

    bool m_can_vert_scroll ;
    bool m_has_vert_scroll_bar ;
    bool m_can_horz_scroll ;
    bool m_has_horz_scroll_bar ;

    MainScrollMode m_main_scroll_mode ;

};
