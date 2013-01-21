#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <time.h>
#include <cfg.h>

#include "../json/json.h"




int	getCreateReq(Json::Value& current,int nReqID,int nSeq, jobbase** lpjob);
int	getQueryReq(Json::Value& current,int nReqID,int nSeq, jobbase** lpjob);

int createJsonresponse(int nReq,int nSeq,int nResp,Json::Value & result, char** lpResult,unsigned int* nResultSize)
{
     Json::FastWriter	fast_writer;
     std::string	strTarget;
     unsigned int	nSize = 0;
     unsigned int	nData = 0;
     

     result[JSK_REQ_SEQ]       = nSeq;
     result[JSK_REQ_ID]	       = nReq;
     result[JSK_RESPONSE_CODE] = nResp;
        
         

     strTarget = fast_writer.write(result);
     nData     = strTarget.size();
     nSize     = nData + sizeof(int);     
     *lpResult = (char*)malloc(nSize);
     
     bzero(*lpResult,nSize);
     *(int*)(*lpResult) = nSize;
     memcpy(*lpResult+sizeof(int),strTarget.c_str(),nData);
     *nResultSize = nSize;
     
     return 0;
}

int createresponse(int nReq,int nSeq,int nResp,int nJobId, char** lpResult,unsigned int* nResultSize)
{
     Json::Value	root;
     Json::FastWriter	fast_writer;
     std::string	strTarget;
     unsigned int	nSize = 0;
     unsigned int	nData = 0;
     

     root[JSK_REQ_SEQ]	     = nSeq;
     root[JSK_REQ_ID]	     = nReq;
     root[JSK_RESPONSE_CODE] = nResp;
     
     if(nJobId > 0)
     {
	  root[JSK_JOB_ID] = nJobId;
     }
         

     strTarget = fast_writer.write(root);
     nData     = strTarget.size();
     nSize     = nData + sizeof(int);     
     *lpResult = (char*)malloc(nSize);
     
     bzero(*lpResult,nSize);
     *(int*)(*lpResult) = nSize;
     memcpy(*lpResult+sizeof(int),strTarget.c_str(),nData);
     *nResultSize = nSize;
     
     return 0;
}

bool fetchJSKInt(Json::Value& current,const char* lpszKey, int* nResult)
{
     if(current.isMember(lpszKey) == true)
     {
	  if(current[lpszKey].isInt() == true)
	  {
	       *nResult = (int)current[lpszKey].asInt();
	       return true;
	  }
     }
     return false;
}

bool fetchJSKString(Json::Value& current,const char* lpszKey, char** lpszResult)
{
     if(current.isMember(lpszKey) == true)
     {
	  if(current[lpszKey].isString() == true)
	  {
	       *lpszResult = (char*)current[lpszKey].asCString();
	       return true;
	  }
     }
     return false;
}


int getrequest(char* lpBuffer,unsigned int uSize,jobbase** lpjob)
{
     Json::Value	root;
     Json::Reader	reader;
     std::string	strSource;     
     bool		bFetch = false;
     bool		bExec  = false;
     int		nSeq   = 0;
     int		nReqID = 0;
     
     

     strSource.append(lpBuffer,uSize);

     bExec = reader.parse(strSource,root);
     if(bExec == true)
     {
	  bFetch = fetchJSKInt(root,JSK_REQ_ID,&nReqID);
	  if(bFetch == true)
	  {
	       bFetch = fetchJSKInt(root,JSK_REQ_SEQ,&nSeq);	       
	       if(bFetch == true)
	       {
		    if(nReqID == REQ_CREATE)
		    {
 			 return getCreateReq(root,nReqID, nSeq, lpjob);
		    }
		    else if (nReqID == REQ_QUERY)
		    {
			 return getQueryReq(root,nReqID, nSeq, lpjob);
		    }
	       }
	  }
     }
     return 1;
}

bool convertStrtoTm(const char* lpBuffer,struct tm& result)
{
     char       sztime[24] = {0};
     if(strlen(lpBuffer) > 20) return false;
     
     strcpy(sztime,lpBuffer);     
     sscanf(sztime
	    ,"%4d-%2d-%2d %2d:%2d:%2d"
	    , &result.tm_year
	    , &result.tm_mon
	    , &result.tm_mday
	    , &result.tm_hour
	    , &result.tm_min
	    , &result.tm_sec);
     return true;

}

int getCreateReq(Json::Value& current,int nReqID,int nSeq, jobbase** lpjob)
{
     bool		bFetch	     = false;
     char*		pszName	     = NULL;
     char*		pbttime	     = NULL;
     char*		pexptime     = NULL;
     int		ninterval    = 0;
     char*		pszCtx	     = NULL;
     int		nContextSize = 0;
     struct tm		bttime	     = {0};
     struct tm		exptime	     = {0};
     jobCreateReq*	pReq	     = NULL;
     
     
     bFetch = fetchJSKString(current,JSK_JOB_NAME,&pszName);
     if(bFetch == false) return 1;
     bFetch = fetchJSKString(current,JSK_JOB_BEGIN_TIME,&pbttime);
     if(bFetch == false) return 1;
     bFetch = fetchJSKString(current,JSK_JOB_EXPIRED_TIME,&pexptime);
     if(bFetch == false) return 1;
     bFetch = fetchJSKInt(current,JSK_JOB_INTERVAL,&ninterval);
     
     bFetch = fetchJSKString(current,JSK_JOB_CONTEXT, &pszCtx);
     if(bFetch == false) return 1;

     if(strlen(pszName) >= JOB_NAME_LENGTH) return 2;

     bFetch = convertStrtoTm(pbttime,bttime);
     if(bFetch == false) return 2;
     bFetch = convertStrtoTm(pexptime,exptime);
     if(bFetch == false) return 2;

     nContextSize = strlen(pszCtx);
     pReq	  = (jobCreateReq*)malloc(sizeof(jobCreateReq)+nContextSize);
     if(pReq == NULL) return 3;
     
     pReq->base.nReqId	= nReqID;
     pReq->base.nSeq	= nSeq;
     pReq->nInterval	= ninterval;
     strcpy(pReq->szName,pszName);
     pReq->tbegintime	= bttime;
     pReq->texpiredtime = exptime;
     pReq->nContextSize = nContextSize;
     strcpy(pReq->context,pszCtx);
     *lpjob		= (jobbase*)pReq;
     
     
     return 0;
}

int getQueryReq(Json::Value& current,int nReqID,int nSeq, jobbase** lpjob)
{
     bool		bFetch = false;
     int		nJobId = 0;
     int		nTrId  = 0;
     jobQueryReq*	pReq   = NULL;
     
     
     
     bFetch = fetchJSKInt(current,JSK_JOB_ID,&nJobId);
     if(bFetch == false) return 1;

     fetchJSKInt(current,JSK_TRY_ID,&nTrId);
     
     pReq	  = (jobQueryReq*)malloc(sizeof(jobQueryReq));
     if(pReq == NULL) return 3;
     
     pReq->base.nReqId = nReqID;
     pReq->base.nSeq   = nSeq;
     pReq->nId	       = nJobId;
     pReq->nTrId       = nTrId;
     
     
     *lpjob		= (jobbase*)pReq;     
     
     return 0;
}
