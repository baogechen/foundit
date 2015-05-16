//////////////////////////////////////////////////////////////////////////
//
//	����:   ��Ƕ��RichEdit�Ķ��������
//
//	
//	����:   2011/11/07
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "RichTextClipboardData.h"

// {FE319284-DC57-4ef5-AC63-EE2007AD3282}
GUID const IID_IClipboardDataObject = 
{ 0xfe319284, 0xdc57, 0x4ef5, { 0xac, 0x63, 0xee, 0x20, 0x7, 0xad, 0x32, 0x82 } };

class IClipboardDataObject : public IUnknown
{
public:
    // ת�����ı���ʽ�ļ��а�����
    virtual bool QueryTextStreamClipboardData(
        Clipboard::Format format,
        std::wstring &data
        ) = 0 ;
};

GUID const CLSID_ReObject = 
{0x2ed4dae8, 0x1944, 0x42be, {0xbd, 0xc3, 0xdc, 0x11, 0xfb, 0x25, 0xa0, 0xc9} } ;

// IOleObject�������ڵ�һλ��ʡȥQueryInterface�鷳������������ɽկ�����˴���
class ReObject : 
    public IOleObject			//Functions for OLE Documents-related functions, related to IOleClientSite. 
    , public IDataObject		//Functions for obtaining renderings from the object. 
    , public IPersistStorage	//Functions for communicating storage information to the object. 
    , public IViewObjectEx		//Functions for asking the object to draw itself. 
    , public IOleInPlaceActiveObject
    , public IOleInPlaceObjectWindowless
    , public IClipboardDataObject
{
public:
    ReObject() ;
    virtual ~ReObject() ;

    void SetHost(HWND host) ;

    // tomEdit���ƿ�ʼʱ���øýӿڣ�ReObjectͨ��update_bound�ж��Լ��Ƿ���Ҫ����
    // ��Ҫ��������ʾ���m_been_drawn_flagΪfalse
    void ResetHide(RECT const &update_bound) ;

    // �����ʾ��ǣ����Ϊfalse��ֹͣ��ʱ��
    void CheckShowOrHide() ;

    bool IsShowInEditJustByBound() ;

    //////////////////////////////////////////////////////////////////////////
    // ReObject virtual methods
protected:
    // �����Ƴ��˱༭��Ŀ�������
    virtual void MoveOutOfVisibleArea() ;

    // ���������˱༭��Ŀ�������
    virtual void MoveIntoVisibleArea() ;

    // ����
    // ReObject������ֻ����ʵ�ָû��ƽӿڼ��ɣ�һ������²�Ҫʵ��IViewObject::Draw�ӿ�
    virtual void Draw(HDC hdcDraw, RECT const &draw_bound) ;

public:
    //////////////////////////////////////////////////////////////////////////
    //IUnknown methods
    virtual HRESULT __stdcall QueryInterface( const IID &riid, void **ppvObject );
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    //////////////////////////////////////////////////////////////////////////
    //IOleObject methods
    virtual HRESULT __stdcall SetClientSite( IOleClientSite *pClientSite );
    virtual HRESULT __stdcall GetClientSite( IOleClientSite **ppClientSite );
    virtual HRESULT __stdcall SetHostNames( LPCOLESTR szContainerApp, LPCOLESTR szContainerObj );
    virtual HRESULT __stdcall Close( DWORD dwSaveOption );
    virtual HRESULT __stdcall SetMoniker( DWORD dwWhichMoniker, IMoniker *pmk );
    virtual HRESULT __stdcall GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk );
    virtual HRESULT __stdcall InitFromData( IDataObject *pDataObject, BOOL fCreation, DWORD dwReserved );
    virtual HRESULT __stdcall GetClipboardData( DWORD dwReserved, IDataObject **ppDataObject );
    virtual HRESULT __stdcall DoVerb( LONG iVerb, 
        LPMSG lpMsg, 
        IOleClientSite *pActiveSite, 
        LONG lIndex, 
        HWND hWndParent, 
        LPCRECT lprcPosRect );
    virtual HRESULT __stdcall EnumVerbs( IEnumOLEVERB **ppEnumOleVerb );
    virtual HRESULT __stdcall Update();
    virtual HRESULT __stdcall IsUpToDate();
    virtual HRESULT __stdcall GetUserClassID( CLSID *pClsid );
    virtual HRESULT __stdcall GetUserType( DWORD dwFormOfType, LPOLESTR *pszUserType );
    virtual HRESULT __stdcall SetExtent( DWORD dwDrawAspect, SIZEL *psizel );
    virtual HRESULT __stdcall GetExtent( DWORD dwDrawAspect, SIZEL *psizel );
    virtual HRESULT __stdcall Advise( IAdviseSink *pAdvSink, DWORD *pdwConnection );
    virtual HRESULT __stdcall Unadvise( DWORD dwConnection );
    virtual HRESULT __stdcall EnumAdvise( IEnumSTATDATA **ppEnumAdvise );
    virtual HRESULT __stdcall GetMiscStatus( DWORD dwAspect, DWORD *pdwStatus );
    virtual HRESULT __stdcall SetColorScheme( LOGPALETTE *pLogpal );

    //////////////////////////////////////////////////////////////////////////
    //IDataObject methods
    //similar to GetClipboardData
    virtual HRESULT __stdcall GetData( FORMATETC *pFormatetcIn, STGMEDIUM *pMedium );
    virtual HRESULT __stdcall GetDataHere( FORMATETC *pFormatetc, STGMEDIUM *pMedium );
    //similar to IsClipboardFormatAvailable
    virtual HRESULT __stdcall QueryGetData( FORMATETC *pFormatetc );
    //The word "canonical" means "the simplest form of something," 
    //In general, the canonical FORMATETC should contain the most general information possible for a specific rendering.
    //The simplest implementation of GetCanonicalFormatEtc copies the input structure to the output structure, 
    //sets the output ptd field to NULL, and returns DATA_S_SAMEFORMATETC. 
    //This says that the data object doesn't care about target devices for the specific FORMATETC. 
    //If, on the other hand, the source does care, it returns NOERROR, filling the output FORMATETC as generally as it can.
    virtual HRESULT __stdcall GetCanonicalFormatEtc( FORMATETC *pFormatetcIn, FORMATETC *pFormatetcOut );
    //similar to SetClipboardData
    virtual HRESULT __stdcall SetData( FORMATETC *pFormatetc, STGMEDIUM *pMedium, BOOL fRelease );
    //similar to EnumClipboardFormats
    virtual HRESULT __stdcall EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc );
    virtual HRESULT __stdcall DAdvise( FORMATETC *pFormatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection );
    virtual HRESULT __stdcall DUnadvise( DWORD dwConnection );
    virtual HRESULT __stdcall EnumDAdvise( IEnumSTATDATA **ppEnumAdvise );

    //////////////////////////////////////////////////////////////////////////
    //IPersistStorage methods;
    virtual HRESULT __stdcall GetClassID( CLSID *pClassID );
    virtual HRESULT __stdcall IsDirty();
    virtual HRESULT __stdcall InitNew( IStorage *pStg );
    virtual HRESULT __stdcall Load( IStorage *pStg );
    virtual HRESULT __stdcall Save( IStorage *pStg, BOOL fSameAsLoad );
    virtual HRESULT __stdcall SaveCompleted( IStorage *pStgNew );
    virtual HRESULT __stdcall HandsOffStorage();

    //////////////////////////////////////////////////////////////////////////
    //IViewObjectEx methods
    //1.IViewObject methods
    virtual HRESULT __stdcall Draw( DWORD dwDrawAspect, 
        LONG lIndex, 
        void *pvAspect, 
        DVTARGETDEVICE *ptd, 
        HDC hdcTargetDev,
        HDC hdcDraw, 
        LPCRECTL lprcBounds, 
        LPCRECTL lprcWBounds, 
        BOOL (__stdcall *pfnContinue)( ULONG_PTR ), 
        ULONG_PTR dwCotinue );
    virtual HRESULT __stdcall GetColorSet( DWORD dwDrawAspect,
        LONG lindex,
        void *pvAspect,
        DVTARGETDEVICE *ptd,
        HDC hicTargetDev,
        LOGPALETTE **ppColorSet );
    virtual HRESULT __stdcall Freeze( DWORD dwDrawAspect, LONG lIndex, void *pvAspect, DWORD *pdwFreeze );
    virtual HRESULT __stdcall Unfreeze( DWORD dwFreeze );
    virtual HRESULT __stdcall SetAdvise( DWORD aspects, DWORD advf, IAdviseSink *pAdvSink );
    virtual HRESULT __stdcall GetAdvise( DWORD *pAspects, DWORD *pAdvf, IAdviseSink **pAdvSink );
    //2.IViewObject2 methods
    virtual HRESULT __stdcall GetExtent(  DWORD dwAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel );
    //3.IViewObjectEx methods
    virtual HRESULT __stdcall GetRect( DWORD dwAspect, LPRECTL pRect );
    virtual HRESULT __stdcall GetViewStatus( DWORD *pdwStatus );
    virtual HRESULT __stdcall QueryHitPoint( DWORD dwAspect, 
        LPCRECT pRectBounds, 
        POINT ptlLoc, 
        LONG lCloseHint, 
        DWORD *pHitResult );
    virtual HRESULT __stdcall QueryHitRect( DWORD dwAspect,
        LPCRECT pRectBound,
        LPCRECT pRectLoc,
        LONG lCloseHint,
        DWORD *pHitResult );
    virtual HRESULT __stdcall GetNaturalExtent( DWORD dwAspect,
        LONG lIndex,
        DVTARGETDEVICE *ptd,
        HDC hicTargetDev,
        DVEXTENTINFO *pExtentInfo,
        LPSIZEL pSizel );

    //////////////////////////////////////////////////////////////////////////
    //IOleInPlaceActiveObject methods
    //1.IOleWindow methods
    virtual HRESULT __stdcall GetWindow( HWND *phWnd );
    virtual HRESULT __stdcall ContextSensitiveHelp( BOOL fEnterMode );
    //2.IOleInPlaceActiveObject methods
    virtual HRESULT __stdcall TranslateAccelerator( LPMSG lpMsg );
    virtual HRESULT __stdcall OnFrameWindowActivate( BOOL fActive );
    virtual HRESULT __stdcall OnDocWindowActivate( BOOL fActive );
    virtual HRESULT __stdcall ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fFrameWindow );
    virtual HRESULT __stdcall EnableModeless( BOOL fEnable );

    //////////////////////////////////////////////////////////////////////////
    //IOleInPlaceObjectWindowless methods
    //1.IOleInPlaceObject methods
    virtual HRESULT __stdcall InPlaceDeactivate();
    virtual HRESULT __stdcall UIDeactivate();
    virtual HRESULT __stdcall SetObjectRects( LPCRECT lprcPosRect, LPCRECT lprcClipRect );
    virtual HRESULT __stdcall ReactivateAndUndo();
    //2.IOleInPlaceObjectWindowless methods
    virtual HRESULT __stdcall OnWindowMessage( UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult );
    virtual HRESULT __stdcall GetDropTarget( IDropTarget **ppDropTarget );

    //////////////////////////////////////////////////////////////////////////
    // IClipboardDataObject methods

    // ת���ɼ��а�����
    virtual bool QueryTextStreamClipboardData(
        Clipboard::Format format,
        std::wstring &data
        ) ;

protected:
    static LONG s_x_pixel_per_inch ;
    static LONG s_y_pixel_per_inch ;

    ULONG			m_ref_count ;

    long            m_width_in_pixel ;
    long            m_height_in_pixel ;
    long			m_width_in_himetric ;	    // ����Ŀ�ȣ���λ��0.01����
    long			m_height_in_himetric ;	    // ����ĸ߶ȣ���λ��0.01����

    IOleClientSite * m_client_site ;
    IAdviseSink * m_advise_sink ;
    HWND m_host ;
    bool m_been_drawn_flag ;        // ����Ƿ񱻻���
    RECT m_draw_bound ;

};
