#include "CSocketTCP.H"
  CSocketTCP::CSocketTCP()
      {
	  	bzero(&servAddr,sizeof(servAddr));
	  	bzero(&clientAddr,sizeof(clientAddr));
		sockfd = -1;
		newsockfd = -1;
		len = sizeof(connetAdrr);
   	
       }
   CSocketTCP::~CSocketTCP()
   	{
   	}
  long    CSocketTCP::Create(char* cIP,int iPort,bool bRebind)
  	{
  	         int opt=1;
  		if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
		{
			#ifdef  _PrintError_
				perror("socket");
			#endif
				return -1;
		}
		clientAddr.sin_family=AF_INET;
		clientAddr.sin_port=htons(iPort);
		if(bRebind)
		{
		  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		}
		if(0 == strlen(cIP))
		{
			clientAddr.sin_addr.s_addr=htonl(INADDR_ANY); 
		}
		else
		{
		     clientAddr.sin_addr.s_addr=inet_addr(cIP); 
		}

		  int ret = bind(sockfd, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
		   if ( -1 == ret)
		   {
		     #ifdef _PrintError
		           printf("bind failed %s \n", strerror(errno));
		      #endif
		      return -2;
		   }
   
		return 0;
  	}
   long CSocketTCP::Connect(char* cIP,int iPort)
   	{
   		if(0 == cIP || 0 == strlen(cIP) ||iPort < 0  )
		{
			return -1;	
		}
		servAddr.sin_family=AF_INET;
		servAddr.sin_port=htons(iPort);
		servAddr.sin_addr.s_addr=inet_addr(cIP); 
   	           if(connect(sockfd,(struct sockaddr*)&servAddr,sizeof(servAddr))<0)
	      {
		   #ifdef   _PrintError_
			perror("connet");
		   #endif
	       }

   	      return 0;
   	}
   long CSocketTCP::Send(const char* cData)
   	{
   		if(NULL == cData || 0 == strlen(cData))
		{
		   return -1;
		}
		long lRet = 0,lTotal = 0;
		long Length = strlen(cData) + 1;
		const char* p = cData;
		do
		{	
   			lRet =  send(sockfd, p,Length , 0);
			p += lRet;
			lTotal += lRet;
		}while(lTotal < Length);
   	 	return Length;
   	}

    long  CSocketTCP::Listen(long lNum )
    	{
	    	   if(0 > lNum)
	   	{
	   	     return  -1;
	   	}
		if(-1 == sockfd)
		{
		   return -2;
		}
		  if(listen(sockfd,lNum)<0)
	       {
	       #ifdef _PrintError_
	         perror("listen");
		 #endif
	        return -3;
	       }
		 if((newsockfd=accept(sockfd,(struct sockaddr*)&connetAdrr,&len))<0)
		 {
		 #ifdef _PrintError_
			  perror("accept");
		 #endif
			return -4;
	      }
    	return 0;
    	}
  long CSocketTCP::Rev(char* cData,long length)
  	{
  	      long  ret = 0,lenth = 0;
	 
  		if(-1 == newsockfd)
		{
		    return -1;
		}
		ret =  recv(newsockfd,cBuffer,MAX,0);
		lenth = MAX > length ?MAX:length;
		memcpy(cData,cBuffer,lenth);
		return ret;
  	}
   void CSocketTCP::Close()
   	{
   		close(sockfd);
   	
   	}