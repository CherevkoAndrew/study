#include <stdio.h>
#include <stdlib.h>
//#include <errno.h>
//#include <string.h>
//#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <dirent.h>
//#include <errno.h>
#include <iostream>
//#include <list>
//#include <vector>
#include <string>
#include <fstream>
//#include <string.h>
//#include <unistd.h>
//#include <time.h>
#include <stdlib.h>
//#include <iomanip>
//#include <sstream>
//#include <algorithm>

#include <time.h>

#include <sys/types.h>
#include <pwd.h>

using namespace std;

int main( int argc, char **argv )
{
	printf("Page size: %ld\n", sysconf(_SC_PAGESIZE));
	printf("Num of pages: %ld\n", sysconf(_SC_PHYS_PAGES));
	printf("Num of available pages: %ld\n", sysconf(_SC_AVPHYS_PAGES));
	//char swaps[20];
	string line;
	cout<<endl;
	//strcpy(swaps, "proc/swaps");
	ifstream swapsFile("/proc/swaps");
	while(getline(swapsFile,line)){
		cout<<line;
	}
	cout<<endl;
}