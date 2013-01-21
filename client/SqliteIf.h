#pragma once


// note: sqlite lib must use UTF-8 codeing,so NO unicode param in or out

typedef unsigned int										HRETCODE;

#define SQLITE_TRANS_LENGTH		32
#define SQLITE_BUFFER_LENGTH		2048
#define SQLITE_PARAM_NAME_LENGTH	32
#define SQLITE_ERR_MSG_LENGTH		240


// define return code value 
#define SQLITE_RETCODE_OK			0
#define SQLITE_RETCODE_E_BASE			0x8A030000
#define SQLITE_E_NO_BAD_PARAMETER		SQLITE_RETCODE_E_BASE + 1		
#define SQLITE_E_NO_IMPLEMENT			SQLITE_RETCODE_E_BASE + 2		
#define SQLITE_E_CAN_NOT_REMOVE_DATAFILE	SQLITE_RETCODE_E_BASE + 3		
#define SQLITE_E_BIND_PARAM			SQLITE_RETCODE_E_BASE + 4
#define SQLITE_E_BIND_DATA_INDEX		SQLITE_RETCODE_E_BASE + 5		
#define SQLITE_E_BIND_DATA_BUFFER		SQLITE_RETCODE_E_BASE + 6		
#define SQLITE_E_BIND_DATA_OBJ			SQLITE_RETCODE_E_BASE + 7		
#define SQLITE_E_DATA_BUFFER_TOO_SMALL		SQLITE_RETCODE_E_BASE + 8





#define SL_SUCCEEDED(hRetCode)	(hRetCode == SQLITE_RETCODE_OK ? 1 : 0)
#define SL_FAILED(hRetCode)	(hRetCode != SQLITE_RETCODE_OK ? 1 : 0)



#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3

#define MAX_PATH 256


typedef struct tagSqliteParamBinding
{
	/************************************************************************/
	/* for input param setting			                                    */
	/************************************************************************/
	char		szParamName[SQLITE_PARAM_NAME_LENGTH];
	unsigned int	uParamType;
	unsigned int	uParamLen;
	unsigned int	uDirectInOrOut;
	unsigned int	uUserDefined;
	/*************************************************************************/
	/* for input param binding data                                         */
	/************************************************************************/
	char*		pData;
	unsigned int	uDataLen;
	unsigned int	uNULLFlag;     
}SqliteParamBinding,*LPSqliteParamBinding;

typedef struct
{
	char	szFileName[MAX_PATH];
	int	bAlwaysCreate;     
}SqliteConfig,*LPSqliteConfig;

typedef struct
{
	char		szFileName[MAX_PATH];
	unsigned int	dwTimeout;     
}SqliteSetting,*LPSqliteSetting;

// error struct
typedef struct
{
	int	nErrID;
	char	szErrText[SQLITE_ERR_MSG_LENGTH];     
}SqliteErr,*LPSqliteErr;

#define SQLITE_DATA_COLUNM		0
#define SQLITE_DATA_PLAINT		1


typedef struct
{
	int						nSetData;			//may be 0 or 1
	int						cColumns;
	union
	{
		struct
		{
			char**			lplpColumnData;		// nSetData = SQLITE_DATA_COLUNM
			char**			lplpColumnName;		// nSetData = SQLITE_DATA_COLUNM
		}ColunmData;
		struct
		{
			void*	hRowData;	// nSetData = SQLITE_DATA_PLAINT
		}PlaintData;		
	};
     
}SqliteCBData,*LPSqliteCBData;



// callback declare  
typedef HRETCODE			(*ppfnSqliteFetchCallBack)(void* hUserData,SqliteCBData* lpCBData);

// flow chart
/* main start
	CreateInstance
		InitEnv
			CreateTable if need
	
	client thread
		CreateInstance 
			Connect
					ExecText
				OR
					PrepareSQL 
					ExecText2
					ReleasePrepareObj
			Close
		ReleaseInstance
	main close 
		ReleaseEnv
	ReleaseInstance
*/


typedef HRETCODE			(*ppfnCreateInstance)(void** lpSqliteHandle);							

typedef void				(*ppfnReleaseInstance)(void* hSqliteHandle);

typedef HRETCODE			(*ppfnInitEnv)(void* hSqliteHandle,SqliteConfig* lpSqliteConfig,SqliteErr* lpErr);			

typedef HRETCODE			(*ppfnReleaseEnv)(void* hSqliteHandle,SqliteErr* lpErr);									

typedef HRETCODE			(*ppfnCreateTable)(void* hSqliteHandle,char* lpszSQLText,SqliteErr* lpErr);

typedef HRETCODE			(*ppfnCreateTable2)(void* hSqliteHandle,char* lpszTableName);

typedef HRETCODE			(*ppfnConnect)(void* hSqliteHandle,SqliteSetting* lpSqliteSetting,SqliteErr* lpErr);	

typedef void				(*ppfnClose)(void* hSqliteHandle);														

typedef HRETCODE			(*ppfnPrepareSQL)(void* hSqliteHandle,char* lpszSQLText,void** lpSqlitePrepareObj,SqliteErr* lpErr);

typedef HRETCODE			(*ppfnReleasePrepareObj)(void* hSqliteHandle,void* hSqlitePrepareObj,SqliteErr* lpErr);

typedef HRETCODE			(*ppfnExecText)(void* hSqliteHandle,char* lpszSQLText,void* hUserData, ppfnSqliteFetchCallBack pfnSqliteFetchCallBack ,SqliteErr* lpErr);

typedef HRETCODE			(*ppfnExecText2)(void* hSqliteHandle,void* hSqlitePrepareObj, unsigned int cParamBinding,SqliteParamBinding* lpParamBinding	,void* hUserData, ppfnSqliteFetchCallBack pfnSqliteFetchCallBack,unsigned int* lpuRowCount,SqliteErr* lpErr); 

typedef HRETCODE			(*ppfnGetCellInt)(SqliteCBData* lpCBData,int nColIndex,int* lpRetBuffer);

typedef HRETCODE			(*ppfnGetCellInt64)(SqliteCBData* lpCBData,int nColIndex, int64_t* lpRetBuffer);

typedef HRETCODE			(*ppfnGetCellText)(SqliteCBData* lpCBData,int nColIndex,char* lpRetBuffer,int nBufferSize,int* lpRetBufferSize);

typedef HRETCODE			(*ppfnGetCellDouble)(SqliteCBData* lpCBData,int nColIndex,double* lpRetBuffer);

typedef HRETCODE			(*ppfnGetCellBlob)(SqliteCBData* lpCBData,int nColIndex,char* lpRetBuffer,int nBufferSize,int* lpRetBufferSize);

typedef void				(*ppfnFormatSLParamData)(SqliteParamBinding* lpSLParamBindingAr,int nIndex, unsigned int uParamType,char* lpData, unsigned int uDataLen,unsigned int uNULLFlag);



typedef struct
{
     ppfnCreateInstance		CreateInstance;
     ppfnReleaseInstance	ReleaseInstance;     
     ppfnInitEnv		InitEnv;
     ppfnReleaseEnv		ReleaseEnv;
     ppfnConnect		Connect;
     ppfnClose			Close;
     ppfnCreateTable		CreateTable;
     ppfnCreateTable2		CreateTable2;
     ppfnPrepareSQL		PrepareSQL;
     ppfnReleasePrepareObj	ReleasePrepareObj;
     ppfnExecText		ExecText;
     ppfnExecText2		ExecText2;
     ppfnGetCellInt		GetCellInt;
     ppfnGetCellInt64		GetCellInt64;	
     ppfnGetCellText		GetCellText;
     ppfnGetCellDouble		GetCellDouble;
     ppfnGetCellBlob		GetCellBlob;
     ppfnFormatSLParamData	FormatSLParamData;     
}SqliteIf,*LPSqliteIf;


