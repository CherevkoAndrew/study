#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <time.h>

#include <sys/types.h>
#include <pwd.h>


char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

 void info(char* pathName) {
	struct stat entryInfo;
	char time[36];
	char type[40];
	char mode[] = "---------";

	if( stat( pathName, &entryInfo ) == 0 ) {
		if( S_ISDIR( entryInfo.st_mode ) ) {            
			DIR *dir = opendir( pathName );
			char deepPathName[PATH_MAX + 1];
			int retval = 0;
			if( dir == NULL ) {
        		printf( "Error opening %s: %s", pathName, strerror( errno ) );
        		return ;
    		}

    		struct dirent entry;
    		struct dirent *entryPtr = NULL;

    		retval = readdir_r( dir, &entry, &entryPtr );
    		while( entryPtr != NULL ) {
        		struct stat entryInfo;

		        if( ( strncmp( entry.d_name, ".", PATH_MAX ) == 0 ) ||
		            ( strncmp( entry.d_name, "..", PATH_MAX ) == 0 ) ) {
		            /* Short-circuit the . and .. entries. */
		            retval = readdir_r( dir, &entry, &entryPtr );
		            continue;
        		}

        		(void)strncpy( deepPathName, pathName, PATH_MAX );
      			(void)strncat( deepPathName, "/", PATH_MAX );
        		(void)strncat( deepPathName, entry.d_name, PATH_MAX );

        		info(deepPathName);

        		retval = readdir_r( dir, &entry, &entryPtr );

        	}
        } else {
        	printf("%s\n", pathName);
        	printf("User: %s\n", getpwuid(entryInfo.st_uid)->pw_name);
        	//printf("%d\n", entryInfo.st_atime);
        	
        	printf("Access: %s\n", formatdate(time, entryInfo.st_atime));

        	if (S_ISBLK(entryInfo.st_mode))
        		strcpy(type , "block device");
        	else if (S_ISCHR(entryInfo.st_mode))
        		strcpy(type ,"character device");
        	else if (S_ISDIR(entryInfo.st_mode))
        		strcpy(type,"directory");
        	else if (S_ISFIFO(entryInfo.st_mode))
        		strcpy(type , "FIFO");
        	else if (S_ISLNK(entryInfo.st_mode))
        		strcpy(type , "link");
        	else if(S_ISREG(entryInfo.st_mode))
        		strcpy(type , "regular file");

        	printf("Type: %s\n", type);


        	if ( entryInfo.st_mode & S_IRUSR ) mode[1] = 'r';    /* 3 bits for user  */
    		if ( entryInfo.st_mode & S_IWUSR ) mode[2] = 'w';
    		if ( entryInfo.st_mode & S_IXUSR ) mode[3] = 'x';

		    if ( entryInfo.st_mode & S_IRGRP ) mode[4] = 'r';    /* 3 bits for group */
		    if ( entryInfo.st_mode & S_IWGRP ) mode[5] = 'w';
		    if ( entryInfo.st_mode & S_IXGRP ) mode[6] = 'x';

		    if ( entryInfo.st_mode & S_IROTH ) mode[7] = 'r';    /* 3 bits for other */
		    if ( entryInfo.st_mode & S_IWOTH ) mode[8] = 'w';
		    if ( entryInfo.st_mode & S_IXOTH ) mode[9] = 'x';

		    printf("Permissions: %s\n", mode);
		    printf("\n");
        }
	} 
}

int main( int argc, char **argv )
{
	char file[] = "../../os";
	info(file);
}