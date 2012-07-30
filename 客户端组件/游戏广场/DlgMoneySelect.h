#ifndef MONEY_SELECT_FILE
#define MONEY_SELECT_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneySelect : public CDialog//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;						//����ˢ��

public:
	CMyComboBox					m_boxTimeSecect;
	CSkinButton					m_btEdit;
	//�ؼ�����
protected:

	//ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:
	
		//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	
	//��������
public:
	//���캯��
	CDlgMoneySelect(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneySelect();

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

	afx_msg void OnBnClickedQuery();



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif