
#ifndef _WLUSER_
#define _WLUSER_

#pragma once

class IWndLess ;
typedef IWndLess * HWL ;

//////////////////////////////////////////////////////////////////////////
// Skin
#define MSG_SKIN_CHANGED							WM_USER + 1000


//////////////////////////////////////////////////////////////////////////
// Internal Window Less Handle

#define HWL_TOP			((HWL)0)
#define HWL_BOTTOM		((HWL)1)

//////////////////////////////////////////////////////////////////////////
// Internal Window Style

#define WS_SIZEPOSCHANGEDNOTIFYPARENT WS_POPUP			// ��С��λ�øı�֪ͨ������
#define WS_ILLUSION			WS_CHILD					// �����񣬿��ü��������޷���HitTest
#define WS_GLOWTEXT			WS_CLIPSIBLINGS				// ��������	

//////////////////////////////////////////////////////////////////////////
// Internal Message

#define WM_INTERNAL_MAX		0x0000FFFF					// High Word�ڲ�ʹ��

//////////////////////////////////////////////////////////////////////////
// utility
#define WM_LBUTTONCLICK		(WM_INTERNAL_MAX)			// ��������Ϣ
#define WM_RBUTTONCLICK		(WM_INTERNAL_MAX - 1)		// �Ҽ������Ϣ

// ����
#define WM_LDOWNHOST		(WM_INTERNAL_MAX - 1000)	// ����������������Ϣ�������㴰��

// wparam: HDC
// lparam: LPRECT
#define WM_QUERYBKGND		(WM_INTERNAL_MAX - 1001)	// ���󱳾�

// wparam: ����ƽ����ɫ�Ҷ�
#define WM_BKGNDCHANGED		(WM_INTERNAL_MAX - 1002)	// �����ı�֪ͨ��Ϣ

//////////////////////////////////////////////////////////////////////////
// Scrollbar

// ��������ʾ������
// wParam: show(TRUE)/hide(FALSE)
// lParam: Scrollbar pointer
#define WM_SCROLLBAR_SHOW	(WM_INTERNAL_MAX - 1100)

//////////////////////////////////////////////////////////////////////////
// WndLess Notify

#define NM_WLFIRST	(0U - 2010U)
typedef struct _tagNMWLHDR
{
	HWL hWlFrom ;
	UINT_PTR idFrom ;
	UINT code ;
} NMWLHDR, *LPNMWLHDR ;

#define NM_SIZED	(NM_WLFIRST - 1)	// ���ڴ�С�ı�
typedef struct _tagNMWLSIZEDINFO
{
	NMWLHDR hdr ;
	SIZE szOld ;
	SIZE szNew ;
}NMWLSIZEDINFO, *LPNMWLSIZEDINFO ;

#define NM_MOVED	(NM_WLFIRST - 2)	// ����λ�øı�
typedef struct _tagNMWLMOVEDINFO
{
	NMWLHDR hdr ;
	POINT ptOld ;
	POINT ptNew ;
}NMWLMOVEDINFO, *LPNMWLMOVEDINFO ;

//////////////////////////////////////////////////////////////////////////
// Internal Control Id

#define CTRLID_TOOLTIP							0x00000000

//////////////////////////////////////////////////////////////////////////
// Internal Timer Id
// ϵͳԤ��2^31����ʱ��
// �Զ��嶨ʱ������С��TIMERID_INTERNAL_FIRST

#define TIMERID_INTERNAL_FIRST					(UINT_PTR)0x10000000
#define TIMERID_INTERNAL_LAST					(UINT_PTR)0xFFFFFFFF

// Edit
#define TIMERID_EDIT_CARET						(UINT_PTR)(TIMERID_INTERNAL_FIRST)

// Scroll Bar
#define TIMERID_SCROLLBAR_ARROW_FIRSTDOWN		(UINT_PTR)(TIMERID_INTERNAL_FIRST + 1)
#define TIMERID_SCROLLBAR_ARROW_KEEPDOWN		(UINT_PTR)(TIMERID_INTERNAL_FIRST + 2)
#define TIMERID_SCROLLBAR_CHANNEL_FIRSTDOWN		(UINT_PTR)(TIMERID_INTERNAL_FIRST + 3)
#define TIMERID_SCROLLBAR_CHANNEL_KEEPDOWN		(UINT_PTR)(TIMERID_INTERNAL_FIRST + 4)

// Animation
#define TIMERID_ANIMATION_PLAY					(UINT_PTR)(TIMERID_INTERNAL_FIRST + 5)

//////////////////////////////////////////////////////////////////////////
// Scroll Bar Commands

#define SB_WHEELUP		47
#define SB_WHEELDOWN	48

//////////////////////////////////////////////////////////////////////////
// String

// �������߾������ı�
#ifdef UNICODE
#define LPSTR_TEXTCALLBACK_OWNER	((LPWSTR)-2L)
#else
#define LPSTR_TEXTCALLBACK_OWNER	((LPSTR)-2L)
#endif

#endif
