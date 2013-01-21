#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "cfg.h"
#include <getopt.h>



void getoptions(int argc,char** argv,bootCfg* lpCfg)
{
     int	opt   = 0;

     
     struct option longopts[] = { {"app",0,NULL,'p'}
	                         ,{"exit",0,NULL,'x'}
				 ,{"debug",0,NULL,'d'}
				 ,{"wait",1,NULL,'w'}
				  ,{"killsemid",0,NULL,'l'} 
				 ,{0,0,0,0}};
     
     while((opt = getopt_long(argc, argv, "pxldw:", longopts, NULL)) != -1)
     {
	  switch(opt)
	  {
	  case 'p':
	       lpCfg->nApp	   = 1;
	       break;		    
	  case 'x':
	       lpCfg->nExitApp	   = 1;
	       break;
	  case 'd':
	       lpCfg->nEnableDebug = 1;
	       break;
	  case 'w':
	       lpCfg->nWait	   = atoi(optarg);
	       break;
	  case 'l':
	       lpCfg->nkillsemid   = 1;
	       break;
	  }
     }
}
