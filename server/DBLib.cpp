#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <mysql.h>
#include "DBIf.h"
#include "DBLib.h"


#define DB_BUFFER_MAX_LENGTH			1024*3*36	// 256*3*36   max char * utf8 * max col selected


static DBOptionItem s_DBOptionItemCollect[] = {	
     {DB_OPT_CONNECT_TIMEOUT		,MYSQL_OPT_CONNECT_TIMEOUT		}
     ,{DB_OPT_COMPRESS			,MYSQL_OPT_COMPRESS			}
     ,{DB_OPT_NAMED_PIPE		,MYSQL_OPT_NAMED_PIPE			}
     ,{DB_INIT_COMMAND			,MYSQL_INIT_COMMAND			}
     ,{DB_READ_DEFAULT_FILE		,MYSQL_READ_DEFAULT_FILE		}
     ,{DB_READ_DEFAULT_GROUP		,MYSQL_READ_DEFAULT_GROUP		}
     ,{DB_SET_CHARSET_DIR		,MYSQL_SET_CHARSET_DIR			}
     ,{DB_SET_CHARSET_NAME		,MYSQL_SET_CHARSET_NAME			}
     ,{DB_OPT_LOCAL_INFILE		,MYSQL_OPT_LOCAL_INFILE			}
     ,{DB_OPT_PROTOCOL			,MYSQL_OPT_PROTOCOL			}
     ,{DB_SHARED_MEMORY_BASE_NAME	,MYSQL_SHARED_MEMORY_BASE_NAME		}
     ,{DB_OPT_READ_TIMEOUT		,MYSQL_OPT_READ_TIMEOUT			}
     ,{DB_OPT_WRITE_TIMEOUT		,MYSQL_OPT_WRITE_TIMEOUT		}
     ,{DB_OPT_USE_RESULT		,MYSQL_OPT_USE_RESULT			}
     ,{DB_OPT_USE_REMOTE_CONNECTION	,MYSQL_OPT_USE_REMOTE_CONNECTION	}
     ,{DB_OPT_USE_EMBEDDED_CONNECTION	,MYSQL_OPT_USE_EMBEDDED_CONNECTION	}
     ,{DB_OPT_GUESS_CONNECTION		,MYSQL_OPT_GUESS_CONNECTION		}
     ,{DB_SET_CLIENT_IP			,MYSQL_SET_CLIENT_IP			}
     ,{DB_SECURE_AUTH			,MYSQL_SECURE_AUTH			}
     ,{DB_REPORT_DATA_TRUNCATION	,MYSQL_REPORT_DATA_TRUNCATION		}
     ,{DB_OPT_RECONNECT			,MYSQL_OPT_RECONNECT			}
     ,{DB_OPT_SSL_VERIFY_SERVER_CERT	,MYSQL_OPT_SSL_VERIFY_SERVER_CERT	}
     
     
};

#define DB_OPTION_ITEM_COUNT			sizeof(s_DBOptionItemCollect) / sizeof(DBOptionItem) 

extern "C" int AppQueryInterface(void** lpAppIf )
{
	
	DBIf*	pDBIf		      = (DBIf*)malloc(sizeof(DBIf));
	if(pDBIf == NULL) return 1;
	pDBIf->pfnCreateInstance      = DB_CreateInstance;		
	pDBIf->pfnReleaseInstance     = DB_ReleaseInstance;
	pDBIf->pfnSetOption	      = DB_SetOption;
	pDBIf->pfnConnect	      = DB_Connect;
	pDBIf->pfnClose		      = DB_Close;
	pDBIf->pfnStartTrans	      = DB_StartTrans;
	pDBIf->pfnSubmitTrans	      = DB_SubmitTrans;
	pDBIf->pfnExecStoredProc      = DB_ExecStoredProc;
	pDBIf->pfnExecStoredProc2     = DB_ExecStoredProc2;
	pDBIf->pfnReleaseRecordBuffer = DB_ReleaseRecordBuffer;
	pDBIf->pfnInitThread	      = DB_InitThread;
	pDBIf->pfnReleaseThread	      = DB_ReleaseThread;
	
	
	mysql_library_init(0, NULL, NULL);	
	*lpAppIf = pDBIf;
	
	return 0;
}

extern "C" void AppReleaseInterface(void* lpAppIf)
{
	if(lpAppIf != NULL) free((DBIf*)lpAppIf);
	mysql_library_end();
}




unsigned int DB_CreateInstance(void** lpDBHandle,LPDBSetting lpDBSetting)
{
	unsigned int	uRetCode = RETCODE_SUCCEEDED;
	LPDBInfo	pDBInfo	 = NULL;
	
	
	if(lpDBHandle == NULL 
		|| lpDBSetting == NULL 
		|| lpDBSetting->pfnAllocMemory == NULL
		|| lpDBSetting->pfnReleaseMemory == NULL)
	{
		return DB_ERROR_PARAMETER_PASS;
	}
	
	pDBInfo	= (DBInfo*)malloc(sizeof(DBInfo));
	
	
	if(pDBInfo == NULL)
	{
	     return DB_ERROR_NO_MEMORY;
	}
	bzero(pDBInfo,sizeof(DBInfo));	
	pDBInfo->pfnAllocMemory	  = lpDBSetting->pfnAllocMemory;
	pDBInfo->pfnReleaseMemory = lpDBSetting->pfnReleaseMemory;
	pDBInfo->uEncoding	  = lpDBSetting->uEncoding;
	pDBInfo->uKeepConnection  = lpDBSetting->uKeepConnection;
	pDBInfo->bAutoSubmit	  = lpDBSetting->bAutoSubmit;
	strcpy(pDBInfo->szEncodingName,lpDBSetting->szEncodingName);	
	*lpDBHandle		  = pDBInfo;	
	return uRetCode;
}

void DB_ReleaseInstance(void* hDBHandle)
{
	if(hDBHandle != NULL) 
	{
		DB_Close(hDBHandle);
		free((LPDBInfo)hDBHandle);
		
	}
}

int DB_InitThread(void* hDBHandle)
{
	mysql_thread_init(); 
	return 1;
}

int DB_ReleaseThread(void* hDBHandle)
{
	mysql_thread_end();
	return 1;
}

unsigned int DB_SetOption(void* hDBHandle,char* lpszOptionname,char* lpszOptionText,LPDBErrorInfo lpDBErrorInfo)
{
	unsigned int	uRetCode       = RETCODE_SUCCEEDED;
	LPDBInfo	pDBInfo	       = NULL;
	mysql_option	curOptionValue = (mysql_option)0;
	int		bFound	       = 0;
	pDBInfo			       = (LPDBInfo)hDBHandle;
	
	if(pDBInfo == NULL || pDBInfo->pMysql == NULL)
	{
	       return DB_ERROR_PARAMETER_PASS;		
	}
	
	for (unsigned int nItemIndex = 0;nItemIndex < DB_OPTION_ITEM_COUNT;nItemIndex++)
	{
		if(strcmp(s_DBOptionItemCollect[nItemIndex].szOptionItemName,lpszOptionname) == 0)
		{
			bFound			= 1;
			curOptionValue	= s_DBOptionItemCollect[nItemIndex].curOptionValue;
			break;
		}		
	}
	if(bFound == 0)
	{
		return DB_ERROR_PARAMETER_PASS;
	}
	uRetCode = mysql_options(pDBInfo->pMysql, curOptionValue, lpszOptionText);
	
	if(DB_FUNC_EXEC_FAILED(uRetCode))
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		return DB_ERROR_EXEC_FUNC;
	}
	return uRetCode;
}

unsigned int DB_Connect(void* hDBHandle, LPDBConnectInfo lpDBConnectInfo,LPDBErrorInfo lpDBErrorInfo)
{
	unsigned int	uRetCode = RETCODE_SUCCEEDED;
	LPDBInfo	pDBInfo	 = NULL;
	char		value	 = 1;
	


	pDBInfo	= (LPDBInfo)hDBHandle;
	if(pDBInfo == NULL)
	{
		return DB_ERROR_PARAMETER_PASS;
	}
	pDBInfo->pMysql	= mysql_init(NULL);
	if(pDBInfo->pMysql == NULL)
	{
		return DB_ERROR_INIT_INSTANCE;
	}


	mysql_options(pDBInfo->pMysql, MYSQL_OPT_RECONNECT, (char *)&value);


	strcpy(pDBInfo->dbConnectInfo.szDBAddress,lpDBConnectInfo->szDBAddress);
	strcpy(pDBInfo->dbConnectInfo.szUserName,lpDBConnectInfo->szUserName);
	strcpy(pDBInfo->dbConnectInfo.szUserPwd,lpDBConnectInfo->szUserPwd);
	strcpy(pDBInfo->dbConnectInfo.szDBName,lpDBConnectInfo->szDBName);


	

	if(mysql_real_connect(pDBInfo->pMysql
						, pDBInfo->dbConnectInfo.szDBAddress
						, pDBInfo->dbConnectInfo.szUserName
						, pDBInfo->dbConnectInfo.szUserPwd
						, pDBInfo->dbConnectInfo.szDBName
						, 0
						,NULL,CLIENT_MULTI_STATEMENTS) == NULL)
	     
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		mysql_close(pDBInfo->pMysql);   
		pDBInfo->pMysql			= NULL;
		return DB_ERROR_EXEC_FUNC;
	}

	if(strlen(pDBInfo->szEncodingName))
	{

		uRetCode	= mysql_set_character_set(pDBInfo->pMysql,pDBInfo->szEncodingName);
		if(DB_FUNC_EXEC_FAILED(uRetCode))
		{
			DB_Close(hDBHandle);
			return uRetCode;
		}
	}
	if(pDBInfo->bAutoSubmit)
	{
		mysql_autocommit(pDBInfo->pMysql,(my_bool)0xFF);
	}	
	return uRetCode;
}

void DB_Close(void* hDBHandle)
{
	LPDBInfo	pDBInfo	= NULL;

	pDBInfo			= (LPDBInfo)hDBHandle;
		
	if(pDBInfo != NULL && pDBInfo->pMysql != NULL)
	{
		mysql_close( pDBInfo->pMysql);   
		pDBInfo->pMysql	= NULL;		
	}
}

unsigned int DB_StartTrans(void* hDBHandle,char* lpszTransName,LPDBErrorInfo lpDBErrorInfo)
{
	unsigned int	uRetCode = RETCODE_SUCCEEDED;
	LPDBInfo	pDBInfo	 = NULL;
	pDBInfo			 = (LPDBInfo)hDBHandle;
	
	if(pDBInfo == NULL || pDBInfo->pMysql == NULL)
	{
		return DB_ERROR_PARAMETER_PASS;
	}
	uRetCode = mysql_ping(pDBInfo->pMysql);
	if(DB_FUNC_EXEC_FAILED(uRetCode))
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		return DB_ERROR_EXEC_FUNC;
	}

	uRetCode = mysql_query(pDBInfo->pMysql, "begin;");   
	if(DB_FUNC_EXEC_FAILED(uRetCode))
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		return DB_ERROR_EXEC_FUNC;
	}
	return uRetCode;
}

unsigned int DB_SubmitTrans(void* hDBHandle,char* lpszTransName,int bCommit,LPDBErrorInfo lpDBErrorInfo)
{
	unsigned int	uRetCode = RETCODE_SUCCEEDED;
	LPDBInfo	pDBInfo	 = NULL;
	pDBInfo			 = (LPDBInfo)hDBHandle;
	
	if(pDBInfo == NULL || pDBInfo->pMysql == NULL)
	{
	     return DB_ERROR_PARAMETER_PASS;
	}
	if(bCommit)
	{
	     uRetCode = mysql_commit(pDBInfo->pMysql);
	}
	else
	{
		uRetCode = mysql_rollback(pDBInfo->pMysql);
	}
	if(DB_FUNC_EXEC_FAILED(uRetCode))
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		return DB_ERROR_EXEC_FUNC;
	}
	return uRetCode;
}

unsigned int DB_ExecStoredProc(void* hDBHandle
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
						,LPDBErrorInfo lpDBErrorInfo)
{
	unsigned int		uRetCode	= RETCODE_SUCCEEDED;
	int			nExecCode	= 0;
	unsigned int		nRetFieldCount	= 0;
	unsigned int		nRetRowCount	= 0;
	unsigned int		nBindIndex	= 0;
	unsigned int		nDBColSizeTotal	= 0;	
        unsigned int		nRowIndex	= 0;
	unsigned int		uParamCount	= 0;
	LPDBInfo		pDBInfo		= NULL;
	MYSQL_STMT*		pSTMT		= NULL;
	MYSQL_BIND*		pParamBind	= NULL;
	MYSQL_BIND*		pRecordBind	= NULL;
	MYSQL_RES*		pRES		= NULL;
	MYSQL_FIELD*		pFields		= NULL;
	my_bool			myBool		= 0;
	LPDBParamBinding	pDBParam	= NULL;
	LPDBRecordBinding	pDBRecord	= NULL;
        char*			pRetBuffer	= NULL;
	char*			pRetBuffer_Cur	= NULL;
	char			chBuffer[DB_BUFFER_MAX_LENGTH];
	char*			pBuffer		= chBuffer;
	my_ulonglong		ulAffectedRow	= 0;
	

	pDBInfo	= (LPDBInfo)hDBHandle;
	if(hDBHandle == NULL 
		|| pDBInfo->pMysql == NULL)
	{
		return DB_ERROR_PARAMETER_PASS;
	}

	if(uDBParamBinding > DB_BINDING_PARAM_COUNT || uDBRecordBinding > DB_BINDING_RECORD_COUNT)
	{
		return DB_ERROR_BIND_PARAMETER_TOO_MORE;
	}

	ClearDBBinding(pDBInfo);
	pSTMT							= mysql_stmt_init(pDBInfo->pMysql); 
	if(pSTMT == NULL)
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		return DB_ERROR_EXEC_FUNC;
	}

	
	myBool							= mysql_stmt_reset(pSTMT);
	if(DB_FUNC_EXEC_FAILED(myBool)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}


	nExecCode						= mysql_stmt_prepare(pSTMT,lpszStoredProcText,(unsigned int)strlen(lpszStoredProcText));
	if(DB_FUNC_EXEC_FAILED(nExecCode))
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}

	uParamCount						= mysql_stmt_param_count(pSTMT);
	if(uParamCount != uDBParamBinding)
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_BIND_PARAMETER_COUNT;
	}

	pParamBind = pDBInfo->bindParam;
	pDBParam   = lpDBParam;
	
	
	// format param
	for (;nBindIndex < uDBParamBinding;nBindIndex++)
	{
		pParamBind->buffer_type	  = (enum_field_types)pDBParam->uParamType;
		pParamBind->buffer	  = pDBParam->pData;
		pParamBind->buffer_length = pDBParam->uParamLen;
		pParamBind->is_null	  = (my_bool*)&pDBParam->uNULLFlag;
		pParamBind->length	  = (long unsigned int*)&pDBParam->uDataLen;
		
		pParamBind++;
		pDBParam++;
	}

	// bind param 
	pParamBind						= pDBInfo->bindParam;
	myBool							= mysql_stmt_bind_param(pSTMT, pParamBind);
	if(DB_FUNC_EXEC_FAILED(myBool)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}

	// exec script
	nExecCode						= mysql_stmt_execute(pSTMT);
	if(DB_FUNC_EXEC_FAILED(nExecCode)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}
	
	
	ulAffectedRow	= mysql_stmt_affected_rows(pSTMT);
	if(ulAffectedRow > 0 && lpAffectedRowCount != NULL)
	{
		*lpAffectedRowCount	= (unsigned int)ulAffectedRow;
	}

	nExecCode					= mysql_stmt_store_result(pSTMT);
	if(DB_FUNC_EXEC_FAILED(nExecCode)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		mysql_stmt_close(pSTMT);
		return DB_ERROR_EXEC_FUNC;
	}

	// check row count

	nRetRowCount					= (int)mysql_stmt_num_rows(pSTMT);
	if(nRetRowCount == 0 || uDBRecordBinding == 0)
	{
		if(lpRecordCount != NULL)
		{
			*lpRecordCount			= nRetRowCount;
		}
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return uRetCode;
	}

	// check column count
	pRES						= mysql_stmt_result_metadata(pSTMT);
	if(pRES == NULL)
	{
		mysql_stmt_free_result(pSTMT);
		mysql_stmt_close(pSTMT);
		return DB_ERROR_EXPECT_RECORD;
	}
	
	nRetFieldCount					= mysql_num_fields(pRES);
	if(nRetFieldCount < uFieldCount)
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return DB_ERROR_RET_COL_MISMATCH;
	}


	// format record 
	pDBRecord					= lpDBRecord;
	
	
	// Total byte in db length and create max buffer 
	pFields						= mysql_fetch_fields(pRES);
	for (nBindIndex = 0; nBindIndex< uDBRecordBinding;nBindIndex++)
	{
		pDBRecord->uRetColOffSet	= nDBColSizeTotal;
		switch (pFields->type)
		{
		case MYSQL_TYPE_SHORT:
			pDBRecord->uRetColSize	= pDBRecord->uDataLen;
			nDBColSizeTotal	       += pDBRecord->uDataLen;
			break;
		case MYSQL_TYPE_LONG:
			pDBRecord->uRetColSize	= pDBRecord->uDataLen;
			nDBColSizeTotal	       += pDBRecord->uDataLen;
			break;
		case MYSQL_TYPE_FLOAT:
			pDBRecord->uRetColSize	= pDBRecord->uDataLen;
			nDBColSizeTotal	       += pDBRecord->uDataLen;
			break;
		case MYSQL_TYPE_DOUBLE:
			pDBRecord->uRetColSize	= pDBRecord->uDataLen;
			nDBColSizeTotal	       += pDBRecord->uDataLen;
			break;
		case MYSQL_TYPE_LONGLONG:
			pDBRecord->uRetColSize	= pDBRecord->uDataLen;
			nDBColSizeTotal	       += pDBRecord->uDataLen;
			break;
		default:
			pDBRecord->uRetColSize	= pFields->length;
			nDBColSizeTotal	       += pFields->length;
			break;			
		}
		pFields++;
		pDBRecord++;
	}

	if(nDBColSizeTotal >= DB_BUFFER_MAX_LENGTH)
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return DB_ERROR_RET_DATA_MORE;
	}

	// bind record 
	pRecordBind						= pDBInfo->bindRecord;
	pDBRecord						= lpDBRecord;
	for (nBindIndex = 0; nBindIndex< uDBRecordBinding;nBindIndex++)
	{
		pRecordBind->buffer_type   = (enum_field_types)pDBRecord->uDataType;
		pRecordBind->buffer	   = pBuffer+ pDBRecord->uRetColOffSet;
		pRecordBind->buffer_length = pDBRecord->uRetColSize;
		pRecordBind->length	   = (long unsigned int*)&pDBRecord->uRetColSize;
		pRecordBind++;
		pDBRecord++;		
	}

	bzero(chBuffer,sizeof(char)*DB_BUFFER_MAX_LENGTH);
	pRecordBind						= pDBInfo->bindRecord;
	myBool							= mysql_stmt_bind_result(pSTMT, pRecordBind);
	if(DB_FUNC_EXEC_FAILED(myBool)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return DB_ERROR_EXEC_FUNC;
	}

	pRetBuffer					= (char*)pDBInfo->pfnAllocMemory(nRetRowCount*uRecordSize);
	pRetBuffer_Cur					= pRetBuffer;
	if(pRetBuffer == NULL) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return DB_ERROR_NO_MEMORY;
	}
	
	//fill in data from record 
	nExecCode						= mysql_stmt_fetch(pSTMT);
	while(nExecCode == 0)
	{
		pDBRecord						= lpDBRecord;
		for (nBindIndex = 0; nBindIndex< uDBRecordBinding;nBindIndex++)
		{
		
				if(pDBRecord->uDataType != MYSQL_TYPE_VAR_STRING
					&& pDBRecord->uDataType != MYSQL_TYPE_STRING)
				{
					memcpy(pRetBuffer_Cur+pDBRecord->uDataOffset
						,pBuffer+pDBRecord->uRetColOffSet
						,pDBRecord->uRetColSize);
				}
				else
				{
					memcpy(pRetBuffer_Cur+pDBRecord->uDataOffset
						,pBuffer+pDBRecord->uRetColOffSet
						,pDBRecord->uDataLen);
				}				
			
			pDBRecord++;
		}
		pRetBuffer_Cur += uRecordSize;
		nRowIndex++;
		nExecCode						= mysql_stmt_fetch(pSTMT);
	}
	if(nRowIndex != nRetRowCount)
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return DB_ERROR_EXEC_FUNC;
	}
	else
	{
		*lpRecordCount					= nRetRowCount;
		*lpRetBuffer					= pRetBuffer;
	}
	ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
	return uRetCode;
}

void DB_ReleaseRecordBuffer(void* hDBHandle,char* lpDataBuffer)
{
	LPDBInfo		pDBInfo			= NULL;
	pDBInfo							= (LPDBInfo)hDBHandle;
	if(lpDataBuffer != NULL)
	{
		pDBInfo->pfnReleaseMemory(lpDataBuffer);
	}
}





unsigned int DB_ExecStoredProc2(void* hDBHandle
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
						  ,LPDBErrorInfo lpDBErrorInfo)
{
	unsigned int		uRetCode	= RETCODE_SUCCEEDED;
	int			nExecCode	= 0;
	unsigned int		nRetFieldCount	= 0;
	int			nRetRowCount	= 0;
	unsigned int		nBindIndex	= 0;
	unsigned int		nRowIndex	= 0;
	unsigned int		uParamCount	= 0;
	LPDBInfo		pDBInfo		= NULL;
	MYSQL_STMT*		pSTMT		= NULL;
	MYSQL_BIND*		pParamBind	= NULL;
	MYSQL_BIND*		pRecordBind	= NULL;
	MYSQL_RES*		pRES		= NULL;
	my_bool			myBool		= 0;
	LPDBParamBinding	pDBParam	= NULL;
	LPDBRecordBinding	pDBRecord	= NULL;
	char*			pBuffer		= NULL;		
	unsigned int		uDataRowLength	= 0;	
	unsigned int		uDataCellSize	= 0;	
	my_ulonglong		ulAffectedRow	= 0;
	
	

	pDBInfo	= (LPDBInfo)hDBHandle;
	
	if(hDBHandle == NULL 
		|| pDBInfo->pMysql == NULL)
	{
		return DB_ERROR_PARAMETER_PASS;
	}

	if(uDBParamBinding > DB_BINDING_PARAM_COUNT || uDBRecordBinding > DB_BINDING_RECORD_COUNT)
	{
		return DB_ERROR_BIND_PARAMETER_TOO_MORE;
	}

	ClearDBBinding(pDBInfo);
	pSTMT = mysql_stmt_init(pDBInfo->pMysql);
	if(pSTMT == NULL)
	{
		SetDBError(pDBInfo,lpDBErrorInfo);
		return DB_ERROR_EXEC_FUNC;
	}


	myBool = mysql_stmt_reset(pSTMT);
	if(DB_FUNC_EXEC_FAILED(myBool)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}


	nExecCode = mysql_stmt_prepare(pSTMT,lpszStoredProcText,strlen(lpszStoredProcText));
	if(DB_FUNC_EXEC_FAILED(nExecCode))
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}

	uParamCount						= mysql_stmt_param_count(pSTMT);
	if(uParamCount != uDBParamBinding)
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_BIND_PARAMETER_COUNT;
	}
	

	pParamBind						= pDBInfo->bindParam;
	pDBParam						= lpDBParam;

	// format param 
	for (;nBindIndex < uDBParamBinding;nBindIndex++)
	{
		pParamBind->buffer_type	  = (enum_field_types)pDBParam->uParamType;
		pParamBind->buffer	  = pDBParam->pData;
		pParamBind->buffer_length = pDBParam->uParamLen;
		pParamBind->is_null	  = (my_bool*)&pDBParam->uNULLFlag;
		pParamBind->length	  = (long unsigned int*)&pDBParam->uDataLen;
		
		pParamBind++;
		pDBParam++;
	}

	// bind param 
	pParamBind						= pDBInfo->bindParam;
	myBool							= mysql_stmt_bind_param(pSTMT, pParamBind);
	if(DB_FUNC_EXEC_FAILED(myBool)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}

	// exec script
	nExecCode						= mysql_stmt_execute(pSTMT);
	if(DB_FUNC_EXEC_FAILED(nExecCode)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,1);
		return DB_ERROR_EXEC_FUNC;
	}

	ulAffectedRow					= mysql_stmt_affected_rows(pSTMT);
	if(ulAffectedRow > 0 && lpAffectedRowCount != NULL)
	{
		*lpAffectedRowCount	= (unsigned int)ulAffectedRow;
	}

	nExecCode						= mysql_stmt_store_result(pSTMT);
	if(DB_FUNC_EXEC_FAILED(nExecCode)) 
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		mysql_stmt_close(pSTMT);
		return DB_ERROR_EXEC_FUNC;
	}

	// check row count

	nRetRowCount					= (int)mysql_stmt_num_rows(pSTMT);
	if(nRetRowCount == 0 || uDBRecordBinding == 0)
	{
		if(lpRecordCount != NULL)
		{
			*lpRecordCount			= nRetRowCount;
		}
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return uRetCode;
	}

	// check column count 
	pRES							= mysql_stmt_result_metadata(pSTMT);
	if(pRES == NULL)
	{
		mysql_stmt_free_result(pSTMT);
		mysql_stmt_close(pSTMT);
		return DB_ERROR_EXPECT_RECORD;
	}

	nRetFieldCount					= mysql_num_fields(pRES);
	if(nRetFieldCount < uDBRecordBinding)
	{
		SetDBStmtError(pSTMT,lpDBErrorInfo,0);
		ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
		return DB_ERROR_RET_COL_MISMATCH;
	}

	do 
	{
		
		pRecordBind						= pDBInfo->bindRecord;
		pDBRecord						= lpDBRecord;
		for (nBindIndex = 0; nBindIndex< uDBRecordBinding;nBindIndex++)
		{
			pDBRecord->uRetColSize	   = 0;
			pRecordBind->buffer	   = NULL;
			pRecordBind->buffer_length = 0;			
			pRecordBind->buffer_type   = (enum_field_types)0;
			pRecordBind->length	   = (long unsigned int*)&pDBRecord->uRetColSize;
			
			pRecordBind++;
			pDBRecord++;
		}
		pRecordBind						= pDBInfo->bindRecord;
		myBool							= mysql_stmt_bind_result(pSTMT, pRecordBind);
		if(DB_FUNC_EXEC_FAILED(myBool)) 
		{
			SetDBStmtError(pSTMT,lpDBErrorInfo,0);
			ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
			return DB_ERROR_EXEC_FUNC;
		}
		nExecCode						= mysql_stmt_fetch(pSTMT);
		if(nExecCode != MYSQL_DATA_TRUNCATED)  
		{
			break;
		}
		
		
		pDBRecord						= lpDBRecord;
		pBuffer							= NULL;		
		uDataRowLength					= 0;	

		for (nBindIndex = 0; nBindIndex< uDBRecordBinding;nBindIndex++)
		{
			uDataCellSize				= 0;
			switch (pDBRecord->uDataType)
			{
			case MYSQL_TYPE_DECIMAL:
				uDataCellSize			= 4;	
				break;
			case MYSQL_TYPE_TINY:
				uDataCellSize			= 1;	
				break;
			case MYSQL_TYPE_SHORT:
				uDataCellSize			= 2;	
				break;
			case MYSQL_TYPE_LONG:
				uDataCellSize			= 4;	
				break;
			case MYSQL_TYPE_FLOAT:
				uDataCellSize			= 4;	
				break;
			case MYSQL_TYPE_DOUBLE:
				uDataCellSize			= 8;	
				break;
			case MYSQL_TYPE_NULL:
				uDataCellSize			= 1;	
				break;
			case MYSQL_TYPE_TIMESTAMP:
				uDataCellSize			= 1;	
				break;
			case MYSQL_TYPE_LONGLONG:
				uDataCellSize			= 8;	
				break;
			case MYSQL_TYPE_INT24:
				uDataCellSize			= 3;	
				break;
			case MYSQL_TYPE_DATE:
			case MYSQL_TYPE_TIME:
			case MYSQL_TYPE_DATETIME:
				uDataCellSize			= sizeof(MYSQL_TIME);	
				break;
			case MYSQL_TYPE_YEAR:
				uDataCellSize			= 1;	
				break;
			case MYSQL_TYPE_NEWDATE:
				uDataCellSize			= 1;	
				break;
			case MYSQL_TYPE_VARCHAR:
				if(pDBRecord->uUnicodeFlag == DB_UNICODE_FLAG)
				{
					pDBRecord->uRetColSize	= DB_BUFFER_SIZE(pDBRecord->uRetColSize);
					uDataCellSize		=  pDBRecord->uRetColSize;	
				}
				else
				{
					uDataCellSize		= pDBRecord->uRetColSize;	
				}
				break;
			case MYSQL_TYPE_BIT:
			case MYSQL_TYPE_NEWDECIMAL:
			case MYSQL_TYPE_ENUM:
			case MYSQL_TYPE_SET:
				uDataCellSize			= pDBRecord->uRetColSize;	
				break;
			case MYSQL_TYPE_TINY_BLOB:
			case MYSQL_TYPE_MEDIUM_BLOB:
			case MYSQL_TYPE_LONG_BLOB:
			case MYSQL_TYPE_BLOB:
				uDataCellSize			= pDBRecord->uRetColSize;	
				break;
			case MYSQL_TYPE_VAR_STRING:
			case MYSQL_TYPE_STRING:
				if(pDBRecord->uUnicodeFlag == DB_UNICODE_FLAG)
				{
					pDBRecord->uRetColSize	= DB_BUFFER_SIZE(pDBRecord->uRetColSize);
					uDataCellSize		=  pDBRecord->uRetColSize;			
				}
				else
				{
					uDataCellSize		= pDBRecord->uRetColSize;	
				}
				break;
			case MYSQL_TYPE_GEOMETRY:
				uDataCellSize			= pDBRecord->uRetColSize;	
				break;
			}
			// update current binding data 
			pDBRecord->uRetColOffSet	= uDataRowLength;
			uDataRowLength				+= uDataCellSize;
			pDBRecord++;
		}
		
		
		pBuffer							= (char*)pDBInfo->pfnAllocMemory(uDataRowLength);
		if(pBuffer == NULL)
		{
			uRetCode = DB_ERROR_NO_MEMORY;
			break;			
		}


		
		pRecordBind = pDBInfo->bindRecord;
		pDBRecord   = lpDBRecord;
		
		for (nBindIndex = 0; nBindIndex< uDBRecordBinding;nBindIndex++)
		{

			pRecordBind->buffer	   = pBuffer + pDBRecord->uRetColOffSet;
			pRecordBind->buffer_length = pDBRecord->uRetColSize;
			pRecordBind->length	   = (long unsigned int*)&pDBRecord->uRetColSize;
			pRecordBind->buffer_type   = (enum_field_types)pDBRecord->uDataType;
			mysql_stmt_fetch_column(pSTMT, pRecordBind, nBindIndex, 0);
			
			pRecordBind++;
			pDBRecord++;
		}
	

		
		uRetCode = pfnDBFetchCallBack(hUserData,hUserBuffer,uDBRecordBinding, lpDBRecord,pDBInfo->uEncoding, pBuffer);
			
		
		pDBInfo->pfnReleaseMemory(pBuffer);
		pBuffer							= NULL;
		if(uRetCode >0) 
		{
		     break;			
		}
		nRowIndex++;
	} while (1);
	
	
	if(pBuffer != NULL)
	{
		pDBInfo->pfnReleaseMemory(pBuffer);
	}

	
	pfnDBFetchCallBack(hUserData,hUserBuffer,uDBRecordBinding,lpDBRecord,0,NULL);

	if(lpRecordCount != NULL)
	{
		*lpRecordCount					= nRetRowCount;
	}
	ReleaseSTMTRes(pDBInfo->pMysql,pRES,pSTMT);
	return uRetCode;
}
