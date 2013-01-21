#include <cstdlib>
#include <cstdio>
#include <new>
#include <string.h>
#include <gtk/gtk.h>
#include "controls.h"
#include "procedure.h"

ControlEvp::ControlEvp(ppfnWidgetCallBack widgetCB, gpointer userData)
{
     hProc		  = NULL;
     this->widgetCallBack = widgetCB;
     this->userData	  = userData;     
}


 ControlEV::ControlEV(char* pEventName)
 {
      strcpy(szEvent,pEventName);
      evpList = NULL;      
 }

Control::Control(char* pszCtlName,GType ctlType)
{
     strcpy(szName,pszCtlName);
     type   = ctlType;
     widRT  = NULL;
     evList = NULL;
}

int ControlEV::addEvp(ppfnWidgetCallBack widgetCallBack, gpointer userData)
{
// append to evplist
     gpointer	dst = {0};
     ControlEvp* pEvp = NULL;

     dst     = g_malloc(sizeof(ControlEvp));
     pEvp    = new(dst)ControlEvp(widgetCallBack,userData);
     evpList = g_slist_prepend(evpList,pEvp);
     
     return 0;
}


int Control::SetSignal(char* pszEvent, ppfnWidgetCallBack widgetCallBack,  gpointer userData)
{
     ControlEV*	pEv   = NULL;
     gpointer	dst   = {0};
     GSList*	pNode = NULL;
     
	

	
// check evlist pszEvent
     for (pNode = evList; pNode != NULL; pNode = g_slist_next(pNode))
     {
	  pEv				    = static_cast<ControlEV*>(pNode->data);
	  if(strcmp(pEv->szEvent,pszEvent) == 0) break;
	  pEv				    = NULL;	  
     }
	


// if no exist. then create ControlEV and insert into evlist
     if(pEv == NULL)
     {
	  dst	 = g_malloc(sizeof(ControlEV));
	  pEv	 = new(dst)ControlEV(pszEvent);
	  evList = g_slist_prepend(evList,pEv);
	  
     }

     // append event callback
     pEv->addEvp(widgetCallBack,userData);
     return 0;
}

Controls::Controls()
{
     ctlList = NULL;
}

int Controls::RegWidget(GtkBuilder* pBuilder,char* pszCtlName,GType ctlType,Control** lpControl)
{
     Control*	pCtl	= NULL;
     gpointer	dst	= {0};
     GSList*	pNode	= NULL;
     GtkWidget* pWidget = NULL;
     

	
     /*ctllist check pszCtlName*/

     for (pNode = ctlList; pNode != NULL; pNode = g_slist_next(pNode))
     {
	  pCtl				      = static_cast<Control*>(pNode->data);
	  if(strcmp(pCtl->szName,pszCtlName) == 0) break;
	  pCtl				      = NULL;	  
     } 


     // if exist ,return it
     if(pCtl == NULL)
     {
	  // create new control ,and insert into ctllist
	  if(ctlType != 0)
	  {
	       pWidget = gtk_widget_new(ctlType,pszCtlName);
	  }
	  else
	  {
	       pWidget = (GtkWidget*)gtk_builder_get_object(pBuilder,pszCtlName);
	  }
	  if(pWidget == NULL) return 1;
	  
	  dst  = g_malloc(sizeof(Control));
	  pCtl = new(dst)Control(pszCtlName,ctlType);
	  pCtl->widRT = pWidget;
	  ctlList = g_slist_prepend(ctlList,pCtl);	  
     }
    *lpControl = pCtl;
    return 0;
}



int Controls::FetchWidget(char* pszCtlName,GtkWidget** lpWidget)
{
     Control*	pCtl  = NULL;
     GSList*	pNode = NULL;
     
	
     for (pNode = ctlList; pNode != NULL; pNode = g_slist_next(pNode))
     {
	  pCtl				      = static_cast<Control*>(pNode->data);
	  if(strcmp(pCtl->szName,pszCtlName) == 0) break;
	  pCtl				      = NULL;	  
     } 

     if(pCtl != NULL) *lpWidget = pCtl->widRT;
     return 0;
}

int Controls::DoRoute(GtkBuilder* pBuilder,void* hUserData)
{
     Control*		pCtl	 = NULL;
     ControlEV*		pEv	 = NULL;
     ControlEvp*	pEvp	 = NULL;
     int		nRet	 = 0;
     GSList*		pCtlNode = NULL;
     GSList*		pCVNode	 = NULL;
     GSList*		pCVPNode = NULL;

     for (pCtlNode = ctlList; pCtlNode != NULL; pCtlNode = g_slist_next(pCtlNode))
     {
	  pCtl = static_cast<Control*>(pCtlNode->data);
	 
	  
	  if(pCtl->widRT == NULL) 
	  {
	       nRet = 1;
	       break;
	  }
	  for (pCVNode = pCtl->evList; pCVNode != NULL; pCVNode = g_slist_next(pCVNode))
	  {
	       pEv = static_cast<ControlEV*>(pCVNode->data);
	       for (pCVPNode = pEv->evpList; pCVPNode != NULL; pCVPNode = g_slist_next(pCVPNode))
	       {
		    pEvp	= static_cast<ControlEvp*>(pCVPNode->data);
		    pEvp->hProc = hUserData;
		    
		    if(g_signal_connect(GTK_OBJECT(pCtl->widRT),pEv->szEvent,G_CALLBACK(pEvp->widgetCallBack),pEvp) == 0)
		    {
			 nRet = 2;
			 break;
		    }
	       }
	       if(nRet !=0) break;
	  }
	  if(nRet !=0) break;
     } 
     return nRet;
}
