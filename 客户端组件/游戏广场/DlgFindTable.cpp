// DlgFindTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "��Ϸ�㳡 .h"
#include "DlgFindTable.h"
#include "RoomViewItem.h"
// CDlgFindTable �Ի���

CDlgFindTable::CDlgFindTable(CWnd * pParentWnd)
	: CSkinDialogEx(IDD_FINDTABLE)
{
	m_pParentWnd = pParentWnd;
	m_wFindTableID = INVALID_TABLE;
	m_wChairID = INVALID_CHAIR;
}

CDlgFindTable::~CDlgFindTable()
{
}

void CDlgFindTable::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_FINDTABLE_BTN_FIND, m_btFind);
	DDX_Control(pDX,IDC_FINDTABLE_BTN_SITDOWN, m_btSitDown);
	__super::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFindTable,  CSkinDialogEx)
	ON_BN_CLICKED(IDC_FINDTABLE_BTN_FIND, OnFindTable)
	ON_BN_CLICKED(IDC_FINDTABLE_BTN_SITDOWN, OnSitDown)
END_MESSAGE_MAP()

void CDlgFindTable::OnFindTable()
{
	if(m_pParentWnd == NULL)
		return ;

	CRoomViewItem* pRoomView = (CRoomViewItem*)m_pParentWnd;

	bool bTwoNull =(((CButton *)GetDlgItem(IDC_CHE_TWONULL))->GetCheck()==BST_CHECKED);
	bool bTwoNull2 = (((CButton *)GetDlgItem(IDC_CHE_TWONULL2))->GetCheck()==BST_CHECKED);
	bool bOneNull = (((CButton *)GetDlgItem(IDC_CHE_ONENULL))->GetCheck()==BST_CHECKED);
	bool bAllNull = (((CButton *)GetDlgItem(IDC_CHE_ALLNULL))->GetCheck()==BST_CHECKED);

	//���������Ӳ�ȫ�յ�
	tagFindTable FindInfo;
	FindInfo.bAllNull=bAllNull;
	FindInfo.bFilterPass=true;
	FindInfo.bNotFull=bTwoNull2;
	FindInfo.bOneNull=bOneNull;
	FindInfo.bTwoNull=bTwoNull;
	FindInfo.wBeginTableID=m_wFindTableID+1;
	FindInfo.wResultTableID=INVALID_TABLE;
	FindInfo.wResultChairID=INVALID_CHAIR;
	bool bSuccess=pRoomView->FindGameTable(FindInfo);
	m_wFindTableID=FindInfo.wResultTableID;
	if(bSuccess == true)
	{
		//��������
		m_wChairID=FindInfo.wResultChairID;
		pRoomView->m_TableFrame.EnsureVisibleTable(m_wFindTableID);
		pRoomView->m_TableFrame.FlashChair(m_wFindTableID,m_wChairID);
	}
	else
	{
		m_wChairID = INVALID_CHAIR;
		ShowInformationEx("δ�ҵ��������������ӣ�",0,MB_ICONINFORMATION,"��ʾ");
	}
}

BOOL CDlgFindTable::OnInitDialog()
{
	__super::OnInitDialog();

	//MoveWindow(0,0,100,100);
	//���ñ���
	SetWindowText(TEXT("��������"));
	((CButton *)GetDlgItem(IDC_CHE_TWONULL))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem(IDC_CHE_TWONULL2))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem(IDC_CHE_ONENULL))->SetCheck(BST_CHECKED);

	return TRUE;
}

void CDlgFindTable::OnSitDown()
{
	if(m_pParentWnd == NULL)
		return ;

	CRoomViewItem* pRoomView = (CRoomViewItem*)m_pParentWnd;

	if(m_wChairID != INVALID_CHAIR && m_wFindTableID != INVALID_TABLE)
	{
		//�Զ�����
		if(!pRoomView->PerformSitDownAction(m_wFindTableID,m_wChairID))
		ShowInformationEx("����ʧ�ܣ�",0,MB_ICONINFORMATION,"��ʾ");
	}
}

// CDlgFindTable ��Ϣ�������
