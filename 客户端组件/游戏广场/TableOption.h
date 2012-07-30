#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "SystemOption.h"

//���ò���
struct tagTableParameter
{
	enInviteMode						enInviteMode;					//����ģʽ
	enCallMode							enCallMode;						//����ģʽ
	TCHAR								szTablePass[PASS_LEN];			//��������
	COLORREF							crFriend;						//������ɫ
	COLORREF							crMe;							//�Լ���ɫ
	bool								bIsTablePass;					//�Ƿ���������
};


class CTableOption : public CSkinDialogEx
{
//	DECLARE_DYNAMIC(CTableOption)
public:
	tagTableParameter					m_TableParameter;				//���ӹ���
	CMyComboBox							m_IpNum;						//���ڵ�IP����
	CServerOption						* m_pServerOption;				//��������
	COLORREF							m_crOwnColor;			//�Լ���ɫ
	COLORREF							m_crFriendColor;			//������ɫ
	CSkinButton							m_btOk;
	CSkinButton							m_btCancle;	
	CSkinButton							m_btEdit;
	CSkinButton							m_btEdit1;

public:
	CTableOption(CWnd* pParent = NULL,CServerOption * pServerOption=NULL);   // standard constructor
	virtual ~CTableOption();

// Dialog Data
	enum { IDD = IDD_TABLE_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//��������
	void SaveSetting();
protected:
	//����
	afx_msg void OnOK();

	afx_msg  void OnBnClickedEdit();

	afx_msg void OnBnClickedEdit1();
	//
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};
