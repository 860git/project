#pragma once

#include "Stdafx.h"
#include "Resource.h"


class SuoJi
{
public:

	SuoJi();
	~SuoJi();
public:
	 
	ITCPSocket					  * m_pIClientSocket;					//�������

	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	int                            m_LpSuoJi;
public:
   
  bool	ConnectServer();

  void SenDdatelock();
  void SenDdateAnlock();

int ShowMessageBox(LPCTSTR pszMessage, UINT nType);





};



















