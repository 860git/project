#pragma once

//////////////////////////////////////////////////////////////////////////



//�һ����ֶ� 
class CBankStorage1 : public CSkinDialogEx1
{
	//��������

	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
    CWebBrowser							m_BrowerAD;						//�������
    CImage							    m_ImageBuffer;						//����λͼ
	CSkinImage                          m_ImageKuang;                   //��½��ͼ
	//��������
public:
	//���캯��
	CBankStorage1(const BYTE bGameAction); 
	//��������
	virtual ~CBankStorage1();
	

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
public:
	void Initm_BrowerAD();
	//��Ϣ����
public:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
    
	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
