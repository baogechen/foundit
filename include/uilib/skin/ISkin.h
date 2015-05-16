#pragma once

#include "uilib/uilib_exports.h"

#include "boost/shared_ptr.hpp"
class IPropertyTree;
class ISkin
{
public:
	enum Mode
	{
		Default = 0,
		SolidColor = 1,
		CustomImage = 2,
		DefaultImage,
	};

	virtual ~ISkin() {}

	virtual std::string GetGUID() const = 0 ;

	// �����ü���
	// ����guid����ΪƤ�����õ�key�����Իᵼ��load��save�ӿڵĲ�һ��
	virtual BOOL Load(boost::shared_ptr<IPropertyTree> config, std::string guid) = 0 ;

	virtual BOOL Save(boost::shared_ptr<IPropertyTree> config) = 0 ;

	virtual BOOL Delete() = 0 ;

	virtual Mode GetMode() const = 0 ;

	// ���ƣ�����Ĳ����Ǹ�������
	virtual void Draw(HDC hdc, int left, int top, int width, int height) const = 0 ;

	// ��������ͼ������Ĳ�����������������
	virtual void DrawThumbnail(HDC hdc, int left, int top, int width, int height) const = 0 ;

	// ��ô��µĻҶȣ������Ʒ���������
	virtual BYTE GetRoughGrayScale() const = 0 ;

    // ��ô��µ���ɫ
    virtual COLORREF GetRoughColor() const = 0 ;

	// ���ʹ��ʱ��
	virtual time_t GetTime() const = 0 ;

	// ����ʹ��ʱ��
	virtual void UpdateUseTime() = 0 ;

	// �������Ϊ������׼�������ݣ��������Ҫ��ɶ������
	virtual BOOL Activate() = 0 ;

	// ��������Ϊ���ƶ�׼��������
	virtual BOOL Deactivate() = 0 ;

	// ǳ�ȿ�����������Ϊ������׼��������
	virtual ISkin * LightDuplicate() const = 0 ;

};


class ISkinMgr
{
public:
	virtual ~ISkinMgr() {};
	virtual ISkin const & GetSkin() const = 0 ;
	virtual void RegisterSkinWindow(HWND wnd) = 0 ;
	virtual BOOL UnRegisterSkinWindow(HWND wnd) = 0 ;
};


UILIB_EXPORT ISkinMgr * GetSkinMgr();


