#include "stdafx.h"
#include <tom.h>

#include "tomEditCallback.h"
#include "tomEdit.h"
#include "ReObject.h"
#include "EnumFormatEtcImpl.h"
#include "utils.h"
#include "common/utils/Graphics.h"
#include "common/MemoryLeakChecker.h"
//////////////////////////////////////////////////////////////////////////
// tomEditCallback

tomEditCallback::tomEditCallback(tomEdit * edit) 
: m_ref_count(1), m_edit(edit)
{

}

tomEditCallback::~tomEditCallback() 
{

}

HRESULT tomEditCallback::QueryInterface(THIS_ REFIID riid, LPVOID FAR * lplpObj) 
{
    *lplpObj = NULL ;
    HRESULT hr = E_NOINTERFACE ;

    if (IsEqualIID(IID_IUnknown, riid))
    {
        *lplpObj = (IUnknown*)(IRichEditOleCallback*)this ;
    }
    else if (IsEqualIID(IID_IRichEditOleCallback, riid))
    {
        *lplpObj = (IRichEditOleCallback*)this ;
    }
    else if (IsEqualIID(IID_IDataObject, riid))
    {
        *lplpObj = (IDataObject*)this ;
    }

    if (NULL != *lplpObj)
    {
        AddRef() ;
        hr = S_OK ;
    }
    return hr ;
}

ULONG tomEditCallback::AddRef()
{
    return ++ m_ref_count ;
}

ULONG tomEditCallback::Release()
{
    ULONG result = -- m_ref_count ;

    if (m_ref_count == 0)
    {
        delete this ;
    }

    return result ;
}

HRESULT tomEditCallback::GetNewStorage(THIS_ LPSTORAGE FAR * lplpstg) 
{
    return S_OK ;
}

HRESULT tomEditCallback::GetInPlaceContext(
    THIS_ LPOLEINPLACEFRAME FAR * lplpFrame,
    LPOLEINPLACEUIWINDOW FAR * lplpDoc,
    LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
    return S_OK ;
}

HRESULT tomEditCallback::ShowContainerUI(THIS_ BOOL fShow) 
{
    return S_OK ;
}

HRESULT tomEditCallback::QueryInsertObject(THIS_ LPCLSID lpclsid, LPSTORAGE lpstg,
                                           LONG cp) 
{
    return S_OK ;
}

HRESULT tomEditCallback::DeleteObject(THIS_ LPOLEOBJECT lpoleobj) 
{
    // Ŀǰ��û�������ɾ���������������
    m_edit->RemoveObject(lpoleobj) ;
    return S_OK ;
}

HRESULT tomEditCallback::QueryAcceptData(THIS_ LPDATAOBJECT lpdataobj,
                                         CLIPFORMAT FAR * lpcfFormat, DWORD reco,
                                         BOOL fReally, HGLOBAL hMetaPict) 
{
    // �ȿ��Ƿ���CF_HTML��ʽ
    FORMATETC html_fetc = {CF_HTML, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL} ;
    if (S_OK == lpdataobj->QueryGetData(&html_fetc))
    {
        // ���д���
        if (fReally)    // fReallyΪTRUE��ʾճ��������ֻ�ǲ���
        {
            STGMEDIUM medium = {TYMED_HGLOBAL, NULL, NULL} ;
            if (S_OK == lpdataobj->GetData(&html_fetc, &medium))
            {
                std::list<Clipboard::HtmlParser::Element> element_group ;
                if (Clipboard::HtmlParser::ParseFragment(medium.hGlobal, element_group))
                {
                    // ��ʼ�����ı��Ͷ���
                    std::list<Clipboard::HtmlParser::Element>::iterator pos ;
                    for (pos = element_group.begin() ; pos != element_group.end() ; ++ pos)
                    {
                        if (pos->type == Clipboard::Ele_Text)
                        {
                            m_edit->ReplaceSel(pos->data.c_str(), TRUE) ;
                        }
                        else if (pos->type == Clipboard::Ele_Image)
                        {
                            m_edit->InsertImageObject(REO_CP_SELECTION, pos->data) ;
                        }
                    }
                }
                //ReleaseStgMedium(&medium) ;
            }
        }
        return S_FALSE ;
    }

    // �ٿ��Ƿ���CF_DIB��ʽ
    FORMATETC dib_fetc = {CF_DIB, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL} ;
    if (S_OK == lpdataobj->QueryGetData(&dib_fetc))
    {
        // ���д���
        if (fReally)    // fReallyΪTRUE��ʾճ��������ֻ�ǲ���
        {
            STGMEDIUM medium = {TYMED_HGLOBAL, NULL, NULL} ;
            if (S_OK == lpdataobj->GetData(&dib_fetc, &medium))
            {
                CString file_path ;
                file_path.Format(_T("C:\\%d.bmp"), GetTickCount()) ;
                hdutils::SaveDIB2BmpFile(medium.hGlobal, file_path) ;
                m_edit->InsertImageObject(REO_CP_SELECTION, LPCTSTR(file_path)) ;
            }
        }
        return S_FALSE ;
    }

    // �ٿ��Ƿ���CF_UNICODETEXT��ʽ
    FORMATETC unicodetext_fetc = {CF_UNICODETEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL} ;
    if (S_OK == lpdataobj->QueryGetData(&unicodetext_fetc))
    {
        // ���д���
        if (fReally)    // fReallyΪTRUE��ʾճ��������ֻ�ǲ���
        {
            STGMEDIUM medium = {TYMED_HGLOBAL, NULL, NULL} ;
            if (S_OK == lpdataobj->GetData(&unicodetext_fetc, &medium))
            {
                wchar_t * text = (wchar_t*)::GlobalLock(medium.hGlobal) ;
                m_edit->ReplaceSel(text, TRUE) ;
                ::GlobalUnlock(medium.hGlobal) ;

                //ReleaseStgMedium(&medium) ;
            }
        }
        return S_FALSE ;
    }

    // ��ϵͳ����
    return S_OK ;
}

HRESULT tomEditCallback::ContextSensitiveHelp(THIS_ BOOL fEnterMode) 
{
    return S_OK ;
}

HRESULT tomEditCallback::GetClipboardData(THIS_ CHARRANGE FAR * lpchrg, DWORD reco,
                                          LPDATAOBJECT FAR * lplpdataobj) 
{
    // û��ѡ���κζ���
    if (lpchrg->cpMin == lpchrg->cpMax)
        return E_NOTIMPL ;

    // ��õ�ǰѡ���������ͣ����ı�������ͼ�����ͼ�Ļ��
    WORD selection_type = m_edit->GetSelectionType() ;

    if ((selection_type & SEL_TEXT) && !(selection_type & SEL_OBJECT))
    {
        // ���ı���ִ��ȱʡ��������
        // ��ȱʡ�����Ḵ�Ƹ��ı��ĸ�ʽ����ɫ�ȣ����Ի������д���
        return E_NOTIMPL ;
    }
    else if (selection_type == SEL_OBJECT)
    {
        // ����ͼ�����Ҫ�ṩCF_BITMAP/CF_DIB��CF_HTML��CF_HDROP���ڲ���׼��ʽ
        // ���ͼ�����
        REOBJECT reo ;
        reo.cbStruct = sizeof(reo) ;
        reo.cp = lpchrg->cpMin ;
        if (S_OK == m_edit->GetObject(REO_IOB_USE_CP, &reo, REO_GETOBJ_POLEOBJ))
        {
            *lplpdataobj = NULL ;
            reo.poleobj->QueryInterface(IID_IDataObject, (void**)lplpdataobj) ;
            reo.poleobj->Release() ;
            if (*lplpdataobj != NULL)
                return S_OK ;
        }
    }
    else if ((selection_type & (SEL_OBJECT)) && (selection_type & SEL_TEXT))
    {
        // ͼ�Ļ��
        if (PrepareClipboardData() && S_OK == QueryInterface(IID_IDataObject, (void**)lplpdataobj))
        {
            if (*lplpdataobj != NULL)
                return S_OK ;
        }
    }

    return E_NOTIMPL ;
}

HRESULT tomEditCallback::GetDragDropEffect(THIS_ BOOL fDrag, DWORD grfKeyState,
                                           LPDWORD pdwEffect) 
{
    //*pdwEffect = DROPEFFECT_COPY ;
    return S_OK ;
}

HRESULT tomEditCallback::GetContextMenu(THIS_ WORD seltype, LPOLEOBJECT lpoleobj,
                                        CHARRANGE FAR * lpchrg,
                                        HMENU FAR * lphmenu) 
{
    return S_OK ;
}

//////////////////////////////////////////////////////////////////////////
// IDataObject Interface

//similar to GetClipboardData
HRESULT tomEditCallback::GetData( FORMATETC *pFormatetcIn, STGMEDIUM *pMedium )
{
    switch (pFormatetcIn->cfFormat)
    {
    case CF_UNICODETEXT :
        {
            pMedium->tymed = TYMED_HGLOBAL ;
            pMedium->pUnkForRelease = NULL ;
            pMedium->hGlobal = m_text_packer.GenerateClipboardData() ;
            return S_OK ;
        }
        break ;

    default :
        if (pFormatetcIn->cfFormat == CF_HTML)
        {
            pMedium->tymed = TYMED_HGLOBAL ;
            pMedium->pUnkForRelease = NULL ;
            pMedium->hGlobal = m_html_packer.GenerateClipboardData() ;
            return S_OK ;
        }
    }

    return E_NOTIMPL ;
}

HRESULT tomEditCallback::GetDataHere( FORMATETC *pFormatetc, STGMEDIUM *pMedium )
{
    return S_OK;
}

//similar to IsClipboardFormatAvailable
HRESULT tomEditCallback::QueryGetData( FORMATETC *pFormatetc )
{
    if (pFormatetc->cfFormat == CF_TEXT
        || pFormatetc->cfFormat == CF_UNICODETEXT
        || pFormatetc->cfFormat == CF_HTML
        )
    {
        return S_OK ;
    }

    return DV_E_FORMATETC ;
}

//The word "canonical" means "the simplest form of something," 
//In general, the canonical FORMATETC should contain the most general information possible for a specific rendering.
//The simplest implementation of GetCanonicalFormatEtc copies the input structure to the output structure, 
//sets the output ptd field to NULL, and returns DATA_S_SAMEFORMATETC. 
//This says that the data object doesn't care about target devices for the specific FORMATETC. 
//If, on the other hand, the source does care, it returns NOERROR, filling the output FORMATETC as generally as it can.
HRESULT tomEditCallback::GetCanonicalFormatEtc( FORMATETC *pFormatetcIn, FORMATETC *pFormatetcOut )
{
    return S_OK ;
}

//similar to SetClipboardData
HRESULT tomEditCallback::SetData( FORMATETC *pFormatetc, STGMEDIUM *pMedium, BOOL fRelease )
{
    return DATA_E_FORMATETC ;
}

//similar to EnumClipboardFormats
HRESULT tomEditCallback::EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc )
{
    *ppEnumFormatEtc = NULL;
    if ( dwDirection == DATADIR_GET )
    {
        // ͼ�Ļ�϶���Ҫ�ṩCF_TEXT/CF_UNICODETEXT��CF_HTML���ڲ���׼��ʽ
        // �����ϸ�Ҫ��cfFormat�ֶκ�tymed�ֶε���ȷƥ��
        FORMATETC fetc[2] = 
        {
            {CF_UNICODETEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL}, 
            {CF_HTML, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL}, 
        } ;

        *ppEnumFormatEtc = new EnumFormatEtcImpl(2, fetc) ;

        if ( *ppEnumFormatEtc )
        {
            (*ppEnumFormatEtc)->AddRef();
        }
    }
    return S_OK;
}

HRESULT tomEditCallback::DAdvise( FORMATETC *pFormatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection )
{
    return S_OK ;
}

HRESULT tomEditCallback::DUnadvise( DWORD dwConnection )
{
    return S_OK ;
}

HRESULT tomEditCallback::EnumDAdvise( IEnumSTATDATA **ppEnumAdvise )
{
    return S_OK ;
}

//////////////////////////////////////////////////////////////////////////
// Internal Methods

bool tomEditCallback::PrepareClipboardData() 
{
    m_text_packer.Clear() ;
    m_html_packer.Clear() ;

    bool res = false ;

    // ��õ�ǰѡ��ͼ��
    IRichEditOle * edit_ole = m_edit->GetIRichEditOle() ;
    if (edit_ole != NULL)
    {
        ITextDocument * doc = NULL ;//IID_ITextDocument //__uuidof(ITextDocument)
        GUID guid = __uuidof(ITextDocument) ;
        if (S_OK == edit_ole->QueryInterface(__uuidof(ITextDocument), (void**)&doc) && doc != NULL)    // ��֪�����ж�һ��!= NULL�ǲ����ѿ��ӷ�ƨ
        {
            ITextSelection * sel = NULL ;
            if (S_OK == doc->GetSelection(&sel) && sel != NULL)
            {
                //BSTR * sel_text ;
                long sel_start = 0 ;
                sel->GetStart(&sel_start) ;

                BSTR sel_text ;
                if (S_OK == sel->GetText(&sel_text))    // �����Ҫ�����ͷ�sel_text
                {
                    // ����
                    long i ;
                    std::wstring text_temp ;
                    for (i = 0 ; sel_text[i] != 0 ; ++ i)
                    {
                        if (sel_text[i] == WCH_EMBEDDING)
                        {
                            if (!text_temp.empty())
                            {
                                m_text_packer.Append(Clipboard::Ele_Text, text_temp) ;
                                m_html_packer.Append(Clipboard::Ele_Text, text_temp) ;
                                text_temp.clear() ;
                            }

                            // ��ö���
                            REOBJECT ro ;
                            ro.cbStruct = sizeof(ro) ;
                            ro.cp = sel_start + i ;
                            if (S_OK == edit_ole->GetObject(REO_IOB_USE_CP, &ro, REO_GETOBJ_POLEOBJ))
                            {
                                IClipboardDataObject * cdo = NULL ;
                                if (S_OK == ro.poleobj->QueryInterface(IID_IClipboardDataObject, (void**)&cdo))
                                {
                                    std::wstring text_data ;
                                    if (cdo->QueryTextStreamClipboardData(Clipboard::Text, text_data))
                                    {
                                        m_text_packer.Append(Clipboard::Ele_Image, text_data) ;
                                    }
                                    std::wstring html_data ;
                                    if (cdo->QueryTextStreamClipboardData(Clipboard::Html, html_data))
                                    {
                                        m_html_packer.Append(Clipboard::Ele_Image, html_data) ;
                                    }

                                    cdo->Release() ;
                                }

                                ro.poleobj->Release() ;
                            }
                        }
                        else
                        {
                            text_temp.append(1, (wchar_t)(sel_text[i])) ;
                        }
                    }

                    if (!text_temp.empty())
                    {
                        m_text_packer.Append(Clipboard::Ele_Text, text_temp) ;
                        m_html_packer.Append(Clipboard::Ele_Text, text_temp) ;
                        text_temp.clear() ;
                    }

                    SysFreeString(sel_text) ;
                    res = true ;
                }

                sel->Release() ;
            }

            doc->Release() ;
        }
    }

    return res ;
}
