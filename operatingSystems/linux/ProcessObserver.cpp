#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
//#include "stdafx.h"

using namespace std;



struct Process
{
	int id;
	char name[50];
	char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    int uid;
    int vmSize;
    long unsigned int pTime;
    //long unsigned int stime;
    long unsigned int checkTime;
    unsigned flags;
    long unsigned int timeProp;
};

bool sortByName(const Process* lhs,const Process* rhs) {
	return strcmp(lhs->name, rhs->name) < 0;
}

bool sortByMemory(const Process* lhs,const Process* rhs) {
	return lhs->vmSize < rhs->vmSize;
}

bool sortByPtime(const Process* lhs,const Process* rhs) {
	return lhs->timeProp < rhs->timeProp;
}

class ProcessObserver
{
	
    


public:
	string** users;
	long memTotal;
    int size;
    Process** lst;
	ProcessObserver();
    int updateList();
    int updateProcess(Process* pOld, Process* pNew);
    int readStat(char* f, Process *p);
    int getdir (char* dir, vector<char*> &files);
	~ProcessObserver();
};

ProcessObserver::ProcessObserver() {
	users = new string*[65536];

	ifstream fin("/etc/passwd");

	char ch;
	//char name[40];
	string* name;
	string line;
	int id;
	int p;// = 0; 

	while (getline(fin, line)) {
		istringstream iss(line);
		p=0;
		name = new string();

		for (;name->empty() || name->at(p-1) != ':' ; ++p)
		{
			iss >> ch;
			name->append(1,ch);
		}
		name->pop_back();
		//name[p] ='/0';

		do
		{
			iss >> ch;
		} while (ch != ':');

		iss >> id;
		//fin.getline();
		users[id] = name;
	}

    size = 20000;
    lst = new Process*[size];
    for (int i = 0; i < size; ++i)
    {
        lst[i] = NULL;
    }

   ifstream memInfo("/proc/meminfo");

	memInfo >> line;
	memInfo >> memTotal;
}

ProcessObserver::~ProcessObserver() {
	for (int i = 0; i < 65536; ++i)
	{
		delete users[i];
	}
	delete [] users;
	for (int i = 0; i < size; ++i)
	{
		delete lst[i];
	}
    delete [] lst;
}

int ProcessObserver::updateList() {

    char dir[20];
    char pdir[20];
    strcpy(dir,"/proc/");
    vector<char*> ids =  vector<char*>();
    std::vector<int> files;

    Process* p;
    int lstPointer = 0;
    int procId;

    getdir(dir,ids);

     for (int i = 0; i < ids.size(); ++i)
    {
        files.push_back(atoi(ids[i]));
    }

    for (unsigned int i = 0;i < files.size();i++) {

        procId = files[i];
        //cout<<files[i]<<endl;
        p = new Process;
        p->checkTime = time(NULL);
        strcpy(dir,"/proc/");
        strcpy(pdir,"");timeProp
        sprintf(pdir,"%d",files[i]);    
        strcat(dir,pdir);
        //cout<<dir<<"\n";
        readStat(dir, p);

        for (; lstPointer < procId; ++lstPointer) {        
            /* code */
            delete lst[lstPointer];
            lst[lstPointer] = 0;
        }

        if ( lst[lstPointer]) {
            updateProcess(lst[lstPointer], p);
        } else {
            lst[lstPointer] = p;
        }
        ++lstPointer;
    }
    //cout<<"LLLLLOOOOOOOOL";
    return 0;
}

int ProcessObserver::updateProcess( Process* pOld, Process* pNew) {
    pOld->timeProp = (pNew->pTime  - pOld->pTime);// / (pNew->checkTime - pOld->checkTime);
    pOld->pTime = pNew->pTime;
    pOld->checkTime = pNew->checkTime - pOld->checkTime;
    delete pNew;
    return 0;
}

int ProcessObserver::getdir (char* dir, vector<char*> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir)) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL  ) {
        if (isdigit(dirp->d_name[0]))
        	files.push_back((dirp->d_name));
        //cout<<dirp->d_name<<endl;
    }
    closedir(dp);
    return 0;
}

int ProcessObserver::readStat(char* f, Process *p) {

	char status[50];
	strcpy(status, f);
	strcat(status, "/status");	
    strcat(f, "/stat");
    ifstream fin(f);
    ifstream ufin(status); 
    unsigned long buff;

    long unsigned int utime;
    long unsigned int stime;

    char ch;
    string str;
    if (!fin.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
    else
    {
         //cout<<f;
         int id;
         fin >> p->id;

         fin >>ch;
         fin >>ch;
         int i = 0;
         for (; ch != ')' ; ++i)
         {
             /* code */
            p->name[i] = ch;
            fin>>ch;
         }
         p->name[i] = '\0';
         fin >> p->state;
         fin >> p->ppid;
         fin >> p->pgrp;
         fin >> p->session;
         fin >> p->tty_nr;
         fin >> p->tpgid;
         fin >> p->flags;
         fin >>buff>>buff>>buff>>buff;
         fin >> utime;
         fin >> stime;
         p->pTime = utime + stime;
         //cout<< p->id << endl << p->name <<endl;//<< endl << p->state << endl << p->utime / sysconf(_SC_CLK_TCK) << endl << p->stime / sysconf(_SC_CLK_TCK)<<endl;

        
        fin.close(); // закрываем файл
    }

    if (!ufin.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
    else
    {
    	for (int i = 0; i < 7; ++i)
    	{
    		getline(ufin,str);;
    		
    	}

    	ufin >> str;
    

    	ufin >> p->uid;

    	for (int i = 0; i < 9; ++i)
    	{
    		getline(ufin,str);;
    		
    	}
    	ufin >> str;

    	ufin >> p->vmSize;

    }
}

 int main(int argc, char* argv[]){

 	int period = 4000000;//atoi(argv[1]);
 	//char sortType = atoi(argv[2]);
 	if(argc > 1)
 		period = atoi(argv[1]);
 	//char compare = m;

    ProcessObserver po;
    po.updateList();
    list<Process*> processes;

   for (int i = 0; i < 20; ++i)
    {
    	usleep(period);
	    po.updateList();


	    
	    for (int i = 0; i < po.size; ++i)
	    {
	    	if ( po.lst[i] )
	    		processes.push_back(po.lst[i]);
	    }
		if(argc > 2){
			switch(argv[2][0]) {
				case 'n' : processes.sort(sortByName);
					break;
				case 'm' : processes.sort(sortByMemory);
					break;
				case 'p' : processes.sort(sortByPtime);
			}
		}
	    
	   // processes.sort(sortByMemory);
	    //po.updateList();
	    cout.setf(ios::right);
	    for (std::list<Process*>::iterator it = processes.begin(); it != processes.end(); it++)
	    {
	        //if(po.lst[i]){
	           cout<<setw(6)<<(*it)->id<<" "<<setw(20)<<(*it)->name<<"  ";
	          cout<<setw(10)<<(*it)->state<<" ";
	           cout<<setw(10)<<(long double)((*it)->timeProp) / sysconf(_SC_CLK_TCK)/(*it)->checkTime;
	           cout<<setw(10)<< *po.users[(*it)->uid]<<setw(12)<<(long double)((*it)->vmSize) / po.memTotal<<endl;
	        //}
	    }
	    processes.clear();
	}
	cout<<endl<<argv[2][0]<<endl;
}