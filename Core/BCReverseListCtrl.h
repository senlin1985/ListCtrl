#pragma once
#include "BCListCtrlBase.h"

// BCReverseListCtrl

class BC_API BCReverseListCtrl : public BCListCtrlBase
{
	DECLARE_DYNAMIC(BCReverseListCtrl)

public:
	BCReverseListCtrl();
	virtual ~BCReverseListCtrl();

protected:
	//����
	vector<ListCtrlRowColInfo*>	m_pListRowInfo;		//�е�����
	vector<ListCtrlItemInfo*>	m_pListItemInfo;	//item���������
	ListCtrlItemInfo			m_FirstColItemInfo;

	virtual ListCtrlItemInfo*	GetItemInfo(int _nItem, int _nSubItem);
	virtual ListCtrlRowColInfo*	GetRowColInfo(int _nRow, int _nCol);
	virtual int					GetColCount();

	void		Cleanup();
public:
	int			InsertRow(int _nRow, LPCTSTR _strHeading,  LCDType _eType);

	int			InsertColumn(int _nCol, LPCTSTR _strName, void* _pData,
		int _nFormat = LVCFMT_LEFT,	int _nWidth = -1, int _nSubItem = -1);

	BOOL		DeleteColumn(BOOL nCol);

	void		DeleteAllColumns();

	BOOL		SetRowEnable(int _nRow, BOOL _bEnable);
	BOOL		IsRowEnabled(int _nRow);

	BOOL		SetRowGrayed(int _nRow, BOOL bGrayed);
	BOOL		IsRowGrayed(int _nRow);

	void		SetRowImageList(int _nRow, CImageList* _p);
	DECLARE_MESSAGE_MAP()
};


