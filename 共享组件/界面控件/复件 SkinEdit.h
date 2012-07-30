#pragma once

#ifndef SKIN_EDIT_HEAD_FILE
#define SKIN_EDIT_HEAD_FILE

#include "SkinControls.h"
#include "SkinResourceManager.h"
#include "SkinImage.h"
//��������
//#define SKIN_EDIT_CRFOCUSTX			TEXT("�༭�򽹵���ɫ")				// COLORREF ���� ����д��
//#define SKIN_EDIT_CRFOCUSBK			TEXT("�༭�򽹵㱳��ɫ")			// COLORREF ���� ����д��
//#define SKIN_EDIT_CRNOFOCUSTX		TEXT("�༭��������ɫ")				// COLORREF ���� ����д��
//#define SKIN_EDIT_CRNOFOCUSBK		TEXT("�༭����������ɫ")			// COLORREF ���� ����д��
//#define SKIN_EDIT_CRDISFOCUSTX		TEXT("�༭���ֹ��ɫ")				// COLORREF ���� ����д��
//#define SKIN_EDIT_CRDISFOCUSBK		TEXT("�༭���ֹ����ɫ")			// COLORREF ���� ����д��

//////////////////////////////////////////////////////////////////////////

//�༭������
class CSkinEditAttribute : public CSkinResourceManager
{
	friend class CSkinEdit;

	//���ò���
public:
	COLORREF							m_crFocusTX;					//������ɫ
	COLORREF							m_crFocusBK;					//������ɫ
	COLORREF							m_crNoFocusTX;					//������ɫ
	COLORREF							m_crNoFocusBK;					//������ɫ
	COLORREF							m_crDisFocusTX;					//������ɫ
	COLORREF							m_crDisFocusBK;					//������ɫ

	//GDI ����
public:
	CBrush								m_brFocus;						//����ˢ
	CBrush								m_brNoFocus;					//����ˢ
	CBrush								m_brDisbale;					//����ˢ
	CSkinImage                          m_ImageBottom;					//����ͼƬ
	//��������
protected:
	//���캯��
	CSkinEditAttribute();
	//��������
	virtual ~CSkinEditAttribute();

	//�ӿں���
public:
	//��������
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//�༭��
//class SKIN_CONTROL_CLASS CSkinEdit : public CEdit
//{
//	//��������
//protected:
//	bool							m_bFocus;						//�����־
//	bool							m_bHovering;					//������־
//	//��̬����
//public:
//	static CSkinEditAttribute		m_SkinAttribute;				//��������
//
//	//��������
//public:
//	//���캯��
//	CSkinEdit();
//	//��������
//	virtual ~CSkinEdit();
//
//	//��ѯ����
//public:
//	//��ȡ���Խӿ�
//	static CSkinEditAttribute * GetSkinAttribute() { return &m_SkinAttribute; }
//
//	//��Ϣ���� 
//protected:
//	//�滭�ؼ�
//	//afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
//	//�õ�����
//	afx_msg void OnSetFocus(CWnd * pOldWnd);
//	//ʧȥ����
//	afx_msg void OnKillFocus(CWnd * pNewWnd);
//	//����ƶ�
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	//����뿪
//	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
//	afx_msg void OnPaint();
//	//��Ϣӳ��
//protected:
//	DECLARE_MESSAGE_MAP()
//	DECLARE_DYNAMIC(CSkinEdit)
//};

//�༭��
class SKIN_CONTROL_CLASS CSkinEdit : public CEdit
{
	//��������
protected:
	bool							m_bFocus;						//�����־

	//��̬����
public:
	static CSkinEditAttribute		m_SkinAttribute;				//��������

	//��������
public:
	//���캯��
	CSkinEdit();
	//��������
	virtual ~CSkinEdit();

	//��ѯ����
public:
	//��ȡ���Խӿ�
	static CSkinEditAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//��Ϣ���� 
protected:
	//�滭�ؼ�
	afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
	//�õ�����
	afx_msg void OnSetFocus(CWnd * pOldWnd);
	//ʧȥ����
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//�ػ���Ϣ
    VOID OnNcPaint();

protected:
	//�滭�ؼ�
	VOID DrawControlView(CDC * pDC, COLORREF crColorBorad, COLORREF crColorBK);


	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinEdit)
};

//�༭��
class SKIN_CONTROL_CLASS CSkinEditEx : public CSkinEdit
{
	//��ˢ����
public:
	CBrush							m_brEnable;							//��ˢ��Դ
	CBrush							m_brDisable;						//��ˢ��Դ

	//������ɫ
public:
	COLORREF						m_crEnableBK;						//������ɫ
	COLORREF						m_crDisableBK;						//������ɫ

	//������ɫ
public:
	COLORREF						m_crEnableText;						//������ɫ
	COLORREF						m_crDisableText;					//������ɫ

	//�߿���ɫ
public:
	COLORREF						m_crEnableBorad;					//�߿���ɫ
	COLORREF						m_crDisableBorad;					//�߿���ɫ

	//��������
public:
	//���캯��
	CSkinEditEx();
	//��������
	virtual ~CSkinEditEx();

	//���ܺ���
public:
	//������ɫ
	VOID SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad);
	//������ɫ
	VOID SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad);

	
	//��Ϣ���� 
protected:
	//�ػ���Ϣ
    VOID OnNcPaint();
	//�滭�ؼ�
	HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////

class SKIN_CONTROL_CLASS CMyComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMyComboBox)
	//��������
protected:
	BYTE							m_bFocus;						//�����־
//	static CSkinImage				m_ImageBottom;						//����ͼƬ
	int								iWidth;
	int								iHeight;

public:
	CMyComboBox();
	virtual ~CMyComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnPaint();
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

//////////////////////////////////////////////////////////////////////////

#endif