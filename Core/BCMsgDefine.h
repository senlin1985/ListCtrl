/*************************************************
Copyright (C), ,AiMeng Tech. Co., Ltd.
File name:	BCMsgDefine.h
Author: zhugy	Version: 0.1	Date: 2013-1-15
Description: �Զ�����Ϣ����
*************************************************/


#pragma once
#include "..\include\UserMsgDefine.h"

//Splitter bar draged
#define		WM_SPLITBAR_DRAG WM_IMBASICCONTROL_BASE

//Caption ����ק��ԭ����
#define		WM_CAPTIONBAR_DRAG_OUTOFRANGE WM_IMBASICCONTROL_BASE + 1

#define		WM_SPLITBAR_LBDOWN WM_IMBASICCONTROL_BASE + 2
#define		WM_SPLITBAR_CLICK	WM_IMBASICCONTROL_BASE + 3



//Show Hide Button,�򿪹ر���Ϣ, wParam: FALSE(Hide), TRUE(Show),lParam: btn ָ��
#define		WM_SHOWHIDEBTN_SHOWCHILD	WM_IMBASICCONTROL_BASE + 4

//ListCtrl
/*
//wparam: list ptr, lparam ptr of
struct{	int nItem, 
		int nSubItem,
		LPCTSTR _perText,
		LPCTSTR _newText};
*/
#define		WM_LISTCTRL_EDITED	WM_IMBASICCONTROL_BASE + 5 

/*
//wparam: list ptr, lparam ptr of
struct{	int nItem, 
		int nSubItem,
		int eType,

		vector<CString> vals};
*/
#define		WM_LISTCTRL_COSTOMACTION WM_IMBASICCONTROL_BASE + 6 