#ifndef DOWN_LOAD_SERVICE_HEAD_FILE
#define DOWN_LOAD_SERVICE_HEAD_FILE

#pragma once

#include "AfxInet.h"
#include "Resource.h"
#include "DownLoad.h"

//��˵��
class CDownLoadMission;
class CDownLoadService;
class CDownLoadThread;
//typedef CArrayTemplate<CDownLoadMission *> CDownLoadMissionArray;  2011.3.9

// ���߳����أ� 2011.3.9
typedef CArrayTemplate<CDownLoadThread *> CDownLoadThreadArray;


//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����״̬
enum enDownLoadStatus
{
	enDownLoadStatus_Unknow,			//δ֪״̬
	enDownLoadStatus_Ready,				//׼��״̬
	enDownLoadStatus_DownLoadIng,		//����״̬
	enDownLoadStatus_Finish,			//���״̬
	enDownLoadStatus_Fails,				//����ʧ��
	enDownLoadStatus_Stop,				//����ȡ��

};

//����ö��
enum enDownLoadResult
{
	enDownLoadResult_Noknow,			//û�д���
	enDownLoadResult_Exception,			//�쳣����
	enDownLoadResult_CreateFileFails,	//����ʧ��
	enDownLoadResult_InternetReadError,	//��ȡ����
};

//����״̬
struct tagDownLoadStatus
{
	WORD								wProgress;						//���ؽ���
	TCHAR								szStatus[128];					//״̬����
	enDownLoadStatus					DownLoadStatus;					//����״̬
};

//////////////////////////////////////////////////////////////////////////


//�����߳�
class CDownLoadThread : public CServiceThread
{
	//��������
public:
	DWORD							m_dwMissionID;						//�����ʶ	2011.3.9
	DWORD							m_dwDownLoadType;					//��������	2011.3.9
	tagDownLoadRequest				m_DownLoadRequest;					//��������  2011.3.9

protected:
	bool							m_bPreparative;						//��ʼ��־

	//��Ϣ����
protected:
	TCHAR							m_szTempFile[MAX_PATH];				//��ʱ�ļ�
	TCHAR							m_szLocalFile[MAX_PATH];			//�����ļ�

	//״̬����
protected:
	CCriticalSection				m_CriticalSection;					//�߳�����
	enDownLoadStatus				m_DownLoadStatus;					//����״̬
	enDownLoadResult				m_DownLoadResult;					//���״̬
	DWORD							m_dwOldDownLoadSize;					//�ϴ������ļ���С		// 2011.3.9
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//�̱߳���
protected:
	CFile							m_LocalFile;						//�����ļ�
	CHttpFile						* m_pHttpFile;						//�����ļ�
	CHttpConnection					* m_pHttpConnection;				//��������
	CInternetSession				m_InternetSession;					//����Ự

	//��������
public:
	//���캯��
//	CDownLoadThread();
		//���캯��
	CDownLoadThread( );
	//��������
	virtual ~CDownLoadThread();

	//���ܺ���
public:
	//��ʼ���߳�
	bool InitThread(tagDownLoadRequest * pDownLoadRequest);
	//����״̬
	void GetDownLoadStatus(tagDownLoadStatus & DownLoadStatus);
	//����״̬
	void SetDownLoadStatus( enDownLoadStatus status1 );
	//Ŀ���ļ�
	LPCTSTR GetDownLoadFileName();
	DWORD	GetTotalFileSize() { return m_dwTotalFileSize; }
	DWORD	GetBual();
	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
	//�ر��¼�
	virtual bool OnEventThreadConclude();

	//�̺߳���
private:
	//����׼��
	void DownLoadPreparative();
	//��������
	void DownLoadCleanUp();
};

//////////////////////////////////////////////////////////////////////////
//����ص��ӿ�
interface IDownLoadMissionSink
{
	//����֪ͨ
	virtual void OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadThread * pDownLoadThread)=NULL;
};
///////////////////////////////////////////////////////////////////////////

//��������
class CDownLoadMission : public CSkinDialogEx, public IDownLoadMissionSink
{
	//��������
protected:
//	DWORD								m_dwMissionID;					//�����ʶ	2011.3.9
//	DWORD								m_dwDownLoadType;				//��������	2011.3.9
//	tagDownLoadRequest					m_DownLoadRequest;				//��������  2011.3.9
	IDownLoadMissionSink				* m_pIDownLoadMissionSink;		//�ص��ӿ�

	//�ؼ�����
protected:
	CSkinButton							m_btReTry;						//�򿪰�ť
	CSkinButton							m_btCancel;						//�򿪰�ť
//	CProgressCtrl						m_ProgressCtrl;					//���ȿؼ�
//	CDownLoadThread						m_DownLoadThread;				//�����߳�
	CDownLoadThreadArray				m_DownLoadThreadActive;			//�����̶߳���	2011.3.9
	CDownLoadThreadArray				m_DownLoadThreadRelease;		//�����̶߳���	2011.3.9
	CSkinHyperLink						m_DownLoadUrl;					//���ص�ַ

	CSkinListProgressCtrl				m_DownloadListView;

	CCriticalSection					m_CriticalSection;					//�߳�����

	//��������
public:
	//���캯��
//	CDownLoadMission(IDownLoadMissionSink * pIDownLoadMissionSink);
	CDownLoadMission( );
	//��������
	virtual ~CDownLoadMission();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel();
	//�¼��ӿ�
public:
	//����֪ͨ
	virtual void OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadThread * pDownLoadThread);		// 2011.3.9

	//���ܺ���
public:
	//��ȡ��Ŀ
	virtual INT_PTR __cdecl GetDownLoadMissionCount();
	//�����ʶ
	DWORD GetMissionID() { return 0; } //m_dwMissionID; }
	//��������
//	DWORD GetDownLoadType() { return m_dwDownLoadType; }
	//��ʼ����
	bool StartDownLoad(DWORD dwMissionID, DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest);
	//ֹͣ����
	bool StopDownLoad();
	//�Ա�����
	bool CompareRequest(tagDownLoadRequest * pDownLoadRequest);
	//��ʾ����
	bool DisplayDownLoadFace(bool bDisplay);
//Ѱ������
	CDownLoadThread * SearchDownLoadThread( DWORD dwMissionID );
	CDownLoadThread * SearchDownLoadThread(tagDownLoadRequest * pDownLoadRequest);
	//��Ϣ����
protected:
	//���԰�ť
	afx_msg void OnBnClickedReTry();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HRESULT OnButtonPu(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���ط���
class CDownLoadService : public IDownLoadService
{
	//��������
protected:
	DWORD								m_dwMissionID;					//�����ʶ	(����������)
	IDownLoadServiceSink				* m_pIDownLoadServiceSink;		//�ص��ӿ�
//	CDownLoadMissionArray				m_DownLoadMissionActive;		//��������	2011.3.9
//	CDownLoadMissionArray				m_DownLoadMissionRelease;		//��������	2011.3.9
	CDownLoadMission					m_CDownLoadMission;				 //�������񴰿� 	2011.3.9

	//��������
public:
	//���캯��
	CDownLoadService();
	//��������
	virtual ~CDownLoadService();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ȡ��Ŀ
	virtual INT_PTR __cdecl GetDownLoadMissionCount();
	//���ýӿ�
	virtual bool __cdecl SetDownLoadServiceSink(IUnknownEx * pIUnknownEx);
	//��������
	virtual DWORD __cdecl AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest);


	//�ڲ�����
private:
	//��������
//	CDownLoadMission * SearchMission(tagDownLoadRequest * pDownLoadRequest);
};

//////////////////////////////////////////////////////////////////////////

#endif