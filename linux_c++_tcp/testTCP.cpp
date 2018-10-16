#include "CSocketTCP.H"
int main(int argc,char **argv)
{
//service
	char buffer[1025] = {0};
	CSocketTCP  tcp;
	tcp.Create("192.168.1.234",7774);
	tcp.Listen();
	while(1)
	{
		memset(buffer,0,1025);
		tcp.Rev(buffer,1025);
		puts(buffer);
		
	}
	getchar();


//client
	//char buffer[1025] = {0};
	//CSocketTCP  tcp;
	//tcp.Create("192.168.1.101",7774);//if you don't cretat ,bind ip and port
	//tcp.Connect("192.168.1.101",7777);
	//long lRet = tcp.Send("hello world");
	//printf("send %d\n",lRet);
    return 0;
}
