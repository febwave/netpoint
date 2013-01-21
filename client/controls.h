#ifndef CONTROLS_HEADER_FILE
#define CONTROLS_HEADER_FILE


#include <glib.h>
#include <glib-object.h>

typedef G_MODULE_EXPORT  void (*ppfnWidgetCallBack)();




class ControlEvp
{
public:
     ControlEvp(ppfnWidgetCallBack widgetCB, gpointer userData);
  
private:
     ControlEvp() {}
public:
     ppfnWidgetCallBack widgetCallBack;
     gpointer		userData;     
public:
     void*		hProc;     
};



#define CTL_EVENT_NAME_LENGTH 32

class ControlEV
{
public:
     ControlEV(char* pEventName);
public:
     int	addEvp(ppfnWidgetCallBack widgetCallBack, gpointer userData);
private:
     ControlEV(){}
public:
     char	szEvent[CTL_EVENT_NAME_LENGTH];
     GSList*	evpList;	/*ControlEVP*/
};



#define CTL_NAME_LENGTH 32

class Control
{
public:
     Control(char* pszCtlName,GType ctlType);
public:
     char	szName[CTL_NAME_LENGTH];
     GType	type;
     GtkWidget* widRT;
public:
     GSList*	evList;		/*ControlEV*/
public:
     int	SetSignal(char* pszEvent, ppfnWidgetCallBack widgetCallBack,  gpointer userData);
};

class Controls
{
public:
     virtual int	Initialize(GtkBuilder* pBuilder);
     int		DoRoute(GtkBuilder* pBuilder,void* hUserData);
public:
     Controls();
     virtual ~Controls(){}
public:
     GtkWidget*	mainWindow;
     
protected:
     GSList*	ctlList;		/*Control*/
public:
     int	FetchWidget(char* pszCtlName,GtkWidget** lpWidget);
     int	RegWidget(GtkBuilder* pBuilder,char* pszCtlName,GType ctlType,Control** lpControl);     
};


class DetailControls:public Controls
{
public:
     virtual int	Initialize(GtkBuilder* pBuilder);
public:
     DetailControls();
public:
     virtual ~DetailControls(){}
     
     
};

#endif
