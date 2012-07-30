#ifndef USER_MANAGE_FILE
#define USER_MANAGE_FILE

#include "DlgUserInfoSample.h"
#include "DlgUserInfoXX.h"
#include "DlgUserInfoPass.h"
#include "DlgUserInfoLock.h"


#pragma once
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgUserManage : public CSkinDialogEx1//, CSkinWndObject
{
	//��������
protected:
	CBrush						m_brBkground;						//����ˢ��
	CPngImage					m_pngImageBack;						//����ͼƬ	

public:
	//CTreeCtrl					m_tree;
	CSkinButton					m_btOk;		

	CSkinButton					m_btSample;		
	CSkinButton					m_btXX;		
	CSkinButton					m_btLock;		
	CSkinButton					m_btPass;		
	
	
	CDlgUserSample				m_DlgUserSample;				
	CDlgUserXX					m_DlgUserXX;					
	CDlgUserLock				m_DlgUserLock;
	CDlgUserPass				m_DlgUserPass;
	
	//��������
public:
	//���캯��
	CDlgUserManage();
	//�鹹����
	virtual ~CDlgUserManage();

	void InitRoomName();

	//��������
protected:
	//��������
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ����
	virtual BOOL OnInitDialog();

	//��Ϣ����
protected:
	
	//�滭����
	afx_msg void OnPaint();
	//��ɫ����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

//	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	
	//������Ϣ
	afx_msg void OnClose();

	afx_msg void OnSample();
	afx_msg void OnXX();
	afx_msg void OnLock();
	afx_msg void OnPass();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	

	void OnClose1();
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif