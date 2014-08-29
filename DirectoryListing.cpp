/**
 * Program to List directory and files recursively.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <queue>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include<cstring>
#include <set>

using namespace std;

void DirListing(char *);

int main(int argc, char *argv[])
{

	for(int i=0;i<argc;i++)
		printf("%s \n",argv[i]);
	if(argc < 2)
	{
		printf("Less number of arguments \n");
		return 0;
	}


	char *path = (char *)calloc(256,sizeof(char));
	strcpy(path,argv[1]);

	DirListing(path);
	return 1;
}

void DirListing(char *path)
{
	char *dirpath;
	queue<char *> dir_queue;
	dir_queue.push(path);
	struct stat buf;
	set<string> dir_sort_set;
	set<string>::iterator it;
	while(!dir_queue.empty())
	{
		dir_sort_set.clear();
		path = dir_queue.front();
		dir_queue.pop();
		DIR *dir = opendir(path);
		if(dir == NULL)
		{
			continue;
		}
		struct dirent *dirinfo = readdir(dir);
		while(dirinfo)
		{
			if(!strcmp(dirinfo->d_name,".") || !strcmp(dirinfo->d_name,".."))
			{
				dirinfo = readdir(dir);
				continue;
			}
			dir_sort_set.insert(dirinfo->d_name);

			dirinfo = readdir(dir);
		}

		for (it=dir_sort_set.begin(); it!=dir_sort_set.end(); ++it)
		{
			dirpath = new char[256];
			strcpy(dirpath,path);
			strcat(dirpath,(*it).c_str());
			if(-1 == stat(dirpath,&buf))
			{
				//printf("stat returned error %d for file %s ",errno, dirpath);
				continue;
			}
			if(S_ISDIR(buf.st_mode))
			{
				printf("%s ,D ,0 ,%ld \n",dirpath,buf.st_mtime);
				strcat(dirpath,"/");

				dir_queue.push(dirpath);
			}
			else
			{
				printf("%s ,F ,%ld ,%ld \n",dirpath,buf.st_size,buf.st_mtime);

				delete dirpath;
			}
		}
		delete path;
	}
}


