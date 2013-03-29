// BCListCtrlBase.cpp : implementation file
//

#include "stdafx.h"
#include "IMBasicControls.h"
#include "BCListCtrlBase.h"
#include "BCMsgDefine.h"
#include "Log.h"
#include "BTUsefulTools.h"

static const UINT		COL0_LEFTMARGIN = 5;
static const COLORREF	DISABLE_COLOR = RGB(177, 177, 177);
static const UINT		ID_LISTCTRL_EDIT = 10012;
static const UINT		ID_LISTCTRL_COMBOX = 10013;
// BCListCtrlBase

IMPLEMENT_DYNAMIC(BCListCtrlBase, CListCtrl)

BCListCtrlBase::BCListCtrlBase():m_pCurSelItem(NULL), 
m_iSelItem(-1), m_iSelSubItem(-1)
{

}

BCListCtrlBase::~BCListCtrlBase()
{
}


BEGIN_MESSAGE_MAP(BCListCtrlBase, CListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void BCListCtrlBase::DrawImage( CDC* _pDC, const CRect& rect, 
	CImageList* _pImageList, int _nImage )
{
	POINT pt;
	pt.x = rect.left + (rect.Width() - LISTCTRL_IMAGE_SIZE)/2; 
	pt.y = rect.top + (rect.Height() - LISTCTRL_IMAGE_SIZE)/2;

	_pImageList->Draw(_pDC, _nImage, pt, ILD_TRANSPARENT);
}

void BCListCtrlBase::DrawItemText( CDC* _pDC, const CRect& _rect, LPCTSTR _str,
	int _nFormat, BOOL _bGrayed)
{
	int align = DT_LEFT;
	if (_nFormat & LVCFMT_CENTER)
	{
		align = DT_CENTER;
	}
	else if (_nFormat & LVCFMT_RIGHT)
	{
		align = DT_RIGHT;
	}
	if (!_bGrayed)
	{
		COLORREF clr = _pDC->SetTextColor(DISABLE_COLOR);
		_pDC->DrawText(_str, -1, 
			CRect::CRect(_rect.left,_rect.top,_rect.right,_rect.bottom), 
			align | DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE);
		_pDC->SetTextColor(clr);
	}
	else
	{
		_pDC->DrawText(_str, -1, 
			CRect::CRect(_rect.left,_rect.top,_rect.right,_rect.bottom),
			align | DT_END_ELLIPSIS| DT_VCENTER | DT_SINGLELINE);
	}
}

void BCListCtrlBase::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	int nItem = lpDrawItemStruct->itemID;
	if (nItem < 0)
	{
		return;
	}
	CRect rcCol = lpDrawItemStruct->rcItem;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	LVITEM item;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.mask = LVIF_STATE;
	item.stateMask = 0xFFFF;
	GetItem(&item);

	int nOldDC = pDC->SaveDC();

	BOOL bSelected = item.state & LVIS_SELECTED;
	COLORREF clr = GetSysColor(COLOR_WINDOW);
	if (bSelected)
	{
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clr = GetSysColor(COLOR_HIGHLIGHT);
	}
	else
	{
		pDC->SetBkColor(clr);
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	}

	//依次绘制各subItem
	HPEN hOldPen = (HPEN)::SelectObject(lpDrawItemStruct->hDC, ::CreatePen(PS_SOLID, 1, clr));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(lpDrawItemStruct->hDC, ::CreateSolidBrush(clr));

	LV_COLUMN lvc;
	lvc.mask=LVCF_FMT|LVCF_WIDTH;
	rcCol.right = rcCol.left;

	CString sText;
	ListCtrlRowColInfo* pRowColInfo = NULL;
	ListCtrlItemInfo* pItemInfo = NULL;
	for(int nCol=0; GetColumn(nCol,&lvc); nCol++)
	{
		if (lvc.cx == 0)
		{
			continue;
		}
		//绘制item背景
		rcCol.left = rcCol.right;
		rcCol.right = rcCol.left + lvc.cx;		
		::Rectangle(lpDrawItemStruct->hDC, rcCol.left, rcCol.top, rcCol.right, rcCol.bottom);

		pRowColInfo = GetRowColInfo(nItem, nCol);
		pItemInfo = GetItemInfo(nItem, nCol);

		ASSERT(pRowColInfo);
		switch(pRowColInfo->eType)
		{
		case LCDT_CHECK://Check Image
		case LCDT_RADIO:
		case LCDT_IMAGE:
			{
				DrawImage(pDC, rcCol,pRowColInfo->pImageList,
					pItemInfo->ItemData.nImage);
			}
			break;
		case LCDT_TEXT:
		case LCDT_ENUM:
		case LCDT_INT:
		case LCDT_FLOAT:
		case LCDT_NO:
		case LCDT_UNSET:
			//输出字符串
			{
				sText = CListCtrl::GetItemText(nItem, nCol);
				if (nCol == 0)
				{
					lvc.fmt |= LVCFMT_CENTER;
				}
				if (nCol == 0)	//首列偏移
				{
					rcCol.left += COL0_LEFTMARGIN;
					DrawItemText(pDC, rcCol, sText, LVCFMT_LEFT, 
						pItemInfo->bGrayed);
				}
				else
				{
					DrawItemText(pDC, rcCol, sText, lvc.fmt, 
						pItemInfo->bGrayed);
				}
			}
			break;
		case LCDT_COLOR:
			{
				
			}
			break;
		default:
			;
		}

		//判断是否被选择
		/*if (m_pListItemInfo[nItem][nCol].eStatus == ITEM_SELECTED)
		{
			DrawSelectCursor(pDC, rcCol);
		}*/
		/*else if (m_pListItemInfo[nItem][nCol].eStatus == ITEM_EDITING)
		{
			UpdateEditCtrlPos(rcCol);
		}*/
	}

	::DeleteObject(SelectObject(lpDrawItemStruct->hDC, hOldBrush));
	::DeleteObject(SelectObject(lpDrawItemStruct->hDC, hOldPen));

	
	pDC->RestoreDC(nOldDC); 

}

BOOL BCListCtrlBase::SetItemColor( int _nItem, int _nSubItem, COLORREF _clr )
{
	GetItemInfo(_nItem, _nSubItem)->ItemData.clr = _clr;
	return TRUE;
}

BOOL BCListCtrlBase::SetItemCheck( int _nItem, int _nSubItem, BOOL _bChecked )
{
	GetItemInfo(_nItem, _nSubItem)->ItemData.bChecked = _bChecked;
	return TRUE;
}

BOOL BCListCtrlBase::SetItemEnable( int _nItem, int _nSubItem, BOOL _bEnable, BOOL _bGray )
{
	GetItemInfo(_nItem, _nSubItem)->bEnable = _bEnable;
	GetItemInfo(_nItem, _nSubItem)->bGrayed = _bGray;
	return TRUE;
}

BOOL BCListCtrlBase::SetItemImage( int _nItem, int _nSubItem, int _idx )
{
	GetItemInfo(_nItem, _nSubItem)->ItemData.nImage = _idx;
	return TRUE;
}

BOOL BCListCtrlBase::SetItemText( int nItem, int nSubItem, LPCTSTR lpszText )
{
	return CListCtrl::SetItemText(nItem, nSubItem, lpszText);
}

COLORREF BCListCtrlBase::GetItemColor( int _nItem, int _nSubItem )
{
	return GetItemInfo(_nItem, _nSubItem)->ItemData.clr;
}

BOOL BCListCtrlBase::IsItemChecked( int _nItem, int _nSubItem )
{
	return GetItemInfo(_nItem, _nSubItem)->ItemData.bChecked;
}

BOOL BCListCtrlBase::IsItemEnabled( int _nItem, int _nSubItem )
{
	return GetItemInfo(_nItem, _nSubItem)->bEnable;
}

int BCListCtrlBase::GetItemImage( int _nItem, int _nSubItem )
{
	return GetItemInfo(_nItem, _nSubItem)->ItemData.nImage;
}

BOOL BCListCtrlBase::GetItemText( int nItem, int nSubItem, CString& _str )
{
	_str = CListCtrl::GetItemText(nItem, nSubItem);
	return TRUE;
}

BOOL BCListCtrlBase::OnAttach( int _mode )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!m_CheckList.Create(13, 13, ILC_COLOR24, 4, 2))
	{
		return FALSE;
	}
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_CHECK);
	m_CheckList.Add(&bmp, RGB(255, 255, 255));

	if (!m_editCtrl.Create(ES_AUTOHSCROLL | ES_LEFT | WS_CHILD | WS_BORDER,
		CRect(0,0,0,0), this, ID_LISTCTRL_EDIT))
	{
		return -2;
	}


	if (!m_comboxCtrl.Create(
		WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST |CBS_NOINTEGRALHEIGHT,
		CRect(0,0,10,5),
		this,
		ID_LISTCTRL_COMBOX))
	{
		return -3;
	}

	CFont* pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_editCtrl.SetFont(pFont);
	m_comboxCtrl.SetFont(pFont);

	return TRUE;
}

int BCListCtrlBase::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CListCtrl::OnCreate(lpCreateStruct)<0)
	{
		return -1;
	}
	return 0;
}

void BCListCtrlBase::OnLButtonDown( UINT nFlags, CPoint point )
{
	CRect rect;

	int perItem = m_iSelItem;
	int perSubItem = m_iSelSubItem;

	//SelectItem(-1, -1);
	int selItem = -1;
	int selSubItem = -1;

	size_t num = CListCtrl::GetItemCount();
	size_t colNum = GetColCount();
	for (size_t i=0; i<num; ++i)
	{
		GetSubItemRect(i, 0, LVIR_BOUNDS, rect);
		if (point.y > rect.top && point.y < rect.bottom)
		{
			LV_COLUMN lvc;
			lvc.mask = LVCF_WIDTH;

			for(size_t col=0; col<colNum; ++col)
			{
				GetColumn(col, &lvc);
				rect.right = rect.left + lvc.cx;
				if (lvc.cx == 0)
				{
					continue;
				}
				if (PtInRect(&rect, point))
				{
					if (GetItemInfo(i, col)->bEnable)
					{
						//SelectItem(i, col);
						selItem = i;
						selSubItem = col;
					}
					break;
				}
				rect.left = rect.right;
			}
			break;
		}
	}

	if (selSubItem == perSubItem && selItem == perItem)//相同的再次点击则调出修改控件
	{
		if (m_iSelItem >= 0 && m_iSelSubItem >= 0)
		{
			if(EditItem(rect)>0)
			{
				Update(selItem);
			}
		}
	}
	else//点击新项则隐藏控件并，更新视图
	{
		if (perItem >=0 && perSubItem>=0)
		{
			OnEditSubItem(perItem, perSubItem);
			//HideEditCtrls();
		}
		if (selItem >= 0)	
		{
			Update(selItem);
		}
		if (selItem != perItem && perItem >=0)
		{
			Update(perItem);
		}
		SelectItem(selItem, selSubItem);
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}

void BCListCtrlBase::SelectItem( int _nItem, int _nSubItem )
{
	if (_nItem<0 || _nSubItem<0 || 
		_nSubItem >= (int)GetColCount() ||
		_nItem >= (int)GetItemCount())
	{
		SelectItem(NULL);
	}
	else
	{
		SelectItem(GetItemInfo(_nItem, _nSubItem));
	}
	m_iSelItem = _nItem;
	m_iSelSubItem = _nSubItem;
}

void BCListCtrlBase::SelectItem( ListCtrlItemInfo* _pItem )
{
	if (m_pCurSelItem)
	{
		m_pCurSelItem->eStatus = ITEM_UNSELECTED;
	}
	if (_pItem)
	{
		_pItem->eStatus = ITEM_SELECTED;
	}
	m_pCurSelItem = _pItem;
}

BOOL BCListCtrlBase::EditItem( CRect& _rect )
{
	m_pCurSelItem->eStatus = ITEM_EDITING;

	ListCtrlRowColInfo* pRowColInfo = GetRowColInfo(m_iSelItem, m_iSelSubItem);
	ListCtrlItemInfo* pItem = NULL;
	switch (pRowColInfo->eType)
	{
	case LCDT_TEXT:
	case LCDT_INT:
	case LCDT_FLOAT:
		m_editCtrl.SetWindowText(CListCtrl::GetItemText(m_iSelItem, m_iSelSubItem));
		m_editCtrl.MoveWindow(_rect);
		m_editCtrl.ShowWindow(SW_SHOW);
		break;
	case LCDT_ENUM:
		m_comboxCtrl.ResetContent();
		{
			size_t nCount = pRowColInfo->strVals.size();
			int res;
			for (size_t i=0; i<nCount; ++i)
			{
				res = m_comboxCtrl.AddString(pRowColInfo->strVals[i]);
			}
		}
		m_comboxCtrl.MoveWindow(_rect.left, 
			_rect.top, _rect.Width(), 100);

		m_comboxCtrl.ShowWindow(SW_SHOW);
		break;
	case LCDT_CHECK:
		pItem = GetItemInfo(m_iSelItem, m_iSelSubItem);
		pItem->ItemData.bChecked = 
			!pItem->ItemData.bChecked;
		//修改数据
		{
			m_lastEditInfo.nItem = m_iSelItem;
			m_lastEditInfo.nSubItem = m_iSelSubItem;
			m_lastEditInfo.eType = LCDT_CHECK;
			m_lastEditInfo.newData.bChecked = pItem->ItemData.bChecked;
			m_lastEditInfo.perData.bChecked = !m_lastEditInfo.newData.bChecked;
		}
		return TRUE;
	case LCDT_RADIO:
		OnSelectItemRadio(m_iSelItem, m_iSelSubItem);
		return TRUE;
	default:
		;
	}
	return FALSE;
}

void BCListCtrlBase::OnEditSubItem( int _nItem, int _nSubItem )
{
	CString str, perText;
	//float data;
	ListCtrlItemInfo* pItem = m_pCurSelItem;
	ListCtrlRowColInfo* pRowColInfo = GetRowColInfo(_nItem, _nSubItem);
	switch(pRowColInfo->eType)
	{
	case LCDT_TEXT:
		if (!m_editCtrl.IsWindowVisible())
		{
			return;
		}
		perText = CListCtrl::GetItemText(_nItem, _nSubItem);
		m_editCtrl.GetWindowText(str);

		if (perText != str)
		{
			SetItemText(_nItem, _nSubItem, str);

			m_lastEditInfo.nItem = _nItem;
			m_lastEditInfo.nSubItem =_nSubItem;
			m_lastEditInfo.eType =LCDT_TEXT;
			m_lastEditInfo.perData.str = perText;
			m_lastEditInfo.newData.str = str;
			GetParent()->SendMessage(WM_LISTCTRL_EDITED, (WPARAM)this, (LPARAM)&m_lastEditInfo);
		}
		m_editCtrl.ShowWindow(SW_HIDE);
		break;
	case LCDT_FLOAT:
		if (!m_editCtrl.IsWindowVisible())
		{
			return;
		}
		m_editCtrl.GetWindowText(str);
		perText = CListCtrl::GetItemText(_nItem, _nSubItem);
		if (!IsFloat(str, str.GetLength()))
		{
			DEBUG_OUTPUT_WITH_STR(DEBUG_IMPORTANT_WARNING, 
				"Error: Invalid float number.", str);

			m_editCtrl.SetWindowTextW(perText);
			m_editCtrl.SetFocus();
		}
		else
		{
			if (perText != str)
			{
				SetItemText(_nItem, _nSubItem, str);

				m_lastEditInfo.nItem = _nItem;
				m_lastEditInfo.nSubItem =_nSubItem;
				m_lastEditInfo.eType =LCDT_FLOAT;
				m_lastEditInfo.perData.fData = (float)_ttof(perText);
				m_lastEditInfo.newData.fData = (float)_ttof(str);
				GetParent()->SendMessage(WM_LISTCTRL_EDITED, (WPARAM)this, 
					(LPARAM)&m_lastEditInfo);
			}
			m_editCtrl.ShowWindow(SW_HIDE);
		}
		break;
	case LCDT_INT:
		if (!m_editCtrl.IsWindowVisible())
		{
			return;
		}
		m_editCtrl.GetWindowText(str);
		perText = CListCtrl::GetItemText(_nItem, _nSubItem);
		if (!IsInt(str, str.GetLength()))
		{
			DEBUG_OUTPUT_WITH_STR(DEBUG_IMPORTANT_WARNING, 
				"Error: Invalid float number.", str);

			m_editCtrl.SetWindowTextW(perText);
			m_editCtrl.SetFocus();
		}
		else
		{
			if (perText != str)
			{
				SetItemText(_nItem, _nSubItem, str);

				m_lastEditInfo.nItem = _nItem;
				m_lastEditInfo.nSubItem =_nSubItem;
				m_lastEditInfo.eType =LCDT_INT;
				m_lastEditInfo.perData.nData = _ttoi(perText);
				m_lastEditInfo.newData.nData = _ttoi(str);

				GetParent()->SendMessage(WM_LISTCTRL_EDITED, (WPARAM)this, 
					(LPARAM)&m_lastEditInfo);
			}
			m_editCtrl.ShowWindow(SW_HIDE);
		}
		break;
	case LCDT_ENUM:
		break;
	default:
		;
	}
}

void BCListCtrlBase::OnSelectItemRadio( int _nItem, int _nSubItem )
{
	ListCtrlItemInfo* pInfo = GetItemInfo(_nItem, _nSubItem);

	if (pInfo->ItemData.bChecked)//取消check
	{
		pInfo->ItemData.bChecked = FALSE;
		m_lastEditInfo.nItem = _nItem;
		m_lastEditInfo.nSubItem =_nSubItem;
		m_lastEditInfo.eType =LCDT_RADIO;
		m_lastEditInfo.perData.bChecked = TRUE;
		m_lastEditInfo.newData.bChecked = FALSE;

		GetParent()->SendMessage(WM_LISTCTRL_EDITED, (WPARAM)this, (LPARAM)&m_lastEditInfo);
	}
	//else//set check
	//{
	//	int num = GetItemCount();
	//	ListCtrlItemInfo* pItem = NULL;
	//	for (int i=0; i<num;++i)
	//	{
	//		pItem = GetItemInfo(i, _nSubItem);
	//		if (i == _nItem)
	//		{
	//			GetItemInfo(i, _nSubItem)->ItemData.bChecked = TRUE;

	//			m_lastEditInfo.nItem = _nItem;
	//			m_lastEditInfo.nSubItem =_nSubItem;
	//			m_lastEditInfo.eType =LCDT_RADIO;
	//			m_lastEditInfo.perData.bChecked = FALSE;
	//			m_lastEditInfo.newData.bChecked = TRUE;

	//			GetParent()->SendMessage(WM_LISTCTRL_EDITED, (WPARAM)this, (LPARAM)&m_lastEditInfo);
	//		}
	//		else if (m_pListItemInfo[i][_nSubItem].ItemData.bChecked)
	//		{
	//			m_pListItemInfo[i][_nSubItem].ItemData.bChecked = FALSE;

	//			m_lastEditInfo.nItem = _nItem;
	//			m_lastEditInfo.nSubItem =_nSubItem;
	//			m_lastEditInfo.eType =LCDT_RADIO;
	//			m_lastEditInfo.perData.bChecked = TRUE;
	//			m_lastEditInfo.newData.bChecked = FALSE;

	//			GetParent()->SendMessage(WM_LISTCTRL_EDITED, (WPARAM)this, (LPARAM)&m_lastEditInfo);

	//			Update(i);
	//		}
	//	}
	//}
}



