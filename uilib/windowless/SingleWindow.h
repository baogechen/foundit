#pragma once

#include "BaseWndLessWnd.h"

class SingleWindow : public BaseWndlessWndDestroyListener
{
public:

	typedef BaseWndlessWnd * (*CREATEWINDOWOBJECT)(SingleWindow *) ;

	SingleWindow(CREATEWINDOWOBJECT fun);
	~SingleWindow(void);

	// ��������
	// �򿪵�һ���ڣ�
	// ���δ�������򴴽�������
	// ����Ѵ������򼤻�
	void Open() ;

	// �����Ƿ��Ѵ�
	BOOL IsOpen() ;

	// ��������
	// �رյ�һ���ڣ����ര�ڶ���һ��֧��������ʱdelete�Լ�
	void Close() ;

	BaseWndlessWnd * GetWindow() const ;
	CWndLessRoot * GetWindowRoot() const ;

	//////////////////////////////////////////////////////////////////////////
	// BaseWndlessWndDestroyListener

	virtual void OnSomeWndDestroy(BaseWndlessWnd * wnd) ;

protected:
	/*virtual*/ void Activate() ;
	//virtual HWND CreateSingleWindow() = 0 ;

private:
	//HWND m_single_window ;
	CREATEWINDOWOBJECT m_create_window_fun ;
	BaseWndlessWnd * m_single_window ;
};
