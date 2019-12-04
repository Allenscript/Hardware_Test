#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <typeinfo>
#include <cstring>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <sys/time.h>
#include <signal.h>

#include <algorithm>
#include <fcntl.h>
#include <vector>
#include <stdarg.h>
#include <time.h>


#define BUFLEN 1024
#define MCAST_PORT 1600
#define MCAST_ADDR "224.0.2.111"     /* Addr of Multi BroadCast Group*/
#define MCAST_INTERVAL 5             /* SleepTime of  receive data*/

#define PORT 5000
#define QUEUE 20
const int N = 300;
#define BUFF_SIZE 256                /* size of Buffer*/
using namespace std;
string getip;    // string : Server IP
string x;        // string : operation string
int a=0;     // timer signal
int oldinfo=0;


class TcpTool
{


private:
    int client_sockfd;

    struct sockaddr_in remote_addr; // struct of Server net Addr



public:
    int len;
    char buf[BUFSIZ];  //Buffer of data
    TcpTool(string Sip)
    {
        memset(&remote_addr,0,sizeof(remote_addr)); //init data
        remote_addr.sin_family=AF_INET; // Set to IP mode
        remote_addr.sin_addr.s_addr=inet_addr(Sip.c_str());//Server IP
        remote_addr.sin_port=htons(3000); //Port of Server
        if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
        {
            perror("socket error");
            //return 1;
        }

        /*bind socket to Server*/
        if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
        {
            perror("connect error");
            //return 1;
        }
        printf("connected to server/n");
    }
    ~TcpTool(){
        close(client_sockfd);
    }
    int TcpRecv()
    {
        len=recv(client_sockfd,buf,BUFSIZ,0);
        return len;
    }
    int TcpSend(string sendBuff)  //if status < 0  reconnect
    {

        len=send(client_sockfd,sendBuff.c_str(),strlen(sendBuff.c_str()),0);
        return len;
    }


};


/// \brief Class : a tools of tcp
///  use    TcpServerTool* x=new TcpServerTool(); x->my();
///  hand on  TcpServerTool

class TcpServerTool{
public:
    int co;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;     //select
    int conn;
    TcpTool *mytcp;
    /*set socket*/
    int ss = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET   IPV4   ;SOCK_STREAM   TCP
    struct sockaddr_in server_sockaddr;
    TcpServerTool(string getip){
        // do all init of network
        mytcp=new TcpTool(getip); // tcp socket to send data

        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(PORT);
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*bind*/
        if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1) {
            perror("bind");
            exit(1);
        }
    /*listen*/
        if(listen(ss, QUEUE) == -1) {
            perror("listen");
            exit(1);
        }
    /*connect*/
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
    ///success to return fd， when error  return -1
    conn = accept(ss, (struct sockaddr*)&client_addr, &length);   //need client part of  addr
    if( conn < 0 ) {
        perror("connect");
        exit(1);
    }

    string myinfo="kkkkk";

    mytcp->TcpSend(myinfo);
}


~TcpServerTool(){
    close(conn);
    close(ss);
    mytcp->~TcpTool();
}

void my(){

    while(1) {
        /*clean rfds*/
        FD_ZERO(&rfds);
        // /* set input*/
        // FD_SET(0, &rfds);
        // maxfd = 0;
        /*join this conn*/
        FD_SET(conn, &rfds);
        /*find the biggest*/
        //if(maxfd < conn)
        maxfd = conn;
        /*set time out value */
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*wait*/
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
           // printf("select error\n");
            break;
        }else if(retval == 0){
            if(oldinfo==1){

            string  oldgetinfo=" cat /usr/linkpack.txt  /usr/a.txt  /usr/memtester.txt /usr/stream.txt /usr/randwrite_sd* /usr/randread_sd* /usr/write_sd* /usr/read_sd* c";
            mytcp->TcpSend(oldgetinfo);

            }
            oldinfo=0;
           // printf("server null， client null,waiting...\n");
            continue;
        }else{
            /*recv msg*/

            if(FD_ISSET(conn,&rfds)){
                char buffer[1024];
                memset(buffer, 0 ,sizeof(buffer));
                int len = recv(conn, buffer, sizeof(buffer), 0);

                if(strcmp(buffer, "reboot") == 0){
                    //reboot to load installer
                 //   Test("reboot","reboot");
                    break;
                }else if (strcmp(buffer, "test_start") == 0)
                { //test


               }else if (strcmp(buffer, "test_restart") == 0)
                {    //retest
              
             }else if (strcmp(buffer, "old_start") == 0)
             {
                    //all test result > to .txt   at last  get that to send
                    //1  mem test      expect got
            
            }else if (strcmp(buffer, "killoldtest") == 0)
            {

              
                continue;
                    //killall

            }
        }
    }
}

}
};

int main(int argc, char const *argv[])
{
    

 TcpServerTool* cmdtcp=new TcpServerTool("192.168.7.235");
    cmdtcp->my();

    return 0;
}