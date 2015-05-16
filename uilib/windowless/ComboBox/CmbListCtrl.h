//////////////////////////////////////////////////////////////////////////
//
//	����:   ��Ͽ�ؼ��е��б�ؼ�
//
//	
//	����:   2011/02/17
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

#define LCIS_NORMAL			0			// ��ͨ
#define LCIS_SUBHOVERED		1			// ������
#define LCIS_PRIHOVERED		2			// ������

#define LCIBS_HIDE			0			// ����
#define LCIBS_NORMAL		1			// ��ʾ
#define LCIBS_HOVERED		2			// �������
#define LCIBS_PUSHED		3			// ��갴��

struct LCITEM 
{
	// Ĭ�Ϲ��캯��
	LCITEM(LPCTSTR lpszText)
		: m_strText(lpszText), m_uState(LCIS_NORMAL), m_uBtnState(LCIBS_NORMAL)
	{

	}

	// ����
	CString m_strText ;			// �ı�

	// ����
	int		m_nPosition ;		// ���ԭ��λ��
	int		m_nHeight ;			// �߶�
	int		m_nCurHeight ;		// ��ǰ�߶�
	UINT	m_uState ;			// ״̬

	// ɾ����ť����
	UINT	m_uBtnState ;		// ���״̬
};

typedef std::vector<LCITEM>	VEC_LCITEM ;

// CCmbListCtrl
#define ST_CMBLISTCTRL	_T("St.CmbListCtrl")

class CCmbListCtrl : public CWnd
{
	DECLARE_DYNAMIC(CCmbListCtrl)

public:
	CCmbListCtrl();
	virtual ~CCmbListCtrl();

public:
	enum OPERATION_TYPE
	{
		EXITSEL	= 0,
		SELITEM	= 1,
		DELITEM	= 2,
	};

	typedef struct _tagRESDATA
	{
		OPERATION_TYPE ot ;
		int nData ;				// ��ʾѡ��Ľڵ�
	}RESDATA, LPRESDATA;

	int Popup(
		int x, int y, int cx, int cy,
		HWND hParent,
		RESDATA &res
		) ;

	void LoadImage(
		Image * pImgDelBtn
		) ;

	// ����ַ���
	int AddString(LPCTSTR lpszString) ;

	// ��������
	int SetCurSel(int nIndex) ;

	// ��������߶�
	void SetItemHeight(int nHeight) ;

protected:
	// �ж������Ƿ�ɼ�
	BOOL IsItemVisible(UINT nIndex) ;

	// ����Ӧ��С
	void AutoAdjustSize() ;

	// �������
	int HitTest(CPoint point) ;

	// �ж������Ƿ�ɼ�
	BOOL IsItemVisible(LCITEM const & item) ;

	// ��������
	void DrawItem(CDC * pDC, LCITEM const & item) ;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//afx_msg void OnCancelMode();

protected:
	RESDATA * m_pResData ;

	CRect		m_rcClient ;
	COLORREF	m_crFrame ;					// �߿���ɫ

	CFont		m_ftFontNormal ;			// �����������������
	COLORREF	m_crTextColorNormal ;		// �������������������ɫ
	COLORREF	m_crTextColorHovered ;		// ��껬�����������������ɫ

	COLORREF	m_crBackColorNormal ;		// ����������������ɫ
	COLORREF	m_crBackColorHovered ;		// ��껬��������������ɫ

	int			m_nItemHeightNormal ;		// �������������߶�
	int			m_nItemPosOrigin ;			// ����λ��ԭ��
	int			m_nTrackItemIndex ;			// ��������������
	int			m_nTextOffsetLeft ;			// �����ı���ƫ��
	int			m_nBtnOffsetRight ;			// ɾ����ť��ƫ��
	int			m_nBtnWidth ;				// ɾ����ť���
	int			m_nBtnHeight ;				// ɾ����ť�߶�

	int			m_nBtnPushedItem ;			// ��ǰ��갴�µ���������
	BOOL		m_bBtnPushed ;				// ��ǵ�ǰ����Ƿ���
	CRect		m_rcCurrentBtn ;			// ��ǰ���°�ť��λ�÷�Χ
	Image *		m_pImgDelBtn ;

	// ����Ƿ�ʼ��Ӧ���������Ϣ
	// ��������Ͽ�������ť������û����������������������κβ�����
	// ����û�����������б����ٵ����������ѡ�����
	BOOL		m_bBeginHandleLBtnUp ;

	// ��������
	VEC_LCITEM	m_vecItems ;

};


