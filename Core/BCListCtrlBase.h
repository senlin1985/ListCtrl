#pragma once


// BCListCtrlBase

class BCListCtrlBase : public CListCtrl
{
	DECLARE_DYNAMIC(BCListCtrlBase)

public:
	BCListCtrlBase();
	virtual ~BCListCtrlBase();

protected:
	DECLARE_MESSAGE_MAP()
};


