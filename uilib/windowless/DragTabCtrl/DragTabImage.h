#pragma once


class CDragTabImage : public CWnd, public COleDropSource
{
	DECLARE_DYNAMIC(CDragTabImage)

public:
	CDragTabImage();
	virtual ~CDragTabImage();

	BOOL Create(
		CDC * pDC,
		CRect const &rcInDC,			// ����ȡ��������dc�еķ�Χ
		CWnd * pParent,
		CPoint const &ptInScreen,		// ����ȡ����������Ļ�е�λ��
		CPoint ptDragInScreen			// �����ק������Ļ�е�λ��
		) ;

	BOOL Create(
		CBitmap* pBmp, // �϶�ͼ�������
		CSize const &szItem,			// ����ȡ��������dc�еķ�Χ
		CWnd * pParent,
		CPoint const &ptInScreen,		// ����ȡ����������Ļ�е�λ��
		CPoint ptDragInScreen			// �����ק������Ļ�е�λ��
		) ;

	void DoDragging(CPoint ptDrag) ;
	void EndDrag() ;

	virtual SCODE QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState);
	virtual SCODE GiveFeedback(DROPEFFECT dropEffect);

protected:
	CSize m_szOffset ;

	CDC * m_pdcImage ;
	CBitmap * m_pOldImageBmp ;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


