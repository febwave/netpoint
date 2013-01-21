#include <string.h>
#include "DBIf.h"
#include <time.h>
#include <mysql.h>


void IMS_ClearDBError(DBErrorInfo* lpDBErrorInfo)
{
	bzero(lpDBErrorInfo,sizeof(lpDBErrorInfo));
}

void IMS_InitDBParamBinding(DBParamBinding* lpNewBindingCopy,int nParamCount)
{
	for (int nItemIndex = 0;nItemIndex < nParamCount; nItemIndex++)
	{
		lpNewBindingCopy[nItemIndex].uNULLFlag	= 1;
	}
}


void IMS_GetDBParamBindingCopy(DBParamBinding* lpSrcBinding,DBParamBinding* lpNewBindingCopy,int nParamCount)
{
	memcpy(lpNewBindingCopy,lpSrcBinding,sizeof(DBParamBinding)*nParamCount);
	IMS_InitDBParamBinding(lpNewBindingCopy,nParamCount);
}

void IMS_FillDBParamBindingData(DBParamBinding* lpDBParamBinding1st,int uParamIndex, void* lpData,unsigned int uDataLen,int bIsNULLFlag)
{
	DBParamBinding* pDBParamBinding		= lpDBParamBinding1st+uParamIndex;
	if(bIsNULLFlag == 0)
	{
		pDBParamBinding->pData	  = (char*)lpData;
		pDBParamBinding->uDataLen = uDataLen;
		
	}
	pDBParamBinding->uNULLFlag			= bIsNULLFlag == 1 ? 1:0;	
}



void IMS_GetDBRecordBindingCopy(DBRecordBinding* lpSrcBinding,DBRecordBinding* lpNewBindingCopy,int nParamCount)
{
	memcpy(lpNewBindingCopy,lpSrcBinding,sizeof(DBRecordBinding)*nParamCount);
}



int ConvertSystemTimeToMYSQLTime(struct tm* lpSystemtime,MYSQL_TIME* lpMysqlTime)
{	
     lpMysqlTime->year	    = lpSystemtime->tm_year;
     lpMysqlTime->month     = lpSystemtime->tm_mon;
     lpMysqlTime->day       = lpSystemtime->tm_mday;
     lpMysqlTime->hour      = lpSystemtime->tm_hour;
     lpMysqlTime->minute    = lpSystemtime->tm_min;
     lpMysqlTime->second    = lpSystemtime->tm_sec;
     lpMysqlTime->time_type = MYSQL_TIMESTAMP_DATETIME;
     return 1;
}


#define DB_STRING_BUFFER_LENGTH			65535






