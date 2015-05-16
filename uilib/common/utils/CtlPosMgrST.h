//////////////////////////////////////////////////////////////////////////
//
//	��ǰ���ܣ�ʵ���ܹ��ڴ��ڴ�С�����仯ʱ�ؼ�λ����Ӧ�仯
//	Ԥ�ڹ��ܣ������Ի���ؼ��仯
//			  �Ի���˵����ؼ�����(��ON_UPDATE_COMMAND_UI����)
//	˼·��    ��map��¼������Ϣ����OnSize�е����ؼ�λ��
//
//////////////////////////////////////////////////////////////////////////
//	���ڣ�	2009/3/18
//	�汾��	ST0000, ST0001
//			(ǰһ���汾, ��ǰ�汾)
//	���ߣ�	�����
//
//	��Ȩ���������������ⷢ����ʹ�á��޸ĸô��룬�����뱣�����а汾��Ϣ
//			  ��������İ汾�Ķ���Ϣ��
//
//////////////////////////////////////////////////////////////////////////
//	���ڣ�	2009/3/20
//	�汾��	ST0001, ST0002
//	�޸ģ�	��Ҫ�Ķ�������������Ƕ���ܣ�ʹ֮����ͨ����Ϊ�Ի����һ������
//			�����ƿؼ�����Ϊ��
//			�����Ķ���������˿ؼ�4�ֱ仯ģʽ��ö����X��Y��CX��CY��
//	�޸��ˣ������
//
//	��л���� vc-��ҹ�� �ı��������
//
//////////////////////////////////////////////////////////////////////////
//	���ڣ�	2009/6/18
//	�汾��	ST0002, ST0003
//	�޸ģ�	��ST0002�汾��CExtDialogST������ȡ��CCtlPosMgrST�࣬����Ŀ�괰��
//			����Ƕ������󣬹���ؼ�λ��
//	�޸��ˣ������
//
//	
//  
// 															�����ө���
// 															���֩��逸
// 															�ĩ��੤��
// 															��ӡ��ᰩ�
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

// CtlPosMgrST.h: interface for the CCtlPosMgrST class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLPOSMGRST_H__B19FC00C_2962_4A24_81A3_4BF20A12F04D__INCLUDED_)
#define AFX_CTLPOSMGRST_H__B19FC00C_2962_4A24_81A3_4BF20A12F04D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#include <map>

class CCtlPosMgrST  
{
public:
	CCtlPosMgrST();
	virtual ~CCtlPosMgrST();

// Data
private:
	struct STRETCH_ATTR 
	{
		BOOL _bLock ;
		UINT _nMask ;
		int _nPercentX ;
		int _nPercentY ;
		int _nPercentCX ;
		int _nPercentCY ;
		CRect _rcCtrl ;
		CSize _szParent ;
		STRETCH_ATTR() {}
		STRETCH_ATTR(
			UINT nMask,
			int nPercentX,
			int nPercentY,
			int nPercentCX,
			int nPercentCY,
			const CRect& rcCtrl,
			const CSize& szParent
			)
			: _bLock(FALSE), _nMask(nMask), _nPercentX(nPercentX), _nPercentY(nPercentY), _nPercentCX(nPercentCX), _nPercentCY(nPercentCY),
			_rcCtrl(rcCtrl), _szParent(szParent)
		{}
	};
//	typedef std::map<UINT, STRETCH_ATTR> SAMAP ;
	typedef std::map<HWND, STRETCH_ATTR> SAMAP ;
	SAMAP	m_mapSA ;
	
	HWND	m_hWndHost ;		// ��¼�������ھ��
	CSize	m_szHostClient ;	// ��¼�������ڿͻ�����С
	
public:
	enum {
		X = 0x1, Y = 0x2, CX = 0x4, CY = 0x8
	} ;
		
// Implementation
public:
	// ���������޵�����
	BOOL Create(CWnd *pParentWnd) 
	{
		ASSERT(pParentWnd != NULL) ;

		HWND hWnd = pParentWnd->GetSafeHwnd() ;
		if (::IsWindow(hWnd))
		{
			m_hWndHost = hWnd ;
			CRect rcClient ;
			::GetClientRect(m_hWndHost, rcClient) ;
			m_szHostClient = rcClient.Size() ;
			return TRUE ;
		}
		return FALSE ;
	}
	
	// ��ӿؼ�
	BOOL AddMapControl(
		UINT nIDControl,	// �ؼ�ID
		UINT nMask,			// �ؼ��仯���룺(cy << 3) | (cx << 2) | (y << 1) | (x)
		...					// ��˳�������ṩx��y��cx��cy�ı仯�ٷֱ�
		) ;

	BOOL AddMapControl(
		HWND hCtrl,			// �Ӵ��ھ��
		UINT nMask,
		...
		) ;
	
/*	BOOL AddMapControl(
		UINT nIDControl,	// �ؼ�ID
		BOOL bX,			// �ؼ�λ�õ�x�Ƿ��游���ڱ仯
		int nPercentX,		// �仯�İٷֱ�
		BOOL bY,			// �ؼ�λ�õ�y�Ƿ��游���ڱ仯
		int nPercentY,		// �仯�İٷֱ�
		BOOL bCX,			// �ؼ�λ�õ�cx�Ƿ��游���ڱ仯
		int nPercentCX,		// �仯�İٷֱ�
		BOOL bCY,			// �ؼ�λ�õ�cy�Ƿ��游���ڱ仯
		int nPercentCY		// �仯�İٷֱ�
		) ;

	BOOL AddMapControl(
		UINT nIDControl,
		UINT nMask,			// �ؼ��仯���룺(cy << 3) | (cx << 2) | (y << 1) | (x)
		int nPercentX,
		int nPercentY,
		int nPercentCX,
		int nPercentCY
		) ;*/

	BOOL AddMapControl(
		HWND hCtrl,
		UINT nMask,			// �ؼ��仯���룺(cy << 3) | (cx << 2) | (y << 1) | (x)
		int nPercentX,
		int nPercentY,
		int nPercentCX,
		int nPercentCY
		) ;

	// �Ƴ��ؼ�
	BOOL RemoveMapControl(
		UINT nIDControl
		) ;

	// �Ƴ��ؼ�
	BOOL RemoveMapControl(
		HWND hCtrl
		) ;

	// �����ؼ�
	BOOL LockControl(
		UINT nIDControl
		) ;

	// �����ؼ�
	BOOL UnLockControl(
		UINT nIDControl
		) ;

	// ��������
	BOOL LockControl(
		HWND hCtrl
		) ;

	// ��������
	BOOL UnLockControl(
		HWND hCtrl
		) ;

	// ���¿ؼ�λ��
	BOOL UpdateMapControlPos(
		UINT nIDControl		// �ؼ�ID
		) ;

	// ���¿ؼ�λ��
	BOOL UpdateMapControlPos(
		HWND hCtrl			// �Ӵ��ھ��
		) ;

	// �ı�ؼ�λ��
	BOOL ChangeMapControlPos(
		UINT nIDControl, UINT nMask, ...
		) ;
	BOOL ChangeMapControlPos(
		HWND hCtrl, UINT nMask, ...
		) ;
	BOOL ChangeMapControlPos(
		HWND hCtrl, UINT nMask, int x, int y, int cx, int cy
		) ;
	
	// ���޵��������ӦWM_SIZE��Ϣ
 	void HandleOnSize(UINT nType, int cx, int cy) ;

};

#endif // !defined(AFX_CTLPOSMGRST_H__B19FC00C_2962_4A24_81A3_4BF20A12F04D__INCLUDED_)
