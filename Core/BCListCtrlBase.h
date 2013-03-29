#pragma once
#include <vector>
#include "BCExport.h"
using namespace std;

typedef enum LISTCTRL_COLDATATYPE{
	LCDT_UNSET,
	LCDT_TEXT,
	LCDT_INT,
	LCDT_FLOAT,
	LCDT_ENUM,
	LCDT_CHECK,
	LCDT_RADIO,
	LCDT_IMAGE,
	LCDT_COLOR,
	LCDT_TIME,
	LCDT_NO,
}LCDType;

const int LISTCTRL_IMAGE_SIZE = 13;

typedef struct LISTCTRL_ROWCOLINFO{
	//数据
	LCDType eType;
	vector<CString> strVals;	//枚举时的可选值
	CImageList*		pImageList;	

	//状态
	BOOL	bEnable;
	BOOL	bGrayed;

	LISTCTRL_ROWCOLINFO():pImageList(NULL), bEnable(TRUE),
	eType(LCDT_UNSET){};

}ListCtrlRowColInfo;

enum LISTITEM_STATUS{
	ITEM_UNSELECTED = 0,
	ITEM_SELECTED,
	ITEM_EDITING,
};

typedef struct LISTCTRL_ITEMINFO{
	//状态
	BOOL			bEnable;
	BOOL			bGrayed;
	LISTITEM_STATUS	eStatus;

	//数据
	union	ITEMDATE{
		int			nImage;
		COLORREF	clr;
		BOOL		bChecked;
	}ItemData;

	LISTCTRL_ITEMINFO():bEnable(TRUE), eStatus(ITEM_UNSELECTED){
		ItemData.nImage = 0;
	};

}ListCtrlItemInfo;


typedef struct LISTCTRL_ITEMEDITINFO{
	int nItem;
	int nSubItem;
	LISTCTRL_COLDATATYPE eType;

	union ITEMDATA{
		int nData;
		int nImage;
		COLORREF clr;
		BOOL bChecked;
		LPCTSTR str;
		float fData;
	};

	union ITEMDATA perData, newData;
}ListCtrlItemEditInfo;
// BCListCtrlBase

class BC_API BCListCtrlBase : public CListCtrl
{
	DECLARE_DYNAMIC(BCListCtrlBase)

public:
	BCListCtrlBase();
	virtual ~BCListCtrlBase();

protected:
	
	//GDI Objects
	CImageList	m_CheckList;

	//select
	int m_iSelItem, m_iSelSubItem;
	ListCtrlItemInfo*	m_pCurSelItem;

	//edit ctrls;
	CEdit m_editCtrl;
	CComboBox m_comboxCtrl;

	//最近的修改信息
	ListCtrlItemEditInfo	m_lastEditInfo;

	//Tools
	virtual ListCtrlItemInfo*	GetItemInfo(int _nItem, int _nSubItem) = 0;

	virtual ListCtrlRowColInfo*	GetRowColInfo(int _nRow, int _nCol) = 0;

	virtual int					GetColCount() = 0;

	void		DrawImage(CDC* _pDC, const CRect& rect, CImageList* _pImageList, int _nImage);
	void		DrawItemText(CDC* _pDC, const CRect& _rect, LPCTSTR _str, int _nFormat, BOOL _bGrayed);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	
	void		SelectItem( ListCtrlItemInfo* _pItem );
	void		SelectItem( int _nItem, int _nSubItem );
	BOOL		EditItem( CRect& _rect );

	void		OnEditSubItem(int _nItem, int _nSubItem);
	void		OnSelectItemRadio(int _nItem, int _nSubItem);

public:
	BOOL		SetItemColor(int _nItem, int _nSubItem, COLORREF _clr);
	BOOL		SetItemCheck(int _nItem, int _nSubItem, BOOL _bChecked);
	BOOL		SetItemEnable(int _nItem, int _nSubItem, BOOL _bEnable, BOOL _bGray);
	BOOL		SetItemImage(int _nItem, int _nSubItem, int _idx);
	BOOL		SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);

	COLORREF	GetItemColor(int _nItem, int _nSubItem);
	BOOL		IsItemChecked(int _nItem, int _nSubItem);
	BOOL		IsItemEnabled(int _nItem, int _nSubItem);
	int			GetItemImage(int _nItem, int _nSubItem);
	BOOL		GetItemText(int nItem, int nSubItem, CString& _str);

	BOOL			OnAttach(int _mode);


	DECLARE_MESSAGE_MAP()
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
};


