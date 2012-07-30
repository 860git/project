#pragma once

#ifndef SKIN_BUTTON_HEAD_FILE
#define SKIN_BUTTON_HEAD_FILE

#include "SkinImage.h"
#include "SkinControls.h"
#include "SkinResourceManager.h"
#include "PngButton\\CGdiPlusBitmap.h"
#include "PngButton\\MemDC2.h"
#include "PngImage.h"
//////////////////////////////////////////////////////////////////////////

//�궨��
#define DEF_TEXT_FRAME_COLOR			RGB(255,255,255)				//Ĭ����ɫ
#define DEF_TEXT_COLOR					RGB(10,10,10)					//Ĭ����ɫ
#define DEF_TEXT_COLOR_EX				RGB(10,10,10)					//Ĭ����ɫ

//��������
//#define SKIN_BUTTON_CRTEXTCOLOR			TEXT("��ť������ɫ")			// COLORREF ���� ����д��
//#define SKIN_BUTTON_STRIMAGEPATH		TEXT("��ť����ͼ")				// CString ���� ����д��

#define	WM_BT_LBUTTONDBLCLK				WM_USER+400

//////////////////////////////////////////////////////////////////////////

//��ť������
class CSkinButtonAttribute : public CSkinResourceManager
{
	friend class CSkinButton;

	//���ò���
public:
	COLORREF							m_crTextColor;					//������ɫ

	//GDI ����
public:
	CSkinImage							m_ImageBack;					//��ťλͼ
	tagImageLoadInfo					LoadInfoPng;					//Ĭ��PNG��ť
	tagImageLoadInfo					LoadInfoCheck;					//Ĭ��ѡ��

	//��������
protected:
	//���캯��
	CSkinButtonAttribute();
	//��������
	virtual ~CSkinButtonAttribute();

	//�ӿں���
public:
	//��������
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//��ť��
class SKIN_CONTROL_CLASS CSkinButton : public CButton
{
	friend class CRgnButton;

	//��������
protected:
	bool								m_bShowTextFrame;				//����߿�
	bool								m_bExpand;						//�Ƿ�����
	bool								m_bHovering;					//������־
	HICON								m_hIcon;						//��ťͼ��
	COLORREF							m_crTextColor;					//������ɫ
	COLORREF							m_crTextFrameColor;				//������ɫ
	CSkinImage							m_ImageBack;					//��ťλͼ
	LOGFONT								m_BtnFont;						//��ť����

	//�ӿ�ָ��
protected:
	IMouseEvent							* m_pIMouseEvent;				//����¼�

	//��ť������
public:
	static CSkinButtonAttribute			m_SkinAttribute;				//��ť����
	

	//��������
public:
	//���캯��
	CSkinButton();
	//��������
	virtual ~CSkinButton();

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Ĭ�ϻص�����
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��ȡ���ص��ӿ�
	IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//�������ص��ӿ�
	void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };

	//���ú���
public:
	//����λ��
	bool FixButtonSize();
	//����ͼ��
	bool SetButtonIcon(HICON hIcon);
	//������ɫ
	bool SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//��������
	bool SetTextFont(CFont* pFont, COLORREF crTextColor=DEF_TEXT_COLOR,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//����λͼ
	bool SetButtonImage(LPCTSTR pszFileName, bool bExpandImage);
	//����λͼ
	bool SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, bool bExpandImage);
	//Ĭ�ϰ�ť
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//��ѯ����
public:
	//��ȡͼ��
	HICON GetButtonIcon() { return m_hIcon; }
	//��ȡ��ɫ
	COLORREF GetButtonColor() { return m_crTextColor; }
	//��ȡλͼ����
	bool GetButtonLoadInfo(tagImageLoadInfo & LoadInfo);
	//��ȡ���Խӿ�
	static CSkinButtonAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//˫���¼�
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinButton)
};

//////////////////////////////////////////////////////////////////////////

//����ť��
class SKIN_CONTROL_CLASS CRgnButton : public CButton
{
	//��������
protected:
	bool								m_bHovering;					//������־
	CRgn								m_ButtonRgn;					//��������
	COLORREF							m_crTrans;						//͸����ɫ
	COLORREF							m_crTextColor;					//������ɫ
	CSkinImage							m_ImageBack;					//����λͼ

	//�ӿ�ָ��
protected:
	IMouseEvent							* m_pIMouseEvent;				//����¼�

	//��������
public:
	//���캯��
	CRgnButton();
	//��������
	virtual ~CRgnButton();

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Ĭ�ϻص�����
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//����λͼ
	bool LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans);
	//����λͼ
	bool LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans);
	//��ȡ���ص��ӿ�
	IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//�������ص��ӿ�
	void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };

	//�ڲ����� 
private:
	//��������
	bool CreateControlRgn(COLORREF crTrans);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRgnButton)
};


// CZLButton1 ��֧��PNG��ʽ��ť��֧�ִ���Դ����ͼƬ�Լ����ⲿ�ļ�����ͼƬ
class SKIN_CONTROL_CLASS CPngButton : public CButton
{
	//��Ա����
private:
	CDC										m_dcBk;							//����DC
	CDC										m_dcStd;						//��׼
	CDC										m_dcStdP;						//��׼����
	CDC										m_dcStdH;						//��׼����
	CDC										m_dcAlt;						//����
	CDC										m_dcAltP;						//���水��
	CDC										m_dcAltH;						//�����ж�
	CDC										m_dcGS;							//��ɫ��ť
	CDC*									m_pCurBtn;						//��ǰ��ť

	//��Ա����
protected:
	BOOL									m_bHaveAltImage;				//�л�ͼ
	BOOL									m_bHaveBitmaps;					//λͼ
	BOOL									m_bIsDisabled;					//�ر�
	BOOL									m_bIsToggle;					//����
	BOOL									m_bIsHovering;					//�������
	BOOL									m_bIsTracking;					//������
	int										m_nCurType;						//��ǰ����
	CGdiPlusBitmapResource					m_pAltImage;					//�л�ͼ
	CGdiPlusBitmapResource					m_pStdImage;					//��׼ͼ
	CString									m_tooltext;						//��ʾ��Ϣ
	CToolTipCtrl							m_pToolTip;						//��ʾ����

	//��������
public:
	//���캯��
	CPngButton();
	//��������
	virtual ~CPngButton();

	//��������
public:
	//ö������
	enum	{
				STD_TYPE	= 0,		//��׼����
				ALT_TYPE,				//��������
				DIS_TYPE				//��Ч����
			};

	//���ð�ť����[0Ϊ��׼����,1Ϊ�л�����,2Ϊ��Ч����]
	void SetButtonType(int type);
	//���ⲿ�ļ����ر�׼ͼƬ[����·���ļ���]
	BOOL LoadStdImage(LPCTSTR pFilePath,bool bFixSize=true); 
	//���ⲿ�ļ������л�ͼƬ[����·���ļ���]
	BOOL LoadAltImage(LPCTSTR pFilePath,bool bFixSize=true);
	//���ر�׼ͼƬ[��ԴID,��Դ����]
	BOOL LoadStdImage(UINT id, LPCTSTR pType);
	//�����л�ͼƬ[��ԴID,��Դ����]
	BOOL LoadAltImage(UINT id, LPCTSTR pType,HINSTANCE hInstance);
	//���ð�ť[���Ϊfalse, �رհ���״̬����ʾ��Ч״̬]
	void EnableButton(BOOL bEnable = TRUE);
	//���ÿ���ģʽ[���ΪTRUE,ÿ�ΰ��°�ť���ڱ�׼���ͺ��л������з����л�]
	void EnableToggle(BOOL bEnable = TRUE);
	//�ж��Ƿ�ر�[���ذ�ť�Ƿ�رյ�״̬,TRUEΪ�ر�,FALSEΪ����]
	BOOL IsDisabled(void);
	//���ư�ť
	void SetBkGnd(CDC* pDC);
	//���ð�ť��ʾ����[��ʾ����,Ĭ�ϼ���]
	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	//���ð�ť��ʾ����[��ԴID,Ĭ�ϼ���]
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	//��������
	void SetHorizontal(bool ImagesAreLaidOutHorizontally = FALSE);
	//ɾ����ť��ʾ
	void DeleteToolTip();
	//��ť���
	int GetWidth();
	//ˢ�°�ť
	void UpdateBk();

	//���ܺ���
protected:
	//���Ʊ���
	inline void PaintBk(CDC* pDC);
	//���ư�ť
	inline void PaintBtn(CDC* pDC);
	//��ʼ����ʾ
	void	InitToolTip();					
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//��Ϣ����
protected:
	//�ؼ���ɫ
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//�������
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//�������ָ��
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  
	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
class  SKIN_CONTROL_CLASS CCheckButton:public CButton
{
protected:
	bool									m_bChecked;
	BOOL									m_bHovering;					//�������
	COLORREF								m_crTextColor;						//�ı���ɫ
	COLORREF								m_crBackColor;					//������ɫ
	//CSkinImage								m_ImageBack;					//����λͼ
	CPngImage								m_pngBack;						//����PNG

public:
	static CSkinButtonAttribute				m_SkinAttribute;				//��ť����

public:

	CCheckButton();
	virtual ~CCheckButton();

public: 

	//��ȡ����״̬
	void SetCheck(int nCheck);
	int GetCheck();
	//������ɫ
	void SetTextColor(COLORREF crText, COLORREF crBack=DEF_TEXT_FRAME_COLOR);
	//����λͼ
	bool SetCheckImage(LPCTSTR lpResName, HINSTANCE hInstance);
	//����ͼƬ
	bool SetCheckImage(LPCTSTR pszFileName);

protected:
	//��ȡѡ��״̬
	bool GetCheckStat() {return m_bChecked;}
	
protected:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//�ؼ����໯
	virtual void PreSubclassWindow();

	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	afx_msg void OnMouseMove(UINT, CPoint);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//�Ҽ�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP();

};
class SKIN_CONTROL_CLASS CGdiButton:public CButton
{
	
	//��������
protected:
	bool								m_bShowTextFrame;				//����߿�
	bool								m_bExpand;						//�Ƿ�����
	bool								m_bHovering;					//������־
	HICON								m_hIcon;						//��ťͼ��
	COLORREF							m_crTextColor;					//������ɫ
	COLORREF							m_crTextFrameColor;				//������ɫ					//��ťλͼ
	CPngImage							m_pngBack;						//PNG��ťͼ
	LOGFONT								m_BtnFont;						//��ť����
	CDC										m_dcBk;							//����DC

	//�ӿ�ָ��
protected:
	IMouseEvent							* m_pIMouseEvent;				//����¼�

	//��������
public:
	//���캯��
	CGdiButton();
	//��������
	virtual ~CGdiButton();

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Ĭ�ϻص�����
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//���ú���
public:
	//����λ��
	bool FixButtonSize();
	//����ͼ��
	bool SetButtonIcon(HICON hIcon);
	//������ɫ
	bool SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//��������
	bool SetTextFont(CFont* pFont, COLORREF crTextColor=DEF_TEXT_COLOR,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//����λͼ
	bool SetButtonImage(LPCTSTR pszFileName, bool bExpandImage);
	//����λͼ
	bool SetButtonImage(LPCTSTR lpResName, HINSTANCE hInstance, bool bExpandImage);
	//Ĭ�ϰ�ť
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//��ȡ���ص��ӿ�
	IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//�������ص��ӿ�
	void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };


	//��ѯ����
public:
	//��ȡͼ��
	HICON GetButtonIcon() { return m_hIcon; }
	//��ȡ��ɫ
	COLORREF GetButtonColor() { return m_crTextColor; }
	//���Ʊ���
	inline void PaintBk(CDC* pDC);
	//���ư�ť
	void SetBkGnd(CDC* pDC);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//˫���¼�
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CGdiButton)
};
#endif