

/* define member value */

#define DB_DATABASE_ENCODING_LENGTH	8
#define DB_DATABASE_LIB_NAME_LENGTH	32
#define DB_STOREPROCEDURE_NAME_LENGTH	32
#define DB_OPTION_TEXT_LENGTH		32
#define DB_ERROR_MSG_LENGTH		512
#define DB_DATA_ID_LENGTH		8


/* define option  */
#define DB_OPT_CONNECT_TIMEOUT		"DB_OPT_CONNECT_TIMEOUT"			
#define DB_OPT_COMPRESS			"DB_OPT_COMPRESS"					
#define DB_OPT_NAMED_PIPE		"DB_OPT_NAMED_PIPE"				
#define DB_INIT_COMMAND			"DB_INIT_COMMAND"					
#define DB_READ_DEFAULT_FILE		"DB_READ_DEFAULT_FILE"				
#define DB_READ_DEFAULT_GROUP		"DB_READ_DEFAULT_GROUP"			
#define DB_SET_CHARSET_DIR		"DB_SET_CHARSET_DIR"				
#define DB_SET_CHARSET_NAME		"DB_SET_CHARSET_NAME"				
#define DB_OPT_LOCAL_INFILE		"DB_OPT_LOCAL_INFILE"				
#define DB_OPT_PROTOCOL			"DB_OPT_PROTOCOL"					
#define DB_SHARED_MEMORY_BASE_NAME	"DB_SHARED_MEMORY_BASE_NAME"		
#define DB_OPT_READ_TIMEOUT		"DB_OPT_READ_TIMEOUT"				
#define DB_OPT_WRITE_TIMEOUT		"DB_OPT_WRITE_TIMEOUT"				
#define DB_OPT_USE_RESULT		"DB_OPT_USE_RESULT"				
#define DB_OPT_USE_REMOTE_CONNECTION	"DB_OPT_USE_REMOTE_CONNECTION"		
#define DB_OPT_USE_EMBEDDED_CONNECTION	"DB_OPT_USE_EMBEDDED_CONNECTION"	
#define DB_OPT_GUESS_CONNECTION		"DB_OPT_GUESS_CONNECTION"			
#define DB_SET_CLIENT_IP		"DB_SET_CLIENT_IP"					
#define DB_SECURE_AUTH			"DB_SECURE_AUTH"					
#define DB_REPORT_DATA_TRUNCATION	"DB_REPORT_DATA_TRUNCATION"		
#define DB_OPT_RECONNECT		"DB_OPT_RECONNECT"
#define DB_OPT_SSL_VERIFY_SERVER_CERT	"DB_OPT_SSL_VERIFY_SERVER_CERT"




/* define return code value   */
#define DB_RETCODE_BASE				0x8A010000
#define DB_ERROR_PARAMETER_PASS			DB_RETCODE_BASE + 1					
#define DB_ERROR_BIND_PARAMETER_COUNT		DB_RETCODE_BASE + 2					
#define DB_ERROR_BIND_PARAMETER_TOO_MORE	DB_RETCODE_BASE + 3					
#define DB_ERROR_EXEC_FUNC			DB_RETCODE_BASE + 4					
#define DB_ERROR_RET_COL_MISMATCH		DB_RETCODE_BASE + 5					
#define DB_ERROR_RET_DATA_MORE			DB_RETCODE_BASE + 6					
#define DB_ERROR_NO_MEMORY			DB_RETCODE_BASE + 7					
#define DB_ERROR_INIT_INSTANCE			DB_RETCODE_BASE + 8
#define DB_ERROR_EXPECT_RECORD			DB_RETCODE_BASE + 9




typedef struct tagDBErrorInfo
{
	unsigned int	uErrorNo;
	char		szErrMsg[DB_ERROR_MSG_LENGTH];     
}DBErrorInfo,*LPDBErrorInfo;

typedef void* (*ppfnAllocMemory)(unsigned int size);
typedef void (*ppfnReleaseMemory)(void* lpBuffer);



typedef struct tagDBSetting
{
	ppfnAllocMemory		pfnAllocMemory;
	ppfnReleaseMemory	pfnReleaseMemory;
	char			szDBLibName[DB_DATABASE_LIB_NAME_LENGTH];
	char			szEncodingName[DB_DATABASE_ENCODING_LENGTH];
	unsigned int		uEncoding;
	unsigned int		bAutoSubmit;	
	unsigned int		uKeepConnection;     
}DBSetting,*LPDBSetting;
 
#define DB_UNICODE_FLAG				1

typedef struct tagDBParamBinding
{
	/************************************************************************/
	/* for input param setting			                                    */
	/************************************************************************/
	char		szParamName[DB_STOREPROCEDURE_NAME_LENGTH];
	unsigned int	uParamType;
	unsigned int	uParamLen;
	unsigned int	uDirectInOrOut;
	unsigned int	uUserDefined;
	/************************************************************************/
	/* for input param binding data                                         */
	/************************************************************************/
	char*		pData;
	unsigned int	uDataLen;
	unsigned int	uNULLFlag;     
}DBParamBinding,*LPDBParamBinding;



typedef struct tagDBRecordBinding
{
	/************************************************************************/
	/* for inner using,outer don't fill in these data                       */
	/************************************************************************/
	unsigned int	uRetColOffSet;
	unsigned int	uRetColSize;
     

     
	unsigned int	uDataOffset;	
	unsigned int	uDataLen;	
	unsigned int	uDataType;			
	unsigned int	uUnicodeFlag;
	char		szDataID[DB_DATA_ID_LENGTH];     
}DBRecordBinding,*LPDBRecordBinding;


#define DB_ADDRESS_LENGTH	32
#define DB_USER_NAME_LENGTH	32
#define DB_USER_PWD_LENGTH	32
#define DB_DATABASE_NAME_LENGTH	32
#define DB_DATABASE_TYPE_LENGTH	32



typedef struct tagDBConnectInfo
{
     char	szDBAddress[DB_ADDRESS_LENGTH];
     char	szUserName[DB_USER_NAME_LENGTH];
     char	szUserPwd[DB_USER_PWD_LENGTH];
     char	szDBName[DB_DATABASE_NAME_LENGTH];     
}DBConnectInfo,*LPDBConnectInfo;

typedef unsigned int (*ppfnDBFetchCallBack)(void* hUserData,void* hUserBuffer ,unsigned int uDBRecordBinding, void* lpDBRecordBinding ,unsigned int uDBEncoding,char* lpDataBuffer);
	      

typedef unsigned int (*ppfnCreateInstance)(void** lpDBHandle,LPDBSetting lpDBSetting);
typedef void	(*ppfnReleaseInstance)(void* hDBHandle);
typedef unsigned int (*ppfnSetOption)(void* hDBHandle,char* lpszOptionname,char* lpszOptionText,LPDBErrorInfo lpDBErrorInfo);
typedef unsigned int (*ppfnConnect)(void* hDBHandle, LPDBConnectInfo lpDBConnectInfo,LPDBErrorInfo lpDBErrorInfo);
typedef void	     (*ppfnClose)(void* hDBHandle);
typedef unsigned int (*ppfnStartTrans)(void* hDBHandle,char* lpszTransName,LPDBErrorInfo lpDBErrorInfo);
typedef unsigned int (*ppfnSubmitTrans)(void* hDBHandle,char* lpszTransName,int bCommit,LPDBErrorInfo lpDBErrorInfo);		
typedef int	     (*ppfnStringEncoding)(void* hDBHandle,char* lpszSrc,char* lpszDst,int nDstTextLen);
typedef unsigned int (*ppfnExecStoredProc)(void* hDBHandle
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
					   ,LPDBErrorInfo lpDBErrorInfo);
typedef void		(*ppfnReleaseRecordBuffer)(void* hDBHandle,char* lpDataBuffer);
typedef unsigned int	(*ppfnExecStoredProc2)(void* hDBHandle
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

typedef int			(*ppfnInitThread)(void* hDBHandle);
typedef int			(*ppfnReleaseThread)(void* hDBHandle);

typedef struct tagDBIf 
{	
	ppfnCreateInstance	pfnCreateInstance;			
	ppfnReleaseInstance	pfnReleaseInstance;
	ppfnSetOption		pfnSetOption;
	ppfnConnect		pfnConnect;
	ppfnClose		pfnClose;
	ppfnStartTrans		pfnStartTrans;
	ppfnSubmitTrans		pfnSubmitTrans;
	ppfnStringEncoding	pfnStringEncoding;
	ppfnExecStoredProc	pfnExecStoredProc;
	ppfnExecStoredProc2	pfnExecStoredProc2;
	ppfnReleaseRecordBuffer	pfnReleaseRecordBuffer;
	ppfnInitThread		pfnInitThread;	
	ppfnReleaseThread	pfnReleaseThread;
     
}DBIf,*LPDBIf;


