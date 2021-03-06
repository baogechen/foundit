// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

#ifndef __mshtml_h__
#include <mshtml.h>
#endif

#ifndef __MSHTMDID_H__
#include <mshtmdid.h>
#endif

#ifndef __mshtmhst_h__
#include <mshtmhst.h>
#endif

#ifndef __ATLBASE_H__
#include <atlbase.h>
#endif

#ifndef __exdisp_h__
#include <exdisp.h>
#endif

#include <atlbase.h>
#include <atlcom.h>

#include <afxpriv2.h>
#include <afxocc.h>
#include <afxtempl.h>
#include <float.h>
#include <afxdhtml.h>

#include "DropTarget.h"
#include "json50/json.h"

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif 

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif


// IDispatch
class CDHtmlControlSink;

// IDispatch
class CDHtmlElementEventSink;




namespace hdui
{

// CObject
// CCmdTarget
// COleControlSite
class CBrowserControlSite1;


// CObject
// CCmdTarget
// CWnd
class CDHtmlWnd;


/////////////////////////////////////////////////////////////////////////////
// CBrowserControlSite class - Implements the control site for hosting
// the browser.  Used by CDHtmlWnd
class CBrowserControlSite1 : public COleControlSite,
	public IDocHostUIHandler
{
public:
	CBrowserControlSite1(COleControlContainer* pCtrlCont, CDHtmlWnd *pHandler);

protected:
	// Implementation
	LPUNKNOWN GetInterfaceHook(const void *iid);
	// IUnknown methods
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	// IDocHostUIHandler methods
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo);
	STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
	STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)(LPOLESTR *pchKey, DWORD dw);
	STDMETHOD(GetDropTarget)(IDropTarget *pDropTarget, IDropTarget **ppDropTarget);
	STDMETHOD(GetExternal)(IDispatch **ppDispatch);
	STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut);
	STDMETHOD(FilterDataObject)(IDataObject *pDO, IDataObject **ppDORet);

protected:
	CDHtmlWnd *m_pHandler;
};

/////////////////////////////////////////////////////////////////////////////
// CDHtmlWnd class - Implements a CDialog that hosts the WebBrowser 
// control and allows for handling events generated by HTML elements

#define WM_DOCUMENT_COMPLETE	WM_USER + 1100

class CDHtmlWnd : public CWnd,
	public CDHtmlEventSink
{
public:
	CString m_strCurrentUrl;
	LPTSTR m_szHtmlResID;
	UINT m_nHtmlResID;
	::ATL::CComPtr<IWebBrowser2> m_pBrowserApp;
	::ATL::CComPtr<IHTMLDocument2> m_spHtmlDoc;
	BOOL m_bUseHtmlTitle;

	CDHtmlWnd();
	virtual ~CDHtmlWnd();
	virtual BOOL CreateControlSite(COleControlContainer* pContainer, 
		COleControlSite** ppSite, UINT /* nID */, REFCLSID /* clsid */);
	HRESULT GetDHtmlDocument(IHTMLDocument2 **pphtmlDoc);
	void GetCurrentUrl(CString& szUrl);
	virtual BOOL CanAccessExternal();
	virtual void OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	BOOL Create(
		DWORD dwExStyle,
		DWORD dwStyle,
		LPCTSTR lpszWindowName,
		int x, int y, int cx, int cy,
		HWND hParent,
		HMENU nIDorHMenu
		) ;
	BOOL LoadFromResource(LPCTSTR lpszResource);
	BOOL LoadFromResource(UINT nRes);
	void Navigate(LPCTSTR lpszURL, DWORD dwFlags= 0, LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0);
	void DestroyModeless();

	bool CallJScript(const CString strFunc, const CStringArray& paramArray,CComVariant* pVarResult=NULL);
	bool CallJScript(const CString strFunc,const CString strArg1,CComVariant* pVarResult=NULL);
	bool CallJScript(const CString strFunc,CComVariant* pVarResult=NULL);
	bool GetJScript(CComPtr<IDispatch>& spDisp);

	// DDX functions
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, CString& value);
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, short& value);
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, int& value);
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, long& value);
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, DWORD& value);
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, float& value);
	void DDX_DHtml_ElementText(CDataExchange* pDX, LPCTSTR szId, DISPID dispid, double& value);
	void DDX_DHtml_CheckBox(CDataExchange* pDX, LPCTSTR szId, int& value);
	void DDX_DHtml_Radio(CDataExchange* pDX, LPCTSTR szId, long& value);
	void DDX_DHtml_SelectValue(CDataExchange* pDX, LPCTSTR szId, CString& value);
	void DDX_DHtml_SelectString(CDataExchange* pDX, LPCTSTR szId, CString& value);
	void DDX_DHtml_SelectIndex(CDataExchange* pDX, LPCTSTR szId, long& value);
	void DDX_DHtml_AxControl(CDataExchange *pDX, LPCTSTR szId, DISPID dispid, VARIANT& var);
	void DDX_DHtml_AxControl(CDataExchange *pDX, LPCTSTR szId, LPCTSTR szPropName, VARIANT& var);

	void ShowScroll(bool show);
	// host handler
	void SetHostFlags(DWORD dwFlags);
	void SetExternalDispatch(IDispatch *pdispExternal);
	virtual BOOL IsExternalDispatchSafe();
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo);
	STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
	STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)(LPOLESTR *pchKey, DWORD dw);
	STDMETHOD(GetDropTarget)(IDropTarget *pDropTarget, IDropTarget **ppDropTarget);
	STDMETHOD(GetExternal)(IDispatch **ppDispatch);
	STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut);
	STDMETHOD(FilterDataObject)(IDataObject *pDO, IDataObject **ppDORet);

	HRESULT GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection = NULL);
	HRESULT GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement);
	template <class Q>
	HRESULT GetElementInterface(LPCTSTR szElementId, Q** ppvObj)
	{
		return GetElementInterface(szElementId, __uuidof(Q), (void **) ppvObj);
	}
	HRESULT GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj);
	BSTR GetElementText(LPCTSTR szElementId);
	void SetElementText(LPCTSTR szElementId, BSTR bstrText);
	void SetElementText(IUnknown *punkElem, BSTR bstrText);
	BSTR GetElementHtml(LPCTSTR szElementId);
	void SetElementHtml(LPCTSTR szElementId, BSTR bstrText);
	void SetElementHtml(IUnknown *punkElem, BSTR bstrText);
	VARIANT GetElementProperty(LPCTSTR szElementId, DISPID dispid);
	void SetElementProperty(LPCTSTR szElementId, DISPID dispid, VARIANT *pVar);
	HRESULT GetControlDispatch(LPCTSTR szId, IDispatch **ppdisp);
	VARIANT GetControlProperty(IDispatch *pdispControl, DISPID dispid);
	VARIANT GetControlProperty(LPCTSTR szId, DISPID dispid);
	VARIANT GetControlProperty(LPCTSTR szId, LPCTSTR szPropName);
	void SetControlProperty(IDispatch *pdispControl, DISPID dispid, VARIANT *pVar);
	void SetControlProperty(LPCTSTR szElementId, DISPID dispid, VARIANT *pVar);
	void SetControlProperty(LPCTSTR szElementId, LPCTSTR szPropName, VARIANT *pVar);
	HRESULT GetEvent(IHTMLEventObj **ppEventObj);

	//Implementation
public:
	HRESULT OnDocumentReadyStateChange(IHTMLElement *phtmlElem);

protected:
	class FilesDropTarget : public DropTarget
	{
	public:
		explicit FilesDropTarget(CDHtmlWnd* pHtml) { m_pHtml = pHtml; }
		virtual ~FilesDropTarget() {};
		virtual DWORD OnDragEnter(IDataObject* data_object,
			DWORD key_state,
			POINT cursor_position,
			DWORD effect);
		virtual DWORD OnDragOver(IDataObject* data_object,
			DWORD key_state,
			POINT cursor_position,
			DWORD effect);
		virtual void OnDragLeave(IDataObject* data_object);
		virtual DWORD OnDrop(IDataObject* data_object,
			DWORD key_state,
			POINT cursor_position,
			DWORD effect);
		void OnDropFiles(HDROP hDropInfo, Json::Value& files);

	protected:
		CDHtmlWnd* m_pHtml;
	};

protected:
	BOOL m_bAttachedControl;
	CString m_strDlgCaption;
	CWnd m_wndBrowser;

	static UINT WM_DESTROYMODELESS;

	DECLARE_DYNAMIC(CDHtmlWnd)
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Initialize();
	void _OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	void _OnNavigateComplete2(LPDISPATCH pDisp, VARIANT FAR* URL);
	void _OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	LRESULT OnDestroyModeless(WPARAM, LPARAM);

	// DDX Functions
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, CString& value, BOOL bSave);
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, short& value, BOOL bSave);
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, int& value, BOOL bSave);
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, long& value, BOOL bSave);
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, DWORD& value, BOOL bSave);
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, float& value, BOOL bSave);
	void DDX_DHtml_ElementText(LPCTSTR szId, DISPID dispid, double& value, BOOL bSave);

	/////////////////////////////////////////////////////////////////////////////
	// Data exchange for special controls
	void DDX_DHtml_CheckBox(LPCTSTR szId, int& value, BOOL bSave);
	void DDX_DHtml_Radio(LPCTSTR szId, long& value, BOOL bSave);
	void DDX_DHtml_SelectValue(LPCTSTR szId, CString& value, BOOL bSave);
	void DDX_DHtml_SelectString(LPCTSTR szId, CString& value, BOOL bSave);
	void DDX_DHtml_SelectIndex(LPCTSTR szId, long& value, BOOL bSave);
	void DDX_DHtml_AxControl(LPCTSTR szId, DISPID dispid, VARIANT& var, BOOL bSave);
	void DDX_DHtml_AxControl(LPCTSTR szId, LPCTSTR szPropName, VARIANT& var, BOOL bSave);
	void OnDDXError(LPCTSTR szId, UINT nIDPrompt, BOOL bSave);

	void __cdecl DDX_DHtml_ElementTextWithFormat(LPCTSTR szId,
		DISPID dispid, LPCTSTR lpszFormat, UINT nIDPrompt, BOOL bSave, ...);
	void DDX_DHtml_ElementTextFloatFormat(LPCTSTR szId,
		DISPID dispid, void* pData, double value, int nSizeGcvt, BOOL bSave);
	BOOL _AfxSimpleScanf(LPCTSTR lpszText,
		LPCTSTR lpszFormat, va_list pData);
	BOOL _AfxSimpleFloatParse(LPCTSTR lpszText, double& d);
	long Select_FindString(IHTMLSelectElement *pSelect, BSTR bstr, BOOL fExact);
	void SetFocusToElement(LPCTSTR szId);

	// event sink
	DWORD m_dwDHtmlEventSinkCookie;
	::ATL::CSimpleArray<CDHtmlControlSink *> m_ControlSinks;
	::ATL::CSimpleArray<CDHtmlElementEventSink *> m_SinkedElements;

	virtual const DHtmlEventMapEntry* GetDHtmlEventMap();

	HRESULT ConnectDHtmlEvents(IUnknown *punkDoc);
	void DisconnectDHtmlEvents();
	HRESULT ConnectDHtmlElementEvents(DWORD_PTR dwThunkOffset = 0);
	BOOL FindSinkForObject(LPCTSTR szName);
	BOOL IsSinkedElement(IDispatch *pdispElem);
	void DisconnectDHtmlElementEvents();

	// host handler
	DWORD m_dwHostFlags;
	::ATL::CComPtr<IDispatch> m_spExternalDisp;
	scoped_refptr<FilesDropTarget> m_pFilesDropTarget;
};

UINT __declspec(selectany) CDHtmlWnd::WM_DESTROYMODELESS = 0;

}

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif

