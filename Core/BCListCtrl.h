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

typedef struct LISTCTRL_COLINFO{
	//数据
	LCDType eType;
	vector<CString> strVals;	//枚举时的可选值
	CImageList*		pImageList;	

	//状态
	BOOL	bEnable;
	BOOL	bGrayed;

	LISTCTRL_COLINFO():pImageList(NULL), bEnable(TRUE){};

}ListCtrlColInfo;

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

class BC_API BCListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(BCListCtrl)

public:
	BCListCtrl();
	virtual ~BCListCtrl();

protected:
	//数据
	vector<ListCtrlColInfo*>	m_pListColInfo;		//列的数组
	vector<ListCtrlItemInfo*>	m_pListItemInfo;	//item数组的数组

	//旋转行列
	BOOL		m_bTransRowCol;
	//GDI Objects
	CImageList	m_CheckList;
	
	//select
	int m_iSelItem, m_iSelSubItem;
	ListCtrlItemInfo*	m_pCurSelItem;

	//edit ctrls;
	CEdit m_editCtrl;
	CComboBox m_comboxCtrl;

	ListCtrlItemEditInfo	m_lastEditInfo;

	DECLARE_MESSAGE_MAP()
	void	Cleanup();
	void	DrawImage(CDC* _pDC, const CRect& rect, CImageList* _pImageList, int _nImage);
	void	DrawItemText(CDC* _pDC, const CRect& _rect, LPCTSTR _str, int _nFormat, BOOL _bEnable);

	void	SelectItem( ListCtrlItemInfo* _pItem );
	void	SelectItem( int _nItem, int _nSubItem );

	BOOL	EditItem( CRect& _rect );

	void	OnEditSubItem(int _nItem, int _nSubItem);
	void	OnSelectItemRadio(int _nItem, int _nSubItem);

	int		InsertItemWithNo(int _nItem);
	void	DeleteItemWidthNo(int _nItem);

public:
	void	TransRowCol(BOOL _b){m_bTransRowCol = _b;}
	BOOL	OnAttach();

	int		InsertColumn(int _nCol, LPCTSTR _strHeading,  LCDType _eType, 
		int _nFormat = LVCFMT_LEFT,	int _nWidth = -1, int _nSubItem = -1);

	int		InsertItem(int _nItem, LPCTSTR _strName, void* _pData);
	BOOL	DeleteItem(BOOL nItem);

	BOOL	SetItemColor(int _nItem, int _nSubItem, COLORREF _clr);
	BOOL	SetItemCheck(int _nItem, int _nSubItem, BOOL _bChecked);
	BOOL	SetItemEnable(int _nItem, int _nSubItem, BOOL _bEnable);
	BOOL	SetItemImage(int _nItem, int _nSubItem, int _idx);
	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);

	COLORREF	SetItemColor(int _nItem, int _nSubItem);
	BOOL		SetItemCheck(int _nItem, int _nSubItem);
	BOOL		SetItemEnable(int _nItem, int _nSubItem);
	int			SetItemImage(int _nItem, int _nSubItem);
	BOOL		GetItemText(int nItem, int nSubItem, CString& _str);

	BOOL		SetColumnEnable(int _nCol, BOOL _bEnable);
	BOOL		IsColumnEnabled(int _nCol);

	BOOL		SetColumnGrayed(int _nCol, BOOL bGrayed);
	BOOL		IsColumnGrayed(int _nCol);

	void		SetColumnImageList(int _nCol,CImageList* _p);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
};



