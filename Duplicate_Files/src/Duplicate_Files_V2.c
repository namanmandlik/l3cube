/*
 ==================================================================================================================================================
 Name        : Duplicate_Files.c
 Author      : The Shrewds (Group id: 41)
 Version     : 1
 Copyright   : Your copyright notice
 Description : Write a program to list duplicate files from hard drive.
 	 	 	   The aim of this assignment is to list all the duplicate files from the hard drive and give user option to remove them or merge them.
 ==================================================================================================================================================
*/

#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define true 1
#define false 0

int duplicateCount = 0;

int FindDuplicates(char* path, char* fileName);
int CompareFiles(char* originalFile, char* currFile);
char *DuplicatePaths[100];

int main(int argc, char *argv[])
{
	int choice,status,i;

	if (argc!=3)
	{
		printf("Usage: %s 'Base Directory' 'Original File Path'\n", argv[0]);
		return -1;
	}

	FindDuplicates(argv[1], argv[2]);
	printf("\n\nFound %d duplicate(s)\n", duplicateCount);

	if(duplicateCount==0)
		return 0;

	printf("\nYou want to delete them or merge them? For Delete:1 Merge:2\n");
	scanf("%d",&choice);

	for(i=0;i<duplicateCount;i++)
	{
			printf("\nFile:%s\n",DuplicatePaths[i]);
	}
	switch(choice)
	{
	case 1:
		for(i=0;i<duplicateCount;i++)
		{
			status=remove(DuplicatePaths[i]);
			if(status==0)
				printf("\nFile: %s deleted successfully..",DuplicatePaths[i]);
		}
		status=remove(argv[2]);
		if(status==0)
			printf("\nFile: %s deleted successfully..",argv[2]);
		break;

	case 2:
		for(i=0;i<duplicateCount;i++)
		{
			status=remove(DuplicatePaths[i]);
			if(status==0)
				printf("\nFile: %s deleted successfully..",DuplicatePaths[i]);

		}
		break;

	default: printf("\nWrong choice entered...Try again later...");

	}
	return 0;
}


int FindDuplicates(char* path, char* fileName)
{
	DIR *dir;
	struct dirent *dp;
	struct stat statp;

	char absoluteFilePath[255];

	if ((dir = opendir(path))== NULL)
	{
		//perror("Failed to open directory");
		//printf("Path:%s\n",path);
		return -1;
	}


	while ((dp =readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".")==0 || strcmp(dp->d_name,"..")==0)
		{
			//Do nothing
		}
		else
		{
			strcpy(absoluteFilePath, path);

			strcat(absoluteFilePath, "/");

			strcat(absoluteFilePath, dp->d_name);

			stat(absoluteFilePath, &statp);

			if (S_ISDIR(statp.st_mode))		//is a directory...
			{
				FindDuplicates(absoluteFilePath, fileName);
			}
			else if (S_ISREG(statp.st_mode))	//is a file...
			{
				if(strcmp(absoluteFilePath,fileName)==0)
				{
					//do nothing
				}
				else if (CompareFiles(fileName, absoluteFilePath))
				{
					printf("%s\n", absoluteFilePath);

					DuplicatePaths[duplicateCount]=(char*) malloc (sizeof(char)*(strlen(absoluteFilePath)+1));
					strcpy(DuplicatePaths[duplicateCount],absoluteFilePath);

					duplicateCount++;
				}
			}
		}
	}
	closedir(dir);
	return 0;
}


int CompareFiles(char* originalFile, char* currFile)
{
	struct stat statOriginal, statCurr;
	stat(originalFile, &statOriginal);
	stat(currFile, &statCurr);

	if ((int) statOriginal.st_size != (int) statCurr.st_size)
		return false;

	int fd_Original  = open(originalFile, O_RDONLY);
	int fd_Current	= open(currFile, O_RDONLY);

	if (fd_Original == -1 || fd_Current == -1)
		return false;

	int length=1024, bytesRead;
	char *bufferOriginal  = (char*) malloc(length * sizeof(char));
	char *bufferCurr      = (char*) malloc(length * sizeof(char));


	while(true)
	{
		bytesRead = read(fd_Original, bufferOriginal, length);

		if (bytesRead <= 0)
			break;

		bytesRead = read(fd_Current, bufferCurr, bytesRead);

		if (strcmp(bufferOriginal, bufferCurr))
			return false;
	}

	return true;
}
