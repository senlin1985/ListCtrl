// BCListView.cpp : implementation file
//

#include "stdafx.h"
#include "IMBasicControls.h"
#include "BCListView.h"
#include "BCListCtrl.h"
#include "BCReverseListCtrl.h"

// BCListView

IMPLEMENT_DYNCREATE(BCListView, CView)

BCListView::BCListView(BCLIST_TYPE _e):m_eType(_e)
{

}

BCListView::~BCListView()
{
}

BEGIN_MESSAGE_MAP(BCListView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// BCListView drawing

void BCListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// BCListView diagnostics

#ifdef _DEBUG
void BCListView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void BCListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// BCListView message handlers


int BCListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	switch(m_eType)
	{
	case LIST_DEFAULT:
		m_pListCtrl = new CListCtrl;
		break;
	case LIST_REVERSE:
		m_pListCtrl = new BCReverseListCtrl;
		break;
	case LIST_NORMAL:
		m_pListCtrl = new BCListCtrl;
		break;
	}
	if (!m_pListCtrl->Create(WS_VISIBLE | WS_CHILD| WS_BORDER|
		LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED | LVS_REPORT | 
		LVS_SHOWSELALWAYS | LVS_SINGLESEL, 
		CRect(0, 0, 0, 0),
		this,
		AFX_IDW_PANE_FIRST))
	{
		return -2;
	}

	m_pListCtrl->SetExtendedStyle(
		LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);


	return 0;
}


void BCListView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_pListCtrl->MoveWindow(0, 0, cx, cy);
}
