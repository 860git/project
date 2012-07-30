#ifndef MONEY_MANAGER_FILE
#define MONEY_MANAGER_FILE

#pragma once
#include "DlgStatus.h"
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyStatus
{
	enOperateMoneyStatus_NULL,
	enOperateStatus_Save,
	enOperateStatus_Get,
	enOperateStatus_Refresh
};

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneyManager : public CDialog//, CSkinWndObject
{
	//��������
protected:
	CDlgStatus					m_DlgStatus;					//״̬����
	CBrush						m_brBkground;						//����ˢ��
	enOperateMoneyStatus		m_enOperateStatus;					//״̬����
	
	LONGLONG					m_lTotalMoney;						//��Ǯ��
	LONGLONG					m_lRoomMoney;						//����Ǯ��
public:
		
	//�ؼ�����
protected:

	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:
	CComboBox					m_RoomSelect;
	CSkinButton					m_btSave;						//��
	CSkinButton					m_btGet;						//ȡ
	CSkinButton					m_btRefresh;						//ˢ��
	CMyComboBox					m_chkRoom;

	//����״̬
	void SetStatus(enOperateMoneyStatus enOperateMoneyStatus){m_enOperateStatus = enOperateMoneyStatus;}
	//��������
public:
	//���캯��
	CDlgMoneyManager(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneyManager();

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
	//������Ϣ
	afx_msg void OnClose();
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
	//��������
	bool ConnectServer();
	//���¿ؼ�
	void UpdateControls();
	//��ʽ������
	CString LongToString(LONGLONG lScore);
	//
	INT64 GetDlgItemInt64(UINT nID);


	afx_msg void OnBnClickedRefresh();

	//��Ϣ����
protected:
	
	//�滭����
	afx_msg void OnPaint();
	//��ɫ����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedGet();

	//���뷿��
	afx_msg void OnEnChangeRoom();
	//
	afx_msg void OnEnChangeNum();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif