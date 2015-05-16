#pragma once



class IWndLess;

class Caret
{
public:
    Caret() ;

public:
    BOOL CreateCaret(IWndLess * wnd, int width, int height) ;
    BOOL DestroyCaret() ;

    BOOL SetCaretPos(int x, int y) ;
    BOOL GetCaretPos(POINT &point) const ;

    BOOL ShowCaret() ;
    BOOL HideCaret() ;

    BOOL SetCaretBlinkTime(UINT milliseconds) ;
    UINT GetCaretBlinkTime() const ;

    BOOL SetCaretBlinkTimerId() ;
    UINT_PTR GetCaretBlinkTimerId() const ;

public:
    void Draw(HDC hdc, RECT const &update) ;
    BOOL OnTimer(UINT_PTR timer_id) ;

protected:
    void Update() ;

private:
    bool m_initialized ;

    IWndLess * m_wnd ;

    int m_width ;
    int m_height ;

    int m_x ;
    int m_y ;

    bool m_show ;       // �Ƿ�ʼ��ʾ���
    bool m_appeared ;   // ��ǰ��˸�Ĺ������ʾ���ǲ���ʾ��
    int m_hide_count ;  // ���ش���

    UINT_PTR m_blink_timer_id ;
    UINT m_blink_time ;

};
