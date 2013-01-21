#ifndef PROCEDURE_HEADER_FILE
#define PROCEDURE_HEADER_FILE

class Controls;



enum
{
     COL_LIST_ID = 0,
     COL_LIST_NAME,
     N_COLUMNS
};

enum
{
     COL_LIST_D_ID = 0,
     COL_LIST_D_RESP,
     N_D_COLUMNS
};

class Proc
{
public:
     Proc(){m_pControls = NULL; hSqliteHandle = NULL; nTrIDBase = 0;}
     void Set(Controls* pControls);
     
public:
     G_MODULE_EXPORT static void	doMenuItem_Close(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doMainwin_onExit(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void	doMenuItem_OpenStore(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doInput_Clear(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doRemoveItem(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doInsertItem(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doOpenDetail(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doJob_Selected(GtkWidget* widget,GtkTreePath* path,GtkTreeViewColumn *column, gpointer  userData);



protected:
     Controls*	m_pControls;
     void*	hSqliteHandle;
     int	nTrIDBase;
     
private:
     void	FillJobList();
     
     int 	UpdateTaskDetail(void* lpTaskInfo,int setvalue);    
     int	SendTaskInfo(void* lpTaskInfo);
     int	SendRemoveTask(int nID);

};

class DetailProc
{
public:
     DetailProc(){m_pControls = NULL; hSqliteHandle = NULL;nTaskID = 0;nTrIDBase = 0;}
     void				Set(Controls* pControls);
public:
     /*for detail*/
     G_MODULE_EXPORT static void        doDeleteDetailItem(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doSyncDetailItem(GtkWidget* widget,gpointer userData);
     G_MODULE_EXPORT static void        doResult_Selected(GtkWidget* widget,GtkTreePath* path,GtkTreeViewColumn *column, gpointer  userData);
     G_MODULE_EXPORT static void        doShow(GtkWidget* widget,gpointer userData);
protected:
     Controls*				m_pControls;
public:
     void*				hSqliteHandle;
     int				nTaskID;
     int				nTrIDBase;
private:
     int				UpdateDetail(void* lpRespInfo,int setvalue);
     int				SendSync(int nID,int nTrID,int* pRecvCount,void** lpRecvItems);
     
     
};


#endif
