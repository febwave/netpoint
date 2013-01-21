#pragma once


#define SQLITE_SUCCEEDED(slRet)		(((int)(slRet)) == 0)
#define SQLITE_FAILED(slRet)		(((int)(slRet)) != 0)




typedef unsigned int						DATAREF;					


typedef struct
{
	char		szDataFileName[MAX_PATH];
	sqlite3*	pSqlite; 
	DATAREF		nRef;	// for pSqlite     
}SqliteInfo,*LPSqliteInfo;


typedef struct 
{
	void*			hUserData;
	ppfnSqliteFetchCallBack	pfnSqliteFetchCallBack;     
}SqliteCBInfo,*LPSqliteCBInfo;



//declare
extern HRETCODE	slCreateInstance(void** lpSqliteHandle);
extern void	slReleaseInstance(void* hSqliteHandle);
extern HRETCODE	slInitEnv(void* hSqliteHandle,SqliteConfig* lpSqliteConfig,SqliteErr* lpErr);
extern HRETCODE	slReleaseEnv(void* hSqliteHandle,SqliteErr* lpErr);
extern HRETCODE	slConnect(void* hSqliteHandle,SqliteSetting* lpSqliteSetting,SqliteErr* lpErr);	
extern void	slClose(void* hSqliteHandle);									
extern HRETCODE	slCreateTable(void* hSqliteHandle,char* lpszSQLText,SqliteErr* lpErr);
extern HRETCODE	slCreateTable2(void* hSqliteHandle,char* lpszTableName);
extern HRETCODE	slPrepareSQL(void* hSqliteHandle,char* lpszSQLText,void** lpSqlitePrepareObj,SqliteErr* lpErr);
extern HRETCODE	slReleasePrepareObj(void* hSqliteHandle,void* hSqlitePrepareObj,SqliteErr* lpErr);
extern HRETCODE	slExecText(void* hSqliteHandle,char* lpszSQLText,void* hUserData, ppfnSqliteFetchCallBack pfnSqliteFetchCallBack ,SqliteErr* lpErr);
extern HRETCODE	slExecText2(void* hSqliteHandle,void* hSqlitePrepareObj, unsigned int cParamBinding,SqliteParamBinding* lpParamBinding	,void* hUserData, ppfnSqliteFetchCallBack pfnSqliteFetchCallBack	,unsigned int* lpuRowCount,SqliteErr* lpErr); 
extern HRETCODE slGetCellInt(SqliteCBData* lpCBData,int nColIndex,int* lpRetBuffer);
extern HRETCODE slGetCellInt64(SqliteCBData* lpCBData,int nColIndex,int64_t* lpRetBuffer);
extern HRETCODE slGetCellText(SqliteCBData* lpCBData,int nColIndex,char* lpRetBuffer,int nBufferSize,int* lpRetBufferSize);
extern HRETCODE slGetCellDouble(SqliteCBData* lpCBData,int nColIndex,double* lpRetBuffer);
extern HRETCODE slGetCellBlob(SqliteCBData* lpCBData,int nColIndex,char* lpRetBuffer,int nBufferSize,int* lpRetBufferSize);
extern void	slFillSimpleSLParamData(SqliteParamBinding* lpSLParamBindingAr,int nIndex, unsigned int uParamType,char* lpData, unsigned int uDataLen,unsigned int uNULLFlag);


// helper function
extern void	ResetSqliteErrInfo(SqliteErr* lpErr);
extern void	SetSqliteErrInfo(int slRet,char* lpszErrMsg,  SqliteErr* lpErr);
extern void	ReleaseErrMsg(char* lpszErrMsg);
extern void	SetSqliteDBErrInfo(sqlite3 *lpSqlite,SqliteErr* lpErr);
extern DATAREF	ObjAddRef(SqliteInfo* lpSqliteInfo);
extern DATAREF	ObjReleaseRef(SqliteInfo* lpSqliteInfo);
extern HRETCODE	UpdateSqliteParam(SqliteInfo* lpSqliteInfo,sqlite3*	lpSqlite,SqliteSetting* lpSqliteSetting);
extern HRETCODE RemoveDataFile(char* lpszDataFile);



