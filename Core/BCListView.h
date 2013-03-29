#pragma once
#include "BCExport.h"
// BCListView view
class BCListCtrlBase;

enum BCLIST_TYPE{
	LIST_DEFAULT,
	LIST_NORMAL,
	LIST_REVERSE
};

class BC_API BCListView : public CView
{
	DECLARE_DYNCREATE(BCListView)

protected:
	BCListView(BCLIST_TYPE _e = LIST_NORMAL);           // protected constructor used by dynamic creation
	virtual ~BCListView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	BCLIST_TYPE	m_eType;
	CListCtrl*	m_pListCtrl;

	CListCtrl*	GetListCtrl(){return m_pListCtrl;}
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


