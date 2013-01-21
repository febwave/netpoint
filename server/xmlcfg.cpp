#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <time.h>
#include "cfg.h"

#define BUFFERSIZE 1024 * 1024 * 3
#define READALINE 1024 * 1024 * 1



int initxmlcfg(void** lpCtxt,void** lpxmlDoc)
{
    
    xmlParserCtxtPtr	 ctxt;
    xmlDocPtr		 doc;
    char		*file = "npconf.xml";
        
    ctxt = xmlNewParserCtxt();
      
    doc = xmlCtxtReadFile(ctxt, file, "GBK", XML_PARSE_DTDATTR|XML_PARSE_NOERROR);
    if (doc == NULL)
    {
        printf("Can't parse the content: %s\n", file);
        return 0;
    }
    *lpCtxt   = ctxt;
    *lpxmlDoc = doc;
    return 1;    
}

int getservercfg(void* lpxmlDoc,NetCfg* lpcfg)
{
    xmlDocPtr		 pxmlDoc;
    xmlNodePtr		 cur;
    xmlNodePtr		 curpath;
    xmlXPathContextPtr	 context = NULL;
    xmlXPathObjectPtr	 result	 = NULL;
    xmlNodeSetPtr	 nodeset;
    xmlAttrPtr		 attrPtr;
    xmlChar		*key;
    xmlChar		*xpath	 = BAD_CAST("/application/server");
    
    
    pxmlDoc = (xmlDocPtr)lpxmlDoc;
    
    cur = xmlDocGetRootElement(pxmlDoc);
    if (cur == NULL)
    {
        printf("Can't get the root element\n");
        return 0;
    }

    context = xmlXPathNewContext(pxmlDoc);
    result  = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);           
    if (result != NULL)
    {
	 if (xmlXPathNodeSetIsEmpty(result->nodesetval) == 0)
	 {
	      nodeset = result->nodesetval;
	      if(nodeset->nodeNr > 0)
	      {
		   curpath = nodeset->nodeTab[0];
		   if(curpath != NULL)
		   {
			attrPtr = curpath->properties;
			while (attrPtr != NULL)
			{
			    if (!xmlStrcmp(attrPtr->name, BAD_CAST "ip"))
			    {
				 key = xmlGetProp(curpath,(const xmlChar *)"ip");
				 printf("ip in xpath: %s\n", key);
				 strcpy(lpcfg->szServeraddress,(char*)key);
				 xmlFree(key);
			    }
			    else if(xmlStrcmp(attrPtr->name,BAD_CAST"port") == 0)
			    {
				 key = xmlGetProp(curpath,(const xmlChar *)"port");
				 printf("port in xpath: %s\n", key);
				 lpcfg->nport = atoi((char*)key);
				 xmlFree(key);
			    }
			    attrPtr = attrPtr->next;
		       }
		  }
	     }
	}
	xmlXPathFreeObject(result);
    }    

    return 1;
}

int getsempkey(void* lpxmlDoc,int* lpResult)
{
    xmlDocPtr		 pxmlDoc;
    xmlNodePtr		 cur;
    xmlNodePtr		 curpath;
    xmlXPathContextPtr	 context = NULL;
    xmlXPathObjectPtr	 result	 = NULL;
    xmlNodeSetPtr	 nodeset;
    xmlAttrPtr		 attrPtr;
    xmlChar		*key;
    xmlChar		*xpath	 = BAD_CAST("/application/semkey");
    
    
    pxmlDoc = (xmlDocPtr)lpxmlDoc;
    
    cur = xmlDocGetRootElement(pxmlDoc);
    if (cur == NULL)
    {
        printf("Can't get the root element\n");
        return 0;
    }

    context = xmlXPathNewContext(pxmlDoc);
    result  = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);           
    if (result != NULL)
    {
	 if (xmlXPathNodeSetIsEmpty(result->nodesetval) == 0)
	 {
	      nodeset = result->nodesetval;
	      if(nodeset->nodeNr > 0)
	      {
		   curpath = nodeset->nodeTab[0];
		   if(curpath != NULL)
		   {
			attrPtr = curpath->properties;
			while (attrPtr != NULL)
			{
			    if (!xmlStrcmp(attrPtr->name, BAD_CAST "value"))
			    {
				 key = xmlGetProp(curpath,(const xmlChar *)"value");
				 printf("semid: %s\n", key);
				 *lpResult = atoi((char*)key);
				 xmlFree(key);
			    }
			    attrPtr = attrPtr->next;
		       }
		  }
	       
	      }
	 }
 	xmlXPathFreeObject(result);
    }
    return 0;
}

int getdatabasecfg(void* lpxmlDoc,dbCfg* lpCfg)
{
    xmlDocPtr		 pxmlDoc;
    xmlNodePtr		 cur;
    xmlNodePtr		 curpath;
    xmlXPathContextPtr	 context = NULL;
    xmlXPathObjectPtr	 result	 = NULL;
    xmlNodeSetPtr	 nodeset;
    xmlAttrPtr		 attrPtr;
    xmlChar		*key;
    xmlChar		*xpath	 = BAD_CAST("/application/database");
    
    
    pxmlDoc = (xmlDocPtr)lpxmlDoc;
    
    cur = xmlDocGetRootElement(pxmlDoc);
    if (cur == NULL)
    {
        printf("Can't get the root element\n");
        return 0;
    }

    context = xmlXPathNewContext(pxmlDoc);
    result  = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);           
    if (result != NULL)
    {
	 if (xmlXPathNodeSetIsEmpty(result->nodesetval) == 0)
	 {
	      nodeset = result->nodesetval;
	      if(nodeset->nodeNr > 0)
	      {
		   curpath = nodeset->nodeTab[0];
		   while (curpath != NULL)
		   {
			cur = curpath->xmlChildrenNode;
			while(cur != NULL)
			{
			     if (xmlStrcmp(cur->name, (const xmlChar *)"address") == 0)
			     {
				  key = xmlNodeListGetString(static_cast<xmlDocPtr>(lpxmlDoc), cur->xmlChildrenNode, 1);
				  printf("db address: %s\n", key);
				  strcpy(lpCfg->szaddr,(char*)key);
				  xmlFree(key);
			     }
			     else if(xmlStrcmp(cur->name, (const xmlChar *)"port") == 0)
			     {
				  key = xmlNodeListGetString(static_cast<xmlDocPtr>(lpxmlDoc), cur->xmlChildrenNode, 1);
				  printf("db port: %s\n", key);
				  lpCfg->nport =  atoi((char*)key);
				  xmlFree(key);
			     }
			     else if(xmlStrcmp(cur->name, (const xmlChar *)"dbinst") == 0)
			     {
				  key = xmlNodeListGetString(static_cast<xmlDocPtr>(lpxmlDoc), cur->xmlChildrenNode, 1);
				  printf("db dbinst: %s\n", key);
				  strcpy(lpCfg->szinstname,(char*)key);
				  xmlFree(key);
			     }
			     else if(xmlStrcmp(cur->name, (const xmlChar *)"account") == 0)
			     {
				  attrPtr = cur->properties;
				  while (attrPtr != NULL)
				  {
				       if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
				       {
					    key = xmlGetProp(cur,(const xmlChar *)"name");
					    printf("db accout name: %s\n", key);
					    strcpy(lpCfg->szaccname,(char*)key);
					    xmlFree(key);
				       }
				       else if (!xmlStrcmp(attrPtr->name, BAD_CAST "password"))
				       {
					    key = xmlGetProp(cur,(const xmlChar *)"password");
					    printf("db account password: %s\n", key);
					    strcpy(lpCfg->szaccpwd,(char*)key);
					    xmlFree(key);
				       }
				       attrPtr = attrPtr->next;					     
				  }
			     }
			     cur = cur->next;
			     
			}
		       curpath = curpath->next;
		   }
	      }
	 }
   	xmlXPathFreeObject(result);
    }
    return 0;
}


void freexmlcfg(void* lpCtxt,void* lpxmlDoc)
{
    xmlParserCtxtPtr	pCtxt;
    xmlDocPtr		pxmlDoc;

    pCtxt   = (xmlParserCtxtPtr)lpCtxt;
    pxmlDoc = (xmlDocPtr)lpxmlDoc;
    
    
    xmlFreeDoc(pxmlDoc);
    xmlFreeParserCtxt(pCtxt);
    xmlCleanupParser(); 
}


