#ifndef MONEY_CLOSE_FILE
#define MONEY_CLOSE_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneyClose : public CDialog//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;						//����ˢ��

public:
	CMyComboBox					m_boxTimeSecect;
	//�ؼ�����
protected:


public:
	
		//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	
	//��������
public:
	//���캯��
	CDlgMoneyClose(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneyClose();

	//��������
public:
	
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

	afx_msg void OnOk();

	

	



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif