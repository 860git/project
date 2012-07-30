#include "stdafx.h"
#include"resource.h"
#include "DlgMoneyClose.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyClose, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDOK, OnOk)
	
	
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMoneyClose::CDlgMoneyClose(CWnd* pParent): CDialog(IDD_CLOSE_BANK_TIP, pParent)//, CSkinWndObject(this)
{
	//���ñ���

}

//�鹹����
CDlgMoneyClose::~CDlgMoneyClose()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//��������
void CDlgMoneyClose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SELECT, m_boxTimeSecect);
	
}

//��ʼ����
BOOL CDlgMoneyClose::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();

	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
	//SetWindowText(TEXT(""));
	
	
	//GetDlgItem(IDC_ZD)->EnableWindow(false);	
	//GetDlgItem(IDC_SELECT)->EnableWindow(false);
	//GetDlgItem(IDC_DESC)->EnableWindow(false);	
	//GetDlgItem(IDC_SELECT)->EnableWindow(TRUE);
	
	m_boxTimeSecect.Clear();
	m_boxTimeSecect.InsertString(0,"1");
	m_boxTimeSecect.InsertString(1,"3");
	m_boxTimeSecect.InsertString(2,"5");
	m_boxTimeSecect.InsertString(3,"10");
	m_boxTimeSecect.InsertString(4,"30");
	m_boxTimeSecect.InsertString(5,"50");
	m_boxTimeSecect.SetCurSel(0);

	if(g_GlobalOption.m_BankCloseMode==enBankRule_LK)
		SetButtonSelected(IDC_LK,true);
	else if(g_GlobalOption.m_BankCloseMode==enBankRule_ZD)
	{
		SetButtonSelected(IDC_ZD,true);
		CString strTime;
		strTime.Format("%d", g_GlobalOption.m_BankCloseTime);
		m_boxTimeSecect.SelectString(0, strTime);
	}
	else
		SetButtonSelected(IDC_LEAVE,true);

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgMoneyClose::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgMoneyClose::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("�˳�"));

	return;
}

//���¿ؼ�
void CDlgMoneyClose::UpdateControls()
{
	//��������
	UpdateData(false);

}

//��ɫ����
HBRUSH CDlgMoneyClose::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BGROUND_COLOR);
			return m_brBkground;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//��ʾ��Ϣ
void CDlgMoneyClose::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgMoneyClose::OnClose()
{
	CDialog::OnClose();
}

////////////////////////////////////////////////////////////////////////////////////
//�Ƿ�ѡ��
bool CDlgMoneyClose::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//ѡ�а�ť
void CDlgMoneyClose::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

void CDlgMoneyClose::OnOk()
{
	//����ģʽ
	if (IsButtonSelected(IDC_LK)) 
	{
		g_GlobalOption.m_BankCloseMode=enBankRule_LK;
	}
	else if (IsButtonSelected(IDC_ZD)) 
	{
		g_GlobalOption.m_BankCloseMode=enBankRule_ZD;
		CString strTime;
		GetDlgItemText(IDC_SELECT,strTime);
		g_GlobalOption.m_BankCloseTime = atoi(strTime.GetBuffer(0));
	}
	else 
		g_GlobalOption.m_BankCloseMode=enBankRule_LEAVE;
 
 	//g_GlobalOption.m_BankCloseTip = IsButtonSelected(IDC_LK);
	g_GlobalOption.m_bCloseQuery = IsButtonSelected(IDC_TIP);


	CDialog::OnOK();
}

