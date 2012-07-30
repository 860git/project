#ifndef MONEY_RECORD_FILE
#define MONEY_RECORD_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

enum enOperateMoneyRecordStatus
{
	enOperateMoneyRecordStatus_NULL,
	enOperateRecordStatus_Save,
};


////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneyRecord : public CDialog//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;						//����ˢ��
	int							nPageNum;							//��ʾ��ҳ��
	int							nCurrentPage;						//��ǰҳ

	CSkinListCtrl				m_RecordListCtrl;		
	enOperateMoneyRecordStatus	m_enOperateStatus;					//״̬����
public:
	int								m_nCount;						//��¼��
	tagUserGiveRecord				GiveRecord[200];				//��¼

public:
		
	//�ؼ�����
protected:

	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ
public:
	
	CSkinButton					m_btFirstPage;
	CSkinButton					m_btEndPage;
	CSkinButton					m_btLastPage;
	CSkinButton					m_btNextPage;
	CSkinButton					m_btQuery;
	//��������
public:
	//���캯��
	CDlgMoneyRecord(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneyRecord();

	//��������
public:
	//��������
	bool SendData();
	//��ʼ������
	bool InitRoomName();

		//����״̬
	void SetStatus(enOperateMoneyRecordStatus enOperateMoneyRecordStatus){m_enOperateStatus = enOperateMoneyRecordStatus;}

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

	void InitTable();

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


	afx_msg void OnBnClickedGet();

	afx_msg void OnBnClickedFirst();
	afx_msg void OnBnClickedLast();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedEnd();



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif