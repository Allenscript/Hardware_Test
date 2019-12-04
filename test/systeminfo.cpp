
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<cstdlib>
#include<string>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<vector>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
using namespace std;
const int N = 300;
string Test2(string x){
    char line[N];
    FILE *fp;
    string cmd = "ls -l";
    // system call
    const char *sysCommand = x.data();
    if ((fp = popen(sysCommand, "r")) == NULL) {
        cout << "error" << endl;
        return 0;
    }
    while (fgets(line, sizeof(line)-1, fp) != NULL){
        string li = line;
        return li;
    }
    pclose(fp);
}
int a,t;
void timer_handler (int signum)
{	a++;
	if(a>=20){

	//	Test2("killall start-burn start");
		a=0;
	 //printf("trigger...\n"); 
	 Test2("echo  num >> a.txt")   ;     
	 
	 t++;
	}
	if (t==10){

		//Test2("killall start-burn start");
		_exit(0);
	}
}

int main ()
{
    

    struct sigaction sa;
    struct itimerval timer;
    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset(&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after 100 msec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 999999;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 999999;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 999999;
    /* Start a virtual timer. It counts down whenever this process is
    executing. */
    setitimer (ITIMER_VIRTUAL, &timer, NULL);


    // Test2("./start");

    /* Do busy work. */
    while(1)
    {
    }
    return 0;
}










// #include <iostream>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include<cstdlib>
// #include<string>
// #include<cstdio>
// #include<cstring>
// #include<iostream>
// #include<algorithm>
// #include<vector>
// using namespace std;
// const int N = 300;
// std::string& trim(std::string &);
// string Test(string x)
// {
	
//     char line[N];
//     FILE *fp;
//     int ln=0;
//     int memnum;
//     // System function
//     const char *sysCommand = x.data();
//     if ((fp = popen(sysCommand, "r")) == NULL)
//     {
//         cout << "error" << endl;
//     }
//     while (fgets(line, sizeof(line)-1, fp) != NULL)
//     {
//         	cout<<line;
//     }

//     pclose(fp);
//     // return res;
// }
// string Test2(string x){
//     char line[N];
//     FILE *fp;
//     string cmd = "ls -l";
//     // system call
//     const char *sysCommand = x.data();
//     if ((fp = popen(sysCommand, "r")) == NULL) {
//         cout << "error" << endl;
//         return 0;
//     }
//     while (fgets(line, sizeof(line)-1, fp) != NULL){
//         string li = line;
//         return li;
//     }
//     pclose(fp);
// }

// void SplitString(const string& s, vector<string>&    v, const string& c)
// {
//     std::string::size_type pos1, pos2;
//     pos2 = s.find(c);
//     pos1 = 0;
//     while(std::string::npos != pos2)
//     {
//         v.push_back(s.substr(pos1, pos2-pos1));
//         pos1 = pos2 + c.size();
//         pos2 = s.find(c, pos1);
//     }
//     if(pos1 != s.length())
//         v.push_back(s.substr(pos1));
// }

// int main()
// {
// // 任务: 296 total,   1 running, 233 sleeping,   0 stopped,   0 zombie
// // %Cpu(s):  0.1 us,  0.1 sy,  0.0 ni, 99.8 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
// // KiB Mem :  3989264 total,  2444428 free,  1065016 used,   479820 buff/cache
// // KiB Swap:  3998716 total,  3567868 free,   430848 used.  2673116 avail Mem 

// string me="top -n 1 |grep -e \"Mem :\" | cut -d \",\" -f 1 | cut -d \":\" -f 2 |cut -d \" \" -f 3";
// string free="top -n 1 |grep -e \"Mem :\" | cut -d \",\" -f 2| cut -d \":\" -f 2|cut -d \"f\" -f 1";
// //Test(me);
// string xx=Test2(free).c_str();

//  vector<string> v ;
//  SplitString(xx,v,"r");
 
//  trim(v[0]);
 

// cout<<v[0]<<endl;


// long b=atoi(xx.c_str());


//  //cout<<100-(b/(float)a*100)<<"%"<<endl;

// //system("top -n 1 |grep Cpu | cut -d \",\" -f 1 | cut -d \":\" -f 2");
// //system("top -n 1 |grep Cpu | cut -d \",\" -f 2 ");
// //system("top -n 1 |grep -e \"Mem :\" | cut -d \",\" -f 1 | cut -d \":\" -f 2 |cut -d \" \" -f 3");
// //system("top -n 1 |grep -e \"Mem :\" | cut -d \",\" -f 2 |cut -d \" \" -f 3");


// return 0;
// }




// std::string& trim(std::string &s) 
// {
//     if (s.empty()) 
//     {
//         return s;
//     }

//     s.erase(0,s.find_first_not_of(" "));
//     s.erase(s.find_last_not_of(" ") + 1);
//     return s;
// }