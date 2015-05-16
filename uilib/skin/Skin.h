#pragma once

#include <list>

#include "skin\ISkin.h"

#include "common\utils\ImageEx\Shared_ImageEx.h"
#include "common\utils\CanvasDC.h"

class Skin : public ISkin
{
public:
	Skin() ;

	//virtual void SetGUID(std::string const &guid) { m_guid = guid ; }
	virtual std::string GetGUID() const { return m_guid ; }

	// �����ü���
	virtual BOOL Load(boost::shared_ptr<IPropertyTree> config, std::string guid) ;

	virtual BOOL Save(boost::shared_ptr<IPropertyTree> config) ;

	// ���ʹ��ʱ��
	virtual time_t GetTime() const { return m_use_time ; }

	// ����ʹ��ʱ��
	virtual void UpdateUseTime() { m_use_time = ::_time64(NULL) ; }

	// �������Ϊ������׼�������ݣ��������Ҫ��ɶ������
	virtual BOOL Activate() { return TRUE ; }

	// ��������Ϊ���ƶ�׼��������
	virtual BOOL Deactivate() { return TRUE ; }

	// ǳ�ȿ�����������Ϊ������׼��������
	virtual ISkin * LightDuplicate() const { return NULL ; }

protected:
	std::string m_guid ;
	time_t m_use_time ;
};

class CustomImageSkin : public Skin
{
public:
	enum ImageMode
	{
		Single = 0,
		Tile = 1,
	} ;

public:
	CustomImageSkin(void);
	~CustomImageSkin(void);

	// ��ʼ��

	// �����ü��ش���
	// ��δ����
	virtual BOOL Load(boost::shared_ptr<IPropertyTree> config, std::string guid) ;

	// �û���ק�ļ�����
	// ��ʱ������������
	BOOL Create(CString const &img_path, ImageMode mode, POINT const &origin) ;

	// ����Ƥ������
	virtual BOOL Save(boost::shared_ptr<IPropertyTree> config) ;

	// ɾ��Ƥ������
	virtual BOOL Delete() ;

	// ����Ƥ����������Ƥ��ͼƬ����m_skin_dc
	virtual BOOL Activate() ;

	// ������Ƥ��������m_skin_dc
	virtual BOOL Deactivate() ;

	// ǳ�ȿ�����������Ϊ������׼��������
	virtual ISkin * LightDuplicate() const ;

	inline POINT GetOrigin() const { return m_origin ; }
	void SetOrigin(int x, int y) 
	{
		m_origin.x = x ;
		m_origin.y = y ;
	}

	void SetThumbnailSize(int width, int height) 
	{
		if (width > 0) m_thumbnail_size.cx = width ;
		if (height > 0) m_thumbnail_size.cy = height ;
	}

	inline int GetWidth() const { return m_width ; }
	inline int GetHeight() const { return m_height ; }
	inline int GetImageWidth() const { return m_image_width ; }
	inline int GetImageHeight() const { return m_image_height ; }

	inline int GetLeft() const { return m_origin.x - ((m_image_mode == Single) ? m_few_edge_width : 0) ; }
	inline int GetTop() const { return m_origin.y - ((m_image_mode == Single) ? m_few_edge_width : 0) ; }
	inline int GetRight() const { return m_origin.x + m_image_width ; }
	inline int GetBottom() const { return m_origin.y + m_image_height ; }

	inline void SetLeft(int left) { m_origin.x = left + ((m_image_mode == Single) ? m_few_edge_width : 0) ; }
	inline void SetTop(int top) { m_origin.y = top + ((m_image_mode == Single) ? m_few_edge_width : 0) ; }
	inline void SetRight(int right) { m_origin.x = right - m_image_width ; }
	inline void SetBottom(int bottom) { m_origin.y = bottom - m_image_height ; }

	inline ImageMode GetImgMode() const { return m_image_mode ; }
	void SetImgMode(ImageMode mode) ;

	// ����ͼƬ�ļ�
	void SaveImageFile() ;

	//////////////////////////////////////////////////////////////////////////
	// ISkin Interface

	virtual Mode GetMode() const { return CustomImage ; }
	virtual void Draw(HDC hdc, int left, int top, int width, int height) const ;	// �����left,top,width,height�Ǹ�������
	virtual void DrawThumbnail(HDC hdc, int left, int top, int width, int height) const ;
    virtual BYTE GetRoughGrayScale() const ;
    virtual COLORREF GetRoughColor() const ;

protected:
	BOOL InitializeContent( Shared_ImageEx& img ) ;
	void UnInitializeContent() ;

protected:
	BOOL m_initialized ;				// TRUE��ʾ�ѳ�ʼ����FALSE��ʾδ��ʼ��
	BOOL m_activated ;					// TRUE��ʾ�Ѽ���
	//BOOL m_init_from_load_or_create ;	// TRUE��ʾ�Ǵ����ü��أ�FALSE��ʾ�û�����

	CString m_image_path ;
	ImageMode m_image_mode ;
	POINT m_origin ;
	CString m_name ;
	CString m_thumbnail_path ;
	SIZE m_thumbnail_size ;			// ��ʱ��¼����������ΪƤ�����ԣ���������ͼͼƬ���ɵ�

	CCanvasDC m_skin_dc ;			// Ƥ��dc
	COLORREF m_padding_color ;		// ���ɫ
	int m_few_edge_width ;			// ģ����Ե���

	int m_width ;					// Ƥ�����
	int m_height ;					// Ƥ���߶�

	int m_image_width ;				// ͼƬ���
	int m_image_height ;			// ͼƬ�߶�

	BYTE m_rough_gray_scale ;

	Shared_ImageEx m_image_thumbnail ;

};

//////////////Default Image Skin//////////////////////
class DefaultImageSkin : public CustomImageSkin
{
public:
	DefaultImageSkin();
	//{
	//	POINT origin = {0, 0} ;
	//	Create(_T("anniversary_bkgnd.jpg"),CustomImageSkin::Single, origin);
	//	m_image_thumbnail = Shared_ImageEx::FromId(_T("anniversary_bkgnd.jpg"));
	//	m_guid = "DefaultImageSkin";
	//}
	virtual BOOL Activate() ;
	virtual void SetGUID(std::string const &/*guid*/) { /* do nothing !!! */ }
	virtual BOOL Load(boost::shared_ptr<IPropertyTree> config, std::string guid) {return TRUE; };
	virtual BOOL Save(boost::shared_ptr<IPropertyTree> config) {return TRUE; };
	virtual BOOL Delete() {return TRUE; };
	virtual Mode GetMode() const { return DefaultImage; }
	ISkin * LightDuplicate() const 
	{
		DefaultImageSkin * copy_skin = new DefaultImageSkin ;
		return copy_skin ;
	}

private:
	UINT m_imgId;
};
//////////////////////////////////////////////////////////

class DefaultSkin : public Skin
{
public:
	DefaultSkin() ;

	//////////////////////////////////////////////////////////////////////////
	// ISkin Interface

	virtual void SetGUID(std::string const &/*guid*/) { /* do nothing !!! */ }

	virtual Mode GetMode() const { return Default ; }

	// Ĭ��Ƥ���Ļ����ɸ����������о��������ṩ�������
	virtual void Draw(HDC /*hdc*/, int /*left*/, int /*top*/, int /*width*/, int /*height*/) const { ASSERT(FALSE) ; }

	virtual void DrawThumbnail(HDC hdc, int left, int top, int width, int height) const ;

    virtual BYTE GetRoughGrayScale() const { ASSERT(FALSE) ; return 0 ;}
    virtual COLORREF GetRoughColor() const { ASSERT(FALSE) ; return 0 ; }

	// ɾ��Ĭ��Ƥ��û������
	virtual BOOL Delete() { return FALSE ; }

	ISkin * LightDuplicate() const { return new DefaultSkin ; }

protected:
	Shared_ImageEx m_image_base_wnd_bkgnd ;

};

class SolidColorSkin : public Skin
{
public:
	SolidColorSkin() ;
	SolidColorSkin(COLORREF color) ;

	COLORREF GetColor() const { return m_color ; }

	//////////////////////////////////////////////////////////////////////////
	// ISkin Interface

	virtual BOOL Load(boost::shared_ptr<IPropertyTree> config, std::string guid) ;
	virtual BOOL Save(boost::shared_ptr<IPropertyTree> config) ;
	virtual Mode GetMode() const { return SolidColor ; }
	virtual void Draw(HDC hdc, int left, int top, int width, int height) const ;
	virtual void DrawThumbnail(HDC hdc, int left, int top, int width, int height) const ;
    virtual BYTE GetRoughGrayScale() const ;
    virtual COLORREF GetRoughColor() const { return m_color ; }
	virtual BOOL Delete() { return FALSE ; }

	ISkin * LightDuplicate() const ;

private:
	COLORREF m_color ;

};

namespace SkinHelper
{
	//ISkin * Load(boost::shared_ptr<IPropertyTree> config) ;
	//BOOL Save(boost::shared_ptr<IPropertyTree> config, ISkin * skin) ;

	// ������һ��ʹ�õ�Ƥ��
	ISkin * GetLatestSkin(boost::shared_ptr<IPropertyTree> config) ;
	void SetLatestSkin(boost::shared_ptr<IPropertyTree> config, ISkin * skin) ;

	typedef std::list<ISkin*> SkinList ;
	void LoadRecentlySkinList(boost::shared_ptr<IPropertyTree> config, SkinList & skin_list) ;
	//void SaveRecentlySkinList(boost::shared_ptr<IPropertyTree> config, SkinList & skin_list) ;

	// ���Ƥ��
	void AddSkin(boost::shared_ptr<IPropertyTree> config, ISkin * skin) ;

	// ɾ��Ƥ��
	void DeleteSkin(boost::shared_ptr<IPropertyTree> config, ISkin * skin) ;

} ;
