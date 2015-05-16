#pragma once

#include "IWLButton.h"

class IWLButtonAL : public IWLButton
{
public:
	virtual ~IWLButtonAL(void){};

public:
	// ��ť����
	enum BTN_TYPE
	{
		PUSH		= 0x0000,
		CHECK		= 0x0001,
		RADIO		= 0x0002,
		LINK		= 0x0003,		// ��ť����ʱ���ƶ��ı����������ȥʱ��Ϊ��״
	} ;

	// ��ť���
	enum BTN_STYLE
	{
		NORMAL			= 0x0000,
		PUSHLIKE		= 0x0001,		// Check��Radio���ʱ��Ч
		FITFRGNDSIZE	= 0x0002,		// ����ǰ���Զ�������С
		FITTEXTSIZE		= 0x0004,		// ���������Զ�������С
	} ;

	// �������
	enum BKGND_STYLE
	{
		NOBKGND		= 0x0000,
		HANDPAINT	= 0x0001,		// �ֻ汳������ָ��������ɫ�������
		TRANSLUCENT	= 0x0002,		// ����͸����ʹ�ø����ڱ�����Ϊ������
	} ;

	// ǰ�����
	enum FRGND_STYLE
	{
		NOFRGND		= 0x0000,		// ��ǰ��
		FIXED		= 0x0001,		// �̶�����ǰ��
		FITX		= 0x0002,		// �̶�ǰ���߶ȣ�x�������01...12��ֻ���
		FITY		= 0x0004,		// �̶�ǰ����ȣ�y�������01...12��ֻ���
		FITXY		= 0x0006,		// x��y���������01...12����(δʵ��)
	} ;

	// �ı�����ı�����icon��text
	// BTN_STYLE���Զ�������Сʱ��CENTER��LEFT��RIGHT��TOP��BOTTOM��Ч
	// FITTEXTSIZE
	//	FITFRGNDSIZE + FITX: TOP, BOTTOM ��Ч
	//	FITFRGNDSIZE + FITY: LEFT, RIGHT ��Ч
	enum TEXT_STYLE
	{
		// 		TS_NEAR		= 0x0001,
		// 		TS_CENTER	= 0x0002,
		// 		TS_FAR		= 0x0004,
		// 		TS_VERTICAL	= 0x0008,		// ��ֱ����(δʵ��)

		TS_LEFT		= 0x0001,
		TS_HCENTER	= 0x0000,
		TS_RIGHT	= 0x0002,
		TS_TOP		= 0x0010,
		TS_VCENTER	= 0x0000,
		TS_BOTTOM	= 0x0020,

		TS_CENTER	= 0x0000,
	} ;

	enum BKGND_COLOR_INDEX
	{
		BCI_NORMAL				= 0x0001,
		BCI_HOT					= 0x0002,
		BCI_PUSHED				= 0x0004,
		BCI_DEFAULT				= 0x0008,
		BCI_DISABLE				= 0x0010,

		BCI_UNCHECKED			= 0x0001,
		BCI_UNCHECKED_HOT		= 0x0002,
		BCI_UNCHECKED_PUSHED	= 0x0004,
		BCI_UNCHECKED_DISABLE	= 0x0008,
		BCI_CHECKED				= 0x0010,
		BCI_CHECKED_HOT			= 0x0020,
		BCI_CHECKED_PUSHED		= 0x0040,
		BCI_CHECKED_DISABLE		= 0x0080,

		BCI_ALL					= 0xFFFF,
	};


public:
	// ����ǰ��
	virtual void SetFrgnd(
		Image * pImgFrgnd,
		int nStateCount,
		DWORD dwFrgndStyle,
		BOOL bDefer = FALSE
		)  = 0;

	// ���÷���
	virtual void SetSign(
		Image * pImgCheckSign,
		BOOL bDefer = FALSE
		) = 0;

	// ����ͼ��
	virtual void SetIcon(
		Image * pImgIcon,
		BOOL bDefer = FALSE,
		BOOL bAction = TRUE
		) = 0;

	// ��������
	virtual void SetType(
		DWORD dwBtnType,
		BOOL bDefer = FALSE
		) = 0;

	// ���÷��
	virtual void SetStyle(
		DWORD dwBtnStyle,
		BOOL bDefer = FALSE
		) = 0;

	// ���ñ������
	virtual void SetBkgndStyle(
		DWORD dwBkgndStyle,
		BOOL bUpdate = FALSE
		)  = 0;

	// �����ı����
	virtual void SetTextStyle(
		DWORD dwTextStyle,
		BOOL bDefer = FALSE
		) = 0;

	virtual void SetMargin(int nLeft, int nTop, int nRight, int nBottom, BOOL bDefer = FALSE)  = 0;
	virtual void SetSignTextSpace(int nSignTextSpace, BOOL bDefer = FALSE)  = 0;
	virtual void SetIconTextSpace(int nIconTextSpace, BOOL bDefer = FALSE)  = 0;

	virtual void SetBkgndColor(DWORD dwMask, ...)   = 0;
	virtual void SetTextColor(DWORD dwMask, ...)  = 0;

	virtual void EndDeferRecalcLayout()  = 0;

	virtual void SetUrl(CString const & sUrl)  = 0;

};

typedef CWndLessImpBase< CWLButtonShell<IWLButtonAL> >  WLButtonALImpBase;

template <class IT = IWLButtonAL, class IMPT = WLButtonALImpBase > 
class CWLButtonALShell : public CWLButtonShell<IT, IMPT> 
{
public:
	CWLButtonALShell() {}
	virtual ~CWLButtonALShell(){}

	// ����ǰ��
	virtual void SetFrgnd(
		Image * pImgFrgnd,
		int nStateCount,
		DWORD dwFrgndStyle,
		BOOL bDefer = FALSE
		) 
	{
		if (m_wlimp)
		{
			return m_wlimp->SetFrgnd(pImgFrgnd, nStateCount, dwFrgndStyle, bDefer);
		}
	};

	// ���÷���
	virtual void SetSign(
		Image * pImgCheckSign,
		BOOL bDefer = FALSE
		) 
	{
		if (m_wlimp)
		{
			return m_wlimp->SetSign(pImgCheckSign, bDefer);
		}
	};

	// ����ͼ��
	virtual void SetIcon(
		Image * pImgIcon,
		BOOL bDefer = FALSE,
		BOOL bAction = TRUE
		) 
	{
		if (m_wlimp)
		{
			return m_wlimp->SetIcon(pImgIcon, bDefer, bAction);
		}
	};

	// ��������
	virtual void SetType(
		DWORD dwBtnType,
		BOOL bDefer = FALSE
		)  
	{
		if (m_wlimp)
		{
			return m_wlimp->SetType(dwBtnType, bDefer);
		}
	};

	// ���÷��
	virtual void SetStyle(
		DWORD dwBtnStyle,
		BOOL bDefer = FALSE
		) 
	{
		if (m_wlimp)
		{
			return m_wlimp->SetStyle(dwBtnStyle, bDefer);
		}
	};

	// ���ñ������
	virtual void SetBkgndStyle(
		DWORD dwBkgndStyle,
		BOOL bUpdate  = FALSE
		)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetBkgndStyle(dwBkgndStyle, bUpdate);
		}
	};

	// �����ı����
	virtual void SetTextStyle(
		DWORD dwTextStyle,
		BOOL bDefer = FALSE
		)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetTextStyle(dwTextStyle, bDefer);
		}
	};

	virtual void SetMargin(int nLeft, int nTop, int nRight, int nBottom, BOOL bDefer = FALSE)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetMargin(nLeft, nTop, nRight, nBottom, bDefer);
		}
	};
	virtual void SetSignTextSpace(int nSignTextSpace, BOOL bDefer = FALSE)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetSignTextSpace(nSignTextSpace, bDefer);
		}
	};
	virtual void SetIconTextSpace(int nIconTextSpace, BOOL bDefer = FALSE)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetIconTextSpace(nIconTextSpace, bDefer);
		}
	};

	virtual void SetBkgndColor(DWORD dwMask, ...) 
	{
		if (m_wlimp)
		{
			va_list marker;
			va_start(marker, dwMask) ;
			int i ;
			COLORREF colors[6] = {0};
			for (i = 0 ; i < 6 ; ++ i)
			{
				if (dwMask & (1 << i))
				{
					colors[i] = va_arg(marker, COLORREF) ;
				}
			}
			va_end(marker);

			m_wlimp->SetBkgndColor(dwMask,colors[0],colors[1],colors[2],colors[3],colors[4],colors[5]);
			return;
		}
	};
	virtual void SetTextColor(DWORD dwMask, ...) 
	{
		if (m_wlimp)
		{
			va_list marker;
			va_start(marker, dwMask) ;
			int i ;
			COLORREF colors[8] = {0};
			for (i = 0 ; i < 8 ; ++ i)
			{
				if (dwMask & (1 << i))
				{
					colors[i] = va_arg(marker, COLORREF) ;
				}
			}
			va_end(marker);
			
			m_wlimp->SetTextColor(dwMask,colors[0],colors[1],colors[2],colors[3],colors[4],colors[5],colors[6],colors[7]);
			return;
		}
	};

	virtual void EndDeferRecalcLayout() 
	{
		if (m_wlimp)
		{
			return m_wlimp->EndDeferRecalcLayout();
		}
	};

	virtual void SetUrl(CString const & sUrl)
	{
		if (m_wlimp)
		{
			return m_wlimp->SetUrl(sUrl);
		}
	};

};

typedef CWLButtonALShell<> WLButtonAL;

UILIB_EXPORT BOOL WLButtonALProcess(WLButtonAL & button, LPCTSTR sStyle) ;



