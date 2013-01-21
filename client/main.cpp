#include <gtk/gtk.h>
#include "controls.h"
#include "procedure.h"






int main (int argc, char **argv)
{ 
     GtkBuilder	*gtkBuilder;
     Controls*	 pControls = NULL;
     Proc*	 pProc	   = NULL;
     
     
     
     pControls = new Controls();
     pProc     = new Proc();

     
     
     
     gtk_set_locale();
     /* Initialize the widget set */
     gtk_init (&argc, &argv); 
     /* Create the main window */


     gtkBuilder= gtk_builder_new(); 
     gtk_builder_add_from_file(gtkBuilder,"npMain.glade",NULL);

     if(pControls->Initialize(gtkBuilder) == 0)
     {
	  pControls->DoRoute(gtkBuilder, (void*)pProc);
	  pProc->Set(pControls);  
	  gtk_widget_show_all (pControls->mainWindow);
	  gtk_main ();
	
     }
     g_object_unref ( G_OBJECT(gtkBuilder) );
     delete pControls;
     delete pProc;
     return 0;
}
