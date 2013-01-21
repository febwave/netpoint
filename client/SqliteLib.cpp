
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <sqlite3.h>
#include <sys/types.h>
#include "SqliteIf.h"
#include "SqliteLib.h"


int AppQueryInterface(/*out*/void** lpAppIf )
{
	if(lpAppIf == NULL) return 0;
	SqliteIf*	pIf    = new SqliteIf();
	if(pIf == NULL) return 0;
	pIf->CreateInstance    = slCreateInstance;
	pIf->ReleaseInstance   = slReleaseInstance;	
	pIf->InitEnv	       = slInitEnv;
	pIf->ReleaseEnv	       = slReleaseEnv;
	pIf->Connect	       = slConnect;
	pIf->Close	       = slClose;
	pIf->CreateTable       = slCreateTable; 
	pIf->PrepareSQL	       = slPrepareSQL;
	pIf->ReleasePrepareObj = slReleasePrepareObj;
	pIf->ExecText	       = slExecText;
	pIf->ExecText2	       = slExecText2;
	pIf->GetCellInt	       = slGetCellInt;
	pIf->GetCellInt64      = slGetCellInt64;
	pIf->GetCellText       = slGetCellText;
	pIf->GetCellDouble     = slGetCellDouble;
	pIf->GetCellBlob       = slGetCellBlob;
	pIf->FormatSLParamData = slFillSimpleSLParamData;
	*lpAppIf	       = pIf;
	
	return 1;
}

 void AppReleaseInterface(void* lpAppIf)
{
	if(lpAppIf != NULL)
	{
		delete (SqliteIf*)lpAppIf;		
	}
}







