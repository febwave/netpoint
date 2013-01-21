
#include <gtk/gtk.h>
#include <time.h>
#include "controls.h"
#include "procedure.h"


int SimpleRegWidget(Controls* pCtls,GtkBuilder* pBuilder,char* pszCtlName,GType type, char* pszEvName,ppfnWidgetCallBack pfnWidgetCallBack,void* pUserData)
{
     Control*	pCtl = NULL;
     int	nRet = 0;
    
     nRet = pCtls->RegWidget(pBuilder, pszCtlName,type,&pCtl);
     if(nRet != 0) return nRet;

     
     nRet = pCtl->SetSignal(pszEvName,pfnWidgetCallBack,pUserData);
     if(nRet != 0) return nRet;

     return nRet;
}


int Controls::Initialize(GtkBuilder* pBuilder)
{
     Control*		 pCtl	       = NULL;
     GtkListStore*	 pLStore       = NULL;
     GtkTreeViewColumn	*column	       = NULL;
     GtkCellRenderer	*renderer_text = NULL;
     int		 nRet	       = 0;
     
     mainWindow = GTK_WIDGET(gtk_builder_get_object(pBuilder,"mainWin"));
     if(mainWindow == NULL) return 1;

     nRet = SimpleRegWidget(this,pBuilder, "mainWin",0,(char*)"destroy",(GCallback)Proc::doMainwin_onExit,NULL);
     if(nRet != 0) return nRet;

     nRet = SimpleRegWidget(this,pBuilder, "miClose",0,(char*)"activate",(GCallback)Proc::doMenuItem_Close,NULL);
     if(nRet != 0) return nRet;

     nRet = SimpleRegWidget(this,pBuilder, "miOpen",0,(char*)"activate",(GCallback)Proc::doMenuItem_OpenStore,NULL);
     if(nRet != 0) return nRet;

     nRet = SimpleRegWidget(this,pBuilder, "btnClear",0,(char*)"clicked",(GCallback)Proc::doInput_Clear,NULL);
     if(nRet != 0) return nRet;

     nRet = SimpleRegWidget(this,pBuilder, "btnRemove",0,(char*)"clicked",(GCallback)Proc::doRemoveItem,NULL);
     if(nRet != 0) return nRet;

     nRet = SimpleRegWidget(this,pBuilder, "btnAdd",0,(char*)"clicked",(GCallback)Proc::doInsertItem,NULL);
     if(nRet != 0) return nRet;

     
     nRet = SimpleRegWidget(this,pBuilder, "btnResult",0,(char*)"clicked",(GCallback)Proc::doOpenDetail,NULL);
     if(nRet != 0) return nRet;

     

     nRet = RegWidget(pBuilder,"txtID",0,&pCtl);
     if(nRet != 0) return nRet;
     
     gtk_editable_set_editable(GTK_EDITABLE(pCtl->widRT),0);
     gtk_entry_set_text(GTK_ENTRY(pCtl->widRT),"0");

     nRet = RegWidget(pBuilder,"txtName",0,&pCtl);
     if(nRet != 0) return nRet;

     nRet = RegWidget(pBuilder,"spInterval",0,&pCtl);
     if(nRet != 0) return nRet;
     gtk_spin_button_set_range(GTK_SPIN_BUTTON(pCtl->widRT),1,100);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(pCtl->widRT),1);
     gtk_spin_button_set_increments(GTK_SPIN_BUTTON(pCtl->widRT),1,5);
   
     nRet = RegWidget(pBuilder,"rbTask",0,&pCtl);
     if(nRet != 0) return nRet;
     
     
     nRet = RegWidget(pBuilder,"calBegin",0,&pCtl);
     if(nRet != 0) return nRet;

     time_t tt = time(NULL);
     struct tm* tmLocal = localtime((const time_t*)&tt);
     
     gtk_calendar_select_month(GTK_CALENDAR(pCtl->widRT),tmLocal->tm_mon ,tmLocal->tm_year + 1900);
     gtk_calendar_select_day(GTK_CALENDAR(pCtl->widRT),tmLocal->tm_mday);
     
     

     nRet = RegWidget(pBuilder,"calExp",0,&pCtl);
     if(nRet != 0) return nRet;
     gtk_calendar_select_month(GTK_CALENDAR(pCtl->widRT),tmLocal->tm_mon ,tmLocal->tm_year + 1900);
     gtk_calendar_select_day(GTK_CALENDAR(pCtl->widRT),tmLocal->tm_mday);


     nRet = RegWidget(pBuilder,"jobliststore",0,&pCtl);
     if(nRet != 0) return nRet;

    


     nRet = RegWidget(pBuilder,"tvJob",0,&pCtl);
     if(nRet != 0) return nRet;    

     renderer_text = gtk_cell_renderer_text_new();
     column	   = gtk_tree_view_column_new_with_attributes("job ID", renderer_text, "text",COL_LIST_ID,NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(pCtl->widRT), column);


     column	   = gtk_tree_view_column_new_with_attributes("job Name", renderer_text, "text",COL_LIST_NAME,NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(pCtl->widRT), column);

     nRet = pCtl->SetSignal("row-activated",(GCallback)(Proc::doJob_Selected),NULL);
     if(nRet != 0) return nRet;     

     
     return nRet;	  
}


DetailControls::DetailControls()
{
     Controls();
  
}
     

int DetailControls::Initialize(GtkBuilder* pBuilder)
{
     Control*		 pCtl	       = NULL;
     GtkListStore*	 pLStore       = NULL;
     GtkTreeViewColumn	*column	       = NULL;
     GtkCellRenderer	*renderer_text = NULL;
     int		 nRet	       = 0;
     
     mainWindow = GTK_WIDGET(gtk_builder_get_object(pBuilder,"detailwindow"));
     if(mainWindow == NULL) return 1;


     nRet = SimpleRegWidget(this,pBuilder, "detailwindow",0,(char*)"show",(GCallback)DetailProc::doShow,NULL);
     if(nRet != 0) return nRet;
     
     nRet = SimpleRegWidget(this,pBuilder, "btnDel",0,(char*)"clicked",(GCallback)DetailProc::doDeleteDetailItem,NULL);
     if(nRet != 0) return nRet;

     nRet = SimpleRegWidget(this,pBuilder, "btnSync",0,(char*)"clicked",(GCallback)DetailProc::doSyncDetailItem,NULL);
     if(nRet != 0) return nRet;

     nRet = RegWidget(pBuilder,"ltsResult",0,&pCtl);
     if(nRet != 0) return nRet;

     nRet = RegWidget(pBuilder,"txtTrID",0,&pCtl);
     if(nRet != 0) return nRet;
     
     gtk_editable_set_editable(GTK_EDITABLE(pCtl->widRT),0);
     gtk_entry_set_text(GTK_ENTRY(pCtl->widRT),"0");

     nRet = RegWidget(pBuilder,"txtResp",0,&pCtl);
     if(nRet != 0) return nRet;

     nRet = RegWidget(pBuilder,"txtResult",0,&pCtl);
     if(nRet != 0) return nRet;
     
     nRet = RegWidget(pBuilder,"tvResult",0,&pCtl);
     if(nRet != 0) return nRet;    

     renderer_text = gtk_cell_renderer_text_new();
     column	   = gtk_tree_view_column_new_with_attributes("result trid", renderer_text, "text",COL_LIST_D_ID,NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(pCtl->widRT), column);


     column	   = gtk_tree_view_column_new_with_attributes("result resp", renderer_text, "text",COL_LIST_D_RESP,NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(pCtl->widRT), column);

     nRet = pCtl->SetSignal("row-activated",(GCallback)(DetailProc::doResult_Selected),NULL);
     if(nRet != 0) return nRet;     

     
     
     return 0;
}
