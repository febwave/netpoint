#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <mysql.h>
#include <DBIf.h>
#include <DBLib.h>



void ClearDBBinding(LPDBInfo lpDBInfo)
{
	bzero(lpDBInfo->bindParam,sizeof(lpDBInfo->bindParam));
	bzero(lpDBInfo->bindRecord,sizeof(lpDBInfo->bindRecord));
}

void SetDBError(LPDBInfo lpDBInfo,LPDBErrorInfo lpDBErrorInfo)
{
	lpDBErrorInfo->uErrorNo		= mysql_errno(lpDBInfo->pMysql);
	strcpy(lpDBErrorInfo->szErrMsg,mysql_error(lpDBInfo->pMysql));
}

void SetDBStmtError(MYSQL_STMT* lpSTMT,LPDBErrorInfo lpDBErrorInfo,int bAutoReleaseSTMT)
{
	lpDBErrorInfo->uErrorNo		= mysql_stmt_errno(lpSTMT);
	strcpy(lpDBErrorInfo->szErrMsg,mysql_stmt_error(lpSTMT));
	if(bAutoReleaseSTMT)
	{
		mysql_stmt_close(lpSTMT);
	}
		
}

void ReleaseSTMTRes(MYSQL* lpMysql,MYSQL_RES* lpRES,MYSQL_STMT* lpSTMT)
{
	int		nRet	= 0;
	mysql_free_result(lpRES);
	mysql_stmt_free_result(lpSTMT);
	mysql_next_result(lpMysql);	/*must add this function , otherwise you can't close stmt */
	mysql_stmt_close(lpSTMT);
}
