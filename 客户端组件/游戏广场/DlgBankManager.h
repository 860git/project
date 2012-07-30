#ifndef MONEY_BANK_FILE
#define MONEY_BANK_FILE

#include "DlgMoneyManager.h"
#include "DlgMoneyGive.h"
#include "DlgMoneyRecord.h"
#include "DlgMoneyPass.h"
#include "DlgMoneySelect.h"
#include "DlgMoneyClose.h"

#pragma once
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgMoneyBank : public CSkinDialogEx1//, CSkinWndObject
{
	//��������
protected:
	CBrush						m_brBkground;						//����ˢ��
	CPngImage					m_pngImageBack;						//����ͼƬ	

public:
	//CTreeCtrl					m_tree;
	CSkinButton					m_btOk;		
	CSkinButton					m_btCancle;							//�رհ�ť
	CSkinButton					m_btGive;		
	CSkinButton					m_btRecord;		
	CSkinButton					m_btSave;		
	CSkinButton					m_btPass;		
	CSkinButton					m_btSelect;		
	
	CDlgMoneyManager			m_DlgMoneyManager;				//���ֶ���ȡ		//�¼ӹ���
	CDlgMoneyGive				m_DlgMoneyGive;					//���ֶ�����		//�¼ӹ���
	CDlgMoneyRecord				m_DlgMoneyRecord;
	CDlgMoneySelect				m_DlgMoneySelect;
	CDlgMoneyPass				m_DlgMoneyPass;

	CDlgMoneyClose				m_DlgMoneyClose;
	//��������
public:
	//���캯��
	CDlgMoneyBank(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgMoneyBank();

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

	afx_msg void OnGive();
	afx_msg void OnRecord();
	afx_msg void OnSave();
	afx_msg void OnPass();
	afx_msg void OnSelect();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnClose1();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif