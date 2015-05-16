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
#include "wndless/IWndLess.h"
#include "IWLAnimation.h"

class WLAnimationImp :
	public WLAnimationImpBase
{
public:
	WLAnimationImp(void);
	virtual ~WLAnimationImp(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// Operation
	virtual BOOL Open(LPCTSTR lpszPath) ;
	virtual BOOL Open(UINT nID) ;
	virtual BOOL Open(Shared_ImageEx &imgAnimation) ;

	virtual BOOL Open(LPCTSTR lpszPath, UINT uCol, UINT uRow) ;
	virtual BOOL Open(UINT nID, UINT uCol, UINT uRow) ;
	virtual BOOL Open(Shared_ImageEx &imgAnimation, UINT uCol, UINT uRow) ;

	virtual void Close() ;

	virtual UINT SetPlayElapse(UINT uPlayElapse) ;

	virtual void Play() ;
	virtual void Stop() ;
	virtual BOOL IsValid() 
	{
		return !(m_imgAnimation.IsNull()) ;
	}

    virtual BOOL MakeGray(BOOL make_gray) 
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
