#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <string.h>
#include <time.h>
#include "cfg.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <mysql.h>
#include <DBIf.h>
#include "DBLib.h"
#include "DBParamBinding.h"
#include "../json/json.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "JsonValue.h"

   

#define HOSTLEN 256
#define IMS_DB_DATA_ID_LENGTH 8

typedef struct tagDBDataCallBackInfo
{
	char		szDataID[IMS_DB_DATA_ID_LENGTH];
	int		bMultiRows;
	int		bInsertLastFlag;									
	JsonWriter*	pJsonWriter;
	JsonValue*	pArNode;
	JsonValue*	pCurNode;     
}DBDataCallBackInfo,*LPDBDataCallBackInfo;


void*	networkthread(void* obj);

int	doclientReq(dbCfg* pdbcfg,char* lpBuffer,unsigned int uSize, char** lpResult,unsigned int* uResultSize);
int	insertNewjob(dbCfg* pdbcfg,jobbase* lpJob, char** lpResult,unsigned int* uResultSi1ze);
int	queryjob(dbCfg* pdbcfg,jobbase* lpJob, char** lpResult,unsigned int* uResultSize);
int	getrequest(char* lpBuffer,unsigned int uSize,jobbase** lpjob);
int	createJsonresponse(int nReq,int nSeq,int nResp,Json::Value & result, char** lpResult,unsigned int* nResultSize);

int	initdispatch(int ninterval,dbCfg*  pdbcfg);


#define DB_STRING_BUFFER_LENGTH 4096

unsigned int db_Request_Data_CallBack(void* hDBWorkItemInfo
				      ,void* hUserBuffer
				      , unsigned int uDBRecordBinding
				      ,void* lpDBRecord
				      ,unsigned int uDBEncoding
				      ,char*	lpDataBuffer)
     {
     unsigned int		hRetCode	    = 0;
     LPDBRecordBinding		pDBRecord	    = (LPDBRecordBinding)lpDBRecord;
     DBDataCallBackInfo*	pDBDataCallBackInfo = (DBDataCallBackInfo*)hUserBuffer;
     JsonWriter*		pJsonWriter	    = pDBDataCallBackInfo->pJsonWriter;
     JsonValue*			pCurNode	    = pDBDataCallBackInfo->pCurNode;
     JsonValue*			pArNode		    = pDBDataCallBackInfo->pArNode;
     JsonValue			curJValue;
     char			szBuffer[DB_STRING_BUFFER_LENGTH];
     
	

     // if lpDataBuffer is empty ,then it is end of recordset 
     if(lpDataBuffer != NULL)
     {
	  for (unsigned int uBindIndex = 0;uBindIndex< uDBRecordBinding;uBindIndex++)
	  {
	       bzero(szBuffer,DB_STRING_BUFFER_LENGTH);
	       if(pDBRecord->uRetColSize > 0)
	       {
		    switch (pDBRecord->uDataType)
		    {
		    case DB_TYPE_DECIMAL:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_TINY:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_SHORT:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(short*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(short*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(short*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_LONG:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_FLOAT:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_DOUBLE:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_NULL:
			 break;
		    case DB_TYPE_TIMESTAMP:
			 memcpy(szBuffer,lpDataBuffer + pDBRecord->uRetColOffSet,pDBRecord->uRetColSize);
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,szBuffer);
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,szBuffer);
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,szBuffer);
			 }
			 break;
		    case DB_TYPE_LONGLONG:
		    case DB_TYPE_INT24:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
			 break;
		    case DB_TYPE_DATE:
		    case DB_TYPE_TIME:
		    case DB_TYPE_DATETIME:
		    case DB_TYPE_YEAR:
		    case DB_TYPE_NEWDATE:
		    case DB_TYPE_VARCHAR:
			 memcpy(szBuffer,lpDataBuffer + pDBRecord->uRetColOffSet,pDBRecord->uRetColSize);
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,szBuffer);
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,szBuffer);
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,szBuffer);
			 }
			 break;
		    case DB_TYPE_BIT:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_NEWDECIMAL:
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   pCurNode->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			      else
			      {
				   pJsonWriter->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			      }
			 }
			 else
			 {
			      curJValue.SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
			 }
			 break;
		    case DB_TYPE_ENUM:
		    case DB_TYPE_SET:
		    case DB_TYPE_TINY_BLOB:
		    case DB_TYPE_MEDIUM_BLOB:
		    case DB_TYPE_LONG_BLOB:
		    case DB_TYPE_BLOB:
		    case DB_TYPE_VAR_STRING:
		    case DB_TYPE_STRING:
		    case DB_TYPE_GEOMETRY:
			 memcpy(szBuffer,lpDataBuffer + pDBRecord->uRetColOffSet,pDBRecord->uRetColSize);
			 if(pDBDataCallBackInfo->bMultiRows == 0)
			 {
			      if(pCurNode != NULL)
			      {
				   if(strcmp(JKEY_SVR_JSON_OBJ,pDBRecord->szDataID))
				   {
					pCurNode->SetValue(pDBRecord->szDataID,szBuffer);
				   }
				   else
				   {
					Json::Reader	jsonReader;
					Json::Value	tempValue;
					std::string	strJsonSrc;
					strJsonSrc		= std::string((char*)szBuffer,strlen(szBuffer));	
					if(jsonReader.parse(strJsonSrc,tempValue,false) == true)
					{
					     pCurNode->SetValue(tempValue);
					}

				   }
			      }
			      else
			      {
				   if(strcmp(JKEY_SVR_JSON_OBJ,pDBRecord->szDataID))
				   {
					pJsonWriter->SetValue(pDBRecord->szDataID,szBuffer);
				   }
				   else
				   {
					Json::Reader	jsonReader;
					Json::Value		tempValue;
					std::string		strJsonSrc;
					strJsonSrc		= std::string((char*)szBuffer,strlen(szBuffer));	
					if(jsonReader.parse(strJsonSrc,tempValue,false) == true)
					{
					     pJsonWriter->SetValue(JKEY_SVR_JSON_OBJ,tempValue);
					}
				   }
			      }
			 }
			 else
			 {
			      if(strcmp(JKEY_SVR_JSON_OBJ,pDBRecord->szDataID))
			      {
				   curJValue.SetValue(pDBRecord->szDataID,szBuffer);
			      }
			      else
			      {
				   Json::Reader	jsonReader;
				   Json::Value		tempValue;
				   std::string		strJsonSrc;
				   strJsonSrc		= std::string((char*)szBuffer,strlen(szBuffer));	
				   if(jsonReader.parse(strJsonSrc,tempValue,false) == true)
				   {
					curJValue.SetValue(JKEY_SVR_JSON_OBJ,tempValue);
				   }
			      }
			 }
			 break;					
		    }
	       }
	       pDBRecord++;
	  }
	  if(pDBDataCallBackInfo->bMultiRows)
	  {
	       pArNode->SetValue(&curJValue);
	  }
     }
     else
     {
	  if(pDBDataCallBackInfo->bMultiRows && pDBDataCallBackInfo->bInsertLastFlag)
	  {
			
	       if(pCurNode != NULL)
	       {
		    pCurNode->SetValue();
		    pCurNode->SetValue(pDBDataCallBackInfo->szDataID,*pArNode);
	       }
	       else
	       {
		    pArNode->SetValue();
		    pJsonWriter->SetValue(pDBDataCallBackInfo->szDataID,*pArNode);
	       }
	  }
     }
     return hRetCode;
	
}







void showprogress(char* lpszNotice)
{
     printf("%s\n",lpszNotice);
     fflush(stdout);
     sleep(1);
}

void showerrmsg(char* lpszNotice)
{
     printf("errcode is %d\n",errno);
     perror(lpszNotice);
}



void* svrthreadproc(void* obj)
{
     int		nRet	      = 0;
     int		ntry	      = 0;
     appsetting*	pAps	      = NULL;
     struct sockaddr_in saddr	      = {0};
     struct hostent*	hp	      = NULL;
     int		sock_id	      = 0;
     int		sock_cli      = 0;
     char hostname[HOSTLEN]	      = {0};
     workitemCfg*	pwiCfg	      = NULL;
     pthread_t		netthreadid   = {0};
     pthread_attr_t	attr_detached = {0};
     
     
     
     
     

     pAps = (appsetting*)(obj);

     initdispatch(2,&pAps->dbc);
     
     pthread_attr_init(&attr_detached);
     pthread_attr_setdetachstate(&attr_detached,PTHREAD_CREATE_DETACHED);
     
     /* in app thread , tcp socket accept and create work thread
	tcp socket listen and wait accept*/
     sock_id = socket(PF_INET,SOCK_STREAM,0);
     if(sock_id == -1)
     {
	  showerrmsg("create sock fail");
	  pthread_exit(NULL);	  
     }
     
     bzero((void*)&saddr,sizeof(saddr));
     showprogress("prepare socket listen");
     gethostname(hostname,HOSTLEN);
     /*   hp		   = gethostbyname(hostname);
	  bcopy((void*)hp->h_addr,(void*)&saddr.sin_addr,hp->h_length);*/
     saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

     saddr.sin_port	   = htons(pAps->nc.nport);
     saddr.sin_family	   = AF_INET;
     
     if(bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr)) != 0)
     {
	  showerrmsg("bind sock fail");
	  pthread_exit(NULL);	       
     }
     showprogress("prepare socket accept");
     if(listen(sock_id,1) != 0)
     {
	  showerrmsg("listen sock fail");
	  pthread_exit(NULL);	            	  
     }

     while(1)
     {
	  sock_cli = accept(sock_id,NULL,NULL);
	  if(sock_cli == -1)
	  {
	       showerrmsg("accept failed");
	       break;
	  }
	  pwiCfg	  = (workitemCfg*)malloc(sizeof(workitemCfg));
	  pwiCfg->sock_id = sock_cli;
	  memcpy((void*)&pwiCfg->dbc,(void*)&pAps->dbc,sizeof(dbCfg));
	  
	  nRet = pthread_create(&netthreadid,&attr_detached,networkthread,pwiCfg);
	  if(nRet != 0)
	  {
       	       showerrmsg("pthread create failed.");
	       break;
	  }
	  
     }     
     pthread_exit(NULL);
}


void* networkthread(void* lpObj)
{
     time_t		thetime	       = {0};
     workitemCfg*	pwi	       = (workitemCfg*)lpObj;
     char               szBuffer[1024] = {0};
     char               szSend[1024]   = {0};
     unsigned int	uSize	       = 0;
     unsigned int	uResultSize    = 0;
     int		nOffset	       = 0;
     int		nRecvSize      = 0;
     int		nSendSize      = 0;
     int		nResult	       = 0;
     char*		pBuffer	       = NULL;
     
          
     
     
     
     bzero(szBuffer,1024);
     while(nOffset < 4)
     {
	  nRecvSize = recv(pwi->sock_id,szBuffer+nOffset,4,0);
	  if(nRecvSize == -1)
	  {
	       break;
	  }
	  nOffset += nRecvSize;	  
     }
     if(nRecvSize > 0)
     {
	  uSize	= *(unsigned int*)(szBuffer);
	  bzero(szBuffer,1024);
	  if(uSize < 1024)
	  {
	       nOffset = 0;	       
	       while(nOffset < uSize)
	       {
		    nRecvSize = recv(pwi->sock_id,szBuffer+nOffset,uSize-nOffset,0);
		    if(nRecvSize == -1)
		    {
			 break;
		    }
		    nOffset += nRecvSize;

	       }
	       if(nRecvSize == uSize)
	       {
		    nResult = doclientReq(&pwi->dbc,szBuffer,uSize,&pBuffer,&uResultSize);
	       }

	       nOffset = 0;
	       while(nOffset < uResultSize)
	       {
		    nSendSize = send(pwi->sock_id,pBuffer+nOffset,uResultSize-nOffset,0);
		    if(nSendSize == -1)
		    {
			 break;
		    }
		    nOffset += nSendSize;
	       }
	       if(pBuffer != NULL)
	       {
		    free(pBuffer);
	       }
	  }
     }
     close(pwi->sock_id);
     free(lpObj);
     pthread_exit(NULL);
}




int doclientReq(dbCfg*  pdbcfg, char* lpBuffer,unsigned int uSize, char** lpResult,unsigned int* uResultSize)
{
     int	nResult = 0;
     jobbase*	pReq	= NULL;
     
     
     /* resolve json and create mysql instance */
     nResult = getrequest(lpBuffer,uSize,&pReq);
     if(nResult != 0)
     {
	  return createresponse(0,0,404,0,lpResult, uResultSize);
     }

     /* return json data to client     */
     if(pReq->nReqId == REQ_CREATE)
     {

	  nResult = insertNewjob(pdbcfg,pReq,lpResult,uResultSize);
     }
     else if(pReq->nReqId == REQ_QUERY)
     {
	  nResult = queryjob(pdbcfg,pReq,lpResult,uResultSize);
     }
     free(pReq);     
     return nResult;
     
     /* in dispatch thread, pipe client request to shell to store into database*/
}

void* AllocMemory(unsigned int size)
{
     return malloc(size);
}

void ReleaseMemory(void* lpBuffer)
{
     free(lpBuffer);
}


int connectdb(dbCfg* pdbcfg,void** lpDBHandle,void** lpDBIf)
{
     void*		pDBHandle     = NULL;
     void*		pAppIf	      = NULL;
     DBIf*		pDBIf	      = NULL;
     DBSetting		dbsetting     = {0};
     DBConnectInfo	dbConnectInfo = {0};
     DBErrorInfo	dbError	      = {0};
     int		nRet	      = 0;
     
     
     
     nRet			= AppQueryInterface(&pAppIf);
     if(nRet != 0) goto LB_EXIT;
     pDBIf			= (DBIf*)pAppIf;
     dbsetting.pfnAllocMemory	= AllocMemory;
     dbsetting.pfnReleaseMemory = ReleaseMemory;
     dbsetting.uKeepConnection	= 1;
     dbsetting.bAutoSubmit	= 1;
     pDBIf->pfnCreateInstance(&pDBHandle,&dbsetting);
     strcpy(dbConnectInfo.szDBAddress,pdbcfg->szaddr);
     strcpy(dbConnectInfo.szDBName,pdbcfg->szinstname);
     strcpy(dbConnectInfo.szUserName,pdbcfg->szaccname);
     strcpy(dbConnectInfo.szUserPwd,pdbcfg->szaccpwd);
     nRet			= pDBIf->pfnConnect(pDBHandle,&dbConnectInfo,&dbError);
     if(nRet != 0) goto LB_EXIT;
     *lpDBHandle		= pDBHandle;
     *lpDBIf			= pDBIf;
     
LB_EXIT:
     if(nRet != 0)
     {
	  if(pDBHandle != NULL)
	  {
	       pDBIf->pfnReleaseInstance(pDBHandle);
	  }
	  AppReleaseInterface(pDBIf);
     }
     return nRet;

}

void disconnectdb(void* pDBHandle,void* lpDBIf)
{
     DBIf*		pDBIf	      = NULL;
     pDBIf = (DBIf*)lpDBIf;
     if(pDBHandle != NULL)
     {
	  pDBIf->pfnClose(pDBHandle);
	  pDBIf->pfnReleaseInstance(pDBHandle);     
     }
     AppReleaseInterface(pDBIf);
}

	      
int insertJob(void* pDBHandle,void* lpDBIf,jobCreateReq*  lpReq,Json::Value & result)
{
	
     DBParamBinding	dbParamBinding[5];
     DBRecordBinding	dbRecordBinding[1];
     DBDataCallBackInfo dbCallBackInfo = {0};
     DBIf*		pDBIf	       = NULL;
     DBErrorInfo	dbErrorInfo    = {0};
     int		nRet	       = 0;
     unsigned int	uRecordCount   = 0;
     JsonWriter 	jsonWriter;
     JsonValue		curJsonValue;
     MYSQL_TIME		btTime	       = {0};
     MYSQL_TIME		expTime	       = {0};
     
	  
	
     DBParamBinding dbPB_InsertTask[] = {
	  {"tskname",DB_TYPE_VAR_STRING,45 ,0,0,NULL,0,0}
	  ,{"tskbegintime",DB_TYPE_DATETIME,0 ,0,0,NULL,0,0}
	  ,{"tskexpiredtime",DB_TYPE_DATETIME,0 ,0,0,NULL,0,0}
	  ,{"tskinterval",DB_TYPE_LONG	,4  ,0,0,NULL,0,0}
	  ,{"tskData",DB_TYPE_VAR_STRING,1024 ,0,0,NULL,0,0}	     
     };

     DBRecordBinding dbRB_InsertTask[] = {
	  {0	,0	,0	,0	,DB_TYPE_LONG,0,JSK_JOB_ID},
     };

     pDBIf = (DBIf*)lpDBIf;
     IMS_ClearDBError(&dbErrorInfo);

     dbCallBackInfo.bMultiRows	    = 0;
     dbCallBackInfo.bInsertLastFlag = 0;
     dbCallBackInfo.pJsonWriter	    = &jsonWriter;
     dbCallBackInfo.pCurNode	    = &curJsonValue;
     

     IMS_FillDBParamBindingData(dbPB_InsertTask,0,(void*)lpReq->szName,strlen(lpReq->szName),0);
     ConvertSystemTimeToMYSQLTime(&lpReq->tbegintime,&btTime);
     IMS_FillDBParamBindingData(dbPB_InsertTask,1,&btTime,0,0);
     ConvertSystemTimeToMYSQLTime(&lpReq->texpiredtime,&expTime);
     IMS_FillDBParamBindingData(dbPB_InsertTask,2,&expTime,0,0);
     IMS_FillDBParamBindingData(dbPB_InsertTask,3,&lpReq->nInterval,sizeof(int),0);
     IMS_FillDBParamBindingData(dbPB_InsertTask,4,lpReq->context,lpReq->nContextSize,0);     
     
     // pDBIf->pfn
     nRet = DB_ExecStoredProc2(pDBHandle
				      ,"call inserttask(?,?,?,?,?);"
				      ,5
				      ,dbPB_InsertTask
				      ,1
				      ,dbRB_InsertTask
				      ,&uRecordCount
				      ,NULL
				      ,NULL
				      ,&dbCallBackInfo
				      ,db_Request_Data_CallBack
				      ,NULL
				      ,&dbErrorInfo);
     
     if(nRet == 0)
     {
	  result = curJsonValue.GetJValue();
     }	
     return nRet;	 
}

int queryJobResult(void* pDBHandle,void* lpDBIf,jobQueryReq*  lpReq,Json::Value & result)
{
  
     DBDataCallBackInfo dbCallBackInfo = {0};
     DBIf*		pDBIf	       = NULL;
     DBErrorInfo	dbErrorInfo    = {0};
     int		nRet	       = 0;
     unsigned int	uRecordCount   = 0;
     JsonWriter 	jsonWriter;     
     JsonValue		curJsonValue;
     
	  
	
     DBParamBinding dbParamBinding[] = {
	  {"tskid",DB_TYPE_LONG	,0 ,0,0,NULL,0,0}
     	  ,{"trid",DB_TYPE_LONG	,0 ,0,0,NULL,0,0}
     };

     DBRecordBinding dbRecordBinding[] = {
	  {0	,0	,0	,0	,DB_TYPE_LONG,0,JSK_JOB_EXECUTE_CODE}
	  ,{0	,0	,0	,0	,DB_TYPE_LONG,0,JSK_JOB_RESULT}
	  ,{0	,0	,0	,0	,DB_TYPE_VARCHAR,0,JSK_JOB_RESPONSE_DATA}	  
     };

     pDBIf = (DBIf*)lpDBIf;
     IMS_ClearDBError(&dbErrorInfo);

     dbCallBackInfo.bMultiRows	    = 1;
     dbCallBackInfo.bInsertLastFlag = 0;
     dbCallBackInfo.pJsonWriter	    = &jsonWriter;
     dbCallBackInfo.pArNode	    = &curJsonValue;
     

     IMS_FillDBParamBindingData(dbParamBinding,0,&lpReq->nId,sizeof(int),0);
     IMS_FillDBParamBindingData(dbParamBinding,1,&lpReq->nTrId,sizeof(int),0);
     
     
     nRet = DB_ExecStoredProc2(pDBHandle
				      ,"call querytaskresult(?,?);"
				      ,2
			              ,dbParamBinding
				      ,3
				      ,dbRecordBinding
				      ,&uRecordCount
				      ,NULL
				      ,NULL
				      ,&dbCallBackInfo
				      ,db_Request_Data_CallBack
				      ,NULL
				      ,&dbErrorInfo);
     
     if(nRet == 0)
     {
	  result[JSK_ARRAY] = curJsonValue.GetJValue();
     }	
     return nRet;
         
}




int fetchJobContext(void* pDBHandle,void* lpDBIf,  Json::Value & result)
{
     DBDataCallBackInfo dbCallBackInfo = {0};
     DBIf*		pDBIf	       = NULL;
     DBErrorInfo	dbErrorInfo    = {0};
     int		nRet	       = 0;
     unsigned int	uRecordCount   = 0;
     JsonWriter 	jsonWriter;     
     JsonValue		curJsonValue;    

     DBRecordBinding dbRecordBinding[] = {
	  {0	,0	,0	,0	,DB_TYPE_LONG,0,JSK_JOB_ID}
	  ,{0	,0	,0	,0	,DB_TYPE_VARCHAR,0,JSK_JOB_CONTEXT}	  
     };

     pDBIf = (DBIf*)lpDBIf;
     IMS_ClearDBError(&dbErrorInfo);

     dbCallBackInfo.bMultiRows	    = 1;
     dbCallBackInfo.bInsertLastFlag = 0;
     dbCallBackInfo.pJsonWriter	    = &jsonWriter;
     dbCallBackInfo.pArNode	    = &curJsonValue;  
     
     
     nRet = DB_ExecStoredProc2(pDBHandle
				      ,"call fetchtask();"
				      ,0
			              ,NULL
				      ,2
				      ,dbRecordBinding
				      ,&uRecordCount
				      ,NULL
				      ,NULL
				      ,&dbCallBackInfo
				      ,db_Request_Data_CallBack
				      ,NULL
				      ,&dbErrorInfo);
     
     if(nRet == 0)
     {
	  result[JSK_ARRAY] = curJsonValue.GetJValue();
     }	
     return nRet;
}

int insertJobResult(void* pDBHandle,void* lpDBIf, int tskID,int retCode, int nDataSize,char* lpResp, Json::Value & result)
{
     DBDataCallBackInfo dbCallBackInfo = {0};
     DBIf*		pDBIf	       = NULL;
     DBErrorInfo	dbErrorInfo    = {0};
     int		nRet	       = 0;
     unsigned int	uRecordCount   = 0;
     JsonWriter 	jsonWriter;     
     JsonValue		curJsonValue;    


     	
     DBParamBinding dbParamBinding[] = {
	  {"tskid",DB_TYPE_LONG,4 ,0,0,NULL,0,0}
	  ,{"retcode",DB_TYPE_LONG	,4  ,0,0,NULL,0,0}
	  ,{"resp",DB_TYPE_VAR_STRING,1024 ,0,0,NULL,0,0}	     
     };

     
     DBRecordBinding dbRecordBinding[] = {
	  {0	,0	,0	,0	,DB_TYPE_LONG,0,JSK_JOB_EXECUTE_CODE}	  
     };

     pDBIf = (DBIf*)lpDBIf;
     IMS_ClearDBError(&dbErrorInfo);

     IMS_FillDBParamBindingData(dbParamBinding,0,(void*)&tskID,sizeof(int),0);
     IMS_FillDBParamBindingData(dbParamBinding,1,(void*)&retCode,sizeof(int),0);
     IMS_FillDBParamBindingData(dbParamBinding,2,lpResp,nDataSize,0);     
          

     dbCallBackInfo.bMultiRows	    = 0;
     dbCallBackInfo.bInsertLastFlag = 0;
     dbCallBackInfo.pJsonWriter	    = &jsonWriter;
     dbCallBackInfo.pCurNode	    = &curJsonValue;  
     
     
     nRet = DB_ExecStoredProc2(pDBHandle
				      ,"call insertresult(?,?,?);"
				      ,3
			              ,dbParamBinding
				      ,1
				      ,dbRecordBinding
				      ,&uRecordCount
				      ,NULL
				      ,NULL
				      ,&dbCallBackInfo
				      ,db_Request_Data_CallBack
				      ,NULL
				      ,&dbErrorInfo);
     
     if(nRet == 0)
     {
	  result = curJsonValue.GetJValue();
     }	
     return nRet;
}



int insertNewjob(dbCfg* pdbcfg,jobbase* lpJob, char** lpResult,unsigned int* uResultSize)
{
     /* in work thread , connect mysql and update database
	after connected,then make up of sql statement to excute */
     jobCreateReq*	jc	  = (jobCreateReq*)lpJob;
     void*		pDBHandle = NULL;
     void*		pDBIf     = NULL;
     int		nRet	  = 0;
     Json::Value	result;
     
     nRet = connectdb(pdbcfg,&pDBHandle,&pDBIf);
     if(nRet == 0)
     {
	  nRet = insertJob(pDBHandle, pDBIf,jc,result);
	  if(nRet == 0)
	  {
	       createJsonresponse(lpJob->nReqId,lpJob->nSeq,0, result, lpResult,uResultSize);
	  }	  
	  disconnectdb(pDBHandle,pDBIf);
	  return 0;
     }
     
     
     return createresponse(lpJob->nReqId,lpJob->nSeq,nRet,0,lpResult, uResultSize);
}

int queryjob(dbCfg* pdbcfg,jobbase* lpJob, char** lpResult,unsigned int* uResultSize)
{
     /* in work thread , connect mysql and update database
	after connected,then make up of sql statement to excute     */

     jobQueryReq*	jc	  = (jobQueryReq*)lpJob;
     void*		pDBHandle = NULL;
     void*		pDBIf     = NULL;
     int		nRet	  = 0;
     Json::Value	result;
     
     nRet = connectdb(pdbcfg,&pDBHandle,&pDBIf);
     if(nRet == 0)
     {
	  nRet = queryJobResult(pDBHandle, pDBIf,jc,result);
	  //pass nRet = fetchJobContext(pDBHandle,pDBIf,  result);
	  //pass nRet = insertJobResult(pDBHandle,pDBIf,5,100,10,"1234567890",result);
	  if(nRet == 0)
	  {
	      createJsonresponse(lpJob->nReqId,lpJob->nSeq,0, result, lpResult,uResultSize);
	  }	  
	  disconnectdb(pDBHandle,pDBIf);
	  return 0;
     }
     
     
     return createresponse(lpJob->nReqId,lpJob->nSeq,nRet,0,lpResult, uResultSize);     

}



