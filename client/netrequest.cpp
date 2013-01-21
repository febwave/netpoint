#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int	tcconnect(char* lpszNetAddress,int nPort,int* pSocket);
size_t	tcsend(int nSocket,size_t nSize,char* sndBuffer);
size_t	tcrecv(int nSocket,size_t nSize,char* rcvBuffer);

void	tcclose(int nSocket);


void tcclose(int nSocket)
{
     if(nSocket != 0)
     {
	  shutdown(nSocket,SHUT_RDWR);
	  close(nSocket);
     }
}

int tcconnect(char* lpszNetAddress,int nPort,int* pSocket)
{
     struct sockaddr_in address	= {0};
     int		nSocket	= 0;
     int		nResult	= 0;
     

     nSocket = socket(AF_INET,SOCK_STREAM,0);
     if(nSocket != 0)
     {
	  address.sin_family      = AF_INET;
	  address.sin_addr.s_addr = inet_addr(lpszNetAddress);	       
	  address.sin_port	  = htons(nPort);	 
	  nResult = connect(nSocket,(struct sockaddr*)&address,sizeof(address));
	  if(nResult != -1)
	  {

	       *pSocket = nSocket;
	       return 0;
	  }
	  else
	  {
	       nResult = errno;
	       perror("");
	       tcclose(nSocket);
	  }
     }           
     return 1;
}

size_t tcsend(int nSocket,size_t nSize,char* sndBuffer)
{   
     size_t	nOffset	= 0;
     size_t	nLen	= 0;
     size_t	nResult	= 0;
         

     if(nSocket != 0)
     {
	  nLen = nSize;
	  do
	  {
	       nResult	= send(nSocket,sndBuffer+nOffset,nLen,0);
	       if(nResult < 0) break;
	       nOffset += nResult;
	       nLen    -= nResult;	       
	  }while(nOffset < nSize);
	  if(nResult != -1) return 0;
     }     
     return nResult;
}
 
size_t tcrecv(int nSocket,size_t nSize,char* rcvBuffer)
{
     size_t	nOffset	= 0;
     size_t	nLen	= 0;
     size_t	nResult	= 0;

     
     nLen = nSize;
     do
     {
	  nResult = recv(nSocket,rcvBuffer+nOffset,nLen,0);
	  if(nResult <= 0) break;
	  nOffset += nResult;
	  nLen    -= nResult;	       
     }while(nOffset < nSize);
     if(nResult != -1) return 0;	       
         
     return nResult;
}


int netReq(char* lpszNetAddress,int nPort,size_t nSize,char* sndBuffer,size_t* pRespSize,char** pRecvBuffer)
{
     int	nSocket	   = 0;
     int	nResult	   = 0;
     size_t	nRetSize   = 0;
     char*	pNetBuffer = NULL;
     
     
     nResult = tcconnect(lpszNetAddress,nPort,&nSocket);
     if(nResult != 0) return nResult;
     nResult = tcsend(nSocket,sizeof(int),(char*)&nSize);
     if(nResult == 0)
     {
	  nResult = tcsend(nSocket,nSize,sndBuffer);
	  if(nResult == 0)
	  {
	       nResult = tcrecv(nSocket,sizeof(int),(char*)&nRetSize);
	       if(nResult == 0 && nRetSize > 0)
	       {
		    pNetBuffer = new char[nRetSize];
		    if(pNetBuffer == NULL)
		    {
			 nResult = 2;
		    }
		    else
		    {
			 memset(pNetBuffer,0,nRetSize);
			 nResult = tcrecv(nSocket,nRetSize,pNetBuffer);
			 if(nResult == 0)
			 {
			      *pRecvBuffer = pNetBuffer;
			      *pRespSize   = nRetSize;			      
			 }
		    }
	       }
	  }
     }
     tcclose(nSocket);
     return nResult;
}


void releaseNetBuffer(char* pBuffer)
{
     delete pBuffer;
}
