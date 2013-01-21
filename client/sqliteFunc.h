#pragma once



typedef struct  
{
	int nTrID;
}FetchTrIDCfg;


#define TSK_NAME_LENGTH		48
#define TSK_DATETIME_LENGTH	24
#define TSK_DATA_LENGTH		1024


typedef struct
{
     int	nID;
     char	szName[TSK_NAME_LENGTH];
     char	szBeginTime[TSK_DATETIME_LENGTH];
     char	szExpiredTime[TSK_DATETIME_LENGTH];
     int	nInterval;
     int	nSize;
     int	nTrID;     
     char*	pszTsk;     
}TaskInfo;


typedef struct
{

     TaskInfo* pResult;
}FetchTskCfg;

typedef struct
{
     int	nID;
     int	nTrID;
     int	nResult;
     int	nSize;
     char*	pszData;     
}RespInfo;

typedef struct
{
     RespInfo*	pResult;
}FetchRespCfg;

typedef struct
{
     int	nID;
     char	szName[TSK_NAME_LENGTH]; 
}FetchTaskHead;

typedef struct
{
     GSList* pSLst; /*cach FetchTaskHead*/
}FetchTaskListCfg;


typedef struct
{
     GSList*	pSLst;		/*it is null before request*/
}FetchList;


typedef struct
{
     int	nTrID;
     int	nResult;     
}FetchRespHead;

typedef struct
{
     GSList* pSLst; /*cach FetchRespHead*/
}FetchRespListCfg;


extern HRETCODE	npc_insert_task(void* hSqliteHandle, int nID,char* pszName,char* pszBeginTime,char* pszExpiredTime,int nInterval,int nSize,char* pszTsk, SqliteErr* pSer);
extern HRETCODE	npc_insert_response(void* hSqliteHandle,int nID, int nTrID,int nResult,int nSize,char* pszResp,SqliteErr* pSer);
extern HRETCODE	npc_update_task_trid(void* hSqliteHandle,int nID,int nTrID,SqliteErr* pSer);
extern HRETCODE	npc_del_resp(void* hSqliteHandle,int nID,int nTrID,SqliteErr* pSer);
extern HRETCODE	npc_del_task(void* hSqliteHandle,int nID,SqliteErr* pSer);
extern HRETCODE	npc_get_task_trid(void* hSqliteHandle,int nID,int& nTrID, SqliteErr* pSer);
extern HRETCODE	npc_get_task_detail(void* hSqliteHandle,int nID,TaskInfo** pTaskInfo,SqliteErr* pSer);
extern HRETCODE	npc_get_resp_detail(void* hSqliteHandle,int nID,int nTrID,RespInfo** pRespInfo,SqliteErr* pSer);
extern HRETCODE	npc_get_task_list(void* hSqliteHandle,int nBaseID,FetchList*  feList,SqliteErr* pSer);
extern HRETCODE	npc_get_resp_list(void* hSqliteHandle,int nID,int nBaseTrID,FetchList*  feList,SqliteErr* pSer);

