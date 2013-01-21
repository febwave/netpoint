#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "cfg.h"




int main(int argc,char** argv)
{
     NetCfg		nc	    = {0};
     dbCfg		dc	    = {0};
     appsetting		aps	    = {0};
     int		nRet	    = 0;
     void*		pCtxt;
     void*		pXmlDoc;
     bootCfg		btcfg	    = {0};
     pthread_t		svrthreadid = {0};

     
     
     
 
     
     /* get options from args */
     getoptions(argc,argv,&btcfg);
     if(btcfg.nExitApp == 1)
     {
	  printf("exit flag\n");
     }

     if(btcfg.nEnableDebug == 1)
     {
	  printf("debug flag\n");
     }

     printf("wait time %d\n",btcfg.nWait );
     
     /* xml  */
     nRet = initxmlcfg(&pCtxt,&pXmlDoc);
     /* get ip address and port */
     nRet = getservercfg(pXmlDoc,&nc);
     if(nRet > 0)
     {
	  printf("get address %s port is %d \n",nc.szServeraddress,nc.nport);
     }
     /* get database address and account */
     nRet = getdatabasecfg(pXmlDoc,&dc);
 


     if(pCtxt != NULL)
     {
	  freexmlcfg(pCtxt,pXmlDoc);
     }
     
     
     /* create app thread or service support
	create app thread */
     btcfg.nApp = 1;

     if(btcfg.nApp == 1)
     {
	  memcpy(&aps.dbc,&dc,sizeof(dbCfg));
	  memcpy(&aps.nc,&nc,sizeof(NetCfg));
	  nRet = pthread_create(&svrthreadid,NULL,svrthreadproc,&aps);
	  if(nRet != 0)
	  {
	       printf("pthread create failed. retsult is %d\n",nRet);
	       exit(-1);
	  }
     }     

     
     nRet = pthread_join(svrthreadid,NULL);
     if(nRet != 0)
     {
	  printf("thread join failed. retsult is %d\n",errno);
	  exit(-1);
     }
     
     exit(0);
     return 0;
}
