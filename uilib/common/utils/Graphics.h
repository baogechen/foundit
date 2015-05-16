//////////////////////////////////////////////////////////////////////////
//
//	����:   ͼ����ͨ�ú���
//
//	
//	����:   2011/02/25
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "uilib_exports.h"

namespace hdutils {

extern COLORREF crTextColor ;

UILIB_EXPORT COLORREF GetTextColor();

// �������������
UILIB_EXPORT void LineStretch(
				 HDC hdcDest,
				 int nXDest,
				 int nYDest,
				 int nWidthDest,
				 int nHeightDest,
				 Image *pImage,
				 int nXSrc,
				 int nYSrc,
				 int nWidthSrc,
				 int nHeightSrc,
				 const ImageAttributes* pImgAttr = NULL,
				 int nFixedFront = -1,	// ǰ�˹̶�����
				 int nFixedBack = -1,	// ��˹̶�����
				 BOOL bHorz = TRUE		// Ĭ��ˮƽ����
				 ) ;

// ���������
UILIB_EXPORT void SurfaceStretch(
					HDC hdcDest,
					int nXDest,
					int nYDest,
					int nWidthDest,
					int nHeightDest,
					Image *pImage,
					int nXSrc,
					int nYSrc,
					int nWidthSrc,
					int nHeightSrc,
					const ImageAttributes *imageAttributes = NULL,
					int nFixedLeft = -1,	// ��˹̶�����
					int nFixedRight = -1,	// �Ҷ˹̶�����
					int nFixedTop = -1,		// �϶˹̶�����
					int nFixedBottom = -1	// �¶˹̶�����
					) ;

// ��ƽ��
UILIB_EXPORT void LineTile(
			  HDC hdcDest,
			  int nXDest,
			  int nYDest,
			  int nWidthDest,
			  int nHeightDest,
			  HDC hdcSrc,
			  int nXSrc,
			  int nYSrc,
			  int nWidthSrc,
			  int nHeightSrc,
			  int nFixedFront = -1,	// ǰ�˹̶�����
			  int nFixedBack = -1,	// ��˹̶�����
			  BOOL bHorz = TRUE		// Ĭ��ˮƽƽ��
			  ) ;

// ��ƽ�̣���Alphaͨ��
UILIB_EXPORT void AlphaLineTile(
				   HDC hdcDest,
				   int nXDest,
				   int nYDest,
				   int nWidthDest,
				   int nHeightDest,
				   HDC hdcSrc,
				   int nXSrc,
				   int nYSrc,
				   int nWidthSrc,
				   int nHeightSrc,
				   int nFixedFront = -1,	// ǰ�˹̶�����
				   int nFixedBack = -1,		// ��˹̶�����
				   BOOL bHorz = TRUE		// Ĭ��ˮƽƽ��
				   ) ;

// ��ƽ�̣���Alphaͨ��
UILIB_EXPORT void AlphaSurfaceTile(
					  HDC hdcDest,
					  int nXDest,
					  int nYDest,
					  int nWidthDest,
					  int nHeightDest,
					  HDC hdcSrc,
					  int nXSrc,
					  int nYSrc,
					  int nWidthSrc,
					  int nHeightSrc,
					  BOOL bBorderMode = FALSE,		// �Ƿ�ֻ���Ʊ߿�
					  int nFixedLeft = -1,			// ��˹̶�����
					  int nFixedRight = -1,			// �Ҷ˹̶�����
					  int nFixedTop = -1,			// �϶˹̶�����
					  int nFixedBottom = -1			// �¶˹̶�����
					  ) ;

// ģ����λͼ
UILIB_EXPORT void Blur(HBITMAP hBitmap) ;

// ����32λ�豸�޹�λͼ
UILIB_EXPORT HBITMAP Create32Bitmap(int nWidth, int nHeight) ;

// �����������ݵ�32λλͼ
UILIB_EXPORT HBITMAP CreateCompatible32Bitmap(HDC hDC, int nWidth, int nHeight) ;

// ��Imageת����λͼ
UILIB_EXPORT HBITMAP ConvertImage2Bitmap(HDC hDC, Image * pImage) ;

// ��Imageת���ɻ���DC
// BOOL ConvertImage2CanvasDC(CCanvasDC &dcDest, HDC hDC, Image * pImage) ;

// �ޱ��λ���ͼƬ
UILIB_EXPORT void DrawImageIndeformable(
						   Graphics &gph,
						   Image * pImage,
						   CRect const &rcDest,
                           ImageAttributes * ia = NULL
						   ) ;
UILIB_EXPORT void DrawImageIndeformable(
						   Graphics &gph,
						   Image * pImage,
						   CRect const &rcDest,
                           int nLeft, int nTop, int nWidth, int nHeight,
                           ImageAttributes * ia = NULL
						   ) ;
//���ƴ�����Ч��������
UILIB_EXPORT int DrawText( HDC hdc,
			 LPCTSTR lpchText,
			 int cchText,
			 LPRECT lprc,
			 UINT format,
			 COLORREF shadowColor
			 );

// ʹ32λλͼ��͸����
UILIB_EXPORT BOOL Opaque32Bitmap(HBITMAP hBmp, COLORREF crInit = RGB(0, 0, 0)) ;

// ��ñ���ƽ���Ҷ�
UILIB_EXPORT BYTE GetRoughGrayScale(HDC hdc, LPRECT lprc) ;

// ���Ʒ������֣����Զ����ݱ����Ҷ�
UILIB_EXPORT void DrawGlowText(HDC hdc, LPCTSTR lpszText, int cchText, LPRECT lprc, UINT format) ;

// ���Ʒ�������
UILIB_EXPORT void DrawGlowText(HDC hdc, LPCTSTR lpszText, int cchText, LPRECT lprc, UINT format, COLORREF crHalo, COLORREF crBkgnd) ;

UILIB_EXPORT BOOL RenderBitmap(HBITMAP bmp, COLORREF color);

UILIB_EXPORT BOOL SaveDIB2BmpFile(HGLOBAL dib, LPCTSTR file_path) ;

UILIB_EXPORT HGLOBAL ConvertDDB2DIB(HBITMAP ddb) ;

} ;
