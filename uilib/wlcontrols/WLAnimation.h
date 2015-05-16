//////////////////////////////////////////////////////////////////////////
//
//	����:   �����ؼ���������ʾ��������Ȼ���ö��������Ǿ�ֹ��
//
//	
//	����:   2011/02/21
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "..\windowless\wndless.h"
#include "..\utils\ImageEx\Shared_ImageEx.h"

class WLAnimation :
	public CWndLess
{
public:
	WLAnimation(void);
	virtual ~WLAnimation(void);

public:
	enum STYLE
	{
		AS_AUTOFITIMAGE		= 0x00000001,	// ����Ӧͼ���С
		AS_CMBD				= 0x00000002,	// ��϶���
	};
	//////////////////////////////////////////////////////////////////////////
	// Operation
	BOOL Open(LPCTSTR lpszPath) ;
	BOOL Open(UINT nID) ;
	BOOL Open(Shared_ImageEx &imgAnimation) ;

	BOOL Open(LPCTSTR lpszPath, UINT uCol, UINT uRow) ;
	BOOL Open(UINT nID, UINT uCol, UINT uRow) ;
	BOOL Open(Shared_ImageEx &imgAnimation, UINT uCol, UINT uRow) ;

	void Close() ;

	UINT SetPlayElapse(UINT uPlayElapse) ;

	void Play() ;
	void Stop() ;
	BOOL IsValid() 
	{
		return !(m_imgAnimation.IsNull()) ;
	}

    BOOL MakeGray(BOOL make_gray) 
    {
        if (m_make_gray == make_gray)
            return m_make_gray ;

        BOOL old = m_make_gray ;
        m_make_gray = make_gray ;
        UpdateView() ;

        return old ;
    }

protected:
	//////////////////////////////////////////////////////////////////////////
	// IWndLess Implement
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual void OnDraw(HDC hDC, RECT const &rcUpdate) ;

	//////////////////////////////////////////////////////////////////////////
	// Message Handler
	void OnTimer(UINT_PTR nIDEvent) ;

	//////////////////////////////////////////////////////////////////////////
	// Inner Function

	void PlayCallbackFucntion() ;

// Data
protected:
	Shared_ImageEx m_imgAnimation ;

	UINT m_uImageColCount ;		// ͼ������
	UINT m_uImageRowCount ;		// ͼ������
	UINT m_uPlayElapse ;		// ���ż��ʱ��
	UINT m_uFrameIndex ;		// ����֡����
	UINT m_uFrameWidth ;
	UINT m_uFrameHeight ;

    BOOL m_make_gray ;          // ����Ƿ�ҶȻ�

};
