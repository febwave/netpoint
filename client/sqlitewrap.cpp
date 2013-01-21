#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <sqlite3.h>
#include "SqliteIf.h"
#include "SqliteLib.h"

#define SQLITE_EXEC_TIMEOUT				30*1000


HRETCODE slCreateInstance(void** lpSqliteHandle)
{
	SqliteInfo*	pInfo = NULL;
	

	pInfo		= new SqliteInfo();
	bzero(pInfo,sizeof(SqliteInfo));
	*lpSqliteHandle	= pInfo;
	
	return SQLITE_RETCODE_OK;
}

void slReleaseInstance(void* hSqliteHandle)
{
	SqliteInfo*		pInfo		= NULL;

	pInfo			= (SqliteInfo*)hSqliteHandle;
	if(pInfo->pSqlite != NULL)
	{
		sqlite3_close(pInfo->pSqlite);
		pInfo->pSqlite	= NULL;
	}
	delete pInfo;
}

HRETCODE	slConnect(void* hSqliteHandle,SqliteSetting* lpSqliteSetting,SqliteErr* lpErr)
{
	SqliteInfo*	pInfo	  = NULL;
	sqlite3*	pSqlite   = NULL;
	char*		pszErrMsg = NULL;
	int		slRet	  = 0;
	
	

	pInfo = (SqliteInfo*)hSqliteHandle;
	if(pInfo->pSqlite == NULL)
	{
		strcpy(pInfo->szDataFileName,lpSqliteSetting->szFileName);
		slRet = sqlite3_open_v2(pInfo->szDataFileName, &pSqlite,SQLITE_OPEN_READWRITE,NULL);
		if(SQLITE_FAILED(slRet))
		{
			SetSqliteDBErrInfo(pSqlite,lpErr);
			sqlite3_close(pSqlite);
			return SQLITE_E_NO_BAD_PARAMETER;
		}		
		pInfo->pSqlite = pSqlite;		
	}
	else
	{
	     pSqlite = pInfo->pSqlite;
	}
	slRet = sqlite3_exec(pSqlite,"PRAGMA SYNCHRONOUS=OFF;", NULL, 0, &pszErrMsg);
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
		ReleaseErrMsg(pszErrMsg);	
	}	
	slRet = sqlite3_exec(pSqlite,"PRAGMA journal_mode=OFF;", NULL, 0, &pszErrMsg);
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
		ReleaseErrMsg(pszErrMsg);	
	}	
	slRet = sqlite3_busy_timeout(pSqlite,lpSqliteSetting->dwTimeout > 0 ? lpSqliteSetting->dwTimeout : SQLITE_EXEC_TIMEOUT);
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
		ReleaseErrMsg(pszErrMsg);	
	}	

	ObjAddRef(pInfo);
	return SQLITE_RETCODE_OK;
}

void slClose(void* hSqliteHandle)
{
	SqliteInfo*		pInfo		= NULL;
	sqlite3*		pSqlite 	= NULL;
	DATAREF			nRef		= 0;

	pInfo			= (SqliteInfo*)hSqliteHandle;
	pSqlite			= pInfo->pSqlite;
	if(pSqlite != NULL)
	{
		nRef		= ObjReleaseRef(pInfo);
		if(nRef == 0)
		{
			sqlite3_close(pSqlite);
			pInfo->pSqlite	= NULL;
		}
	}
}


HRETCODE	slCreateTable(void* hSqliteHandle,char* lpszSQLText,SqliteErr* lpErr)
{
	SqliteInfo*	pInfo	  = NULL;
	sqlite3*	pSqlite   = NULL;
	char*		pszErrMsg = NULL;
	int		slRet	  = 0;
	

	pInfo	= (SqliteInfo*)hSqliteHandle;
	pSqlite	= pInfo->pSqlite;
	slRet	= sqlite3_exec(pSqlite,lpszSQLText, NULL, 0, &pszErrMsg);
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
		ReleaseErrMsg(pszErrMsg);
		return SQLITE_E_NO_BAD_PARAMETER;
		
	}	
	slRet = sqlite3_exec(pSqlite,"PRAGMA SYNCHRONOUS=OFF;", NULL, 0, &pszErrMsg);
	if(SQLITE_FAILED(slRet))
	{
		 SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
		 ReleaseErrMsg(pszErrMsg);	
	}	
	return SQLITE_RETCODE_OK;
}





HRETCODE	slInitEnv(void* hSqliteHandle,SqliteConfig* lpSqliteConfig,SqliteErr* lpErr)
{
	HRETCODE	hRet	  = SQLITE_RETCODE_OK;
	int		slRet	  = 0;
	sqlite3*	pSqlite   = NULL;
	SqliteInfo*	pInfo	  = NULL;
	
	

	pInfo			= (SqliteInfo*)hSqliteHandle;

	lpSqliteConfig->bAlwaysCreate	= 0;
	if(lpSqliteConfig->bAlwaysCreate == 1)
	{
		// remove old file
		hRet		= RemoveDataFile(lpSqliteConfig->szFileName);
		if(SL_FAILED(hRet))
		{
			return hRet;
		}
	}
	strcpy(pInfo->szDataFileName,lpSqliteConfig->szFileName);
	slRet = sqlite3_open_v2(pInfo->szDataFileName, &pSqlite,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL);
	
	
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteDBErrInfo(pSqlite,lpErr);
		sqlite3_close(pSqlite);
		return SQLITE_E_NO_BAD_PARAMETER;
	}
	else
	{
		/*slRet			= sqlite3_exec(pSqlite,"PRAGMA SYNCHRONOUS=OFF;", NULL, 0, &pszErrMsg);
		if(SQLITE_FAILED(slRet))
		{
			SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
			ReleaseErrMsg(pszErrMsg);	
		}	*/
		pInfo->pSqlite	= pSqlite;
		ObjAddRef(pInfo);
	}
	return SQLITE_RETCODE_OK;
}


HRETCODE	slReleaseEnv(void* hSqliteHandle,SqliteErr* lpErr)
{
	HRETCODE	hRet  = SQLITE_RETCODE_OK;
	SqliteInfo*	pInfo = NULL;
	

	pInfo			= (SqliteInfo*)hSqliteHandle;
	ObjReleaseRef(pInfo);
	sqlite3_close(pInfo->pSqlite);
	// remove old file
//	hRet			= RemoveDataFile(pInfo->szDataFileName);
	return hRet;
}


HRETCODE	slPrepareSQL(void* hSqliteHandle,char* lpszSQLText,void** lpSqlitePrepareObj,SqliteErr* lpErr)     
{
	SqliteInfo*	pInfo	= NULL;
	sqlite3_stmt*	pSTMT	= NULL;
	sqlite3*	pSqlite = NULL;
	int		slRet	= 0;
	

	pInfo	= (SqliteInfo*)hSqliteHandle;
	pSqlite	= pInfo->pSqlite;
	slRet	= sqlite3_prepare_v2(pSqlite,lpszSQLText,-1,&pSTMT,NULL);
	
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteDBErrInfo(pSqlite,lpErr);
		return SQLITE_E_NO_BAD_PARAMETER;
	}	
	*lpSqlitePrepareObj	= pSTMT;
	return SQLITE_RETCODE_OK;
}

HRETCODE	slReleasePrepareObj(void* hSqliteHandle,void* hSqlitePrepareObj,SqliteErr* lpErr)
{
	SqliteInfo*	pInfo	= NULL;
	sqlite3_stmt*	pSTMT	= NULL;
	sqlite3*	pSqlite = NULL;
	int		slRet	= 0;

	

	pInfo	= (SqliteInfo*)hSqliteHandle;
	pSqlite	= pInfo->pSqlite;
	pSTMT	= (sqlite3_stmt*)hSqlitePrepareObj;
	slRet	= sqlite3_finalize(pSTMT);
	
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteDBErrInfo(pSqlite,lpErr);
		return SQLITE_E_NO_BAD_PARAMETER;
	}	
	return SQLITE_RETCODE_OK;
}

void slFillSimpleSLParamData(SqliteParamBinding* lpSLParamBindingAr,int nIndex, unsigned int uParamType,char* lpData, unsigned int uDataLen,unsigned int  uNULLFlag)
{
	lpSLParamBindingAr[nIndex].uParamType = uParamType;
	lpSLParamBindingAr[nIndex].pData      = lpData;
	lpSLParamBindingAr[nIndex].uDataLen   = uDataLen;
	lpSLParamBindingAr[nIndex].uNULLFlag  = uNULLFlag;
	
}



HRETCODE slCheckCellParam(SqliteCBData* lpCBData,int nColIndex)
{
	if(lpCBData == NULL) return SQLITE_E_NO_BAD_PARAMETER;
	if(lpCBData->nSetData == SQLITE_DATA_COLUNM
		|| lpCBData->nSetData == SQLITE_DATA_PLAINT)
	{
		if(nColIndex >= 0 && nColIndex < lpCBData->cColumns)
		{
			if(lpCBData->nSetData == SQLITE_DATA_PLAINT)
			{
				if(lpCBData->PlaintData.hRowData == NULL)
				{
					return SQLITE_E_BIND_DATA_OBJ;
				}
			}	
			else
			{
				if(lpCBData->ColunmData.lplpColumnData == NULL
					|| lpCBData->ColunmData.lplpColumnName == NULL)
				{
					return SQLITE_E_BIND_DATA_OBJ;
				}				
			}

			return SQLITE_RETCODE_OK;
		}
		return SQLITE_E_BIND_DATA_INDEX;
	}
	return SQLITE_E_NO_IMPLEMENT;
}


int GetCellIntByPlaint(sqlite3_stmt* lpSTMT,int nColIndex)
{
	return sqlite3_column_int(lpSTMT,nColIndex);
}

int64_t GetCellInt64ByPlaint(sqlite3_stmt* lpSTMT,int nColIndex)
{
	return sqlite3_column_int64(lpSTMT,nColIndex);
}


int GetCellIntByColumn(SqliteCBData* lpCBData ,int nColIndex)
{
	if(lpCBData->ColunmData.lplpColumnData[nColIndex] != NULL)
	{
		return atoi(lpCBData->ColunmData.lplpColumnData[nColIndex]);
	}
	return 0;
}

int64_t GetCellInt64ByColumn(SqliteCBData* lpCBData ,int nColIndex)
{
	if(lpCBData->ColunmData.lplpColumnData[nColIndex] != NULL)
	{
	     /*	return atoi64(lpCBData->ColunmData.lplpColumnData[nColIndex]);*/
	}
	return 0;
}

HRETCODE slGetCellInt(SqliteCBData* lpCBData,int nColIndex,int* lpRetBuffer)
{
	HRETCODE	hRet		= SQLITE_RETCODE_OK;
	hRet		= slCheckCellParam(lpCBData,nColIndex);
	if(SL_FAILED(hRet))  return hRet;
	if(lpRetBuffer == NULL) return SQLITE_E_BIND_DATA_BUFFER;
	if(lpCBData->nSetData == SQLITE_DATA_PLAINT)
	{
		*lpRetBuffer		= GetCellIntByPlaint((sqlite3_stmt*)lpCBData->PlaintData.hRowData,nColIndex);
	}
	else
	{
		*lpRetBuffer		= GetCellIntByColumn(lpCBData, nColIndex);
	}
	return SQLITE_RETCODE_OK;
}

HRETCODE slGetCellInt64(SqliteCBData* lpCBData,int nColIndex,int64_t* lpRetBuffer)
{
	HRETCODE	hRet		= SQLITE_RETCODE_OK;
	hRet		= slCheckCellParam(lpCBData,nColIndex);
	if(SL_FAILED(hRet))  return hRet;
	if(lpRetBuffer == NULL) return SQLITE_E_BIND_DATA_BUFFER;
	if(lpCBData->nSetData == SQLITE_DATA_PLAINT)
	{
		*lpRetBuffer		= GetCellInt64ByPlaint((sqlite3_stmt*)lpCBData->PlaintData.hRowData,nColIndex);
	}
	else
	{
		*lpRetBuffer		= GetCellInt64ByColumn(lpCBData, nColIndex);
	}
	return SQLITE_RETCODE_OK;
}



// get Text
char* GetCellTextByPlaint(sqlite3_stmt* lpSTMT,int nColIndex)
{
	if(sqlite3_column_bytes(lpSTMT,nColIndex) > 0)
	{
		return (char*)sqlite3_column_text(lpSTMT,nColIndex);
	}
	return NULL;
}	

char* GetCellTextByColumn(SqliteCBData* lpCBData,int nColIndex)
{
	return (char*)lpCBData->ColunmData.lplpColumnData[nColIndex];
}

HRETCODE slGetCellText(SqliteCBData* lpCBData,int nColIndex,char* lpRetBuffer,int nBufferSize,int* lpRetBufferSize)
{
	HRETCODE	hRet	 = SQLITE_RETCODE_OK;
	char*		pszText	 = NULL;
	int		nTextLen = 0;
	
	hRet		= slCheckCellParam(lpCBData,nColIndex);
	if(SL_FAILED(hRet))  return hRet;
	if(lpCBData->nSetData == SQLITE_DATA_PLAINT)
	{
		pszText		= GetCellTextByPlaint((sqlite3_stmt*)lpCBData->PlaintData.hRowData,nColIndex);			
	}
	else
	{
		pszText		= GetCellTextByColumn(lpCBData,nColIndex);
	}
	if(pszText == NULL)
	{
		return SQLITE_RETCODE_OK;
	}
	
	nTextLen		= strlen(pszText);	
	if(lpRetBufferSize != NULL)
	{
		*lpRetBufferSize	= nTextLen;
	}
	if(nBufferSize > nTextLen && lpRetBuffer != NULL)
	{
		strcpy(lpRetBuffer,pszText);		
		return SQLITE_RETCODE_OK;
	}
	return SQLITE_E_DATA_BUFFER_TOO_SMALL;
}



// get double
double GetCellDoubleByPlaint(sqlite3_stmt* lpSTMT,int nColIndex)
{
	return sqlite3_column_double(lpSTMT,nColIndex);
}

double GetCellDoubleByColumn(SqliteCBData* lpCBData ,int nColIndex)
{
	if(lpCBData->ColunmData.lplpColumnData[nColIndex] != NULL)
	{
		return atof(lpCBData->ColunmData.lplpColumnData[nColIndex]);
	}
	return 0;
}


HRETCODE slGetCellDouble(SqliteCBData* lpCBData,int nColIndex,double* lpRetBuffer)
{
	HRETCODE	hRet		= SQLITE_RETCODE_OK;
	hRet		= slCheckCellParam(lpCBData,nColIndex);
	if(SL_FAILED(hRet))  return hRet;
	if(lpRetBuffer == NULL) return SQLITE_E_BIND_DATA_BUFFER;
	if(lpCBData->nSetData == SQLITE_DATA_PLAINT)
	{
		*lpRetBuffer		= GetCellDoubleByPlaint((sqlite3_stmt*)lpCBData->PlaintData.hRowData,nColIndex);
	}
	else
	{
		*lpRetBuffer		= GetCellDoubleByColumn(lpCBData, nColIndex);
	}
	return SQLITE_RETCODE_OK;
}



// get Blob
int GetCellBlobByPlaint(sqlite3_stmt* lpSTMT,int nColIndex,void** lplpRetData)
{
	int			nBlobSize	= 0;
	nBlobSize	= sqlite3_column_bytes(lpSTMT,nColIndex);
	if(nBlobSize > 0)
	{
		*lplpRetData	= (void*)sqlite3_column_blob(lpSTMT,nColIndex);
	}
	return nBlobSize;
}	

int  GetCellBlobByColumn(SqliteCBData* lpCBData,int nColIndex,void** lplpRetData)
{
	int			nBlobSize	= 0;
	if(lpCBData->ColunmData.lplpColumnData[nColIndex] != NULL)
	{
		*lplpRetData	= (void*)lpCBData->ColunmData.lplpColumnData[nColIndex];	
		nBlobSize		= strlen(lpCBData->ColunmData.lplpColumnData[nColIndex]);	// no need to add \0
	}
	return nBlobSize;
}


HRETCODE slGetCellBlob(SqliteCBData* lpCBData,int nColIndex,char* lpRetBuffer,int nBufferSize,int* lpRetBufferSize)
{
	HRETCODE	hRet	 = SQLITE_RETCODE_OK;
	void*		pData	 = NULL;
	int		nDataLen = 0;
	
	hRet		= slCheckCellParam(lpCBData,nColIndex);
	if(SL_FAILED(hRet))  return hRet;
	if(lpCBData->nSetData == SQLITE_DATA_PLAINT)
	{
		nDataLen		= GetCellBlobByPlaint((sqlite3_stmt*)lpCBData->PlaintData.hRowData,nColIndex,&pData);			
	}
	else
	{
		nDataLen		= GetCellBlobByColumn(lpCBData,nColIndex,&pData);
	}
	if(lpRetBufferSize != NULL)
	{
		*lpRetBufferSize	= nDataLen;
	}
	if(nDataLen == 0)
	{
		return SQLITE_RETCODE_OK;
	}
	if(nBufferSize >= nDataLen && lpRetBuffer != NULL)
	{
		memcpy(lpRetBuffer,pData,nDataLen);		
		return SQLITE_RETCODE_OK;
	}
	return SQLITE_E_DATA_BUFFER_TOO_SMALL;
}



int sqlitecallback(void* UserData,int cColumns,char** lplpColumnData,char** lplpColumnName)
{
	SqliteCBInfo*	pCBInfo	= NULL;
	SqliteCBData	CBData	= {0};
	HRETCODE	hRet	= SQLITE_RETCODE_OK;
	

	pCBInfo				 = (SqliteCBInfo*)UserData;
	CBData.cColumns			 = cColumns;
	CBData.nSetData			 = SQLITE_DATA_COLUNM;
	CBData.ColunmData.lplpColumnData = lplpColumnData;
	CBData.ColunmData.lplpColumnName = lplpColumnName;
	hRet				 = pCBInfo->pfnSqliteFetchCallBack(UserData,&CBData);
	
	return hRet;
}


HRETCODE slExecText(void* hSqliteHandle,char* lpszSQLText,void* hUserData ,ppfnSqliteFetchCallBack pfnSqliteFetchCallBack ,SqliteErr* lpErr)
{
	SqliteInfo*	pInfo	  = NULL;
	sqlite3*	pSqlite   = NULL;
	char*		pszErrMsg = NULL;
	int		slRet	  = SQLITE_RETCODE_OK;
	SqliteCBInfo	cbInfo	  = {0};
	

	pInfo			= (SqliteInfo*)hSqliteHandle;
	pSqlite			= pInfo->pSqlite;
	if(pfnSqliteFetchCallBack == NULL)
	{
		slRet = sqlite3_exec(pSqlite,lpszSQLText, NULL, 0, &pszErrMsg);		
	}
	else
	{
		cbInfo.hUserData	      = hUserData;
		cbInfo.pfnSqliteFetchCallBack = pfnSqliteFetchCallBack;
		slRet			      = sqlite3_exec(pSqlite,lpszSQLText, sqlitecallback, &cbInfo, &pszErrMsg);
		
	}
	if(SQLITE_FAILED(slRet))
	{
		SetSqliteErrInfo(slRet,pszErrMsg,lpErr);
		ReleaseErrMsg(pszErrMsg);	
	}
	return slRet;
}

HRETCODE	slExecText2(void* hSqliteHandle
						,void* hSqlitePrepareObj
						,unsigned int cParamBinding
						,SqliteParamBinding* lpParamBinding
						,void* hUserData
						,ppfnSqliteFetchCallBack pfnSqliteFetchCallBack
						,unsigned int* lpuRowCount
						,SqliteErr* lpErr)
{
	SqliteInfo*	pInfo	     = NULL;
	sqlite3*	pSqlite      = NULL;
	sqlite3_stmt*	pSTMT	     = NULL;
	int		slRet	     = SQLITE_OK;
	SqliteCBData	CBData	     = {0};
	int		nColumnCount = 0;
        unsigned int	uRowCount    = 0;
	HRETCODE	hRet	     = SQLITE_RETCODE_OK;
	


	pInfo			= (SqliteInfo*)hSqliteHandle;
	pSqlite			= pInfo->pSqlite;
	pSTMT			= (sqlite3_stmt*)hSqlitePrepareObj;

	// ³õÊŒ»¯bind
	slRet			= sqlite3_clear_bindings(pSTMT);
	if(SQLITE_SUCCEEDED(slRet))
	{
		// ³õÊŒ»¯×ŽÌ¬
		slRet		= sqlite3_reset(pSTMT);		
		if(SQLITE_SUCCEEDED(slRet))
		{
			// bind data
			for (unsigned int uIndex = 0; uIndex < cParamBinding;uIndex++)
			{
				slRet		= SQLITE_E_BIND_PARAM;
				switch (lpParamBinding[uIndex].uParamType)
				{
				case SQLITE_INTEGER:
					slRet	= sqlite3_bind_int(pSTMT,uIndex+1,*(int*)(lpParamBinding[uIndex].pData));
					break;
				case SQLITE_FLOAT:
					slRet	= sqlite3_bind_double(pSTMT,uIndex+1,*(double*)(lpParamBinding[uIndex].pData));
					break;
				case SQLITE_BLOB:
					slRet	= sqlite3_bind_blob(pSTMT,uIndex+1,lpParamBinding[uIndex].pData,lpParamBinding[uIndex].uDataLen,SQLITE_STATIC);
					break;
				case SQLITE_NULL:
					slRet	= sqlite3_bind_null(pSTMT,uIndex+1);
					break;
				case SQLITE3_TEXT:
					slRet	= sqlite3_bind_text(pSTMT,uIndex+1,(char*)(lpParamBinding[uIndex].pData),lpParamBinding[uIndex].uDataLen,SQLITE_STATIC);
					break;
				default:
					break;
				}
				if(SQLITE_FAILED(slRet))
				{
					break;
				}
			}

			if(SQLITE_SUCCEEDED(slRet))
			{
				// exec
				nColumnCount = sqlite3_column_count(pSTMT);
				if(nColumnCount > 0)
				{
					do 
					{
						slRet = sqlite3_step(pSTMT);
						if(slRet != SQLITE_ROW)
						{
							if(slRet == SQLITE_DONE)
							{
								slRet	= SQLITE_OK;
							}
							break;
						}
						uRowCount++;
						if(pfnSqliteFetchCallBack != NULL)
						{
							CBData.cColumns	= nColumnCount;
							CBData.nSetData	= SQLITE_DATA_PLAINT;
							CBData.PlaintData.hRowData	= pSTMT;
							hRet			= pfnSqliteFetchCallBack(hUserData,&CBData);
							if(SL_FAILED(hRet))
							{
								slRet	= SQLITE_OK;
								break;
							}
						}						
					} while (1);
				}
				else
				{
					// žüÐÂ²Ù×÷
					slRet = sqlite3_step(pSTMT);
					if(slRet == SQLITE_DONE)
					{
						slRet	= SQLITE_OK;
					}
				}
			}
		}
	}
	if(SQLITE_FAILED(slRet) && slRet != SQLITE_DONE)
	{
		SetSqliteDBErrInfo(pSqlite,lpErr);	
	}
	if(lpuRowCount != NULL)
	{
		*lpuRowCount	= uRowCount;
	}
	if(slRet == SQLITE_DONE && uRowCount > 0 )
	{
		slRet	= SQLITE_OK;
	}
	return slRet;
}


HRETCODE sl_start_trans(void* hSqliteHandle,SqliteErr &ser)
{
     HRETCODE hRet = SQLITE_RETCODE_OK;

     hRet = slExecText(hSqliteHandle,"begin exclusive;",NULL, NULL,&ser);
     
     return hRet;     
}

HRETCODE sl_close_trans(void* hSqliteHandle,int nCommit,SqliteErr &ser)
{
     HRETCODE hRet = SQLITE_RETCODE_OK;
      if(nCommit == 1)
      {
	   hRet = slExecText(hSqliteHandle,"commit;",NULL, NULL,&ser);
      }
      else
      {
	   hRet = slExecText(hSqliteHandle,"rollback;",NULL, NULL,&ser);
      }
      return hRet;     
}
