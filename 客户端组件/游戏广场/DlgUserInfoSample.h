#ifndef USER_SAMPLE_FILE
#define USER_SAMPLE_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

enum enOperateUserSampleStatus
{
	enOperateUserSampleStatus_NULL,
	enOperateUserSampleStatus_Save,
	
};

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgUserSample : public CDialog//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;						//����ˢ��
	enOperateUserSampleStatus	m_enOperateStatus;					//״̬����
	CSkinButton					m_btEdit;							//�༭

public:
		
	//�ؼ�����
protected:

	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:
	
	CComboBox					m_UserSelect;
	//����״̬
	void SetStatus(enOperateUserSampleStatus enOperateUserSampleStatus){m_enOperateStatus = enOperateUserSampleStatus_NULL;}
	//��������
public:
	//���캯��
	CDlgUserSample(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgUserSample();

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