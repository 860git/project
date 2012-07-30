#ifndef USER_LOCK_FILE
#define USER_LOCK_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

enum enOperateUserLockStatus
{
	enOperateUserLockStatus_NULL,
	enOperateUserLockStatus_Save,
	
};

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgUserLock : public CDialog//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;					//����ˢ��
	enOperateUserLockStatus		m_enOperateStatus;					//״̬����
	CSkinButton					m_btEdit;						//�༭��

public:
		
	//�ؼ�����
protected:

	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:
	

	//����״̬
	void SetStatus(enOperateUserLockStatus enOperateUserLockStatus){m_enOperateStatus = enOperateUserLockStatus_NULL;}
	//��������
public:
	//���캯��
	CDlgUserLock(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgUserLock();

	//��������
public:
	//��������
	bool SendData();
	//��ʼ������
	bool InitRoomName();
	

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


	//��Ϣ����
protected:
	
	//�滭����
	afx_msg void OnPaint();
	//��ɫ����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//������Ϣ
	afx_msg void OnClose();

	afx_msg void OnOK();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif