#ifndef LOCK_MACHINE_FILE_HEAD
#define LOCK_MACHINE_FILE_HEAD
#pragma once

class CDlgLockMachine : public CSkinDialogEx
{
	//��������
public:
	CString								m_strPassword;
	CString								m_strDescribe;
	bool								m_bLock;
    // CImage							    m_ImageBuffer;						//����λͼ
	//CSkinImage                          m_ImageKuang;                   //��½��ͼ
	 CSkinEdit							m_edUserPassword;				//�û�����
TCHAR								m_szPassword1[PASS_LEN];			//��Ϸ����
	//�ؼ�����
private:
	CSkinButton							m_btOK;
	CSkinButton							m_btCancel;
	

public:
	CDlgLockMachine(CWnd* pParent = NULL);
	virtual ~CDlgLockMachine();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
          
public:
	//�ػ���Ϣ
	//afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
		DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgLockMachine)
protected:
	virtual void OnOK();
};

#endif