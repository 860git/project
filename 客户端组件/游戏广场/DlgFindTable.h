#pragma once
#include "Stdafx.h"
#include "Resource.h"
// CDlgFindTable �Ի���

class CDlgFindTable : public CSkinDialogEx
{

public:
	CWnd							* m_pParentWnd;						//������
	WORD							m_wFindTableID;
	WORD							m_wChairID;							//

	CSkinButton						m_btFind;
	CSkinButton						m_btSitDown;

public:
	CDlgFindTable(CWnd * pParentWnd);   // ��׼���캯��
	virtual ~CDlgFindTable();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	//��������
	afx_msg void OnFindTable();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//����
	afx_msg void OnSitDown();

	DECLARE_MESSAGE_MAP()
};
