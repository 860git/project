#ifndef MONEY_PASS_FILE
#define MONEY_PASS_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyPassStatus
{
	enOperateMoneyPassStatus_NULL,
	enOperateEditStatus_Save,
};

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneyPass : public CDialog//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;						//����ˢ��

	enOperateMoneyPassStatus	m_enOperateStatus;					//״̬����

public:
		
	//�ؼ�����
protected:
	CSkinButton					m_btEdit;							//�༭��ť
	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:

	
	//��������
public:
	//���캯��
	CDlgMoneyPass(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneyPass();

	//��������
public:
	//��������
	bool SendData();
	//��ʼ������
	bool InitRoomName();

		//����״̬
	void SetStatus(enOperateMoneyPassStatus enOperateMoneyPassStatus){m_enOperateStatus = enOperateMoneyPassStatus;}
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
	//������Ϣ
	afx_msg void OnClose();

	//��Ϣ����
protected:
	
	//�滭����
	afx_msg void OnPaint();
	//��ɫ����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);



	afx_msg void OnBnClickedEdit();


	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif