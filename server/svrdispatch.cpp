#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "cfg.h"
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <mysql.h>
#include <DBIf.h>
#include "DBLib.h"
#include "DBParamBinding.h"
#include "../json/json.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "JsonValue.h"

#include <signal.h>

void disppyjob(int,siginfo_t*,void*);

int	set_ticker(int n_msecs);

static dbCfg  s_pdbcfg = {0};

int initdispatch(int ninterval,dbCfg*  pdbcfg)
{
     struct sigaction	sg;
     sigset_t		blocked;
     int		nRet = 0;
     


     memcpy((void*)&s_pdbcfg,(void*)pdbcfg,sizeof(dbCfg));
     
     sg.sa_sigaction = disppyjob;
     sg.sa_flags     = SA_NODEFER;
     

     sigemptyset(&blocked);
     sigaddset(&blocked,SIGQUIT);
     sg.sa_mask = blocked;

     nRet = set_ticker(ninterval*1000);
     nRet = sigaction(SIGALRM,&sg,NULL);
     if(nRet == -1)
     {
	  perror("initdispatch");
     }     
     return nRet;	  
}

int set_ticker(int n_msecs)
{
     struct itimerval	new_timeset;
     long		n_sec;
     long		n_usecs;

     n_sec   = n_msecs /1000;
     n_usecs = (n_msecs % 1000)*1000L;

     new_timeset.it_interval.tv_sec  = n_sec;
     new_timeset.it_interval.tv_usec = n_usecs;
     
     new_timeset.it_value.tv_sec  = n_sec;
     new_timeset.it_value.tv_usec = n_usecs;

     return setitimer(ITIMER_REAL,&new_timeset,NULL);
     
     
}

int	connectdb(dbCfg* pdbcfg,void** lpDBHandle,void** lpDBIf);
void	disconnectdb(void* pDBHandle,void* lpDBIf);
int	fetchJobContext(void* pDBHandle,void* lpDBIf,  Json::Value & result);
int	insertJobResult(void* pDBHandle,void* lpDBIf, int tskID,int retCode, int nDataSize,char* lpResp, Json::Value & result);
#define BUFFER_SIZE 1024

void disppyjob(int,siginfo_t*,void*)
{
     void*		pDBHandle    = NULL;
     void*		pDBIf	     = NULL;
     int		nRet	     = 0;
     Json::Value	jFetchJob;
     Json::Value	jFetchItems;
     Json::Value	jFetchItem;
     Json::Value	jResult;
     int		nItemCount   = 0;
     int		tskID	     = 0;
     int		retCode	     = 0;
     const char*        pJobContext  = NULL;
     FILE*		fsJob	     = NULL;
     FILE*		fPipe	     = NULL;
     char		szBuffer[BUFFER_SIZE];
     int		nRead	     = 0;
     int		nBufferIndex = 0;
     
     
     
	  
     
     nRet = connectdb(&s_pdbcfg,&pDBHandle,&pDBIf);
     if(nRet == 0)
     {
	  // fetch python task context
	  nRet = fetchJobContext(pDBHandle, pDBIf,jFetchJob);
	  if(nRet == 0)
	  {
	       // resolve fetchjob
	       if(jFetchJob.isMember(JSK_ARRAY) == true)
	       {
		    if(jFetchJob[JSK_ARRAY].isArray() == true)
		    {
			 nItemCount = jFetchJob[JSK_ARRAY].size();
			 if(nItemCount > 0)
			 {
			      jFetchItems = jFetchJob[JSK_ARRAY];
			      for(int nItemIndex = 0;nItemIndex < nItemCount;nItemIndex++)
			      {	  
				   jFetchItem.clear();
				   tskID       = 0;
				   pJobContext = NULL;
				   jFetchItem  = jFetchItems[nItemIndex];
				   if(jFetchItem.isMember(JSK_JOB_ID) == true)
				   {
					if(jFetchItem[JSK_JOB_ID].isInt() == true)
					{
					     tskID = jFetchItem[JSK_JOB_ID].asInt();
					}					
				   }
				   if(jFetchItem.isMember(JSK_JOB_CONTEXT) == true)
				   {
					if(jFetchItem[JSK_JOB_CONTEXT].isString() == true)
					{
					     pJobContext = jFetchItem[JSK_JOB_CONTEXT].asCString();
					}					
				   }
				   if(tskID <= 0 || pJobContext == NULL ) continue;
				   printf("task id %d. job :%s\n",tskID,pJobContext);

				   // create file
				   fsJob = fopen("dojob.py","w");

				   // write context to file
				   fwrite(pJobContext,sizeof(char),strlen(pJobContext),fsJob);

				   // close file
				   fclose(fsJob);
				   
				   // popen python file-name
				   fPipe = popen("python dojob.py","r"); 
				   		   
				   // if child, do task
				   if(fPipe != NULL)
				   {
					memset(szBuffer,0,sizeof(szBuffer));
					while(1)
					{
					     nRead = fread(szBuffer+nBufferIndex,sizeof(char),BUFFER_SIZE-nBufferIndex,fPipe);
					     if(nRead == 0)
					     {
						  break;
					     }
					     nBufferIndex += nRead;
					}

					// if parent
					// pclose and get process exit code ,and from pipe get execute data
					retCode = pclose(fPipe);
				   }
				   // delete file
				   unlink("dojob.py");
				   
				   // update db
				   insertJobResult(pDBHandle,pDBIf, tskID,retCode, nBufferIndex,szBuffer, jResult);
			      }
			 }
		    }
	       }
	       
	       
	  }
	  // clean resource
	  disconnectdb(pDBHandle,pDBIf);
     }
   
}
