#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include "SqliteIf.h"
#include "SqliteLib.h"

void ResetSqliteErrInfo(SqliteErr* lpErr)
{
	if(lpErr != NULL)
	{
		bzero(lpErr,sizeof(SqliteErr));
	}
}


// called by operate data
void SetSqliteErrInfo(int slRet,char* lpszErrMsg,  SqliteErr* lpErr)
{
	if(lpErr != NULL)
	{
		ResetSqliteErrInfo(lpErr);
		lpErr->nErrID		= slRet;
		strcpy(lpErr->szErrText,lpszErrMsg);
	}
}

void ReleaseErrMsg(char* lpszErrMsg)
{
	sqlite3_free(lpszErrMsg);
}


// called by connect 
void SetSqliteDBErrInfo(sqlite3 *lpSqlite,SqliteErr* lpErr)
{
	if(lpErr != NULL)
	{
		ResetSqliteErrInfo(lpErr);
		lpErr->nErrID		= sqlite3_errcode(lpSqlite);
		strcpy(lpErr->szErrText,sqlite3_errmsg(lpSqlite));
	}
}


DATAREF ObjAddRef(SqliteInfo* lpSqliteInfo)
{
	DATAREF		uRef		= 0;
	
	lpSqliteInfo->nRef++;
	uRef				= lpSqliteInfo->nRef;
	
	return uRef;
}


DATAREF ObjReleaseRef(SqliteInfo* lpSqliteInfo)
{
	DATAREF		uRef		= 0;
	
	lpSqliteInfo->nRef--;
	uRef				= lpSqliteInfo->nRef;
	
	return uRef;
}

HRETCODE RemoveDataFile(char* lpszDataFile)
{
	unsigned int	dwRet = 0;
	int		bRet  = 0;
	

	
	unlink(lpszDataFile);
	return SQLITE_RETCODE_OK;
	
}

HRETCODE	UpdateSqliteParam(SqliteInfo* lpSqliteInfo,sqlite3*	lpSqlite,SqliteSetting* lpSqliteSetting)
{
	return SQLITE_RETCODE_OK;
}


