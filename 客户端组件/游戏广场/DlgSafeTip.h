#ifndef SAFE_TIP_FILE
#define SAFE_TIP_FILE

#include "DlgUserManager.h"
#pragma once
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgSafeTip : public CSkinDialogEx//, CSkinWndObject
{
	//��������
protected:
	
	CBrush						m_brBkground;						//����ˢ��
	CSkinButton					m_btSet;
	CSkinButton					m_BtOk;
	CSkinButton					m_btCancle;

public:
	
	//�ؼ�����
protected:


public:
	CDlgUserManage			  m_DlgUserManager1;				
	
	
	//��������
public:
	//���캯��
	CDlgSafeTip(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgSafeTip();

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
	
	afx_msg void OnSet();

	



	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif