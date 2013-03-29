// BCListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IMBasicControls.h"
#include "BCListCtrl.h"
#include "BCMsgDefine.h"
#include "BTUsefulTools.h"
#include "Log.h"

IMPLEMENT_DYNAMIC(BCListCtrl, CListCtrl)

BCListCtrl::BCListCtrl()
{

}

BCListCtrl::~BCListCtrl()
{
	Cleanup();
}


BEGIN_MESSAGE_MAP(BCListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int BCListCtrl::InsertColumn( int _nCol, LPCTSTR _strHeading, LCDType _eType,
	int _nFormat, int _nWidth, int _nSubItem)
{
	ListCtrlRowColInfo* pInfo = new ListCtrlRowColInfo;
	pInfo->eType = _eType;
	if ((int)m_pListColInfo.size() <= _nCol)
	{
		m_pListColInfo.push_back(pInfo);
	}
	else
	{
		m_pListColInfo.insert(m_pListColInfo.begin() + _nCol, pInfo);
	}

	if (_eType == LCDT_RADIO || _eType == LCDT_CHECK)
	{
		pInfo->pImageList = &m_CheckList;
	}

	return CListCtrl::InsertColumn(_nCol, _strHeading, _nFormat, _nWidth, _nSubItem);
}

int BCListCtrl::InsertItem( int _nItem, LPCTSTR _strName, void* _pData )
{
	int colNum = (int)m_pListColInfo.size();
	ListCtrlItemInfo* pInfo = new ListCtrlItemInfo[colNum];
	if (_nItem >= (int)m_pListItemInfo.size())
	{
		m_pListItemInfo.push_back(pInfo);
	}
	else
	{
		m_pListItemInfo.insert(m_pListItemInfo.begin() + _nItem, pInfo);
	}

	for (int i=0; i<colNum; ++i)
	{
		pInfo[i].bEnable = m_pListColInfo[i]->bEnable;
	}

	int res ;
	if (m_pListColInfo[0]->eType == LCDT_NO)
	{
		res = InsertItemWithNo(_nItem);
	}
	else
	{
		res = CListCtrl::InsertItem(_nItem, _strName);
	}
	SetItemData(res, (DWORD_PTR)_pData);

	DEBUG_OUTPUT_WITH_CODE(DEBUG_ALL, "Item in list: ", m_pListItemInfo.size());

	return res;
}


void BCListCtrl::Cleanup(void)
{
	int num = m_pListItemInfo.size();
	for (int i=0; i<num; ++i)
	{
		delete []m_pListItemInfo[i];
	}
	m_pListItemInfo.clear();
	
	num = m_pListColInfo.size();
	for (int i=0; i<num; ++i)
	{
		delete m_pListColInfo[i];
	}
	m_pListColInfo.clear();
}


// BCListCtrl message handlers
void BCListCtrl::SetColumnImageList(int _nCol, CImageList* _p )
{
	m_pListColInfo[_nCol]->pImageList = _p;
}

int BCListCtrl::InsertItemWithNo( int _nItem )
{
	CString strNo;
	strNo.Format(_T("%d"), _nItem + 1);
	int res =  CListCtrl::InsertItem(_nItem, strNo);

	int num = m_pListItemInfo.size();
	for (int i=res+1; i < num; ++i)
	{
		strNo.Format(_T("%d"), i+1);
		CListCtrl::SetItemText(i, 0, strNo);
	}
	return res;
}

BOOL BCListCtrl::DeleteItem( BOOL nItem )
{
	
	if(CListCtrl::DeleteItem(nItem))
	{
		delete [](m_pListItemInfo[nItem]);
		m_pListItemInfo.erase(m_pListItemInfo.begin() + nItem);

		if (m_pListColInfo[0]->eType == LCDT_NO)
		{
			DeleteItemWidthNo(nItem);
		}

		if (m_iSelItem == nItem)
		{
			m_iSelItem = -1;
			m_iSelSubItem = -1;
			m_pCurSelItem = NULL;
		}

		DEBUG_OUTPUT_WITH_CODE(DEBUG_ALL, "Item in list: ", m_pListItemInfo.size());
		return TRUE;
	}
	return FALSE;
	
}

void BCListCtrl::DeleteItemWidthNo( int _nItem )
{
	CString strNo;
	int num = m_pListItemInfo.size();
	for (int i=_nItem; i<num; ++i)
	{
		strNo.Format(_T("%d"), i+1);
		CListCtrl::SetItemText(i, 0, strNo);
	}
}

BOOL BCListCtrl::SetColumnEnable( int _nCol, BOOL _bEnable )
{
	if(m_pListColInfo[_nCol]->bEnable != _bEnable)
	{
		m_pListColInfo[_nCol]->bEnable = _bEnable;
		int num = m_pListItemInfo.size();
		for (int i=0; i<num; ++i)
		{
			m_pListItemInfo[i][_nCol].bEnable = _bEnable;
		}
		Update(-1);
	}
	return TRUE;
}

BOOL BCListCtrl::SetColumnGrayed( int _nCol, BOOL bGrayed )
{
	if(m_pListColInfo[_nCol]->bGrayed != bGrayed)
	{
		m_pListColInfo[_nCol]->bGrayed = bGrayed;
		int num = m_pListItemInfo.size();
		for (int i=0; i<num; ++i)
		{
			m_pListItemInfo[i][_nCol].bGrayed = bGrayed;
		}
		Update(-1);
	}
	return TRUE;
}

ListCtrlItemInfo* BCListCtrl::GetItemInfo( int _nItem, int _nSubItem )
{
	return &m_pListItemInfo[_nItem][_nSubItem];
}

int BCListCtrl::GetColCount()
{
	return (int)m_pListColInfo.size();
}

ListCtrlRowColInfo* BCListCtrl::GetRowColInfo( int _nRow, int _nCol )
{
	return m_pListColInfo[_nCol];
}
