//////////////////////////////////////////////////////////////////////////
//
//	����:   ֻ���մ�����Ϣ�Ĵ��ڻ���
//	
//	����:   2010/09/23
// 															�����ө���
// 															���֩��E��
// 															�ĩ��੤��
// 															��ӡ����
// 															�����۩���
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "uilib/uilib_exports.h"

// CWndMsgOnly

class CWndMsgOnly : public CWnd 
{
	DECLARE_DYNAMIC(CWndMsgOnly)

public:
	CWndMsgOnly();
	virtual ~CWndMsgOnly();

	BOOL Create() ;

// Data
private:
	static CString s_sClassName ;

protected:
	DECLARE_MESSAGE_MAP()

};


