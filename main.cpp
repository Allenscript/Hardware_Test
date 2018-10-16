/**
 * @file    main.cpp
 * @brief   HwPro
 * @author  yyc
 * @version 3.1-release
 * @date    2018.9.13
 * @bug     
 * @warning Enter just Once
 */
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
//#include <time.h>
#include <pthread.h>

#define BUFLEN 1024
#define MCAST_PORT 1600
#define MCAST_ADDR "224.0.2.111"     /* Addr of Multi BroadCast Group*/
#define MCAST_INTERVAL 2             /* SleepTime of  receive data*/

#define PORT 5000
#define QUEUE 20
const int N = 300;
#define BUFF_SIZE 256                /* size of Buffer*/
using namespace std;

string getip;    // string : Server IP
string x;        // string : operation string
string global;
int a=0;     // timer signal
int oldinfo=0;

/**

 * judge str

 */
/**
  * @param str[] ip to judge
  * @return is that str[] a ip addr
  */
int IsIp(char str[]);

/**

 * String Split

 */
/**
  * @param S string to do
  * @param V string vector
  * @param C Symbol of Split
  * @see SplitString(const string& s, vector<string>&    v, const string& c);
  * @return void
  */
void SplitString(const string& s, vector<string>&    v, const string& c);
/**

 * String trim

 */
/**
  * @param S string to trim
  * @return void
  */
void trim(string &s);

/**

 * get serverip

 */
/**
  * net function
  * @param no
  * @return char *  serverip
  */

void WaitServer();

/**

 * Test Board

 */
/**
  * @return msg : infomation of Board
  */
string TestSN();

/**

 * Test  SN

 */
/**
  * @return msg infomation of SN
  */
string TestBoard();

/**

 * Test  CPU

 */
/**
  * @return msg infomation of CPU
  */
string TestCpu();
/**

 * Test Mem

 */
/**
  * @return msg infomation of Memery
  */
string TestMem();
/**

 * Test Disk

 */
/**
  * @return msg infomation of Disk
  */
string TestDisk();
/**

 * Test other

 */
/**
  * @param string l operation
  * @param string X Tips
  * @return msg infomation of Testing
  */
string TestOth(string l,string x);
//string BoardName,BiosV,CpuInfo,CpuNum,MemInfo,MemSize,MemFrequen,DiskNum,DiskInfo,DiskV,NetBoardInfo;
string oldtestcmd="oldtest.sh";
string myip="ifconfig  | grep \"inet 172.16\" | awk '{print $2 }'";
//string myip="ifconfig  | grep \"inet 192.16\" | awk '{print $2 }'";

string SN="dmidecode -t 1 | grep Manufacturer | cut -d \":\" -f 2";
string BoardName="dmidecode | grep \"Product Name\"";
string BiosV="dmidecode -s bios-version";
string CpuInfo="cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c";
string CpuCoreNum="cat /proc/cpuinfo  |grep \"cpu cores\" | uniq -c | cut -f2 -d:";
string CpuNum="cat /proc/cpuinfo | grep \"physical id\" |sort|uniq|wc -l";
string MemInfo="dmidecode | grep \"Manufacturer\"";
string MemSize="dmidecode|grep -A5 \"Memory Device\"|grep Size|grep -v Range";    // -v No
string MemFrequen="dmidecode |grep \"Configured Clock Speed\"";
string DiskNum="lsblk -o TYPE | grep -i disk|wc -l";
string DiskInfo="cat /proc/scsi/scsi | grep \"Model\" ";
string DiskV="smartctl -a /dev/sda | more|grep \"Firmware Version\"";
string NetBoardInfo="lspci | grep Ethernet";
string Graphics="lspci -vnn | grep VGA -A 1 ";


//get data to operation
//! Test funtion of set shell
/*!
  get infomation of operation
  string x : operation shell
*/


string Test(string x,string res)
{

    char line[N];
    FILE *fp;
    int ln=0;
    int memnum;
    // System function
    const char *sysCommand = x.data();
    if ((fp = popen(sysCommand, "r")) == NULL)
    {
        cout << "error" << endl;
    }
    while (fgets(line, sizeof(line)-1, fp) != NULL)
    {
        if(x==BoardName)
        {
            if(ln==0)
            {

            }
            else
            {
                string li=line;
                trim(li);
                string b=":";
                string::size_type idx;
                idx=li.find(b);
                if(idx == string::npos )//unexist
                {
                    res.append(li);
                    res.append("  ");
                }
                else//exist
                {
                    vector<string> v ;
                    SplitString(li,v,":");
                    trim(v[1]);
                    res.append(v[1]);
                }
            }
            ln++;
        }
        else if(x==SN)
        {
            string ll=line;
            trim(ll);
            res.append(ll);

        }
        else if(x==DiskInfo)
        {
            string li=line;
            string b=":";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )// unexist
            {
            }
            else//exist
            {
                vector<string> v ;
                SplitString(li,v,":");
                string li=v[2];
                trim(li);
                string b="R";
                string::size_type idx;
                idx=li.find(b);
                if(idx == string::npos )//unexist
                {

                }
                else //exist
                {
                    SplitString(li,v,"Rev");
                    res.append(v[4]);
                }
            }
        }
        else if(x==MemInfo)
        {

            string li=line;
            string b=":";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )//unexist
            {
            }
            else//exist
            {
                vector<string> v ;
                SplitString(li,v,":");
                string li=v[1];
                trim(li);
                string b="To Be Filled";
                string::size_type idx;
                idx=li.find(b);
                if(idx == string::npos )//unexist
                {
                    string b="[Empty]";
                    string::size_type idx;
                    idx=li.find(b);
                    if(idx == string::npos )//unexist
                    {


                    string b="GenuineIntel";
                    string::size_type idx;
                    idx=li.find(b);
                    if(idx == string::npos )//unexist
                    {


                    string b="Not Specified";
                    string::size_type idx;
                    idx=li.find(b);
                    if(idx == string::npos )//unexist
                    {
                        res.append("*");
                        res.append(li);
                    }
                    else
                    {
                    } 
                    }
                    else
                    {
                    }   
                    }
                    else
                    {
                    }
                }
                else
                {
                }
            }

        }
        else if(x==MemSize)
        {
            memnum++;
            string li=line;
            string b=":";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )//unexist
            {}
            else//exist
            {
                vector<string> v ;
                SplitString(li,v,":");
                string li=v[1];
                trim(li);
                string b="No Module";
                string::size_type idx;
                idx=li.find(b);
                if(idx == string::npos )//unexist
                {
                    res.append(li);
                }
                else
                {

                }
            }
        }
        else if(x==MemFrequen)
        {

            string li=line;
            string b=":";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )        //unexist
            {}
            else//exist
            {
                vector<string> v ;
                SplitString(li,v,":");
                string li=v[1];
                trim(li);
                string b="Unknown";
                string::size_type idx;
                idx=li.find(b);
                if(idx == string::npos )//unexist
                {
                    res.append(li);
                }
                else
                {

                }
            }
        }
        else if(x==DiskV)
        {

            string li=line;
            trim(li);
            string b=":";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )//exist
            {}
            else //exist
            {
                vector<string> v ;
                SplitString(li,v,":");
                string li=v[1];
                trim(li);
                string b="XX";
                string::size_type idx;
                idx=li.find(b);
                if(idx == string::npos )//unexist
                {
                    res.append(li);
                }
                else {}
            }
        }
        else if(x==Graphics)
        {
            string li=line;
            string b="Subsystem:";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )//unexist
            {}
            else//exist
            {
                vector<string> v ;
                SplitString(li,v,"Subsystem:");
                string li=v[1];
                trim(li);
                res.append(li);
            }
        }
        else if(x==NetBoardInfo)
        {
            string li=line;
            string b="controller:";
            string::size_type idx;
            idx=li.find(b);
            if(idx == string::npos )//does not exist
            {}
            else//exist
            {
                vector<string> v ;
                SplitString(li,v,"controller:");
                string li=v[1];
                trim(li);
                res.append(li);
            }
        }
        else
        {
            string ll=line;
            trim(ll);
            res.append(ll);
        }

    }
    pclose(fp);
    return res;
}






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
    ~TcpTool()
    {
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

class TcpServerTool
{
public:
    int co;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;     //select
    int conn;
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    TcpTool *mytcp;
    /*set socket*/
    int ss = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET   IPV4   ;SOCK_STREAM   TCP
    struct sockaddr_in server_sockaddr;
    TcpServerTool(string getip)
    {
        // do all init of network
        mytcp=new TcpTool(getip); // tcp socket to send data
        printf("11\n");
        server_sockaddr.sin_family = AF_INET;
        printf("22\n");
        server_sockaddr.sin_port = htons(PORT);
        printf("33\n");
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        /*bind*/
        if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1)
        {
            perror("bind");
            printf("bind error\n");
            exit(1);
        }
        /*listen*/
        if(listen(ss, QUEUE) == -1)
        {
            printf("listen error\n");
            perror("listen");
            exit(1);
        }
        /*connect*/

        printf("44\n");

        printf("55\n");

        sleep(2);
        string myinfo="";
        myinfo.append(TestSN());
        global=myinfo;
        myinfo.append(":");
        myinfo.append(Test(myip,x));
        printf("%s\n", myip.c_str());
        mytcp->TcpSend(myinfo);
        mytcp->~TcpTool();

        //  success to return fd， when error  return -1
        // conn = accept(ss, (struct sockaddr*)&client_addr, &length);   //need client part of  addr
        // if( conn < 0 )
        // {
        //     perror("connect");
        //     printf("connect error\n");
        //     exit(1);
        // }
        printf("666\n");
        
    }


    ~TcpServerTool()
    {
        close(conn);
        close(ss);
        mytcp->~TcpTool();
    }
    int np_socket_alive (int conn_fd)
    {
        char buff[32];
        int recv_buff = recv (conn_fd, buff, sizeof (buff), MSG_PEEK);
        int sockErr = errno;

        if (recv_buff > 0)  // Get Data
            return 1;
        if ((recv_buff == -1) && (sockErr == EWOULDBLOCK))  // No Data recieved
            return 1;
        else
           return -1;
    }
    void my()
    {

        while(1)
        {


            char buffer[1024];
            memset(buffer, 0,sizeof(buffer));
            //  recv(sockfd, buff, buff_size,MSG_DONTWAIT);  
            int len = recv(conn, buffer, sizeof(buffer),MSG_DONTWAIT);
            printf(" 222 %s\n", buffer);
            printf("%d\n",np_socket_alive(ss));
            if(np_socket_alive(ss))
            {

                //close(conn);
                mytcp->~TcpTool();
              //  mytcp=new TcpTool(getip); // tcp socket to send data


                /*bind*/

                /*listen*/

                /*connect*/


                ///success to return fd， when error  return -1
                conn = accept(ss, (struct sockaddr*)&client_addr, &length);   //need client part of  addr
                if( conn < 0 )
                {
                    perror("connect");
                    printf("connect error\n");
                    exit(1);
                }else{

                    printf("accept new connect \n");
                }


               

            }
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
            if(retval == -1)
            {
                // printf("select error\n");
                break;
            }
            else if(retval == 0)
            {
                if(oldinfo==1)
                {
                     mytcp=new TcpTool(getip);
                    string  oldgetinfo="old_test:";
                    oldgetinfo.append(Test(" cat /usr/linkpack.txt  /usr/a.txt  /usr/memtester.txt /usr/stream.txt /usr/randwrite_sd* /usr/randread_sd* /usr/write_sd* /usr/read_sd* ","c"));
                    mytcp->TcpSend(oldgetinfo);

                }
                oldinfo=0;
                // printf("server null， client null,waiting...\n");
                continue;
            }
            else
            {
                /*recv msg*/

                if(FD_ISSET(conn,&rfds))
                { mytcp=new TcpTool(getip);
                    char buffer[1024];
                    memset(buffer, 0,sizeof(buffer));
                    int len = recv(conn, buffer, sizeof(buffer), MSG_DONTWAIT);

                    if(strcmp(buffer, "reboot") == 0)
                    {
                        //reboot to load installer
                        Test("reboot","reboot");
                        printf("reboot\n");
                        break;
                    }
                    else if (strcmp(buffer, "test_start") == 0)
                    {
                        //test
                       // mytcp->~TcpTool();
                       // mytcp=new TcpTool(getip);
                        
                        string MachineInfo;
                        //MachineInfo.append("hwinfo");
                        //MachineInfo.append(":");
                       
                        MachineInfo.append(TestBoard());
                        
                        MachineInfo.append("#");
                        MachineInfo.append(TestCpu());
                        
                        MachineInfo.append("#");
                        MachineInfo.append(TestMem());
                        
                        MachineInfo.append("#");
                        MachineInfo.append(TestDisk());
                        
                        MachineInfo.append("#");
                        MachineInfo.append(TestOth(NetBoardInfo,"NetBoardInfo:"));
                        
                        MachineInfo.append("#");
                        MachineInfo.append(TestOth(Graphics,"Graphics:"));
                        
                        MachineInfo.append("#");
                        MachineInfo.append(TestSN());
                        //
                        
                        mytcp->TcpSend(MachineInfo);

                    }
                    else if (strcmp(buffer, "test_restart") == 0)
                    {
                        //retest
                        //   mytcp->~TcpTool();
                        //   mytcp=new TcpTool(getip);
                        string MachineInfo;
                        //MachineInfo.append("hwinfo");
                        //MachineInfo.append(":");
                        MachineInfo.append(TestBoard());
                        MachineInfo.append("#");
                        MachineInfo.append(TestCpu());
                        MachineInfo.append("#");
                        MachineInfo.append(TestMem());
                        MachineInfo.append("#");
                        MachineInfo.append(TestDisk());
                        MachineInfo.append("#");
                        MachineInfo.append(TestOth(NetBoardInfo,"NetBoardInfo:"));
                        MachineInfo.append("#");
                        MachineInfo.append(TestOth(Graphics,"Graphics:"));
                        MachineInfo.append("#");
                        MachineInfo.append(TestSN());
                        mytcp->TcpSend(MachineInfo);
                    }
                    else if (strcmp(buffer, "old_start") == 0)
                    {
                       // mytcp=new TcpTool(getip);
                        //all test result > to .txt   at last  get that to send
                        //1  mem test      expect got
                        string mem=Test(MemSize,x);
                        mem.erase(mem.length()-3,3);
                        int memsize = atoi(mem.c_str());
                        string memtester="";
                        memtester.append("/monitor/memtester ");
                        memtester.append(mem);
                        memtester.append(" 10  | grep -v got> /usr/memtester.txt");
                        Test(memtester,"memtester");
                        string memstream="/monitor/stream > /usr/stream.txt";
                        Test(memstream,"memstream");
                        //2  fio test
                        //pre test cmd
                        // 1) randwrite
                        string randwrite1="/monitor/fio -filename=/dev/sd";
                        string randwrite2=" -direct=1 -thread -rw=randwrite -ioengine=libaio -bs=1M -size=100G -numjobs=1 -iodepth=32 -runtime=120 -time_based -name=stresssda";
                        // 2) write
                        string write1="/monitor/fio -filename=/dev/sd";
                        string write2=" -direct=1 -thread -rw=write -ioengine=libaio -bs=1M -size=100G -numjobs=1 -iodepth=32 -runtime=120 -time_based -name=stresssda";
                        // 3) randread
                        string randread1="/monitor/fio -filename=/dev/sd";
                        string randread2=" -direct=1 -thread -rw=randread -ioengine=libaio -bs=1M -size=100G -numjobs=1 -iodepth=32 -runtime=120 -time_based -name=stresssda";
                        // 4) read
                        string read1="/monitor/fio -filename=/dev/sd";
                        string read2=" -direct=1 -thread -rw=read -ioengine=libaio -bs=1M -size=100G -numjobs=1 -iodepth=32 -runtime=120 -time_based -name=stresssda";
                        //check disk num
                        co = atoi(Test(DiskNum,x).c_str());
                        // count DiskNum & test disk
                        for(int i=0; i<co; i++)
                        {
                            string ss=("a"+i);
                            //build randwritesdx.txt
                            string randwrite = randwrite1;
                            randwrite.append(ss+randwrite2+" > /usr/randwrite_sd"+ss+".txt");

                            string write = write1;
                            write.append(ss+write2+" > /usr/write_sd"+ss+".txt");

                            string randread = randread1;
                            randread.append(ss+randread2+" > /usr/randread_sd"+ss+".txt");

                            string read = read1;
                            read.append(ss+read2+" > /usr/read_sd"+ss+".txt");

                            Test(randwrite,"randwrite");
                            Test(write,"write");
                            Test(randread,"randread");
                            Test(read,"read");

                            // loop to test disk
                        }

                        //3 CPU test      1) Linkpack 2)HPL
                        // 1) Linkpack
                        string linkpack="/monitor/runme_xeon64 > /usr/linkpack.txt";
                        Test(linkpack,"123");
                        // 2) HPL    1.get used%    2. get HPL.out

                        //just run start burn
                        Test("nice -n 19 /monitor/start","123");
                        //4 get all data  to send
                        /// just cat *.txt with rule

                    }
                    else if (strcmp(buffer, "killoldtest") == 0)
                    {

                        Test("killall xhpl start-burn start","123");
                        continue;
                        //killall

                    }
                }






            }

        }

    }
};
/// \brief Class TcpTool : a tools of tcp
///
///  hand on  TcpTool(),TcpSend(),TcpRecv()
void* mytimer(void* args)
{
 int kk=0;
for(kk=0;kk<20;kk++){


    //cout << "Hello Runoob！" <<kk<< endl;
    Test("date >> /usr/a.txt","date");
    Test("nice -n -20 top -b -n 1 |grep Cpu >> /usr/a.txt","cpu");
    Test("nice -n -20 top -b -n 1 |grep Mem >> /usr/a.txt","mem");

    sleep(1800);
    if(kk>=19){
        //oldinfo=1;
        Test("killall xhpl start-burn start","123");
        TcpTool * mytcp=new TcpTool(getip);
    string  oldgetinfo="old_test:";
        oldgetinfo.append(global+"#");

      oldgetinfo.append(Test(" cat /usr/linkpack.txt  /usr/a.txt  /usr/memtester.txt /usr/stream.txt /usr/randwrite_sd* /usr/randread_sd* /usr/write_sd* /usr/read_sd* ","c"));
      mytcp->TcpSend(oldgetinfo);

    }
}
    return 0;
}

int main()
{


    pthread_t tids[1];
    for(int i = 0; i < 1; ++i)
    {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, mytimer, NULL);
        if (ret != 0)
        {
           cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    
   






      Test("echo main.cpp> /usr/main.txt","2");

    //  Test("chmod -R 777 /monitor/*","1");
  
   
   
    WaitServer();
   
    // printf("%s\n",getip.c_str());
//! Get ServerIP to begin tcp socket
    // getip="192.168.1.234";
//got ip and up the tcp socket to System Server
    TcpServerTool* cmdtcp=new TcpServerTool(getip);
    cmdtcp->my();


 pthread_exit(NULL);

    return 0;
}

void SplitString(const string& s, vector<string>&    v, const string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}
void trim(string &s)
{

    if( !s.empty() )
    {
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }

}
static void setnonblocking(int sockfd)
{
    int flag = fcntl(sockfd, F_GETFL, 0);
    if (flag < 0)
    {
        perror("fcntl F_GETFL fail");
        return;
    }
    if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0)
    {
        perror("fcntl F_SETFL fail");
    }
}

void WaitServer()
{
    printf("WaitServer\n");
    struct sockaddr_in local_addr;              /*Local Addr*/
    int fd = socket(AF_INET, SOCK_DGRAM, 0);     /*set Socket*/
    if (fd == -1)
    {
        perror("socket()");
        exit(1);
    }
    setnonblocking(fd);
    int yes = 1;
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
    {
        perror("Reusing ADDR failed");
        exit(1);
    }
    /*init Local Addr*/
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(MCAST_PORT);
    /* bind socket*/
    int err = bind(fd,(struct sockaddr*)&local_addr, sizeof(local_addr)) ;
    if(err < 0)
    {
        perror("bind()");
        exit(1);
    }
    /*set loop enable*/
    int loop = 1;
    err = setsockopt(fd,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));
    if(err < 0)
    {
        perror("setsockopt():IP_MULTICAST_LOOP");
        exit(1);
    }
    /*join to Multi Broadcast Group*/
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); /*Multi Broadcast Group Addr*/
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*local net injet*/
    /* take local to Group*/
    err = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
    if (err < 0)
    {
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        exit(1);
    }
    int times = 0;
    socklen_t addr_len = sizeof(local_addr);
    char buff[BUFF_SIZE];
    int n = 0;
    /* loop to get message from Multi Broadcast Group*/
    for(times = 0; times < 200; times++)
    {
        //
        printf("wait\n");
        memset(buff, 0, BUFF_SIZE);                 /* empty the buffer */
        /*Recevive*/
        printf("wait2\n");
        n = recvfrom(fd, buff, BUFF_SIZE, 0,(struct sockaddr*)&local_addr,&addr_len);
        if( n== -1)
        {
            perror("recvfrom()");
        }
        printf("wait3\n");
        char str[256] = "";
        strcpy(str,buff);
        if(IsIp(str))
        {
            printf("wait4\n");
            getip=buff;
            cout<<"yes";
            break;
        }
        printf("wait5\n");
        printf("Recv %dst message from server:%s\n", times, buff);
        sleep(MCAST_INTERVAL);
    }
    /*exit*/
    err = setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));
    close(fd);
}
int IsIp(char str[])
{
    int a,b,c,d;
    char temp[100];
    if((sscanf(str,"%d.%d.%d.%d",&a,&b,&c,&d))!=4)
        return 0;
    sprintf(temp,"%d.%d.%d.%d",a,b,c,d);
    if(strcmp(temp,str) != 0)
        return 0;
    if(!((a <= 255 && a >= 0)&&(b <= 255 && b >= 0)&&(c <= 255 && c >= 0)))
        return 0;
    else
        return 1;
}

string TestSN()
{
    string msg;
    msg.append(Test(SN,x));
    msg.erase(msg.length()-1,1);
    return msg;
}

string TestBoard()
{
    string msg;
    msg.append("BoardName:");
    msg.append(Test(BoardName,x));
    msg.append("/");
    msg.append("BisoVersion:");
    msg.append(Test(BiosV,x));
    return msg;
}
string TestCpu()
{
    string msg;
    msg.append("CpuNum:");
    msg.append(Test(CpuNum,x));
    msg.append("/");
    msg.append("CpuCore:");
    msg.append(Test(CpuCoreNum,x));
    msg.append("/");
    msg.append("CpuInfo:");
    msg.append(Test(CpuInfo,x));
    return msg;
}
string TestMem()
{
    string msg;
    msg.append("MemInfo:");
    msg.append(Test(MemInfo,x));
    msg.append("/");
    msg.append("MemSize:");
    msg.append(Test(MemSize,x));
    msg.append("/");
    msg.append("MemFreq:");
    msg.append(Test(MemFrequen,x));
    return msg;
}
string TestDisk()
{
    string msg;
    msg.append("DiskInfo:");
    msg.append(Test(DiskInfo,x));
    // get DiskNum
    msg.append("/");
    msg.append("DiskNum:");
    msg.append(Test(DiskNum,x));
    msg.append("/");
    int co = atoi(Test(DiskNum,x).c_str());
    // count DiskNum
    msg.append("DiskVer:");
    for(int i=0; i<co; i++)
    {
        string s="smartctl -a /dev/sd";
        string ss=("a"+i);
        s.append(ss);
        s.append("| more|grep \"Firmware Version\"");
        string fv=Test(s,x);
        //vector<string> v ;
       // SplitString(fv,v,":");
            msg.append(fv);
        //msg.append(v[1]);
        msg.append("/");
    }
    return msg;
}
string TestOth(string l,string x)
{
    string msg;
    msg.append(Test(l,x));
    return msg;
}
