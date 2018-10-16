#include <stdio.h>
#define _PrintError_ 
#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>
#define MAX 1025
class CSocketTCP
{
public:
   CSocketTCP();
   ~CSocketTCP();
   long  Create(char* cIP ="",int iPort = 0,bool bRebind = false);
   long Connect(char* cIP,int iPort);
   long Send(const char* cData);
   long  Listen(long lNum = 5);
  long Rev(char* cData,long length);
  void Close();
private:
	struct sockaddr_in servAddr,clientAddr,connetAdrr;
	int sockfd,newsockfd;
	int  len;
	char cBuffer[MAX];
  	
   
};
