#ifndef MONEY_GIVE_FILE
#define MONEY_GIVE_FILE

#pragma once
#include "DlgStatus.h"
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyGiveStatus
{
	enOperateMoneyGiveStatus_NULL,
	enOperateGiveStatus_Save,
	enOperateGiveStatus_Refresh
};

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneyGive : public CDialog//, CSkinWndObject
{
	//��������
protected:
	CDlgStatus					m_DlgStatus;					//״̬����
	CBrush						m_brBkground;						//����ˢ��
	enOperateMoneyGiveStatus	m_enOperateStatus;					//״̬����
	bool						m_bIsOk;						//ȷ����
	
	LONGLONG					m_lTotalMoney;						//��Ǯ��
	LONGLONG					m_lRoomMoney;						//����Ǯ��
public:
		
	//�ؼ�����
protected:

	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:
	CComboBox					m_UserSelect;
	CSkinButton					m_btSave;						//��
	CSkinButton					m_btRefresh;					//ˢ��
	CMyComboBox					m_chkUserID;			

public:
	//����״̬
	void SetStatus(enOperateMoneyGiveStatus enOperateMoneyGiveStatus){m_enOperateStatus = enOperateMoneyGiveStatus;}
	//��������
public:
	//���캯��
	CDlgMoneyGive(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneyGive();

	//��������
public:
	//��������
	bool SendData();
	//��ʼ������
	bool InitRoomName();
	//���ҷ���
	DWORD GetGameKind(CString m_strKindName);
	//����Ǯ��
	void SetMoney(LONGLONG TotalMoney,LONGLONG RoomMoney);

	//��������
protected:
	//��������
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ����
	virtual BOOL OnInitDialog();

	//��������
public:
	//��ʾ��Ϣ
	void ShowMessage(TCHAR *pMessage);
	//��ʾȷ��
	void ShowOk(LPCTSTR szTargetAccount,DWORD dwTargetUserID,LONGLONG lScore);
	//��������
	bool ConnectServer();
	//���¿ؼ�
	void UpdateControls();
	//������Ϣ
	virtual void OnClose();
	//��ʽ������
	CString LongToString(LONGLONG lScore);
	//
	afx_msg void OnBnClickedRefresh();
	//
	INT64 GetDlgItemInt64(UINT nID);

	//��Ϣ����
protected:
	
	//�滭����
	afx_msg void OnPaint();
	//��ɫ����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedSave();


	//���뷿��
	afx_msg void OnEnChangeUser();
	//
	afx_msg void OnEnChangeNum();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif