#ifndef BANK_PASS_FILE_HEAD
#define BANK_PASS_FILE_HEAD
#pragma once

class CDlgBankPass : public CSkinDialogEx
{
	//��������
public:
	CString								m_strPassword;
	CString								m_strDescribe;
	CImage							    m_ImageBuffer;					//����λͼ
	CSkinImage                          m_ImageKuang;                   //��½��ͼ
	bool								m_bIsLeave;						//�Ƿ��뿪ע��
	TCHAR								m_szPassword1[PASS_LEN];		//��Ϸ����

	CPasswordControl					m_edUserPassword;				//�û�����
	CTime								m_tmLogin;						//��¼����ʱ��
	//�ؼ�����
private:
	CSkinButton							m_btOK;
	CSkinButton							m_btCancel;



public:
	CDlgBankPass(CWnd* pParent = NULL);
	virtual ~CDlgBankPass();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
          
public:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
		//ȡ����Ϣ
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgBankPass)
protected:
	virtual void OnOK();
};

#endif