#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <glib.h> 
#include <sqlite3.h>
#include "SqliteIf.h"
#include "SqliteLib.h"
#include "sqliteFunc.h"
#include "../json/json.h"


#define SQL_BUFFER_SIZE 2048

#define SC_INSERT_TASK "insert into cpytsk([tid],[tname],[tbegintime],[texpiredtime],[tinterval],[tdata],[ttryid]) values (%d ,'%s','%s','%s',%d,'%s',%d);"
#define SC_INSERT_RESP "insert into tskrsp([tryid],[tid],[result] ,[response])	values	(%d,%d,%d,'%s');"
#define SC_GET_TASK_TRID "select  [ttryid] from cpytsk where [tid] = %d;"
#define SC_UPDATE_TASK_TRID "update  cpytsk set [ttryid] = %d where [tid] = %d and [ttryid] < %d;"
#define SC_DEL_TASK "delete  from cpytsk where [tid] = %d;"
#define SC_DEL_RESP "delete  from tskrsp where [tid] = %d and [tryid] = %d;"
#define SC_DEL_RESP_ALL "delete  from tskrsp where [tid] = %d;"
#define SC_GET_TASK_LIST "select [tid],[tname] from cpytsk   where [tid] > %d;"
#define SC_GET_TASK_DETAIL "select [tid],[tname],[tbegintime],[texpiredtime],[tinterval],[ttryid], [tdata]  from cpytsk   where [tid] = %d;"
#define SC_GET_RESP_LIST "select [tryid],[result] from tskrsp   where [tid] = %d and [tryid] > %d;"
#define SC_GET_RESP_DETAIL "select [tryid], [tid], [result],  [response]   from tskrsp   where [tid] = %d and [tryid] = %d;"

HRETCODE npc_insert_task(void* hSqliteHandle, int nID,char* pszName,char* pszBeginTime,char* pszExpiredTime,int nInterval,int nSize,char* pszTsk, SqliteErr* pSer)
{
     HRETCODE	hRet		 = SQLITE_RETCODE_OK;
     char szSQL[SQL_BUFFER_SIZE] = {0};
     
     

     sprintf(szSQL
	     ,SC_INSERT_TASK
	     ,nID
	     ,pszName
	     ,pszBeginTime
	     ,pszExpiredTime
	     ,nInterval
	     ,pszTsk
	     ,0);
     
     hRet = slExecText(hSqliteHandle,szSQL,NULL, NULL,pSer);
     
     return hRet;     
}

HRETCODE npc_insert_response(void* hSqliteHandle,int nID, int nTrID,int nResult,int nSize,char* pszResp,SqliteErr* pSer)
{
     HRETCODE	hRet		 = SQLITE_RETCODE_OK;
     char szSQL[SQL_BUFFER_SIZE] = {0};
     
	 

     sprintf(szSQL
	     ,SC_INSERT_RESP
	     ,nTrID
	     ,nID	     
	     ,nResult
	     ,pszResp);
	 
     hRet = slExecText(hSqliteHandle,szSQL,NULL, NULL,pSer);   
     return hRet;     
}

HRETCODE npc_update_task_trid(void* hSqliteHandle,int nID,int nTrID,SqliteErr* pSer)
{

     HRETCODE hRet = SQLITE_RETCODE_OK;
     char szSQL[SQL_BUFFER_SIZE] = {0};

     sprintf(szSQL
	     ,SC_UPDATE_TASK_TRID
	     ,nTrID
	     ,nID	   
	     ,nTrID);     
     hRet = slExecText(hSqliteHandle,szSQL,NULL, NULL,pSer);          
     return hRet;     
}

HRETCODE npc_del_resp(void* hSqliteHandle,int nID,int nTrID,SqliteErr* pSer)
{
     HRETCODE	hRet		 = SQLITE_RETCODE_OK;	
     char szSQL[SQL_BUFFER_SIZE] = {0};
     
     if(nTrID >0)
     {
	  sprintf(szSQL
		  ,SC_DEL_RESP
		  ,nID
		  ,nTrID);   
     }
     else
     {
	  sprintf(szSQL
		  ,SC_DEL_RESP_ALL
		  ,nID);
     }
     hRet = slExecText(hSqliteHandle,szSQL,NULL, NULL,pSer);

     return hRet;     
}

HRETCODE npc_del_task(void* hSqliteHandle,int nID,SqliteErr* pSer)
{
     HRETCODE	hRet		 = SQLITE_RETCODE_OK;
     char szSQL[SQL_BUFFER_SIZE] = {0};
     

     hRet = npc_del_resp(hSqliteHandle,nID,0,pSer);   
     if(SL_SUCCEEDED(hRet))
     {
	  sprintf(szSQL
		  ,SC_DEL_TASK
		  ,nID);
	  hRet = slExecText(hSqliteHandle,szSQL,NULL, NULL,pSer);
     }
     return hRet;     
}

HRETCODE TrIDFetchCallBack(void* hUserData,SqliteCBData* lpCBData)
{
     HRETCODE		hRet = SQLITE_RETCODE_OK;
     FetchTrIDCfg*	pCfg = (FetchTrIDCfg*)hUserData;
     
	
     hRet = slGetCellInt(lpCBData,0,&pCfg->nTrID); 
     return hRet;
}

HRETCODE npc_get_task_trid(void* hSqliteHandle,int nID,int& nTrID, SqliteErr* pSer)
{
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;
     char szSQL[SQL_BUFFER_SIZE] = {0};
     FetchTrIDCfg	fetchCfg = {0};
     

     sprintf(szSQL
	     ,SC_GET_TASK_TRID
	     ,nID);     	
     hRet = slExecText(hSqliteHandle,szSQL,&fetchCfg, TrIDFetchCallBack,pSer);
     if(SL_SUCCEEDED(hRet))
     {
	  nTrID = fetchCfg.nTrID;
     }
     return hRet;     
}



HRETCODE TskFetchCallBack(void* hUserData, SqliteCBData* lpCBData)
{
     HRETCODE		hRet	       = SQLITE_RETCODE_OK;
     SqliteCBInfo*	pCBInfo	       = NULL;     
     FetchTskCfg*	pCfg	       = NULL;     
     TaskInfo		feResult       = {0};
     int		nRetBufferSize = 0;
     

     pCBInfo = (SqliteCBInfo*)hUserData;
     pCfg    = (FetchTskCfg*)(pCBInfo->hUserData);
     

     hRet = slGetCellInt(lpCBData,0,&(feResult.nID));
     if(SL_FAILED(hRet)) return hRet;	   
     hRet = slGetCellText(lpCBData,1,feResult.szName,TSK_NAME_LENGTH,NULL);
     if(SL_FAILED(hRet)) return hRet;	  	  
     hRet = slGetCellText(lpCBData,2,feResult.szBeginTime,TSK_DATETIME_LENGTH,NULL);
     if(SL_FAILED(hRet)) return hRet;
     hRet = slGetCellText(lpCBData,3,feResult.szExpiredTime,TSK_DATETIME_LENGTH,NULL);
     if(SL_FAILED(hRet)) return hRet;	  	  
     hRet = slGetCellInt(lpCBData,4,&(feResult.nInterval));
     if(SL_FAILED(hRet)) return hRet;
     hRet = slGetCellInt(lpCBData,5,&(feResult.nTrID));
     if(SL_FAILED(hRet)) return hRet;
     hRet = slGetCellText(lpCBData,6,NULL,0,&nRetBufferSize);
     if(SL_FAILED(hRet) && hRet != SQLITE_E_DATA_BUFFER_TOO_SMALL) return hRet;

     if(nRetBufferSize > 0)
     {
	  feResult.nSize = nRetBufferSize+1;
	  feResult.pszTsk = (char*)g_malloc(nRetBufferSize+1);
	  hRet		  = slGetCellText(lpCBData,6,feResult.pszTsk,feResult.nSize,&nRetBufferSize);
	  if(SL_FAILED(hRet))
	  {
	       g_free(feResult.pszTsk);
	       return hRet;
	  }
     }

     pCfg->pResult = (TaskInfo*)g_malloc(sizeof(TaskInfo));
     memcpy(pCfg->pResult,&feResult,sizeof(TaskInfo));   
      
     
     return hRet;
}


HRETCODE npc_get_task_detail(void* hSqliteHandle,int nID,TaskInfo** pTaskInfo,SqliteErr* pSer)
{
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;	
     char szSQL[SQL_BUFFER_SIZE] = {0};
     FetchTskCfg	feCfg	 = {0};
     
     
     sprintf(szSQL
	     ,SC_GET_TASK_DETAIL
	     ,nID);     	
     hRet = slExecText(hSqliteHandle,szSQL,&feCfg, TskFetchCallBack,pSer);
     if(SL_SUCCEEDED(hRet))
     {
	  *pTaskInfo = feCfg.pResult;
     }
     return hRet;     
}




HRETCODE RespFetchCallBack(void* hUserData, SqliteCBData* lpCBData)
{
     HRETCODE		hRet	       = SQLITE_RETCODE_OK;
     FetchRespCfg*	pCfg	       = NULL;
     RespInfo		feResult       = {0};
     int		nRetBufferSize = 0;
     SqliteCBInfo*	pCBInfo	       = NULL;
     
     
     
     
     

     pCBInfo = (SqliteCBInfo*)hUserData;
     pCfg    = (FetchRespCfg*)(pCBInfo->hUserData);
     
     

     hRet = slGetCellInt(lpCBData,0,&(feResult.nID));
     if(SL_FAILED(hRet)) return hRet;
     hRet = slGetCellInt(lpCBData,1,&(feResult.nTrID));
     if(SL_FAILED(hRet)) return hRet;
     hRet = slGetCellInt(lpCBData,2,&(feResult.nResult));
     if(SL_FAILED(hRet)) return hRet; 

     hRet = slGetCellText(lpCBData,3,NULL,0,&nRetBufferSize);
     if(SL_FAILED(hRet) && hRet != SQLITE_E_DATA_BUFFER_TOO_SMALL) return hRet;

     if(nRetBufferSize > 0)
     {
	  feResult.nSize   = nRetBufferSize+1;
	  feResult.pszData = (char*)g_malloc(nRetBufferSize+1);
	  hRet		   = slGetCellText(lpCBData,3,feResult.pszData,feResult.nSize,&nRetBufferSize);
	  
	  if(SL_FAILED(hRet))
	  {
	       g_free(feResult.pszData);
	       return hRet;
	  }
     }

     pCfg->pResult = (RespInfo*)g_malloc(sizeof(RespInfo));
     memcpy(pCfg->pResult,&feResult,sizeof(RespInfo));   
	  
     
     
     return hRet;
}

HRETCODE npc_get_resp_detail(void* hSqliteHandle,int nID,int nTrID,RespInfo** pRespInfo,SqliteErr* pSer)
{
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;	
     char szSQL[SQL_BUFFER_SIZE] = {0};
     FetchRespCfg	feCfg	 = {0};
     

     sprintf(szSQL
	     ,SC_GET_RESP_DETAIL
	     ,nID
	     ,nTrID);     
     hRet = slExecText(hSqliteHandle,szSQL,&feCfg, RespFetchCallBack,pSer);
     if(SL_SUCCEEDED(hRet))
     {
	  *pRespInfo = feCfg.pResult;
     }
     return hRet;     
}




HRETCODE TskLstFetchCallBack(void* hUserData, SqliteCBData* lpCBData)
{
     HRETCODE		hRet	       = SQLITE_RETCODE_OK;
     SqliteCBInfo*	pCBInfo	       = NULL;
     FetchTaskListCfg*	pCfg	       = NULL;     
     FetchTaskHead	feResult       = {0};
     gpointer		dst	       = {0};
     int		nRetBufferSize = 0;
     
     
     pCBInfo = (SqliteCBInfo*)hUserData;
     pCfg    = (FetchTaskListCfg*)(pCBInfo->hUserData);
     

     hRet = slGetCellInt(lpCBData,0,&(feResult.nID)); 
     if(SL_SUCCEEDED(hRet))
     {
	  hRet = slGetCellText(lpCBData,1,feResult.szName,TSK_NAME_LENGTH,&nRetBufferSize);
	  if(SL_SUCCEEDED(hRet))
	  {
	       dst = g_malloc(sizeof(FetchTaskHead));
	       memcpy(dst,&feResult,sizeof(FetchTaskHead));
	       pCfg->pSLst = g_slist_prepend(pCfg->pSLst,dst);
	  }
     }     
     return hRet;
}

/*find bigger than nBaseID ,limit 50*/
HRETCODE npc_get_task_list(void* hSqliteHandle,int nBaseID,FetchList*  feList,SqliteErr* pSer)
{
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;	
     char szSQL[SQL_BUFFER_SIZE] = {0};
     FetchTaskListCfg	felCfg	 = {0};
     
     
     
     
     sprintf(szSQL
	     ,SC_GET_TASK_LIST
	     ,nBaseID);
     hRet = slExecText(hSqliteHandle,szSQL,&felCfg, TskLstFetchCallBack,pSer);
     if(SL_SUCCEEDED(hRet))
     {
	  feList->pSLst = felCfg.pSLst;
     }
     return hRet;     
}




HRETCODE RespLstFetchCallBack(void* hUserData, SqliteCBData* lpCBData)
{
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;
     SqliteCBInfo*	pCBInfo	 = NULL;
     FetchRespListCfg*	pCfg	 = NULL;
     FetchRespHead	feResult = {0};
     gpointer		dst	 = {0};
     


     pCBInfo = (SqliteCBInfo*)hUserData;
     pCfg    = (FetchRespListCfg*)(pCBInfo->hUserData);

     hRet = slGetCellInt(lpCBData,0,&(feResult.nTrID)); 
     if(SL_SUCCEEDED(hRet))
     {
	  hRet = slGetCellInt(lpCBData,1,&(feResult.nResult));
	  if(SL_SUCCEEDED(hRet))
	  {
	       dst = g_malloc(sizeof(FetchRespHead));
	       memcpy(dst,&feResult,sizeof(FetchRespHead));
	       pCfg->pSLst = g_slist_prepend(pCfg->pSLst,dst);
	  }
     }     
     return hRet;
}

/*find bigger than nBaseTrID ,limit 50 */
HRETCODE npc_get_resp_list(void* hSqliteHandle,int nID,int nBaseTrID,FetchList*  feList,SqliteErr* pSer)
{
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;	
     char szSQL[SQL_BUFFER_SIZE] = {0};
     FetchRespListCfg	felCfg	 = {0};
 
     

     sprintf(szSQL
	     ,SC_GET_RESP_LIST
	     ,nID
	     ,nBaseTrID);
     hRet = slExecText(hSqliteHandle,szSQL,&felCfg, RespLstFetchCallBack,pSer);
     if(SL_SUCCEEDED(hRet))
     {
	  feList->pSLst = felCfg.pSLst;
     }     
     
     return hRet;     
}






