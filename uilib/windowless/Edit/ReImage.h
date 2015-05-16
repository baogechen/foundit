//////////////////////////////////////////////////////////////////////////
//
//	����:   ��Ƕ��RichEdit��ͼ�������
//
//	
//	����:   2011/11/23
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "common/utils/Image2/Image2.h"

#include "ReObject.h"

class ImageReDelegate ;

class ReImage : public ReObject
{
public:
    ReImage() ;
    virtual ~ReImage() ;

public:

    void InitFromImageFile(std::wstring const &path, UINT space_width = (UINT)-1) ;
    bool IsGif() const ;

    // ����ͼƬ��ʾ�ռ��ȼ���ͼƬ�����С
    bool CalcSizeBySpaceWidth(UINT space_width) ;

    // ����ͼƬ��ʾ�ռ����Զ�����ͼƬ�����С
    // gif�����е���
    void AutoAdjustSizeBySpaceWidth(UINT space_width) ;

    // ���ԭʼͼƬ·��
    void GetOriginFilePath(std::wstring & path) ;

public:
    //////////////////////////////////////////////////////////////////////////
    // Internal methods
    // ��tomEdit��ImageDelegateʹ��

    // �����Ƴ��˱༭��Ŀ�������
    virtual void MoveOutOfVisibleArea() ;

    // ���������˱༭��Ŀ�������
    virtual void MoveIntoVisibleArea() ;

    // ����ӿ��ṩ����������������ػ�
    bool Redraw() ;

public:

    //////////////////////////////////////////////////////////////////////////
    // ReObject methods
    // ����
    virtual void Draw(HDC hdcDraw, RECT const &draw_bound) ;

    //////////////////////////////////////////////////////////////////////////
    //IOleObject methods
    virtual HRESULT __stdcall Close( DWORD dwSaveOption );

    //////////////////////////////////////////////////////////////////////////
    //IDataObject methods
    virtual HRESULT __stdcall GetData( FORMATETC *pFormatetcIn, STGMEDIUM *pMedium ) ;
    virtual HRESULT __stdcall EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc ) ;

    //////////////////////////////////////////////////////////////////////////
    // IClipboardDataObject methods

    // ת���ɼ��а�����
    virtual bool QueryTextStreamClipboardData(
        Clipboard::Format format,
        std::wstring &data
        ) ;

protected:
    ImageReDelegate * m_image ;
    std::wstring m_image_file_origin_path ; // ͼ���ļ�ԭʼ·��
    UINT m_space_width ;        // ͼƬ���ڵĿռ���
};

class ImageReDelegate : public GifCallback
{
public:
    ImageReDelegate(
        IN const WCHAR* filename,
        IN BOOL useEmbeddedColorManagement = FALSE
        ) ;

    void SetReImage(ReImage * reimage) ;
    void SetBound(int x, int y, int width, int height) ;
    void GetBound(RECT &bound) const ;
    void Draw(HDC dc, RECT const &update_bound) ;

    //////////////////////////////////////////////////////////////////////////
    // Delegate Image2 Method
    bool IsGif() const ;
    void Play() ;
    void Pause() ;
    void Continue() ;
    void Stop() ;
    UINT GetWidth() ;
    UINT GetHeight() ;
    Status GetLastStatus() const ;

    operator Gdiplus::Image * () ;

protected:
    //////////////////////////////////////////////////////////////////////////
    // GifCallback Implement
    virtual bool Update() ;

private:
    Image2 m_image ;

    ReImage * m_object ;
    RECT m_bound ;      // �����������ǲ���host�Ŀͻ�������ϵ

    std::vector<RECT> m_bound_group ;

};
