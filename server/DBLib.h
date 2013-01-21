
#define DB_BINDING_PARAM_COUNT		256
#define DB_BINDING_RECORD_COUNT		256
#define DB_FUNC_EXEC_SUCCEED(nExecCode)	(nExecCode == 0 ? 1 : 0)
#define DB_FUNC_EXEC_FAILED(nExecCode)	(nExecCode != 0 ? 1 : 0)


#define RETCODE_SUCCEEDED		0
#define DB_BUFFER_SIZE(colWidth)	4*(colWidth+1)
#define DB_OPTION_NAME_MAX_LENGTH	64


/* for option map */
typedef struct tagDBOptionItem
{
     char	        szOptionItemName[DB_OPTION_NAME_MAX_LENGTH];
     mysql_option	curOptionValue;     
}DBOptionItem,*LPDBOptionItem;


/* for inner using */
typedef struct tagDBInfo
{
     MYSQL*		pMysql; 
     DBConnectInfo	dbConnectInfo;
     MYSQL_BIND    	bindParam[DB_BINDING_PARAM_COUNT];	
     MYSQL_BIND    	bindRecord[DB_BINDING_RECORD_COUNT];	
     ppfnAllocMemory	pfnAllocMemory;
     ppfnReleaseMemory	pfnReleaseMemory;
     char		szEncodingName[DB_DATABASE_ENCODING_LENGTH];
     unsigned int	uEncoding;
     int		bAutoSubmit;
     unsigned int	uKeepConnection;     
}DBInfo,*LPDBInfo;




/* forward declare */
unsigned int	DB_CreateInstance(void** lpDBHandle,LPDBSetting lpDBSetting);
void		DB_ReleaseInstance(void* hDBHandle);
unsigned int	DB_SetOption(void* hDBHandle,char* lpszOptionname,char* lpszOptionText,LPDBErrorInfo lpDBErrorInfo);
unsigned int	DB_Connect(void* hDBHandle, LPDBConnectInfo lpDBConnectInfo,LPDBErrorInfo lpDBErrorInfo);
void		DB_Close(void* hDBHandle);
unsigned int	DB_StartTrans(void* hDBHandle,char* lpszTransName,LPDBErrorInfo lpDBErrorInfo);
unsigned int	DB_SubmitTrans(void* hDBHandle,char* lpszTransName,int bCommit,LPDBErrorInfo lpDBErrorInfo);
int		DB_StringEncoding(void* hDBHandle,char* lpszSrc,char* lpszDst,int nDstTextLen);
unsigned int	DB_ExecStoredProc(void* hDBHandle
					  ,char* lpszStoredProcText		
					  ,unsigned int uDBParamBinding
					  ,LPDBParamBinding lpDBParam
					  ,unsigned int uDBRecordBinding
					  ,LPDBRecordBinding lpDBRecord
					  ,unsigned int uRecordSize
					  ,unsigned int uFieldCount
					  ,unsigned int* lpRecordCount
					  ,char** lpRetBuffer
					  ,unsigned int* lpAffectedRowCount
					  ,LPDBErrorInfo	lpDBErrorInfo);
void	DB_ReleaseRecordBuffer(void* hDBHandle,char* lpDataBuffer);
unsigned int	DB_ExecStoredProc2(void* hDBHandle
					   ,char* lpszStoredProcText		
					   ,unsigned int uDBParamBinding
					   ,LPDBParamBinding lpDBParam
					   ,unsigned int uDBRecordBinding
					   ,LPDBRecordBinding lpDBRecord
					   ,unsigned int* lpRecordCount
					   ,unsigned int* lpRecordTotal
					   ,void* hUserData
					   ,void* hUserBuffer
					   ,ppfnDBFetchCallBack pfnDBFetchCallBack
					   ,unsigned int* lpAffectedRowCount
					   ,LPDBErrorInfo lpDBErrorInfo);

int		DB_InitThread(void* hDBHandle);
int		DB_ReleaseThread(void* hDBHandle);

/* for inner using */
void 		ClearDBBinding(LPDBInfo lpDBInfo);
void 		SetDBError(LPDBInfo lpDBInfo,LPDBErrorInfo lpDBErrorInfo);
void 		SetDBStmtError(MYSQL_STMT* lpSTMT,LPDBErrorInfo lpDBErrorInfo,int bAutoReleaseSTMT);
void 		ReleaseSTMTRes(MYSQL* lpMysql,MYSQL_RES* lpRES,MYSQL_STMT* lpSTMT);
