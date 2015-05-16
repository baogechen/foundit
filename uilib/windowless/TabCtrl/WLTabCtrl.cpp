// TabCtrlST.cpp : implementation file
//

#include "stdafx.h"
#include "WLTabCtrl.h"
#include "WLTabItem.h"
#include "logex.h"

namespace uilib_global
{
	const int TOOL_TIP_INIT_TIME = 300;			//���ͣ�� TOOL_TIP_INIT_TIME����֮�� ��ʾtip
	const int TOOL_TIP_RESHOW_TIME = 100;			//��ȡ��ָ���һ�������ƶ�����һ������ʱ������������ʾ������ʾ����Ҫ��ʱ�䳤��
	const int TOOL_TIP_AUTOPOP_TIME = 5000;			//��� һֱͣ������ʾȥ tip����ʱ�� AUTOPOP ���MFC������
};

template<>
UILIB_EXPORT WLTabCtrlImpBase *CreateDefaultImp< WLTabCtrlImpBase >(IWLTabCtrl * wrapper)

{
	WLTabCtrlImp<> *ret = new WLTabCtrlImp<>(FALSE);
	ret->SetWrapper(wrapper);
	return ret;
};

