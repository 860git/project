#include "Stdafx.h"
#include "Resource.h"
#include "TableFrame.h"
#include "GlobalUnits.h"
#include "..\..\������\Include\GameFrameView.h"

//////////////////////////////////////////////////////////////////////////

//��ʾ��ʽ
#define	NT_RIGHT						0x01								//�Ҷ���
#define	NT_CENTER						0x02								//�ж���
#define	NT_LEFT							0x04								//�����
#define	NT_TOP							0x10								//�϶���
#define	NT_VCENTER						0x20								//�ж���
#define	NT_BOTTOM						0x40								//�¶���

//////////////////////////////////////////////////////////////////////////

#define RESOURCE_VER					1									//��Դ�汾

//��Դ���ýṹ
struct tagTableResourceInfo
{
	COLORREF							crName;								//������ɫ
	COLORREF							crNote;								//��ע��ɫ
	COLORREF							crTableID;							//������ɫ
	POINT								ptLock;								//��λ��
	RECT								rcNote;								//��עλ��
	RECT								rcTableID;							//����λ��
	RECT								rcName[MAX_CHAIR];					//����λ��
	RECT								rcChair[MAX_CHAIR];					//����λ��
	POINT								ptReady[MAX_CHAIR];					//׼��λ��
	POINT								ptPlayer[MAX_CHAIR];					//׼��λ��
	INT									nDrawStyle[MAX_CHAIR];				//�����ʽ
};

//��Դ���ýṹ
struct tagTableResourceInfo_Normal
{
	COLORREF							crName;								//������ɫ
	COLORREF							crNote;								//��ע��ɫ
	COLORREF							crTableID;							//������ɫ
	POINT								ptLock;								//��λ��
	RECT								rcNote;								//��עλ��
	RECT								rcTableID;							//����λ��
	RECT								rcName[MAX_CHAIR_NORMAL];			//����λ��
	RECT								rcChair[MAX_CHAIR_NORMAL];			//����λ��
	POINT								ptReady[MAX_CHAIR_NORMAL];			//׼��λ��
	INT									nDrawStyle[MAX_CHAIR_NORMAL];		//�����ʽ
};

//�����ļ��ṹ
struct tagTableConfigFile
{
	WORD								wFileSize;							//�ļ���С
	WORD								wStructVer;							//�ṹ�汾
	tagTableResourceInfo				TableResourceInfo;					//���ò���
};

//�����ļ��ṹ
struct tagTableConfigFile_Normal
{
	WORD								wFileSize;							//�ļ���С
	WORD								wStructVer;							//�ṹ�汾
	tagTableResourceInfo_Normal			TableResourceInfo;					//���ò���
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableResource::CTableResource()
{
	//��־����
	m_bDZShowHand=false;

	//���ñ���
	m_wWidth=0;
	m_wHeight=0;
	m_crName=RGB(0,0,0);
	m_crNote=RGB(0,0,0);
	m_crTableID=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	memset(m_nDrawStyle,0,sizeof(m_nDrawStyle));
	return;
}

//��������
CTableResource::~CTableResource()
{
	DestroyResource();
	return;
}

bool CTableResource::LoadFromFilesSize(bool bHideInfo,LPCTSTR pszGameName, WORD wChairCount, WORD wKindID)
{

	return true;
}
//������Դ
bool CTableResource::LoadFromFiles(bool bHideInfo,LPCTSTR pszGameName, WORD wChairCount, WORD wKindID)
{
	try
	{
		//������Դ
		CFile ResourceFile;
		TCHAR szResourcePath[MAX_PATH];
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TableFrame.DAT"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		if (ResourceFile.Open(szResourcePath,CFile::modeRead,NULL)==FALSE) throw 0;

		TCHAR	 m_szConfigFileName[MAX_PATH]=TEXT("");			
		
	   _snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s%s\\table.ini"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
       
		//��ȡ����
		tagTableConfigFile TableConfig;
		ULONGLONG uLong=ResourceFile.GetLength();
		switch (uLong)
		{
		case sizeof(tagTableConfigFile):
			{
				UINT uReadCount=ResourceFile.Read(&TableConfig,sizeof(TableConfig));
				if (uReadCount!=sizeof(TableConfig)) throw 0;
				if (TableConfig.wFileSize!=sizeof(tagTableConfigFile)) throw 0;
				break;
			}
		case sizeof(tagTableConfigFile_Normal):
			{
				tagTableConfigFile_Normal TableConfig_Normal;
				UINT uReadCount=ResourceFile.Read(&TableConfig_Normal,sizeof(TableConfig_Normal));
				if (uReadCount!=sizeof(TableConfig_Normal)) throw 0;
				if (TableConfig_Normal.wFileSize!=sizeof(TableConfig_Normal)) throw 0;
				if (TableConfig_Normal.wStructVer!=1) throw 0;
                
				TableConfig.TableResourceInfo.crName=TableConfig_Normal.TableResourceInfo.crName;
				TableConfig.TableResourceInfo.crNote=TableConfig_Normal.TableResourceInfo.crNote;
				TableConfig.TableResourceInfo.crTableID=TableConfig_Normal.TableResourceInfo.crTableID;
				TableConfig.TableResourceInfo.ptLock=TableConfig_Normal.TableResourceInfo.ptLock;
				TableConfig.TableResourceInfo.rcNote=TableConfig_Normal.TableResourceInfo.rcNote;
				TableConfig.TableResourceInfo.rcTableID=TableConfig_Normal.TableResourceInfo.rcTableID;
				for (int i=0;i<8;i++)
				{
					TableConfig.TableResourceInfo.rcName[i]=TableConfig_Normal.TableResourceInfo.rcName[i];
					TableConfig.TableResourceInfo.rcChair[i]=TableConfig_Normal.TableResourceInfo.rcChair[i];
					TableConfig.TableResourceInfo.ptReady[i]=TableConfig_Normal.TableResourceInfo.ptReady[i];
					TableConfig.TableResourceInfo.nDrawStyle[i]=TableConfig_Normal.TableResourceInfo.nDrawStyle[i];
				}
				break;
			}
		default: throw 0;
		}
        
		
		  for (int i=0;i<wChairCount;i++)
				{
					TCHAR Seatname[MAX_PATH];
		           _snprintf(Seatname,sizeof(Seatname),TEXT("seat%d"),i);
				   int Number=0;
				   //����λ��
					TableConfig.TableResourceInfo.rcChair[i].left=GetPrivateProfileInt(Seatname, TEXT("bound_left"), Number, m_szConfigFileName);
					TableConfig.TableResourceInfo.rcChair[i].top=GetPrivateProfileInt(Seatname, TEXT("bound_top"), Number, m_szConfigFileName);
					TableConfig.TableResourceInfo.rcChair[i].right=GetPrivateProfileInt(Seatname, TEXT("bound_right"), Number, m_szConfigFileName);
					TableConfig.TableResourceInfo.rcChair[i].bottom=GetPrivateProfileInt(Seatname, TEXT("bound_bottom"), Number, m_szConfigFileName);
                   //����λ��
					TableConfig.TableResourceInfo.rcName[i].left=GetPrivateProfileInt(Seatname, TEXT("name_x"), Number, m_szConfigFileName);
					TableConfig.TableResourceInfo.rcName[i].top=GetPrivateProfileInt(Seatname, TEXT("name_y"), Number, m_szConfigFileName);
                        
					TableConfig.TableResourceInfo.ptReady[i].x=GetPrivateProfileInt(Seatname, TEXT("ready_x"), Number, m_szConfigFileName);
					TableConfig.TableResourceInfo.ptReady[i].y=GetPrivateProfileInt(Seatname, TEXT("ready_y"), Number, m_szConfigFileName);
                    
					TableConfig.TableResourceInfo.ptPlayer[i].x=GetPrivateProfileInt(Seatname, TEXT("player_x"), Number, m_szConfigFileName);
					TableConfig.TableResourceInfo.ptPlayer[i].y=GetPrivateProfileInt(Seatname, TEXT("player_y"), Number, m_szConfigFileName);
					
				}
	    
	
		if (lstrcmp(pszGameName,TEXT("DZShowHand"))==0)
		{
			m_bDZShowHand=true;
			_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\Player.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
			m_ImagePeople.Load(szResourcePath);
			if (m_ImagePeople.IsNull()) throw 0;
		}

		//�������ӱ��
		m_ImageNumBack.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_NUMBACK);
		if (m_ImageNumBack.IsNull()) throw 0;
       
		//������λͼ
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\Locker.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageLock.Load(szResourcePath);
		if (m_ImageLock.IsNull()) 
		{
			m_ImageLock.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_LOCK);
			if (m_ImageLock.IsNull()) throw 0;
		}
		if (m_ImageJinRu.IsNull()) 
		{
			m_ImageJinRu.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_JINRU);
			if (m_ImageJinRu.IsNull()) throw 0;
		}
		//����׼����־
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\UserReady.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageReady.Load(szResourcePath);
		if (m_ImageReady.IsNull()) 
		{
			m_ImageReady.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_READY);
			if (m_ImageReady.IsNull()) throw 0;
			//throw 0;
		}

		//��������ͼ��
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TableStart.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageStart.Load(szResourcePath);
		if (m_ImageStart.IsNull()) 
		{
		//	m_ImageStart.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_READY);
		//	if (m_ImageStart.IsNull()) throw 0;
			throw 0;
		}

		//��������ͼ��2
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TableFree.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageFree.Load(szResourcePath);
		if (m_ImageFree.IsNull()) 
		{
			throw 0;
		}

		//���ؿ�ʼ��־
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TablePlaying.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImagePlay.Load(szResourcePath);
		if (m_ImagePlay.IsNull()) throw 0;
		
		//��������λͼ
		_snprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TableNormal.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageTable.Load(szResourcePath);
		if (m_ImageTable.IsNull()) throw 0;

		//////////////////////////////////////////////////////////////////////////
		//��ȡͼƬ��Ϣ

		for(int i=0;i<4;i++)
		{ 
			for(int j=0;j<17;j++)
			{
				TCHAR szPlayrPath[MAX_PATH];
				_snprintf(szPlayrPath,sizeof(szPlayrPath),szResPath("Room\\TableRes\\IDB_USER_%d.png"),i+j*4+1);
				//m_ImageGrilPlayr3[i][j].LoadImage(AfxGetInstanceHandle(),szPlayrPath );
				m_ImageBoyPlayer1[i][j].LoadImage(szPlayrPath );
			}
		}

		for(int i=0;i<4;i++)
		{ 
			for(int j=0;j<17;j++)
			{
				TCHAR szPlayrPath[MAX_PATH];
				_snprintf(szPlayrPath,sizeof(szPlayrPath),szResPath("Room\\TableRes\\IDB_USER_%d.png"),i+j*4+1+68);
				//m_ImageGrilPlayr3[i][j].LoadImage(AfxGetInstanceHandle(),szPlayrPath );
				m_ImageBoyPlayer2[i][j].LoadImage(szPlayrPath);
			}
		}

		for(int i=0;i<4;i++)
		{ 
			for(int j=0;j<17;j++)
			{
				TCHAR szPlayrPath[MAX_PATH];
				_snprintf(szPlayrPath,sizeof(szPlayrPath),szResPath("Room\\TableRes\\IDB_USER_%d.png"),i+j*4+1+68*2);
				//m_ImageGrilPlayr3[i][j].LoadImage(AfxGetInstanceHandle(),szPlayrPath );
				m_ImageGrilPlayer1[i][j].LoadImage(szPlayrPath );
			}
		}

		for(int i=0;i<4;i++)
		{ 
			for(int j=0;j<17;j++)
			{
				TCHAR szPlayrPath[MAX_PATH];
				_snprintf(szPlayrPath,sizeof(szPlayrPath),szResPath("Room\\TableRes\\IDB_USER_%d.png"),i+j*4+1+68*3);
				//m_ImageGrilPlayr3[i][j].LoadImage(AfxGetInstanceHandle(),szPlayrPath );
				m_ImageGrilPlayer2[i][j].LoadImage(szPlayrPath );
			}
		}


		//��ȡ����
		  m_wWidth=m_ImageTable.GetWidth();
		  m_wHeight=m_ImageTable.GetHeight();
		if(wChairCount<8)
			//m_crBackGround=m_ImageTable.GetPixel(2,2);
			m_crBackGround = RGB(24,49,57);
		else
        m_crBackGround=m_ImageTable.GetPixel(0,0);
       
		//���ò���
		tagTableResourceInfo * pResourceInfo=&TableConfig.TableResourceInfo;
		m_ptLock=pResourceInfo->ptLock;
		m_rcNote=pResourceInfo->rcNote;
		m_crName=pResourceInfo->crName;
		m_crNote=pResourceInfo->crNote;
		m_crTableID=pResourceInfo->crTableID;
		m_rcTableID=pResourceInfo->rcTableID;
        
		m_rcTableID.left=105;
		m_rcTableID.top=200;
		m_rcTableID.right=m_rcTableID.left+20;
		m_rcTableID.bottom=m_rcTableID.top+10;
	
		for (WORD i=0;i<MAX_CHAIR;i++)
		{
			m_ptReady[i]=pResourceInfo->ptReady[i];
			m_rcName[i]=pResourceInfo->rcName[i];
			m_rcChair[i]=pResourceInfo->rcChair[i];
			m_nDrawStyle[i]=pResourceInfo->nDrawStyle[i];
            m_ptPalyer[i]=pResourceInfo->ptPlayer[i];
			//�޸�����--������Ϸһ�����
			if (wChairCount>MAX_CHAIR_NORMAL)
			{
				{
				m_rcChair[0].left=0;
				m_rcChair[0].right=1024;
				m_rcChair[0].top=0;
				m_rcChair[0].bottom=1024;
				}
			}

		}

		return true;
	}
	catch (...) { DestroyResource(); }

	return false;
}

//ж����Դ
void CTableResource::DestroyResource()
{
	//���ñ���
	m_bDZShowHand=false;

	//�ͷ�λͼ
	m_ImageLock.Destroy();
	m_ImagePlay.Destroy();
	m_ImageReady.Destroy();
	m_ImageTable.Destroy();
	m_ImagePeople.Destroy();
	m_ImageStart.Destroy();
	m_ImageFree.Destroy();

	//���ñ���
	m_wWidth=0;
	m_wHeight=0;
	m_crName=RGB(0,0,0);
	m_crNote=RGB(0,0,0);
	m_crTableID=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	memset(m_nDrawStyle,0,sizeof(m_nDrawStyle));

	return;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTableFrame, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//���캯��
CTableView::CTableView()
{
	m_bMouseDown=false;
	m_bFocusFrame=false;
	m_pITableFrameView=NULL;
	m_wHoverChairID=INVALID_CHAIR;
	memset(&m_TableState,0,sizeof(m_TableState));
			//�������ӱ���ͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
    m_ImageViewCenter.SetLoadInfo(IDB_TABLE_BACK,hInstance);

	if (m_MemberImage.GetSafeHandle()==NULL)
	{  

		CBitmap Image;
		BITMAP ImageInfo;
		Image.LoadBitmap(IDB_USER_MEMBER);
		Image.GetBitmap(&ImageInfo);
		m_MemberImage.Create(11,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);
		m_MemberImage.Add(&Image,RGB(255,0,255));
	}

	m_GameRankManager.CreateInstance();
}

//��������
CTableView::~CTableView()
{
}

//�ӿڲ�ѯ
void * __cdecl CTableView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableView,Guid,dwQueryVer);
	return NULL;
}

//��ʼ������
void __cdecl CTableView::InitTableView(WORD wTableID, WORD wChairCount,WORD wKindId, ITableFrameView * pITableFrameView)
{
	m_TableState.wTableID=wTableID;
	m_TableState.wChairCount=wChairCount;
	m_TableState.wKindId = wKindId;
	m_pITableFrameView=pITableFrameView;

	return;
}

//�����û���Ϣ
bool __cdecl CTableView::SetUserInfo(WORD wChairID, IUserItem * pIUserItem)
{
	if (wChairID>=CountArray(m_TableState.pIUserItem)) return false;
	m_TableState.pIUserItem[wChairID]=pIUserItem;
	m_pITableFrameView->UpdateTableView(m_TableState.wTableID,false);
	return true;
}

//������갴��
void __cdecl CTableView::SetMouseDown(bool bMouseDown)
{
	if (m_bMouseDown!=bMouseDown)
	{
		m_bMouseDown=bMouseDown; 
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	}
	return;
}

//���ý�����
void __cdecl CTableView::SetFocusFrame(bool bFocusFrame)
{
	if (m_bFocusFrame!=bFocusFrame)
	{
		m_bFocusFrame=bFocusFrame;
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,false);
	}
	return;
}

//��������λ��
void __cdecl CTableView::SetHoverChairID(WORD wHoverChairID)
{
	if (m_wHoverChairID!=wHoverChairID)
	{
		m_wHoverChairID=wHoverChairID;
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	}
	return;
}

//������Ϸ��־ 
void __cdecl CTableView::SetPlayFlag(bool bPlaying)
{
	if (m_TableState.bPlaying!=bPlaying)
	{
		m_TableState.bPlaying=bPlaying; 
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	}
	return;
}

//���������־
void __cdecl CTableView::SetPassFlag(bool bPass)
{
	if (m_TableState.bPass!=bPass)
	{
		m_TableState.bPass=bPass; 
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,false);
	}
	return;
}

//���ñ�ע
void __cdecl CTableView::SetTableNote(LPCTSTR pszNote)
{
	lstrcpyn(m_TableState.szTableNote,pszNote,CountArray(m_TableState.szTableNote));
	m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	return;
}

//��ȡ�û���Ϣ
IUserItem * __cdecl CTableView::GetUserInfo(WORD wChairID)
{
	if (wChairID>=CountArray(m_TableState.pIUserItem)) return NULL;
	return m_TableState.pIUserItem[wChairID];
}

//��ȡ��������
WORD __cdecl CTableView::GetNullChairCount(WORD & wNullChairID)
{
	WORD wNullCount=0;
	wNullChairID=INVALID_CHAIR;
	for (WORD i=0;i<m_TableState.wChairCount;i++)
	{
		//////////////////////////////////////////////////////////////////////////
		//MODIF ���Ÿ�Ϊ����
		if(m_TableState.wKindId == 5 && m_TableState.wChairCount == 4)
			if(i==1 || i==3) 
				continue;
		//////////////////////////////////////////////////////////////////////////
		if (m_TableState.pIUserItem[i]==NULL)
		{
			++wNullCount;
			wNullChairID=i;
		}
	}
	return wNullCount;
}

//�滭����
void __cdecl CTableView::DrawTable(CDC * pDC, bool bHideInfo, CTableResource * pTableResource, bool bShowUser,int X,int Y)
{
	pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,X,Y);
	//DrawViewImage(pDC,m_ImageViewCenter,enMode_Spread);
	//�滭��ͼ

	if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
	{
		pTableResource->m_ImagePlay.BitBlt(pDC->m_hDC,X,Y);
		//�滭����״̬
		if(m_TableState.wChairCount < MAX_CHAIR_NORMAL)
			pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
	}
	else
	{
		pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,X,Y);
		//�滭����״̬
		if(m_TableState.wChairCount < MAX_CHAIR_NORMAL)
			pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
	}

	//�滭��Ϣ
	COLORREF crBackGround=pTableResource->m_crBackGround;
	IUserFaceRes * pIUserFaceRes=g_GlobalUnits.GetUserFaceRes();
	for (WORD i=0;i<m_TableState.wChairCount;i++)
	{
		//�滭�û�
		if ((bHideInfo==true)&&(bShowUser==true))
		{
			//��������
			CRect rcDrawRect=pTableResource->m_rcName[i];
			LPCTSTR pszString=(m_TableState.wTableID==0)?TEXT("�������"):TEXT("����λ��");

			//λ�ñ���
			int nYPos=0;
			int nDrawStyte=pTableResource->m_nDrawStyle[i];
			int nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS;
			int nNameLength=lstrlen(pszString);
			int nNameHeight=pDC->DrawText(pszString,nNameLength,&rcDrawRect,nDrawFormat|DT_CALCRECT);

			//����λ��
			if (nDrawStyte&NT_LEFT) nDrawFormat|=DT_LEFT;
			else if (nDrawStyte&NT_CENTER) nDrawFormat|=NT_LEFT;
			else nDrawFormat|=DT_RIGHT;
			if (nDrawStyte&NT_VCENTER) nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight)/2;
			else if (nDrawStyte&NT_BOTTOM) nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight);
			if (nYPos<0) nYPos=0;
			rcDrawRect.left=pTableResource->m_rcName[i].left;
			rcDrawRect.right=pTableResource->m_rcName[i].right;
			rcDrawRect.top=pTableResource->m_rcName[i].top+nYPos;
			rcDrawRect.bottom=pTableResource->m_rcName[i].bottom;
            
			//�û���Ϣ
			if (m_TableState.wTableID==0)
			{
				//�ǳ���
				pDC->SetTextColor(pTableResource->m_crName);
				pDC->DrawText(pszString,nNameLength,&rcDrawRect,nDrawFormat);
			}
			else
			{
				//�û�ͷ��
				//pDC->FillSolidRect(&pTableResource->m_rcChair[i],crBackGround);
				//pDC->FillSolidRect(&pTableResource->m_rcChair[i],crBackGround);
					/*pIUserFaceRes->DrawNormalFace(pDC,pTableResource->m_rcChair[i].left+1,
				pTableResource->m_rcChair[i].top+1,m_TableState.pIUserItem[i]->GetUserData()->cbGender,0,0);*/
                
				//�ǳ���
				pDC->SetTextColor(pTableResource->m_crName);
				pDC->DrawText(pszString,nNameLength,&rcDrawRect,nDrawFormat);
			}
		}

		//�滭�û�
		if ((bHideInfo==false)&&(bShowUser==true)&&(m_TableState.pIUserItem[i]!=NULL))
		{
			//��������
			CRect rcDrawRect=pTableResource->m_rcName[i];
			tagUserData * pUserData=m_TableState.pIUserItem[i]->GetUserData();

			//�Զ���ͷ��
			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();

			//�û�ͷ��
			if (pTableResource->m_bDZShowHand==true)
			{
				//��������
				BYTE cbGender=m_TableState.pIUserItem[i]->GetUserData()->cbGender;
				CSize SizePeople(pTableResource->m_ImagePeople.GetWidth()/8,pTableResource->m_ImagePeople.GetHeight()/2);

				//����λ��
				INT nXImagePos=SizePeople.cx*i;
				INT nYImagePos=(cbGender==0)?0:SizePeople.cy;
				pTableResource->m_ImagePeople.AlphaDrawImage(pDC,pTableResource->m_rcChair[i].left+1,pTableResource->m_rcChair[i].top+1,
					SizePeople.cx,SizePeople.cy,nXImagePos,nYImagePos,RGB(255,0,255));
			}
			else
			{
				if(m_TableState.wChairCount>MAX_CHAIR_NORMAL)
				{
				  
				}else
				{

				//pDC->FillSolidRect(&pTableResource->m_rcChair[i],crBackGround);
				/*
	              pIUserFaceRes->DrawNormalFace(pDC,pTableResource->m_rcChair[i].left+1,
					pTableResource->m_rcChair[i].top+1,pUserData->cbGender,pUserData->dwUserID,pUserData->dwCustomFaceVer);*/

int j=1;
if(((pUserData->lScore)>0)&&((pUserData->lScore)<100))j=1;
else if(((pUserData->lScore)>99)&&((pUserData->lScore)<1000))j=2;
else if(((pUserData->lScore)>999)&&((pUserData->lScore)<10000))j=3;
else if(((pUserData->lScore)>9999)&&((pUserData->lScore)<50000)) j=4;
else if(((pUserData->lScore)>49999)&&((pUserData->lScore)<100000))j=5;
else if(((pUserData->lScore)>99999)&&((pUserData->lScore)<300000))j=6;
else if(((pUserData->lScore)>299999)&&((pUserData->lScore)<500000))	j=7;
else if(((pUserData->lScore)>499999)&&((pUserData->lScore)<800000))j=8;
else if(((pUserData->lScore)>799999)&&((pUserData->lScore)<1000000))j=9;
else if(((pUserData->lScore)>999999)&&((pUserData->lScore)<1600000))j=10;
else if(((pUserData->lScore)>1599999)&&((pUserData->lScore)<3200000))j=11;
else if(((pUserData->lScore)>3199999)&&((pUserData->lScore)<6400000))j=12;
else if(((pUserData->lScore)>6399999)&&((pUserData->lScore)<12800000))j=13;
else if(((pUserData->lScore)>12799999)&&((pUserData->lScore)<25600000))j=14;
else if(((pUserData->lScore)>25599999)&&((pUserData->lScore)<51200000))j=15;
else if(((pUserData->lScore)>51199999)&&((pUserData->lScore)<1000000000))j=16;
else if((pUserData->lScore)>=1000000000)j=17;
j-=1;


                    //�滭ͷ��
				    if(m_TableState.wChairCount==3)
					{
						if(pUserData->cbGender==0)
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
								pTableResource->m_ImageGrilPlayer2[i+1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
							}
							else
							{
								pTableResource->m_ImageGrilPlayer1[i+1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
							}
						}
						else
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
							pTableResource->m_ImageBoyPlayer2[i+1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
							}
							else
							{
							pTableResource->m_ImageBoyPlayer1[i+1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
							}
						}
					}
					else if(m_TableState.wChairCount==5)
					{
						if(i>2)
						{
							if(pUserData->cbGender==0)
							{

								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
								pTableResource->m_ImageGrilPlayer2[i-1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
								}
								else
								{
								pTableResource->m_ImageGrilPlayer1[i-1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
								}
								
							}
							else
							{
								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
								pTableResource->m_ImageBoyPlayer2[i-1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
								}
								else
								{
								pTableResource->m_ImageBoyPlayer1[i-1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
								}
							}
						}
						else
						{
							if(pUserData->cbGender==0)
							{

								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
								pTableResource->m_ImageGrilPlayer2[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
								}
								else
								{
								pTableResource->m_ImageGrilPlayer1[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
								}
								
							}
							else
							{
								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
								pTableResource->m_ImageBoyPlayer2[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
								}

								else
								{
								pTableResource->m_ImageBoyPlayer1[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
								}
							}
						}
					}
					else if(m_TableState.wChairCount==4)
					{
						if(i == 0)
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
								pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
								pTableResource->m_ImageGrilPlayer2[0][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,74,137,0,0);
							}
							else
							{
								//pTableResource->m_ImageBoyPlayr3[3][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
								pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
								pTableResource->m_ImageGrilPlayer1[0][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,74,137,0,0);
							}
							//pTableResource->m_ImageGrilPlayer2[1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,182,124,0,0);
						}
						else if( i== 1)
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
								pTableResource->m_ImageGrilPlayer2[1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,182,124,0,0);

							}
							else
							{
								pTableResource->m_ImageGrilPlayer1[1][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,182,124,0,0);
							}
						}else if(i==2)
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
								//pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
								pTableResource->m_ImageGrilPlayer2[2][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);

							}
							else
							{
								//pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
								pTableResource->m_ImageGrilPlayer1[2][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);
 							}
							//pTableResource->m_ImageGrilPlayer2[3][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);
						}else 
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
								  pTableResource->m_ImageGrilPlayer2[3][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,182,124,0,0);		

							}
							else
							{
								pTableResource->m_ImageGrilPlayer1[3][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,182,124,0,0);
							}
						}

					}
					else if(m_TableState.wChairCount==2 /*|| m_TableState.wChairCount==4*/)
					{
						if(pUserData->cbGender==0)
						{
							if(i==0)
							{
								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
									pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageGrilPlayer2[0][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,74,137,0,0);
								}
								else
								{
									//pTableResource->m_ImageBoyPlayr3[3][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
									pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageGrilPlayer1[0][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,74,137,0,0);
									
								}
							}
							else
							{
								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
									//pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageGrilPlayer2[2][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);

								}
								else
								{
									//pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageGrilPlayer1[2][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);
								}
							}
						}
						else
						{
							if(i==0)
							{
								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
									//pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageBoyPlayer2[0][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,74,137,0,0);
								}
								else
								{
									//pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									//pTableResource->m_ImageBoyPlayr3[3][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
									pTableResource->m_ImageBoyPlayer1[0][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,74,137,0,0);
								}
							}
							else
							{
								if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
								{
									//pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageBoyPlayer2[2][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);
								}
								else
								{
									//pTableResource->m_ImageFree.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
									pTableResource->m_ImageBoyPlayer1[2][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,82,124,0,0);
								}
							}
						}
					}
					else
					{
						if(pUserData->cbGender==0)
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
							pTableResource->m_ImageGrilPlayer2[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
							}
							else
							{
							pTableResource->m_ImageGrilPlayer1[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
							}
						}
						else
						{
							if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
							{
							pTableResource->m_ImageBoyPlayer2[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,76,0);
							}
							else
							{
							pTableResource->m_ImageBoyPlayer1[i][j].DrawImage(pDC,pTableResource->m_ptPalyer[i].x,pTableResource->m_ptPalyer[i].y,76,146,0,0);
							}
						}
					}
					///////////////
				}
			}
            
			//ͬ���־
			if ((m_TableState.bPlaying==false)&&(pUserData->cbUserStatus==US_READY))
			{
				pTableResource->m_ImageReady.AlphaDrawImage(pDC,pTableResource->m_ptReady[i].x,
					pTableResource->m_ptReady[i].y,RGB(255,0,255));

			}


			//����״̬����
// 			if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
// 			{
// 				pTableResource->m_ImageStart.AlphaDrawImage(pDC,72,70,RGB(255,0,255));
// 			}
// 			else
// 				pTableResource->m_ImageFree.AlphaDrawImage(pDC,75,
// 				65,RGB(255,0,255));
			//λ�ñ���
			int nYPos=0;
			int nDrawStyte=pTableResource->m_nDrawStyle[i];
			int nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS;
			int nNameLength=lstrlen(pUserData->szGroupName);
			int nNameHeight=pDC->DrawText(pUserData->szGroupName,nNameLength,&rcDrawRect,nDrawFormat|DT_CALCRECT);

			//����λ��
			if (nDrawStyte&NT_LEFT) nDrawFormat|=DT_LEFT;
			else if (nDrawStyte&NT_CENTER) nDrawFormat|=DT_LEFT;
			else nDrawFormat|=DT_LEFT;
			if (nDrawStyte&NT_VCENTER) nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight)/2;
			else if (nDrawStyte&NT_BOTTOM) nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight);
			if (nYPos<0) nYPos=0;
			rcDrawRect.left=pTableResource->m_rcName[i].left;
			rcDrawRect.right=pTableResource->m_rcName[i].right;
			rcDrawRect.top=pTableResource->m_rcName[i].top;
			rcDrawRect.bottom=pTableResource->m_rcName[i].bottom;
            
			//�ǳ���
			pDC->SetTextColor(RGB(255,255,255));

			 if ( 0 <pUserData->cbMemberOrder)
			{  
// 				if(pUserData->cbMemberOrder==1)
// 					pDC->SetTextColor(RGB(255,0,0));
// 				else if(pUserData->cbMemberOrder==2)
// 					pDC->SetTextColor(RGB(1,159,249));
// 				else if(pUserData->cbMemberOrder==3)
// 					pDC->SetTextColor(RGB(205,106,0));
// 				else
// 					pDC->SetTextColor(RGB(150,10,190));
				//if (pUserData->cbMemberOrder> 0)
					pDC->SetTextColor(RGB(3,222,255));
				
			}
			else pDC->SetTextColor(RGB(255,255,255));

			pDC->DrawText(pUserData->szGroupName,nNameLength,&rcDrawRect,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);


			CSize szStr;
			GetTextExtentPoint(pDC->GetSafeHdc(), pUserData->szGroupName, nNameLength, &szStr);
			CRect rcLevel=rcDrawRect;
			TCHAR szLevel[128]={0};
			if (m_GameRankManager.GetInterface()!=NULL)
			{
				LPCTSTR pszGameRank=m_GameRankManager->GetLevelDescribe(pUserData->lScore);
				lstrcpy(szLevel,pszGameRank);
			}else szLevel[0] = 0;
			rcLevel.top+=12;
			rcLevel.bottom+=12;
			pDC->SetTextColor(RGB(234,214,143));

			pDC->DrawText(szLevel,lstrlen(szLevel),&rcLevel,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
			
			//pDC->DrawText(szLevel,lstrlen(szLevel),&rcLevel,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
			
			//////////////////////////////////////////////////////////////////////////
			//���ƻ�Ա
// 			if(m_TableState.wChairCount==2)
// 			{
// 				int iMember =pUserData->cbMemberOrder>3?3:pUserData->cbMemberOrder-1;
// 				if(i == 0)
// 					m_MemberImage.Draw(pDC,iMember,CPoint(62,7),ILD_TRANSPARENT);
// 				else
// 					m_MemberImage.Draw(pDC,iMember,CPoint(110,165),ILD_TRANSPARENT);
// 			}else if (m_TableState.wChairCount==4)
// 			{
// 				int iMember =pUserData->cbMemberOrder>3?3:pUserData->cbMemberOrder-1;
// 				if(i == 0)
// 					m_MemberImage.Draw(pDC,iMember,CPoint(62,7),ILD_TRANSPARENT);
// 				else if(i == 2)
// 					m_MemberImage.Draw(pDC,iMember,CPoint(110,152),ILD_TRANSPARENT);
// 			}
			//////////////////////////////////////////////////////////////////////////
			//�����Լ�λ�ÿ�
			if(GlobalUserData.dwUserID == pUserData->dwUserID)
			{
				CRect rcMeTable(10, 5,pTableResource->m_wWidth-10, pTableResource->m_wHeight);
				pDC->DrawFocusRect(rcMeTable);
			}
		}
		
		//�滭�߿�
	   if(m_wHoverChairID==i)
	   {
		
			if (m_bMouseDown) 
			{
			   //pDC->Draw3dRect(&pTableResource->m_rcChair[i],RGB(125,125,125),RGB(255,255,255));
			}
			else 
			{
				if(m_TableState.wChairCount==1||m_TableState.wChairCount==8)
				{
				  pDC->Draw3dRect(&pTableResource->m_rcChair[i],RGB(255,255,255),RGB(125,125,125));
				  //pTableResource->m_ImageLock.AlphaDrawImage(pDC,nXLock,nYLock,RGB(255,0,255));
				}else if(m_TableState.wChairCount>MAX_CHAIR_NORMAL)
				{
				// pTableResource->m_ImageJinRu.AlphaDrawImage(pDC,pTableResource->m_rcChair[0].left-4,pTableResource->m_rcChair[0].top-2,RGB(255,0,255)); 
				}
				else
				{
					
				if(m_TableState.pIUserItem[i]==NULL)
				{
			     //pDC->Draw3dRect(&pTableResource->m_rcChair[i],RGB(255,255,255),RGB(125,125,125));
			  
			     //pTableResource->m_ImageLock.AlphaDrawImage(pDC,nXLock,nYLock,RGB(255,0,255));
				}
				}
		    }
	      }
	 }

	//pDC->Draw3dRect(&pTableResource->m_rcChair[0],RGB(0,0,0),RGB(0,0,0));

	//�滭��
	if (m_TableState.bPass==true)
	{
		int nXLock=pTableResource->m_ptLock.y-pTableResource->m_ImageLock.GetWidth()/2;
		int nYLock=pTableResource->m_ptLock.y-pTableResource->m_ImageLock.GetHeight()/2;
		if(m_TableState.wChairCount==2 || m_TableState.wChairCount==4)
		{
			nXLock=65;
			nYLock=80+95;
		}
		pTableResource->m_ImageLock.AlphaDrawImage(pDC,nXLock,nYLock,RGB(255,0,255));
	}

	//�滭����
	if (m_TableState.szTableNote[0]==0&&m_TableState.wChairCount<8)
	{
		TCHAR szBuffer[32];
		pDC->SetTextColor(pTableResource->m_crTableID);
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("- %d -"),m_TableState.wTableID+1);
		if (m_TableState.wChairCount==2  || m_TableState.wChairCount==4 )
		{
			CRect rcTableID;
			rcTableID.top=180;
			rcTableID.left=100;
			rcTableID.bottom=210;
			rcTableID.right=120;
			//���ӱ�ű���
			//pTableResource->m_ImageNumBack.AlphaDrawImage(pDC,80,175,RGB(255,0,255));
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcTableID,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
		}
		else
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&pTableResource->m_rcTableID,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
	}
	else
	{
		pDC->SetTextColor(RGB(255,255,0));
		pDC->DrawText(m_TableState.szTableNote,lstrlen(m_TableState.szTableNote),&pTableResource->m_rcTableID,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//��̬����
int					CTableFrame::m_iScrollPos=10;					//��������
int					CTableFrame::m_iWhellTimes=2;					//���ֱ���

//���캯��
CTableFrame::CTableFrame()
{
	//״̬����
	m_bLeftMouseDown=false;
	m_bRightMouseDown=false;
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;
	m_wFrameTableID=INVALID_TABLE;

	//������Ϣ
	m_iCountRow=0;
	m_iXExcursion=0;
	m_iCurrentPos=0;
	m_iMaxScrollPos=0;	
	m_iCountAllLine=0;
	m_iCountVisibleLine=0;

	//���ñ���
	m_wTableCount=0;
	m_wChairCount=0;
	m_bShowUser=false;
	m_bHideInfo=false;

	//����ָ��
	m_pTableViewArray=NULL;
	m_pITableFrameSink=NULL;
	m_wMeTableID = INVALID_TABLE;
	return;
}

//��������
CTableFrame::~CTableFrame()
{
	DestroyTableFrame();
	return;
}

//�ӿڲ�ѯ
void * __cdecl CTableFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameView,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CTableFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID)
{
	CRect rcFrameRect(0,0,0,0);
	DWORD dwStyle=WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	Create(NULL,NULL,dwStyle,rcFrameRect,pParentWnd,uWndID);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	return true;
}

//���ú���
bool __cdecl CTableFrame::InitTableFrame(WORD wTableCount, WORD wChairCount, bool bHideInfo, const tagGameKind * pGameKind, IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pGameKind!=NULL);
	ASSERT(wChairCount<=MAX_CHAIR);

	//��������
	bool bLoadResource=false;

	//��ȡ��Դ
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	GetGameResDirectory(pGameKind,szDirectory,CountArray(szDirectory));

	m_GameRankManager.CreateInstance();
	//��Ϸ�ȼ�
	ASSERT(m_GameRankManager.GetInterface()!=NULL);//pszGameName
	bool bSuccess=m_GameRankManager->LoadGameLevel(szDirectory, GAME_GENRE_GOLD);

	//�ļ�����
	if (m_TableResource.LoadFromFiles(bHideInfo,szDirectory,wChairCount,pGameKind->wKindID)==false) throw TEXT("��Ϸ������Դ����ʧ�ܣ��볢�����°�װ��Ϸ������⣡");
    
	//������Ϸ����
	m_bHideInfo=bHideInfo;
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	m_pTableViewArray=new CTableView[m_wTableCount];
	if (m_pTableViewArray==NULL) throw TEXT("����ϵͳ��Դ���㣬�޷�������Ϸ���ӣ�");
	for (WORD i=0;i<m_wTableCount;i++) (m_pTableViewArray+i)->InitTableView(i,wChairCount,pGameKind->wKindID,this);

	//���ýӿ�
	m_pITableFrameSink=(ITableFrameSink *)pIUnknownEx->QueryInterface(IID_ITableFrameSink,VER_ITableFrameSink);
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink==NULL) throw TEXT("�ӿڲ�ѯʧ�ܣ��޷�������Ϸ���ӣ�");

	m_pToolTipCtrl.Create( this);
	m_pToolTipCtrl.Activate(TRUE);
	m_pToolTipCtrl.AddTool(this, "tips");
	m_pToolTipCtrl.SetTipBkColor(RGB(226,230,242));
	m_pToolTipCtrl.SetMaxTipWidth(5000);

	//��ʼ��ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,20088);
	//m_btStart.SetButtonImage(IDB_PLAZA_HELP, AfxGetInstanceHandle(), false);
	m_btStart.SetButtonImage(szResPath("Room\\Table\\BtStart.png"), false);
	return true;
}

//���ٺ���
void __cdecl CTableFrame::DestroyTableFrame()
{
	//���ٿؼ�
	if (m_hWnd!=NULL) DestroyWindow();
	SafeDeleteArray(m_pTableViewArray);
	m_TableResource.DestroyResource();

	//��������
	m_iCountRow=0;
	m_iXExcursion=0;
	m_iCurrentPos=0;
	m_iMaxScrollPos=0;	
	m_iCountAllLine=0;
	m_iCountVisibleLine=0;

	//״̬����
	m_bLeftMouseDown=false;
	m_bRightMouseDown=false;
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;
	m_wFrameTableID=INVALID_TABLE;

	//���ñ���
	m_wTableCount=0;
	m_wChairCount=0;
	m_bShowUser=false;

	//����ָ��
	m_pTableViewArray=NULL;
	m_pITableFrameSink=NULL;


	return;
}

//��������
void __cdecl CTableFrame::UpdateTableView(WORD wTableID, bool bMustUpdate)
{
	if ((m_hWnd!=NULL)&&((m_bShowUser==true)||(bMustUpdate==true)))
	{
		ITableView * pITableView=GetTableArrayPtr(wTableID);
		if ((pITableView!=NULL)&&(m_iCountRow!=0))
		{
			RECT rcTableView;
			rcTableView.left=(wTableID%m_iCountRow)*m_TableResource.m_wWidth+m_iXExcursion;
			rcTableView.top=(wTableID/m_iCountRow)*m_TableResource.m_wHeight-m_iCurrentPos;
			rcTableView.right=rcTableView.left+m_TableResource.m_wWidth;
			rcTableView.bottom=rcTableView.top+m_TableResource.m_wHeight;
			InvalidateRect(&rcTableView,FALSE);
		}
	}

	return;
}

//��ȡ��Ϸ��ָ��
ITableView * __cdecl CTableFrame::GetTableArrayPtr(WORD wTableID)
{
	if ((m_pTableViewArray==NULL)||(wTableID>=m_wTableCount)) return NULL;
	return (m_pTableViewArray+wTableID);
}

//������ʾ
bool __cdecl CTableFrame::ShowUserInfo(bool bShowUser)
{
	m_bShowUser=bShowUser;
	if (m_hWnd!=NULL) Invalidate(FALSE);
	return true;
}

//������ͼ����
bool __cdecl CTableFrame::EnsureVisibleTable(WORD wTableID)
{
	//Ч�����
	ASSERT(wTableID<m_wTableCount);
	if (wTableID>=m_wTableCount) return false;

	//����λ��
	int iLastPos=m_iCurrentPos;
	int iXPos=(wTableID%m_iCountRow)*m_TableResource.m_wWidth+m_iXExcursion;
	int iYPos=(wTableID/m_iCountRow)*m_TableResource.m_wHeight-m_iCurrentPos;

	//����λ��
	CRect rcTable,rcClient;
	GetClientRect(&rcClient);
	rcTable.SetRect(iXPos,iYPos,iXPos+m_TableResource.m_wWidth,iYPos+m_TableResource.m_wHeight);
	if ((rcTable.bottom>rcClient.bottom)||(rcTable.top<0)) m_iCurrentPos+=rcTable.bottom-rcClient.bottom;
	m_iCurrentPos=__max(0,__min(m_iCurrentPos,m_iMaxScrollPos-m_iCountVisibleLine*m_TableResource.m_wHeight));

	//������ͼ
	if (iLastPos!=m_iCurrentPos)
	{
		SetScrollPos(SB_VERT,m_iCurrentPos);
		ScrollWindow(0,iLastPos-m_iCurrentPos,NULL,NULL);
	}

	return true;
}

//��������
bool __cdecl CTableFrame::FlashTable(WORD wTableID)
{
	//��ȡ����
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	if (pITableView==NULL) return false;

	//����λ��
	CRect rcTable;
	int iTableRow=wTableID%m_iCountRow;
	int iTableLine=wTableID/m_iCountRow;
	rcTable.left=iTableRow*m_TableResource.m_wWidth+m_iXExcursion;
	rcTable.top=iTableLine*m_TableResource.m_wHeight-m_iCurrentPos;
	rcTable.right=rcTable.left+m_TableResource.m_wWidth;
	rcTable.bottom=rcTable.top+m_TableResource.m_wHeight;
	
	//�������
	const int nFlashTimes=18,nStepWidth=4;
	const int nExpandPos=nFlashTimes*nStepWidth;
	rcTable.InflateRect(nExpandPos,nExpandPos);

	//ѭ���滭
	UpdateWindow();
	CDC * pDC=GetDC();
	CPen LinePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));
	CPen * pOldPen=pDC->SelectObject(&LinePen);
	pDC->SetROP2(R2_XORPEN);
	for (int nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		pDC->MoveTo(rcTable.left,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.top);
		Sleep((nFlashTimes-nTimes)*2);
		pDC->MoveTo(rcTable.left,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.top);
		rcTable.DeflateRect(nStepWidth,nStepWidth);
	}
	pDC->SelectObject(pOldPen);
	ReleaseDC(pDC);

	return true;
}

//��������
bool __cdecl CTableFrame::FlashChair(WORD wTableID, WORD wChairID)
{
	//��ȡ����
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	if (pITableView==NULL) return false;

	//��ȡλ��
	CRect rcChair=m_TableResource.m_rcChair[wChairID];

	//��ȡ��ͼλ��
	int iTableRow=wTableID%m_iCountRow;
	int iTableLine=wTableID/m_iCountRow;
	int iXPos=iTableRow*m_TableResource.m_wWidth+m_iXExcursion;
	int iYPos=iTableLine*m_TableResource.m_wHeight-m_iCurrentPos;
	rcChair.OffsetRect(iXPos,iYPos);

	//�������
	const int nFlashTimes=18,nStepWidth=4;
	const int nExpandPos=nFlashTimes*nStepWidth;
	rcChair.InflateRect(nExpandPos,nExpandPos);

	//ѭ���滭
	UpdateWindow();
	CDC * pDC=GetDC();
	CPen LinePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));
	CPen * pOldPen=pDC->SelectObject(&LinePen);
	pDC->SetROP2(R2_XORPEN);
	for (int nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		pDC->MoveTo(rcChair.left,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.top);
		Sleep((nFlashTimes-nTimes)*2);
		pDC->MoveTo(rcChair.left,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.top);
		rcChair.DeflateRect(nStepWidth,nStepWidth);
	}
	pDC->SelectObject(pOldPen);
	ReleaseDC(pDC);

	return true;
}

//�����û���Ϣ
bool __cdecl CTableFrame::SetUserInfo(WORD wTableID, WORD wChairID, IUserItem * pIUserItem)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return false;
	return pITableView->SetUserInfo(wChairID,pIUserItem);
}

//������Ϸ��־ 
void __cdecl CTableFrame::SetPlayFlag(WORD wTableID, bool bPlaying)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView!=NULL) pITableView->SetPlayFlag(bPlaying);
	return;
}

//���������־
void __cdecl CTableFrame::SetPassFlag(WORD wTableID, bool bPass)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView!=NULL) pITableView->SetPassFlag(bPass);
	return;
}

//��ȡ�û���Ϣ
IUserItem * __cdecl CTableFrame::GetUserInfo(WORD wTableID, WORD wChairID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return NULL;
	return pITableView->GetUserInfo(wChairID);
}

//��ȡ��������
WORD __cdecl CTableFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return 0;
	return pITableView->GetNullChairCount(wNullChairID);
}

//��ѯ��Ϸ��־
bool __cdecl CTableFrame::QueryPlayFlag(WORD wTableID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return false;
	return pITableView->QueryPlayFlag();
}

//��ѯ�����־
bool __cdecl CTableFrame::QueryPassFlag(WORD wTableID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return false;
	return pITableView->QueryPassFlag();
}

//��ȡ��������
const tagTableState * CTableFrame::GetTableAttrib(WORD wTableID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return NULL;
	return pITableView->GetTableAttrib();
}

//�ػ���Ϣ
void CTableFrame::OnPaint()
{
	//���� DC
	CPaintDC dc(this);

	//��ȡ����
	CRect rcClient,ClipRect;
	dc.GetClipBox(&ClipRect);
	GetClientRect(&rcClient);

	if(m_wMeTableID != INVALID_TABLE)
		m_btStart.ShowWindow(SW_SHOW);
	else
		m_btStart.ShowWindow(SW_HIDE);
	
	//m_btStart.MoveWindow(100, 600, 20, 20);
	
	//�滭��Ϸ��
	if (m_pTableViewArray!=NULL)
	{
		//��������
		CFont DrawFont;
		DrawFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

		//�������
		int iTableWidth=m_TableResource.m_wWidth;
		int iTableHeight=m_TableResource.m_wHeight;

		//��������ͼ
		
        CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());
			CFont * pOldFont=pDC->SelectObject(&DrawFont);

			pDC->SetBkMode(TRANSPARENT);
			pDC->FillSolidRect(0,0,iTableWidth,iTableHeight,RGB(24,66,99));

		
		//�滭����
		RECT rcTableView;
		ITableView * pITableView=NULL;
		int iBeginRow=(rcClient.left-m_iXExcursion)/iTableWidth;
		int iBeginLine=(m_iCurrentPos+rcClient.top)/iTableHeight;
		int iEndDrawRow=(rcClient.right-m_iXExcursion+iTableWidth-1)/iTableWidth;
		int iEndDrawLine=(m_iCurrentPos+rcClient.bottom+iTableHeight-1)/iTableHeight;
		for (int iLine=iBeginLine;iLine<iEndDrawLine;iLine++)
		{
			for (int iRow=iBeginRow;iRow<iEndDrawRow;iRow++)
			{
				int iTableIndex=iLine*m_iCountRow+iRow;
				rcTableView.left=iRow*iTableWidth+m_iXExcursion;
				rcTableView.top=iLine*iTableHeight-m_iCurrentPos;
				rcTableView.right=rcTableView.left+iTableWidth;
				rcTableView.bottom=rcTableView.top+iTableHeight;
				if ((iRow<m_iCountRow)&&(iTableIndex<m_wTableCount)&&m_wTableCount>0)
				{
					pITableView=GetTableArrayPtr(iLine*m_iCountRow+iRow);
					ASSERT(pITableView!=NULL);
					
					pITableView->DrawTable(pDC,m_bHideInfo,&m_TableResource,m_bShowUser,0,0);

					if(pITableView->GetTableAttrib()->wTableID == m_wMeTableID)
					{
						//m_btStart.MoveWindow(rcTableView.left, rcTableView.top, 20, 20);
						m_btStart.SetWindowPos(NULL,rcTableView.left+95, rcTableView.top+170,0,0,SWP_NOSIZE);
					}
					
                    m_ImageBuffer.BitBlt(dc,rcTableView.left,rcTableView.top,iTableWidth,iTableHeight,0,0);
					//dc.BitBlt(rcTableView.left,rcTableView.top,iTableWidth,iTableHeight,&BufferDC,0,0,SRCCOPY);

				}
				else dc.FillSolidRect(&rcTableView,m_TableResource.m_crBackGround);
			}
		}

		//�滭��϶
		if (m_iXExcursion>ClipRect.left)
	
			dc.FillSolidRect(ClipRect.left,ClipRect.top,m_iXExcursion-ClipRect.left,ClipRect.bottom,m_TableResource.m_crBackGround);

		//������Դ
        
		pDC->SelectObject(pOldFont);
			m_ImageBuffer.ReleaseDC();
			DrawFont.DeleteObject();
	   
		//m_ImageBuffer.BitBlt(dc,rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),rcClip.left,rcClip.top);
	}
	//else dc.FillSolidRect(&ClipRect,m_TableResource.m_crBackGround);

	return;
}

//λ����Ϣ
void CTableFrame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�������
	int iTableWidth=m_TableResource.m_wWidth;
	int iTableHeight=m_TableResource.m_wHeight;

	//����λ��
	if (m_wTableCount!=0)
	{
		int iViewWidth=__max(iTableWidth,cx);
		int iViewHeight=__max(iTableHeight,cy);
		m_iCountRow=iViewWidth/iTableWidth;
		m_iCountVisibleLine=iViewHeight/iTableHeight;
		m_iCountAllLine=(m_wTableCount+m_iCountRow-1)/m_iCountRow;
		m_iXExcursion=(iViewWidth-m_iCountRow*iTableWidth)/2;
		m_ImageBuffer.Destroy();
		m_ImageBuffer.Create(cx,cy,24);
	}
	//���ù�������
	m_iMaxScrollPos=m_iCountAllLine*iTableHeight;
	m_iCurrentPos=__min(m_iCurrentPos,m_iMaxScrollPos-m_iCountVisibleLine*iTableHeight);
	if (m_iCurrentPos<0) m_iCurrentPos=0;
	
	//���ù�����
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize=sizeof(SCROLLINFO);
	ScrollInfo.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin=0;
	ScrollInfo.nMax=m_iMaxScrollPos;
	ScrollInfo.nPage=cy;
	ScrollInfo.nPos=m_iCurrentPos;
	SetScrollInfo(SB_VERT,&ScrollInfo,TRUE);
    
	return;
}

//��Ϣ����
BOOL CTableFrame::PreTranslateMessage(MSG * pMsg)
{
	if ( m_pToolTipCtrl )
	{
		//m_pToolTipCtrl.UpdateTipText(GetDlgItem(IDC_BT_EXCUTE), "werwrwx");
		m_pToolTipCtrl.RelayEvent( pMsg );
	}

	if (pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_iCurrentPos),NULL);
				return TRUE;
			}
		case VK_DOWN:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_iCurrentPos),NULL);
				return TRUE;
			}
		case VK_PRIOR:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_iCurrentPos),NULL);
				return TRUE;
			}
		case VK_NEXT:
		case VK_SPACE:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_iCurrentPos),NULL);
				return TRUE;
			}
		case VK_HOME:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_iCurrentPos),NULL);
				return TRUE;
			}
		case VK_END:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_iCurrentPos),NULL);
				return TRUE;
			}
		}
	}
	return __super::PreTranslateMessage(pMsg);
}

//������Ϣ
void CTableFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	//��ȡ����
	RECT rcClientRect;
	int iLastPos=m_iCurrentPos;
	GetClientRect(&rcClientRect);

	//�ƶ�����
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_iCurrentPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_iCurrentPos=m_iMaxScrollPos-m_iCountVisibleLine*m_TableResource.m_wHeight;
			break;
		}
	case SB_LINEUP:
		{
			m_iCurrentPos-=m_iScrollPos;
			break;
		}
	case SB_PAGEUP:
		{
			m_iCurrentPos-=rcClientRect.bottom;
			break;
		}
	case SB_LINEDOWN:
		{
			m_iCurrentPos+=m_iScrollPos;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_iCurrentPos+=rcClientRect.bottom;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_iCurrentPos=nPos;
			break;
		}
	}
	
	//����λ��
	SetFocus();
	m_iCurrentPos=__min(m_iCurrentPos,m_iMaxScrollPos-rcClientRect.bottom);
	if (m_iCurrentPos<0) m_iCurrentPos=0;
	if (iLastPos!=m_iCurrentPos)
	{
		SetScrollPos(SB_VERT,m_iCurrentPos);
		ScrollWindow(0,iLastPos-m_iCurrentPos,NULL,NULL);
	}


	return;
}

//�����Ϣ
void CTableFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	SetFocus();
	m_pITableFrameSink->OnDoubleHitTable(m_wDownTableID,false);
	return;
}

//�����Ϣ
void CTableFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	SetFocus();
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		SetCapture();
		m_bLeftMouseDown=true;
		m_wDownTableID=TableHitTest(Point);
		if (m_wDownTableID!=INVALID_TABLE)
		{
			ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
			ASSERT(pITableView!=NULL);
			CPoint TablePoint;
			TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
			TablePoint.y=(Point.y+m_iCurrentPos)%m_TableResource.m_wHeight;
			m_wDownChairID=ChairHitTest(TablePoint);
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bLeftMouseDown);
				pITableView->SetHoverChairID(m_wDownChairID);
			}
		}
	}
	return;
}

//�����Ϣ
void CTableFrame::OnLButtonUp(UINT nFlags, CPoint Point)
{
	if ((m_bLeftMouseDown==true)&&(m_bRightMouseDown==false))
	{
		//���԰���
		ReleaseCapture();
		m_bLeftMouseDown=false;
		WORD wTableID=TableHitTest(Point);
		ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
		if (pITableView!=NULL) 
		{
			pITableView->SetMouseDown(false);
			pITableView->SetHoverChairID(INVALID_CHAIR);
		}

		//��ȡλ��
		WORD wChairID=INVALID_CHAIR;
		if (wTableID==m_wDownTableID)
		{
			ITableView * pITableView=GetTableArrayPtr(wTableID);
			if (pITableView!=NULL)
			{
				CPoint TablePoint;
				TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
				TablePoint.y=(Point.y+m_iCurrentPos)%m_TableResource.m_wHeight;
				wChairID=ChairHitTest(TablePoint);
			}
		}

		//������Ϣ
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
			m_pITableFrameSink->OnLButtonHitTable(wTableID,wChairID,false);
		}
		else
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//�����Ϣ
void CTableFrame::OnRButtonDown(UINT nFlags, CPoint Point)
{
	SetFocus();
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		SetCapture();
		m_bRightMouseDown=true;
		m_wDownTableID=TableHitTest(Point);
		if (m_wDownTableID!=INVALID_TABLE)
		{
			ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
			ASSERT(pITableView!=NULL);
			CPoint TablePoint;
			TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
			TablePoint.y=(Point.y+m_iCurrentPos)%m_TableResource.m_wHeight;
			m_wDownChairID=ChairHitTest(TablePoint);
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bRightMouseDown);
				pITableView->SetHoverChairID(m_wDownChairID);
			}
		}
	}
	return;
}

//�����Ϣ
void CTableFrame::OnRButtonUp(UINT nFlags, CPoint Point)
{
	if (m_bRightMouseDown==true)	
	{
		//���԰���
		ReleaseCapture();
		m_bRightMouseDown=false;
		WORD wTableID=TableHitTest(Point);
		ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
		if (pITableView!=NULL) 
		{
			pITableView->SetMouseDown(false);
			pITableView->SetHoverChairID(INVALID_CHAIR);
		}

		//��ȡλ��
		WORD wChairID=INVALID_CHAIR;
		if (wTableID==m_wDownTableID)
		{
			ITableView * pITableView=GetTableArrayPtr(wTableID);
			if (pITableView!=NULL)
			{
				CPoint TablePoint;
				TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
				TablePoint.y=(Point.y+m_iCurrentPos)%m_TableResource.m_wHeight;
				wChairID=ChairHitTest(TablePoint);
			}
		}

		//������Ϣ
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
			m_pITableFrameSink->OnRButtonHitTable(wTableID,wChairID,false);
		}
		else
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//�����Ϣ
BOOL CTableFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	RECT rcClientRect;
	GetClientRect(&rcClientRect);
	int iLastPos=m_iCurrentPos;
	int iMaxPos=m_iMaxScrollPos-rcClientRect.bottom;
	m_iCurrentPos=__min(m_iCurrentPos-zDelta/m_iWhellTimes,iMaxPos);
	if (m_iCurrentPos<0) m_iCurrentPos=0;
	SetScrollPos(SB_VERT,m_iCurrentPos);
	ScrollWindow(0,iLastPos-m_iCurrentPos,NULL,NULL);
	return TRUE;
}

//�����Ϣ
BOOL CTableFrame::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		//��������
		int iTableWidth=m_TableResource.m_wWidth;
		int iTableHeight=m_TableResource.m_wHeight;

		//���԰���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		WORD wNewChairID=INVALID_CHAIR;
		WORD wNewTableID=TableHitTest(MousePoint);
		if (wNewTableID<m_wTableCount)
		{
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_iXExcursion)%iTableWidth;
			TablePoint.y=(MousePoint.y+m_iCurrentPos)%iTableHeight;
			wNewChairID=ChairHitTest(TablePoint);
		}
		
		//���½���
		if (((wNewTableID!=m_wDownTableID)||(wNewChairID!=m_wDownChairID))
			&&((wNewTableID!=INVALID_CHAIR)||(m_wDownChairID!=INVALID_CHAIR)))
		{
			m_pToolTipCtrl.Activate(FALSE);
			if(wNewTableID != INVALID_TABLE && wNewChairID != INVALID_CHAIR)
			{
				//��ȡ�û�
				IUserItem * pIUserItem=GetUserInfo(wNewTableID,wNewChairID);
				if(pIUserItem != NULL)
				{
					m_pToolTipCtrl.UpdateTipText(GetTipsText(pIUserItem), this);
					m_pToolTipCtrl.Activate(TRUE);
				}
			}

			if (m_wDownTableID!=INVALID_TABLE) GetTableArrayPtr(m_wDownTableID)->SetHoverChairID(INVALID_CHAIR);
			if (wNewTableID!=INVALID_TABLE) GetTableArrayPtr(wNewTableID)->SetHoverChairID(wNewChairID);
		}
		
		//��������
		m_wDownTableID=wNewTableID;
		m_wDownChairID=wNewChairID;
		if ((m_wDownTableID!=INVALID_TABLE)&&(m_wDownChairID!=INVALID_CHAIR))
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//���Ӳ���
WORD CTableFrame::TableHitTest(POINT MousePoint)
{
	int iTableWidth=m_TableResource.m_wWidth;
	int iTableHeight=m_TableResource.m_wHeight;
	if ((iTableWidth!=0)&&(iTableHeight!=0))
	{
		MousePoint.y+=m_iCurrentPos;
		int iTableRow=(MousePoint.x-m_iXExcursion)/iTableWidth;
		int iTableLine=MousePoint.y/iTableHeight;
		if ((iTableRow>=0)&&(iTableRow<m_iCountRow))
		{
			WORD wTableID=iTableLine*m_iCountRow+iTableRow;
			if (wTableID<m_wTableCount) 
			{
				if ((wTableID>0)&&(m_bHideInfo==true)) return INVALID_TABLE;
				return wTableID;
			}
		}
	}
	return INVALID_TABLE;
}

//���Ӳ���
WORD CTableFrame::ChairHitTest(POINT MousePoint)
{
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_TableResource.m_rcChair[i].PtInRect(MousePoint)) return i;
	}
	return INVALID_CHAIR;
}

//��ԴĿ¼
void CTableFrame::GetGameResDirectory(const tagGameKind * pGameKind, TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;
    
	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,(LPCTSTR)pGameKind->szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}

LPCTSTR CTableFrame::GetTipsText( IUserItem * pIUserItem )
{
	static TCHAR szBuffer[512]={0};
	tagUserData* pUserData = pIUserItem->GetUserData();
	if(pUserData != NULL)
	{
		TCHAR szWinRate[128], szFleeRate[128];
		LONG lAllCount=pIUserItem->GetUserPlayCount();
		double dWinRate=(double)(pUserData->lWinCount*100)/(double)lAllCount;
		if (dWinRate>=0.01) _snprintf(szWinRate,sizeof(szWinRate),TEXT("%5.2f%%"),dWinRate);

		double dFleeRate=(double)(pUserData->lFleeCount*100)/(double)lAllCount;
		if (dFleeRate>=0.01) _snprintf(szFleeRate,sizeof(szFleeRate),TEXT("%5.2f%%"),dFleeRate);
		else szFleeRate[0]=0;
		
		TCHAR szLevel[128];
		if (m_GameRankManager.GetInterface()!=NULL)
		{
			LPCTSTR pszGameRank=m_GameRankManager->GetLevelDescribe(pUserData->lScore);
			lstrcpyn(szLevel,pszGameRank,sizeof(szLevel));
		}else szLevel[0] = 0;

		//�û�״̬����
		_snprintf(szBuffer, sizeof(szBuffer), "�û���: %s\n    ID: %d\n��  ��: %s\n���ֶ�: %I64d\n"
			"��  ��: %d\n"
			"��  ��: %s\n"
			"    Ӯ: %ld\n"
			"    ��: %ld\n"
			"    ��: %ld\n"
			"    ��: %ld\n"
			"ʤ  ��: %s\n",
			pUserData->szName, pUserData->dwGameID,\
			pUserData->cbGender==0?"Ů":"��",pUserData->lScore, pUserData->lExperience, szLevel,pUserData->lWinCount, pUserData->lLostCount, pUserData->lDrawCount, pUserData->lFleeCount, szWinRate);
		
	}
	return szBuffer;

}

BOOL CTableFrame::OnCommand( WPARAM wParam, LPARAM lParam )
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	switch(nCommandID)
	{
	case 20088:
		m_pITableFrameSink->OnStartGame(0,0,false);
		break;
	}

	return __super::OnCommand(wParam,lParam);
}

bool __cdecl CTableFrame::ShowStartBtn( WORD wTableID,WORD wChairId, bool bShowStart )
{
	if(m_wMeTableID != wTableID)
	{
		m_wMeTableID = wTableID;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////