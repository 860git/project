#include "stdafx.h"
#include "debuglog.h"
/********************************************************************
 * ������       : debuglog()
 * ����         : д����������־�ļ�
 * ����         : 
 * �������     : char *fmt, ...
 * �������     : 
 * ����������� :
 * ����ֵ       : 
 *                
 * �����       : xuerui(rxue@netkiss.com.cn)
 * ������       : 2004-9-12
 ********************************************************************/
time_t timer;
struct tm *t;
char* __file__;
int __line__;

int DebugLogOut(char *fmt, ...)
              {
                   va_list ap;               	         
	               char	sDbgFile[256];
	               char	sOutBuf[4096];
	               time_t	iCurTim;
	               static time_t	iOldTim=0;
	               static int  DbgFd = -1;
	               struct tm stLocTim;
	               struct stat stStat;
                   iCurTim = time(NULL);
                   va_start(ap, fmt);
				   ofstream fout1;
			       stLocTim = *localtime(&iCurTim);	
                   sprintf(sOutBuf,"<%02d��%02d��%02dʱ%02d��%02d>  ",
			        stLocTim.tm_mon + 1,stLocTim.tm_mday,
			       stLocTim.tm_hour,stLocTim.tm_min,stLocTim.tm_sec
			       );

			       sprintf(sDbgFile,"%s.%02d%02d%02d%s","Ox",stLocTim.tm_year % 100,stLocTim.tm_mon + 1,stLocTim.tm_mday,".txt");		
				   //AfxMessageBox(sDbgFile);
		   	       if(stat(sDbgFile,&stStat) == -1)//�����־�ļ�����������
				   {
					   
					   ofstream fout (sDbgFile);					  
		               fout<<sOutBuf;
	                   if(strlen(fmt) > 1024){
	 	                fout<<fmt;
					   }else{
		               va_start(ap,fmt);
		               (void)vsprintf(sOutBuf,fmt,ap);
		               va_end(ap);	
					   strcat(sOutBuf, "\n");
		               fout<<sOutBuf;

 					   //HWND hEdit= ::FindWindow("Notepad","debug.txt - ���±�"); 
					   //if(hEdit!=NULL)
 						//   SendMessage (FindWindowEx(hEdit,NULL,"Edit",""), EM_REPLACESEL, 0, (LPARAM) ((LPSTR)sOutBuf)); 
 					   }		
				   }
				 else{
					
					  fout1.open(sDbgFile,ios_base::out|ios_base::app);//��������׷�ӷ�ʽ��					  
		              fout1<<sOutBuf;
	                  if(strlen(fmt) > 1024){
	 	               fout1<<fmt;
					  }else{
		                va_start(ap,fmt);
		                (void)vsprintf(sOutBuf,fmt,ap);
		                va_end(ap);	
						strcat(sOutBuf, "\n");
		                fout1<<sOutBuf; 
 						//HWND hEdit= ::FindWindow("Notepad","debug.txt - ���±�");
 						//if(hEdit!=NULL)
 						//	SendMessage (FindWindowEx(hEdit,NULL,"Edit",""), EM_REPLACESEL, 0, (LPARAM) ((LPSTR)sOutBuf)); 
					  }		
				 }

	              return 1;                                  
                  va_end(ap);
 }
