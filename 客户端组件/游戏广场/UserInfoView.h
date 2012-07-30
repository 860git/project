#ifndef PLAZA_USER_INFO_VIEW_HEAD_FILE
#define PLAZA_USER_INFO_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include"SuoJi.h"
//////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class CUserInfoView : public CDialog
{
	friend class CPlazaViewItem;
	//״̬����
protected:
	bool							m_bShowInfo;						//��ʾ��־
	bool							m_bCreateFlag;						//������־
	UINT							m_nTitleHeight;						//����߶�
    
	//��Դ����
protected:
	CSkinImage						m_ImageTitleL;						//λͼ����
	CSkinImage						m_ImageTitleM;						//λͼ����
	CSkinImage						m_ImageTitleR;						//λͼ����
    CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageScore;						//����ͼƬ
	bool                            UpdateViewKZ;                       //
	CSkinImage						m_ImageMale;                        //����
	CSkinImage						m_ImageFemale;                      //Ů��
    
	CSkinImage						m_ImageMan;						    //λͼ����
	CSkinImage						m_ImageWoMan;						//λͼ����

	CRect							m_rcFace;							//ͷ��λ��
	bool							m_bMouseOnFace;						//���ͣ��
    
    
	//��������
public:
	//���캯��
	CUserInfoView();
	//��������
	virtual ~CUserInfoView();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK() { return; };
	//ȡ������
	virtual VOID OnCancel() { return; };
public:
   
	//���ܺ���
public:
	//����߶�
	UINT GetTitleHeight();
	//������Ϣ
	VOID UpdateUserInfo();
	//��ʾ��Ϣ
	VOID ShowUserInfo(bool bShowInfo);
	//������Դ
	bool UpdateSkinResource();

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif