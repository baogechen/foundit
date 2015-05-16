#pragma once

#include <list>

#include "common\windowless\basewndlessroot.h"
#include "common\windowless\ButtonAL\WLButtonAL.h"
#include "common\utils\ImageEx\Shared_ImageEx.h"

#include "WLSkinBox.h"
#include "SkinPreviewListener.h"

class WLMainWindowRoot ;

class WLSkinSetupPanel :
	public BaseWndLessRoot,
	public SkinBoxListener,
	public SkinPreviewListener
{
public:
	WLSkinSetupPanel(void);
	virtual ~WLSkinSetupPanel(void);

public:
	void SetMainWindowRoot(WLMainWindowRoot * root) { m_main_window_root = root ; }

protected:
	//////////////////////////////////////////////////////////////////////////
	// IWndLess Interface

	virtual BOOL OnCreate( DWORD dwStyle, int x, int y, int cx, int cy, IWndLess * pParent ) ;
	virtual void OnDestroy() ;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult) ;
	virtual BOOL OnControlCommand(WORD id, WORD code, HWL wnd, LRESULT * pResult) ;
	virtual void OnClose() ;

	//////////////////////////////////////////////////////////////////////////
	// SkinBoxListener

	virtual void OnSelectedSkinBox(WLSkinBox * skin_box) ;
	virtual void OnDblClickSkinBox(WLSkinBox * skin_box) ;

	//////////////////////////////////////////////////////////////////////////
	// SkinPreviewListener Interface

	virtual void OnConfirmUseSkin(CustomImageSkin * skin) ;
	virtual void OnCancelUseSkin(CustomImageSkin * skin) ;

protected:
	void InitImage() ;
	void LoadRecentlySkinList() ;
	void FreeSkinList() ;

	// ѡ��ͼƬ��Ƥ��
	void PickImageToSkin() ;

	// ѡ����ɫ��Ƥ��
	void PickColorToSkin() ;

	// ɾ��ѡ�е�Ƥ��
	void DeleteSelectedSkin() ;

	// ʹ��ѡ�е�Ƥ��
	BOOL ShowSelectedSkin(BOOL save_to_config = FALSE) ;

	// ����ͼƬƤ��
	void LoadImageSkin() ;

	// ��������ɫƤ��
	void LoadColorSkin() ;

protected:

	Shared_ImageEx m_img_btn_frgnd ;
	Shared_ImageEx m_img_gain_skin ;
	Shared_ImageEx m_img_add_skin ;
	Shared_ImageEx m_img_delete_skin ;
	Shared_ImageEx m_img_add_image ;
	Shared_ImageEx m_img_add_color ;

	enum
	{
		ID_BTN_GAIN_SKIN	= 100,
		ID_BTN_ADD_SKIN		= 101,
		ID_BTN_DELETE_SKIN	= 102,
		ID_BTN_ADD_IMAGE	= 103,
		ID_BTN_ADD_COLOR	= 104,
		ID_BTN_OK			= 105,
		ID_BTN_CANCEL		= 106,

		ID_DEFAULT_SKIN_BOX		= 200,
		ID_IMAGE_SKIN_BOX_BEGIN	= 200,
		ID_SYSTEM_COLOR_SKIN_BOX_BEGIN	= 300,
		ID_CUSTOM_COLOR_SKIN_BOX_BEGIN	= 400,

	};

	WLButtonAL m_btn_gain_skin ;
	WLButtonAL m_btn_add_skin ;
	WLButtonAL m_btn_delete_skin ;
	WLButtonAL m_btn_add_image ;
	WLButtonAL m_btn_add_color ;
	WLButtonAL m_btn_ok ;
	WLButtonAL m_btn_cancel ;

	static int const kImageSkinBoxColumnCount = 6 ;
	static int const kImageSkinBoxCount = 11 ;

	static int const kColorSkinBoxColumnCount = 17 ;
	static int const kSystemColorSkinBoxCount = kColorSkinBoxColumnCount * 3 ;
	static int const kRecentlyColorSkinBoxCount = 17 ;

	WLSkinBox m_image_skin_boxes[kImageSkinBoxCount] ;
	WLSkinBox m_system_color_skin_boxes[kSystemColorSkinBoxCount] ;				// ϵͳ��ɫƤ��
	WLSkinBox m_recently_color_skin_boxes[kRecentlyColorSkinBoxCount] ;			// �ù�����ɫƤ��

	typedef std::list<ISkin*> SkinList ;
	SkinList m_image_skin_list ;
	SkinList m_color_skin_list ;

	static COLORREF const kSystemColorList[kSystemColorSkinBoxCount] ;

	SkinList m_system_color_skin_list ;

	WLSkinBox * m_selected_skin_box ;		// ��ǰѡ���Ƥ��
	ISkin * m_old_skin ;					// ����֮ǰ��Ƥ��

	BOOL m_change_skin ;					// ����Ƿ�ı���Ƥ��

	WLMainWindowRoot * m_main_window_root ;

};
