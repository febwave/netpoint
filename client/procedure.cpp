#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include "SqliteIf.h"
#include "SqliteLib.h"
#include "controls.h"
#include "sqliteFunc.h"
#include "procedure.h"
#include "SqliteLib.h"
#include "cfg.h"


#include "../json/json.h"

void Proc::Set(Controls* pControls)
{
     m_pControls = pControls;
}

void Proc::doMenuItem_Close(GtkWidget* widget,gpointer userData)
{
     Proc*		pProc = NULL;
     ControlEvp*	pEvp  = NULL;
     
     
     pEvp = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);
	  
     gtk_widget_destroy(pProc->m_pControls->mainWindow);
}



void Proc::doMenuItem_OpenStore(GtkWidget* widget,gpointer userData)
{
     Proc*		pProc	      = NULL;
     ControlEvp*	pEvp	      = NULL;
     HRETCODE		hRet	      = SQLITE_RETCODE_OK;
     void*		hSqliteHandle = NULL;
     SqliteConfig	scon	      = {0};
     SqliteErr		ser	      = {0};
     SqliteSetting	sset	      = {0};
     


     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);
     
     
     
     hRet = slCreateInstance(&hSqliteHandle);
     if(SL_SUCCEEDED(hRet))
     {
	  strcpy(scon.szFileName,"cpyjob");
	  hRet = slInitEnv(hSqliteHandle,&scon,&ser);
	  if(SL_SUCCEEDED(hRet))
	  {
	       strcpy(sset.szFileName,"cpyjob");
	       hRet = slConnect(hSqliteHandle,&sset,&ser);
	       if(SL_SUCCEEDED(hRet))
	       {
		    pProc->hSqliteHandle = hSqliteHandle;
		    pProc->FillJobList();
	       }
	       else
	       {
	          slReleaseEnv(hSqliteHandle,&ser);
	       }
	  }

	  if(SL_FAILED(hRet))
	  {
	       slReleaseInstance(hSqliteHandle);
	  }	  

     }
}

void Proc::doMainwin_onExit(GtkWidget* widget,gpointer userData)
{
     Proc*		pProc = NULL;
     ControlEvp*	pEvp  = NULL;
     SqliteErr		ser   = {0};
     
     
     pEvp = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);

     if(pProc->hSqliteHandle != NULL)
     {
	  slClose(pProc->hSqliteHandle);
	  slReleaseEnv(pProc->hSqliteHandle,&ser);
	  slReleaseInstance(pProc->hSqliteHandle);	  
     }     
     gtk_main_quit();     
}



int Proc::UpdateTaskDetail(void * lpTaskInfo,int setvalue)
{
     TaskInfo*		pTaskInfo      = NULL;     
     GtkWidget*		pWidget	       = NULL;
     char               szBuffer[4096] = {0};
     int		nBufferSize    = 4096;
     const char*	pszBuffer      = NULL;
     GtkTextIter	startTI;
     GtkTextIter	lastTI;
     
     
     pTaskInfo = static_cast<TaskInfo*>(lpTaskInfo);
     
     m_pControls->FetchWidget("txtID",&pWidget);
     if(pWidget != NULL)
     {
	  if(pTaskInfo == NULL)
	  {
	       gtk_entry_set_text(GTK_ENTRY(pWidget),"0");
	  }
	  else
	  {
	       if(setvalue == 0)
	       {
		    memset(szBuffer,0,nBufferSize);
		    sprintf(szBuffer,"%d",pTaskInfo->nID);
		    gtk_entry_set_text(GTK_ENTRY(pWidget),szBuffer);
	       }
	       else
	       {
		    pszBuffer = gtk_entry_get_text(GTK_ENTRY(pWidget));
		    if(pszBuffer == NULL)
		    {
			 return 1;
		    }
		    pTaskInfo->nID = atoi(pszBuffer);		    
	       }
	  }
     }

     m_pControls->FetchWidget("txtName",&pWidget);
     if(pWidget != NULL)
     {
	  if(pTaskInfo == NULL)
	  {
	       gtk_entry_set_text(GTK_ENTRY(pWidget),"");
	  }
	  else
	  {
	       if(setvalue == 0)
	       {
		    gtk_entry_set_text(GTK_ENTRY(pWidget),pTaskInfo->szName);
	       }
	       else
	       {
		    pszBuffer = gtk_entry_get_text(GTK_ENTRY(pWidget));
		    if(pszBuffer == NULL)
		    {
			 return 1;
		    }
		    sprintf(pTaskInfo->szName,"%s",pszBuffer);
	       }
	  }
     }

     m_pControls->FetchWidget("spInterval",&pWidget);
     if(pWidget != NULL)
     {
	  if(pTaskInfo == NULL)
	  {
	       gtk_spin_button_set_value(GTK_SPIN_BUTTON(pWidget),0);
	  }
	  else
	  {
	       if(setvalue == 0)
	       {
		    memset(szBuffer,0,nBufferSize);
		    sprintf(szBuffer,"%d",pTaskInfo->nInterval);
		    gtk_entry_set_text(GTK_ENTRY(pWidget),szBuffer);
	       }
	       else
	       {
		    pszBuffer = gtk_entry_get_text(GTK_ENTRY(pWidget));
		    if(pszBuffer == NULL)
		    {
			 return 1;
		    }
		    pTaskInfo->nInterval = atoi(pszBuffer);		    
	       }
	       
	  }
     }

     time_t	tt	 = time(NULL);
     struct tm* tmLocal	 = NULL;
     struct tm	tmBuffer = {0};
     
     
     if(pTaskInfo == NULL)
     {
	  tmLocal = localtime((const time_t*)&tt);
     }
     else
     {
	  if(setvalue == 0)
	  {
	       strptime(pTaskInfo->szBeginTime,"%Y-%m-%d %H:%M:%S",&tmBuffer);
	       tmLocal = &tmBuffer;
	  }
	  else
	  {
	       /*  keep tmlocal*/
	       tmLocal = localtime((const time_t*)&tt);
	  }
     }
     
     
     
     m_pControls->FetchWidget("calBegin",&pWidget);
     if(pWidget != NULL)
     {
	  if(setvalue == 0)
	  {
	       gtk_calendar_select_month(GTK_CALENDAR(pWidget),tmLocal->tm_mon ,tmLocal->tm_year + 1900);
	       gtk_calendar_select_day(GTK_CALENDAR(pWidget),tmLocal->tm_mday);
	  }
	  else
	  {
	        gtk_calendar_get_date(GTK_CALENDAR(pWidget)
				      ,(guint*)&(tmBuffer.tm_year)
                                      ,(guint*)&(tmBuffer.tm_mon)
				      ,(guint*)&(tmBuffer.tm_mday));

		sprintf(pTaskInfo->szBeginTime
			,"%04d-%02d-%02d %02d:%02d:%02d"
			,tmBuffer.tm_year
			,tmBuffer.tm_mon+1
			,tmBuffer.tm_mday
			,tmLocal->tm_hour
			,tmLocal->tm_min
			,tmLocal->tm_sec);
	  }
     }

     if(pTaskInfo != NULL)
     {
  	  strptime(pTaskInfo->szExpiredTime,"%Y-%m-%d %H:%M:%S",&tmBuffer);
	  tmLocal = &tmBuffer;
     }

     m_pControls->FetchWidget("calExp",&pWidget);
     if(pWidget != NULL)
     {
	  if(setvalue == 0)
	  {
	       gtk_calendar_select_month(GTK_CALENDAR(pWidget),tmLocal->tm_mon ,tmLocal->tm_year + 1900);
	       gtk_calendar_select_day(GTK_CALENDAR(pWidget),tmLocal->tm_mday);
	  }
	  else
	  {
	       gtk_calendar_get_date(GTK_CALENDAR(pWidget)
				      ,(guint*)&(tmBuffer.tm_year)
                                      ,(guint*)&(tmBuffer.tm_mon)
				      ,(guint*)&(tmBuffer.tm_mday));

		sprintf(pTaskInfo->szExpiredTime
			,"%04d-%02d-%02d 23:59:59"
			,tmBuffer.tm_year
			,tmBuffer.tm_mon+1
			,tmBuffer.tm_mday
			);
	  }
     }

     m_pControls->FetchWidget("rbTask",&pWidget);
     if(pWidget != NULL)
     {
	  GtkTextBuffer*	pTBuffer = NULL;
	  if(setvalue == 0)
	  {
	       pTBuffer = gtk_text_buffer_new(NULL);
	       if(pTaskInfo != NULL && pTaskInfo->nSize > 1)
	       {
		    gtk_text_buffer_set_text(pTBuffer,pTaskInfo->pszTsk,pTaskInfo->nSize-1);		    
	       }
	       gtk_text_view_set_buffer(GTK_TEXT_VIEW(pWidget),pTBuffer);
	       g_object_unref(G_OBJECT(pTBuffer));
	  }
	  else
	  {
	       pTBuffer	= gtk_text_view_get_buffer(GTK_TEXT_VIEW(pWidget));
	       gtk_text_buffer_get_iter_at_offset(pTBuffer,&startTI,0);
	       gtk_text_buffer_get_iter_at_offset(pTBuffer,&lastTI,-1);
	       
	       pszBuffer = gtk_text_buffer_get_text(pTBuffer,&startTI,&lastTI,(gboolean)1);
	       if(pszBuffer != NULL)
	       {
		    pTaskInfo->nSize	= strlen(pszBuffer);
		    if(pTaskInfo->nSize > 0)
		    {		
			 pTaskInfo->pszTsk = (char*)g_malloc(pTaskInfo->nSize +1);
			 strcpy(pTaskInfo->pszTsk,pszBuffer);
		    }
	       }
	       
	  }	  
     }
     return 0;
}


void Proc::doInput_Clear(GtkWidget* widget,gpointer userData)
{
     Proc*		pProc	= NULL;
     ControlEvp*	pEvp	= NULL;
     
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);
     pProc->UpdateTaskDetail(NULL,0);
     
}

void Proc::doJob_Selected(GtkWidget* widget,GtkTreePath* path,GtkTreeViewColumn *column, gpointer  userData)
{
     Proc*		pProc	  = NULL;
     ControlEvp*	pEvp	  = NULL;
     GtkWidget*		pWidget   = NULL;
     GtkTreeSelection*	selection = NULL;
     GtkTreeModel*	model	  = NULL;
     TaskInfo*		pTaskInfo = NULL;     
     GtkTreeIter	iter;
     int		nID	  = 0;
     HRETCODE		hRet	  = SQLITE_RETCODE_OK;
     SqliteErr		ser	  = {0};
     
     
          
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);
     
     pProc->m_pControls->FetchWidget("tvJob",&pWidget);
     
 
     if(pWidget != NULL)
     { 
	  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pWidget));
	  if(selection != NULL)
	  {
	     if (gtk_tree_selection_get_selected(selection, &model, &iter))
	     {
		  gtk_tree_model_get (model, &iter,COL_LIST_ID,&nID,  -1);
		  
		  hRet = npc_get_task_detail(pProc->hSqliteHandle,nID,&pTaskInfo,&ser);
		  if(SL_SUCCEEDED(hRet))
		  {
		       pProc->UpdateTaskDetail(pTaskInfo,0);
		       pProc->nTrIDBase = pTaskInfo->nTrID;
		       g_free(pTaskInfo);
		  }	  
	     }
	  }
     }    
}


void ReleaseListItemBuffer(gpointer data,gpointer user_data)
{
     g_free(data);
}

void ReleaseList(GSList* refList)
{
     if(refList == NULL) return;
     g_slist_foreach(refList,ReleaseListItemBuffer,NULL);
     g_slist_free(refList);
}

void Proc::FillJobList()
{
     HRETCODE		hRet	= 0;
     FetchList		feList	= {0};
     SqliteErr		ser	= {0};
     GSList*		pNode	= NULL;
     FetchTaskHead*	fetch	= NULL;
     GtkTreeIter	iter;
     GtkWidget*		pWidget = NULL;
     GtkListStore*	pJobLS	= NULL;         
     


     m_pControls->FetchWidget("jobliststore",&pWidget);
     if(pWidget != NULL)
     {    
	  pJobLS = GTK_LIST_STORE(pWidget);
     
	  hRet = npc_get_task_list(hSqliteHandle,0,&feList,&ser);
	  if(SL_SUCCEEDED(hRet))
	  {
	       for (pNode = feList.pSLst; pNode != NULL; pNode = g_slist_next(pNode))
	       {
		    fetch = static_cast<FetchTaskHead*>(pNode->data);

		    gtk_list_store_append(pJobLS, &iter);

		    gtk_list_store_set (pJobLS
					, &iter
					,COL_LIST_ID
					, (gint)fetch->nID
					,COL_LIST_NAME
					, fetch->szName
					,-1);
	       }
	       ReleaseList(feList.pSLst);
	  }
     }
}


void Proc::doRemoveItem(GtkWidget* widget,gpointer userData)
{
     Proc*		pProc	 = NULL;
     ControlEvp*	pEvp	 = NULL;
     GtkWidget*		pWidget	 = NULL;
     GtkTreeIter	iter;
     int		nID	 = 0;
     int		nQueryID = 0;     
     char*		pszID	= NULL;     
     HRETCODE		hRet	= SQLITE_RETCODE_OK;
     SqliteErr		ser	= {0};
     GtkTreeModel*      pModel	= NULL;  
     
     
     
          
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);
     
     pProc->m_pControls->FetchWidget("txtID",&pWidget);
     if(pWidget == NULL) return;
     pszID = (char*)gtk_entry_get_text(GTK_ENTRY(pWidget));
     if(pszID == NULL) return;
     nID   = atoi(pszID);
     if(nID == 0) return;

     /* todo remove database  from remote server*/
     
     pProc->m_pControls->FetchWidget("tvJob",&pWidget);  
     if(pWidget == NULL) return;
     pModel					     = gtk_tree_view_get_model(GTK_TREE_VIEW(pWidget));
     if(pModel == NULL) return;
     if(gtk_tree_model_get_iter_first(pModel,&iter) == TRUE)
     {
	  do
	  {
	       gtk_tree_model_get(pModel, &iter,COL_LIST_ID,&nQueryID,  -1);
	       if(nQueryID == nID)
	       {
		    break;
	       }
	       if(gtk_tree_model_iter_next(pModel,&iter) == FALSE)
	       {
		    nID = 0;
		    break;
	       }
	  }while(1);	  
     }

     
     if(nID != 0 && nQueryID != 0)
     {
	  gtk_list_store_remove(GTK_LIST_STORE(pModel),&iter);
     }
   
     hRet = npc_del_task(pProc->hSqliteHandle, nID,&ser);
     if(SL_SUCCEEDED(hRet))
     {
	  pProc->UpdateTaskDetail(NULL,0);	    
     }	
}


void Proc::doInsertItem(GtkWidget* widget,gpointer userData)
{
     Proc*		pProc				   = NULL;
     ControlEvp*	pEvp				   = NULL;
     GtkWidget*		pWidget				   = NULL;
     GtkListStore*	pJobLS				   = NULL;	  
     GtkTreeIter	iter;
     HRETCODE		hRet				   = SQLITE_RETCODE_OK;
     SqliteErr		ser				   = {0};
     TaskInfo		taskInfo			   = {0};
     
         
          
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);
          
     // get ui data
     if(pProc->UpdateTaskDetail(&taskInfo,1) == 1)
     {
	  return;
     }
     if(taskInfo.nID ==  0)
     {
	  /* insert into database  from remote server and get nID*/
	  if(pProc->SendTaskInfo(&taskInfo) == 0)
	  {
	       // insert local
	       if(taskInfo.nID > 0)
	       {
		    hRet = npc_insert_task(pProc->hSqliteHandle
					   ,taskInfo.nID
					   ,taskInfo.szName
					   ,taskInfo.szBeginTime
					   ,taskInfo.szExpiredTime
					   ,taskInfo.nInterval
					   ,taskInfo.nSize
					   ,taskInfo.pszTsk
					   , &ser);
		    if(SL_SUCCEEDED(hRet))
		    {
			 // update ui list
			 pProc->m_pControls->FetchWidget("jobliststore",&pWidget);
			 if(pWidget != NULL)
			 {    
			      pJobLS = GTK_LIST_STORE(pWidget);
			      gtk_list_store_append(pJobLS, &iter);
			      gtk_list_store_set (pJobLS
						  , &iter
						  ,COL_LIST_ID
						  , (gint)taskInfo.nID
						  ,COL_LIST_NAME
						  , taskInfo.szName
						  ,-1);       
			 }
			 // refresh ui
			 pProc->UpdateTaskDetail(NULL,0);
		    }
	       }
	  }
     }
     if(taskInfo.pszTsk != NULL)
     {
	  g_free(taskInfo.pszTsk);
     }
}

int	netReq(char* lpszNetAddress,int nPort,size_t nSize,char* sndBuffer,size_t* pRespSize,char** pRecvBuffer);
void	releaseNetBuffer(char* pBuffer);

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

bool fetchJSKAr(Json::Value& current,const char* lpszKey, Json::Value& arResult)
{
     if(current.isMember(lpszKey) == true)
     {
	  if(current[lpszKey].isArray() == true)
	  {
	       arResult = current[lpszKey];
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

int Proc::SendTaskInfo(void* lpTaskInfo)
{
     int	nRet	    = 0;
     NetCfg	nc	    = {0};
     void*	pCtxt	    = NULL;
     void*	pXmlDoc	    = NULL;
     int	nSocket	    = 0;
     TaskInfo*	pTaskInfo   = NULL;
     size_t	nSize	    = 0;
     char*	sndBuffer   = NULL;
     size_t	nRespSize   = 0;
     char*	pRecvBuffer = NULL;
     
     

     pTaskInfo = static_cast<TaskInfo*>(lpTaskInfo);
     
     nRet = initxmlcfg(&pCtxt,&pXmlDoc);
     /* get ip address and port */
     nRet  = getservercfg(pXmlDoc,&nc);

     if(pCtxt != NULL)
     {
	  freexmlcfg(pCtxt,pXmlDoc);
     }

     // json
     Json::FastWriter	fast_writer;
     Json::Reader	fast_reader;
     Json::Value	result;
     Json::Value	resp;
     std::string	strSource;
     std::string	strTarget;
     unsigned int	nData = 0;
     bool		bFetch = false;
     bool		bExec  = false;
     

     result[JSK_REQ_SEQ]	  = 1;
     result[JSK_REQ_ID]		  = 1;
     result[JSK_JOB_NAME]	  = pTaskInfo->szName;
     result[JSK_JOB_BEGIN_TIME]	  = pTaskInfo->szBeginTime;
     result[JSK_JOB_EXPIRED_TIME] = pTaskInfo->szExpiredTime;
     result[JSK_JOB_INTERVAL]	  = pTaskInfo->nInterval;
     result[JSK_JOB_CONTEXT]	  = pTaskInfo->pszTsk;
     

        
         

     strTarget = fast_writer.write(result);
     nData     = strTarget.size();
     nSize     = nData;     
     sndBuffer = (char*)g_malloc(nSize);
     
     memset(sndBuffer,0,nSize);
     memcpy(sndBuffer,strTarget.c_str(),nData);
     
     nRet = netReq(nc.szServeraddress,nc.nport, nSize, sndBuffer,&nRespSize,&pRecvBuffer);
     if(nRet == 0)
     {
	  strSource.append(pRecvBuffer,nRespSize);
	  bExec = fast_reader.parse(strSource,resp);
	  if(bExec)
	  {
	       fetchJSKInt(resp,JSK_JOB_ID,&(pTaskInfo->nID));
	  }
	  
	  releaseNetBuffer(pRecvBuffer);  
     }
     g_free(sndBuffer);     
     return nRet;
}


void   Proc::doOpenDetail(GtkWidget* widget,gpointer userData)
{
     GtkBuilder		*gtkBuilder;
     DetailControls*	 pDtControls = NULL;
     Proc*		 pProc	     = NULL;
     DetailProc*	 pNewProc    = NULL;
     ControlEvp*	 pEvp	     = NULL;
     GtkWidget*		 pWidget     = NULL;
     const char*	 pszID	     = NULL;
     int		 nID	     = 0;
     
     
     
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<Proc*>(pEvp->hProc);

     pProc->m_pControls->FetchWidget("txtID",&pWidget);
     if(pWidget == NULL) return;
     pszID = gtk_entry_get_text(GTK_ENTRY(pWidget));
     if(pszID == NULL) return;
     nID   = atoi(pszID);
     if(nID == 0) return;

     
     pDtControls	     = new DetailControls();
     pNewProc		     = new DetailProc();
     pNewProc->hSqliteHandle = pProc->hSqliteHandle;
     pNewProc->nTaskID	     = nID;
     pNewProc->nTrIDBase     = pProc->nTrIDBase;
     

     
     gtkBuilder	= gtk_builder_new();
     
     gtk_builder_add_from_file(gtkBuilder,"resultDetail.glade",NULL);
     if(pDtControls->Initialize(gtkBuilder) == 0)
     {
	  pDtControls->DoRoute(gtkBuilder, (void*)pNewProc);
	  pNewProc->Set(pDtControls);  
	  gtk_dialog_run(GTK_DIALOG(pDtControls->mainWindow));
	  gtk_widget_destroy(pDtControls->mainWindow);
     }
     g_object_unref ( G_OBJECT(gtkBuilder));
     delete pDtControls;
     delete pNewProc;
    
     
}

void DetailProc::Set(Controls* pControls)
{
     m_pControls = pControls;
}

void DetailProc::doShow(GtkWidget* widget,gpointer userData)
{
     DetailProc*	pProc	  = NULL;
     ControlEvp*	pEvp	  = NULL;
     GtkWidget*		pWidget   = NULL;
     GtkTreeSelection*	selection = NULL;
     GtkTreeModel*	model	  = NULL;
     FetchRespHead*	pRespHead = NULL;
     GSList*		pNode	  = NULL;      
     GtkTreeIter	iter;
     int		nID	  = 0;
     HRETCODE		hRet	  = SQLITE_RETCODE_OK;
     SqliteErr		ser	  = {0};
     //FetchRespListCfg	feList	  = {0};
     GtkListStore*	pResultLS = NULL;
     FetchList		feList	  = {0};
     
     
     
     
        
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<DetailProc*>(pEvp->hProc);

     pProc->UpdateDetail(NULL,0);
     pProc->m_pControls->FetchWidget("ltsResult",&pWidget);
     if(pWidget != NULL)
     {    
	  pResultLS = GTK_LIST_STORE(pWidget);
     
	  hRet = npc_get_resp_list(pProc->hSqliteHandle,pProc->nTaskID, 0,&feList,&ser);
	  if(SL_SUCCEEDED(hRet))
	  {
	       for (pNode = feList.pSLst; pNode != NULL; pNode = g_slist_next(pNode))
	       {
		    pRespHead = static_cast<FetchRespHead*>(pNode->data);

		    gtk_list_store_append(pResultLS, &iter);

		    gtk_list_store_set (pResultLS
					, &iter
					,COL_LIST_D_ID
					, (gint)pRespHead->nTrID
					,COL_LIST_D_RESP
					, (gint)pRespHead->nResult
					,-1);
	       }
	       ReleaseList(feList.pSLst);
	  }
     }
     
}

int DetailProc::UpdateDetail(void* lpRespInfo,int setvalue)
{
     
     RespInfo*		pRespInfo      = NULL;     
     GtkWidget*		pWidget	       = NULL;
     char               szBuffer[4096] = {0};
     int		nBufferSize    = 4096;
     const char*	pszBuffer      = NULL;
     GtkTextIter	startTI;
     GtkTextIter	lastTI;
     
     
     pRespInfo = static_cast<RespInfo*>(lpRespInfo);
     
     m_pControls->FetchWidget("txtTrID",&pWidget);
     if(pWidget != NULL)
     {
	  if(pRespInfo == NULL)
	  {
	       gtk_entry_set_text(GTK_ENTRY(pWidget),"0");
	  }
	  else
	  {
	       if(setvalue == 0)
	       {
		    memset(szBuffer,0,nBufferSize);
		    sprintf(szBuffer,"%d",pRespInfo->nID);
		    gtk_entry_set_text(GTK_ENTRY(pWidget),szBuffer);
	       }
	       else
	       {
		    pszBuffer = gtk_entry_get_text(GTK_ENTRY(pWidget));
		    if(pszBuffer == NULL)
		    {
			 return 1;
		    }
		    pRespInfo->nID = atoi(pszBuffer);		    
	       }
	  }
     }
     m_pControls->FetchWidget("txtResp",&pWidget);
     if(pWidget != NULL)
     {
	  if(pRespInfo == NULL)
	  {
	       gtk_entry_set_text(GTK_ENTRY(pWidget),"0");
	  }
	  else
	  {
	       if(setvalue == 0)
	       {
		    memset(szBuffer,0,nBufferSize);
		    sprintf(szBuffer,"%d",pRespInfo->nResult);
		    gtk_entry_set_text(GTK_ENTRY(pWidget),szBuffer);
	       }
	       else
	       {
		    pszBuffer = gtk_entry_get_text(GTK_ENTRY(pWidget));
		    if(pszBuffer == NULL)
		    {
			 return 1;
		    }
		    pRespInfo->nResult = atoi(pszBuffer);		    
	       }
	  }
     }
     m_pControls->FetchWidget("txtResult",&pWidget);
     if(pWidget != NULL)
     {
	  GtkTextBuffer*	pTBuffer = NULL;
	  if(setvalue == 0)
	  {
	       pTBuffer = gtk_text_buffer_new(NULL);
	       if(pRespInfo != NULL && pRespInfo->nSize > 1)
	       {
		    gtk_text_buffer_set_text(pTBuffer,pRespInfo->pszData,pRespInfo->nSize-1);		    
	       }
	       gtk_text_view_set_buffer(GTK_TEXT_VIEW(pWidget),pTBuffer);
	       g_object_unref(G_OBJECT(pTBuffer));
	  }
	  else
	  {
	       pTBuffer	= gtk_text_view_get_buffer(GTK_TEXT_VIEW(pWidget));
	       gtk_text_buffer_get_iter_at_offset(pTBuffer,&startTI,0);
	       gtk_text_buffer_get_iter_at_offset(pTBuffer,&lastTI,-1);
	       
	       pszBuffer = gtk_text_buffer_get_text(pTBuffer,&startTI,&lastTI,(gboolean)1);
	       if(pszBuffer != NULL)
	       {
		    pRespInfo->nSize	= strlen(pszBuffer);
		    if(pRespInfo->nSize > 0)
		    {		
			 pRespInfo->pszData = (char*)g_malloc(pRespInfo->nSize +1);
			 strcpy(pRespInfo->pszData,pszBuffer);
		    }
	       }
	       
	  }	  
     }
     return 0;
}

void DetailProc::doResult_Selected(GtkWidget* widget,GtkTreePath* path,GtkTreeViewColumn *column, gpointer  userData)
{
     DetailProc*	pProc	  = NULL;
     ControlEvp*	pEvp	  = NULL;
     GtkWidget*		pWidget   = NULL;
     GtkTreeSelection*	selection = NULL;
     GtkTreeModel*	model	  = NULL;
     RespInfo*		pRespInfo = NULL;     
     GtkTreeIter	iter;
     int		nTrID	  = 0;     
     HRETCODE		hRet	  = SQLITE_RETCODE_OK;
     SqliteErr		ser	  = {0};
     
     
         
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<DetailProc*>(pEvp->hProc);
     
     pProc->m_pControls->FetchWidget("tvResult",&pWidget);
     
 
     if(pWidget != NULL)
     { 
	  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pWidget));
	  if(selection != NULL)
	  {
	     if (gtk_tree_selection_get_selected(selection, &model, &iter))
	     {
		  gtk_tree_model_get (model, &iter,COL_LIST_D_ID,&nTrID,  -1);

		  hRet = npc_get_resp_detail(pProc->hSqliteHandle,pProc->nTaskID, nTrID,&pRespInfo,&ser);
		  if(SL_SUCCEEDED(hRet))
		  {
		       pProc->UpdateDetail(pRespInfo,0);
		       if(pRespInfo != NULL)
		       {
			    g_free(pRespInfo->pszData);
			    g_free(pRespInfo);
		       }
		  }
	     }
	  }
     }
}

void DetailProc::doDeleteDetailItem(GtkWidget* widget,gpointer userData)
{
     DetailProc*	pProc	 = NULL;
     ControlEvp*	pEvp	 = NULL;
     GtkWidget*		pWidget	 = NULL;
     GtkListStore*	pJobLS	 = NULL;	  
     GtkTreeIter	iter;
     HRETCODE		hRet	 = SQLITE_RETCODE_OK;
     SqliteErr		ser	 = {0};

     const char*	pszID	 = NULL;
     int		nQueryID = 0;
     int		nID	 = 0;
     GtkTreeModel*      pModel	 = NULL;
     
     
         
          
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<DetailProc*>(pEvp->hProc);

     pProc->m_pControls->FetchWidget("txtTrID",&pWidget);
     if(pWidget == NULL) return;
     pszID = (char*)gtk_entry_get_text(GTK_ENTRY(pWidget));
     if(pszID == NULL) return;
     nID   = atoi(pszID);
     if(nID == 0) return;

     
     
     pProc->m_pControls->FetchWidget("tvResult",&pWidget);  
     if(pWidget == NULL) return;
     pModel					     = gtk_tree_view_get_model(GTK_TREE_VIEW(pWidget));
     if(pModel == NULL) return;
     if(gtk_tree_model_get_iter_first(pModel,&iter) == TRUE)
     {
	  do
	  {
	       gtk_tree_model_get(pModel, &iter,COL_LIST_D_ID,&nQueryID,  -1);
	       if(nQueryID == nID)
	       {
		    break;
	       }
	       if(gtk_tree_model_iter_next(pModel,&iter) == FALSE)
	       {
		    nID = 0;
		    break;
	       }
	  }while(1);	  
     }

     
     if(nID != 0 && nQueryID != 0)
     {
	  gtk_list_store_remove(GTK_LIST_STORE(pModel),&iter);
     }
   
     hRet = npc_del_resp(pProc->hSqliteHandle,pProc->nTaskID,nID,&ser);
     if(SL_SUCCEEDED(hRet))
     {
	  pProc->UpdateDetail(NULL,0);	    
     }	
           
}

void DetailProc::doSyncDetailItem(GtkWidget* widget,gpointer userData)
{
     DetailProc*	pProc	   = NULL;
     ControlEvp*	pEvp	   = NULL;
     GtkWidget*		pWidget	   = NULL;
     GtkListStore*	pJobLS	   = NULL;	  
     GtkTreeIter	iter;
     HRETCODE		hRet	   = SQLITE_RETCODE_OK;
     SqliteErr		ser	   = {0};
     TaskInfo		taskInfo   = {0};
     int		nRecvCount = 0;
     RespInfo*		pRespInfo  = NULL;
     GtkListStore*	pResultLS  = NULL;
     
     
          
     pEvp  = reinterpret_cast<ControlEvp*>(userData);
     pProc = static_cast<DetailProc*>(pEvp->hProc);
     
     if(pProc->SendSync(pProc->nTaskID,pProc->nTrIDBase,&nRecvCount, (void**)&pRespInfo) == 0)
     {
	  pProc->m_pControls->FetchWidget("ltsResult",&pWidget);
	  if(pWidget != NULL)
	  {    
	       pResultLS = GTK_LIST_STORE(pWidget);
	       for(int nRecvIndex = 0;nRecvIndex < nRecvCount;nRecvIndex++)
	       {
		    hRet = npc_insert_response(pProc->hSqliteHandle
					   , pProc->nTaskID
					   ,pRespInfo[nRecvIndex].nTrID
					   ,pRespInfo[nRecvIndex].nResult
					   ,0
					   ,pRespInfo[nRecvIndex].pszData
					   ,&ser);
		    if(SL_FAILED(hRet)) continue;

		    
		    gtk_list_store_append(pResultLS, &iter);

		    gtk_list_store_set (pResultLS
					, &iter
					,COL_LIST_D_ID
					, (gint)pRespInfo[nRecvIndex].nTrID
					,COL_LIST_D_RESP
					, (gint)pRespInfo[nRecvIndex].nResult
					,-1);

		    if(	 pProc->nTrIDBase < pRespInfo[nRecvIndex].nTrID)
		    {
			 hRet = npc_update_task_trid(pProc->hSqliteHandle, pProc->nTaskID,pRespInfo[nRecvIndex].nTrID,&ser);
			 if(SL_SUCCEEDED(hRet))
			 {
			      pProc->nTrIDBase = pRespInfo[nRecvIndex].nTrID;
			 }
		    }
		    
	       }
	  }
	  if(pRespInfo != NULL)
	  {
     	       for(int nRecvIndex = 0;nRecvIndex < nRecvCount;nRecvIndex++)
	       {
		    g_free(pRespInfo[nRecvIndex].pszData);
	       }
	       g_free(pRespInfo);
	  }
     }          
}


int DetailProc::SendSync(int nID,int nTrID,int* pRecvCount,void** lpRecvItems)
{

     int	nRet	    = 0;
     NetCfg	nc	    = {0};
     void*	pCtxt	    = NULL;
     void*	pXmlDoc	    = NULL;
     int	nSocket	    = 0;     
     size_t	nSize	    = 0;
     char*	sndBuffer   = NULL;
     size_t	nRespSize   = 0;
     char*	pRecvBuffer = NULL;
     RespInfo*	pRespInfo   = NULL;
     
     

    
     
     nRet = initxmlcfg(&pCtxt,&pXmlDoc);
     /* get ip address and port */
     nRet  = getservercfg(pXmlDoc,&nc);

     if(pCtxt != NULL)
     {
	  freexmlcfg(pCtxt,pXmlDoc);
     }

     // json
     Json::FastWriter	fast_writer;
     Json::Reader	fast_reader;
     Json::Value	result;
     Json::Value	resp;
     std::string	strSource;
     std::string	strTarget;
     unsigned int	nData  = 0;
     bool		bFetch = false;
     bool		bExec  = false;
     Json::Value	arResult;
     char*		pszBuffer;
     

     result[JSK_REQ_SEQ] = 1;
     result[JSK_REQ_ID]	 = 2;
     result[JSK_JOB_ID]  = nID;
     result[JSK_TRY_ID]	 = nTrID;
            
         

     strTarget = fast_writer.write(result);
     nData     = strTarget.size();
     nSize     = nData;     
     sndBuffer = (char*)g_malloc(nSize);
     
     memset(sndBuffer,0,nSize);
     memcpy(sndBuffer,strTarget.c_str(),nData);
     
     nRet = netReq(nc.szServeraddress,nc.nport, nSize, sndBuffer,&nRespSize,&pRecvBuffer);
     if(nRet == 0)
     {
	  strSource.append(pRecvBuffer,nRespSize);
	  bExec = fast_reader.parse(strSource,resp);
	  if(bExec)
	  {
	       if(fetchJSKAr(resp,JSK_ARRAY, arResult) == true)
	       {
		    int nSize = arResult.size();
		    if(nSize > 0)
		    {
			 pRespInfo = (RespInfo*)g_malloc(sizeof(RespInfo)*nSize);
			 memset(pRespInfo,0,sizeof(RespInfo)*nSize);
			 for(int nIndex = 0; nIndex < nSize; nIndex++)
			 {
			      pszBuffer = NULL;
			      fetchJSKInt(arResult[nIndex],JSK_JOB_EXECUTE_CODE,&(pRespInfo[nIndex].nTrID));
			      fetchJSKInt(arResult[nIndex],JSK_JOB_RESULT,&(pRespInfo[nIndex].nResult));
			      fetchJSKString(arResult[nIndex],JSK_JOB_RESPONSE_DATA,&pszBuffer);
			      if(pszBuffer != NULL)
			      {
				   pRespInfo[nIndex].nSize	= strlen(pszBuffer);
				   if(pRespInfo[nIndex].nSize > 0)
				   {		
					pRespInfo[nIndex].pszData = (char*)g_malloc(pRespInfo->nSize +1);
					strcpy(pRespInfo[nIndex].pszData,pszBuffer);
				   }
			      }
			 }
			 *pRecvCount  = nSize;
			 *lpRecvItems = pRespInfo;
			 
		    }
	       }
	       
	  }
	  
	  releaseNetBuffer(pRecvBuffer);  
     }
     g_free(sndBuffer);     
     return nRet;    
}
