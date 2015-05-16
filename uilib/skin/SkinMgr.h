#pragma once
#include "uilib/skin/ISkin.h"
#include <set>
class CSkinMgr :
	public ISkinMgr
{
public:
	CSkinMgr(void);
	virtual ~CSkinMgr(void);

	virtual ISkin const & GetSkin() const;
	virtual void RegisterSkinWindow(HWND wnd);
	virtual BOOL UnRegisterSkinWindow(HWND wnd);

	// ������һ��ʹ�õ�Ƥ��
	ISkin * GetLatestSkin();

	// ���Ƥ��
	void AddSkin(ISkin * skin);

	// ɾ��Ƥ��
	void DeleteSkin(ISkin * skin);

	// ʹ��Ƥ��
	void UseSkin(ISkin * skin, BOOL save_to_config);

private:
	ISkin * CSkinMgr::ChangeSkin(ISkin * skin);

	ISkin * m_skin ;
	std::set<HWND> m_all_skin_wnds ;		// all request skin windows
};
