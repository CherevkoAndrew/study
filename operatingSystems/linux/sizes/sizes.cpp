/* readdir_demo main()
 * 
 * проходит через указанные каталоги 
 * и передает их в метод process_directory().
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <list>  
#include <vector>   // подключаем заголовок списка
#include <iterator> 

#include <GL/glut.h>

using namespace std;

long long maxSize;
list<long> sizes;
int length;
int maxHeight;
long* statistics;
int step;

 unsigned process_directory( char *theDir )
{
    DIR *dir = NULL;
    struct dirent entry;
    struct dirent *entryPtr = NULL;
    int retval = 0;
    unsigned count = 0;
    char pathName[PATH_MAX + 1];
    long tmpSize;

    /* открыть указанный каталог, если возможно. */
	dir = opendir( theDir );
    if( dir == NULL ) {
        printf( "Error opening %s: %s", theDir, strerror( errno ) );
        return 0;
    }

    retval = readdir_r( dir, &entry, &entryPtr );
    while( entryPtr != NULL ) {
        struct stat entryInfo;
        
        if( ( strncmp( entry.d_name, ".", PATH_MAX ) == 0 ) ||
            ( strncmp( entry.d_name, "..", PATH_MAX ) == 0 ) ) {
            /* Short-circuit the . and .. entries. */
            retval = readdir_r( dir, &entry, &entryPtr );
            continue;
        }

        (void)strncpy( pathName, theDir, PATH_MAX );
        (void)strncat( pathName, "/", PATH_MAX );
        (void)strncat( pathName, entry.d_name, PATH_MAX );
        
        if( lstat( pathName, &entryInfo ) == 0 ) {
            /* вызов stat() был успешным, так что продолжаем. */
            count++;
            
            if( S_ISDIR( entryInfo.st_mode ) ) {            
/* каталог */
               // printf( "processing %s/\n", pathName );
                count += process_directory( pathName );
            } else if( S_ISREG( entryInfo.st_mode ) ) { 
/* обычный файл */
                //printf( "\t%s has %lld bytes\n",
                    //pathName, (long long)entryInfo.st_size );
            	tmpSize = (long)entryInfo.st_size;
            	sizes.push_back(tmpSize);
            	if (maxSize < tmpSize) {
            		maxSize = tmpSize;
            	}

            } else if( S_ISLNK( entryInfo.st_mode ) ) { 
/* символическая ссылка */
                char targetName[PATH_MAX + 1];
                if( readlink( pathName, targetName, PATH_MAX ) != -1 ) {
                   // printf( "\t%s -> %s\n", pathName, targetName );
                } else {
                   // printf( "\t%s -> (invalid symbolic link!)\n", pathName );
                }
            }
        } else {
            printf( "Error statting %s: %s\n", pathName, strerror( errno ) );
        }

          retval = readdir_r( dir, &entry, &entryPtr );
    }
    
    /* закрытие каталога и возвращение числа элементов. */
    (void)closedir( dir );
    return count;
}

void getStatistics(int step) {
	length = maxSize / step + 1;
	statistics = new long[length];
	for (int i = 0; i < length; ++i)
	{
		statistics[i]= 0L;
	}

	for(std::list<long>::iterator it=sizes.begin(); it != sizes.end(); ++it)
		statistics[ *it / step ]++;
}

void renderBitmapString(float x,float y,char* text) {  
  char *c;
  glRasterPos2f(x, y);
  for (c=text; *c != '\0'; c++) {
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_10 , *c);
  }
}

void draw(void)
{
  //Background color
  glClearColor(9,9,9,1);
  glClear(GL_COLOR_BUFFER_BIT );
  glColor3f(0.0, 0.0, 0.4);
  glBegin(GL_LINES);
  	 glVertex2i(0, 0);
  	 glVertex2i(100, 0);
  	 glVertex2i(0, 0);
  	 glVertex2i(0, 100);
  glEnd();

  char text[10] = "hello";

  float stepWidth = 100./length;
  if (stepWidth < 0.1)
  {
  	stepWidth = 0.1;
  }
  int sub1 = 10/stepWidth;
  float a = 0;

  for (int i = 0; stepWidth * i < 100 && i < length ; ++i)
  {
  	glBegin(GL_QUADS);
		glVertex2f( stepWidth * i, 0.);
		glVertex2f(stepWidth * i, statistics[i] * 100. / maxHeight );//x/max*100
		glVertex2f(stepWidth * (i+1) , statistics[i] * 100. / maxHeight );
		glVertex2f( stepWidth * (i+1), 0.);
  	glEnd();

  	if (stepWidth * i > a) {
  		sprintf(text,"%8.2le", ( double)i*step);
  		renderBitmapString(stepWidth * i, -5, text);
  		a+=10;
  	}
  	
  }
//char hello[] = "Hell0";
//  //Draw order
  glFlush();
}

void reshape(int w, int h)
{
        glViewport(0, 0, w, h);
        glColor3f(1.0f,0.0f,0.0f);
       
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-10, 130, -10, 130);
       
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

int main( int argc, char **argv )
{
    int idx = 0;
    unsigned count = 0;
    
    char path[] = "/home/adi/Dropbox";
  //  for( idx = 1; idx < argc; idx++ ) {
        count += process_directory( path/*argv[idx] */);
  //  }
    printf("type step size\n");
    scanf("%d", &step);

    getStatistics(step);



    for (int i = 0; i < length; ++i)
    {
    	if(statistics[i] > maxHeight)
    		maxHeight = statistics[i];
    	//printf("%ld\n", statistics[i]);
    }

	glutInit(&argc, argv);
	//Simple buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition(50,25);
	glutInitWindowSize(1000,500);
	glutCreateWindow("Green window");
	//Call to the drawing function
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutMainLoop();
  //return 0;
    delete [] statistics;
    return EXIT_SUCCESS;
}

