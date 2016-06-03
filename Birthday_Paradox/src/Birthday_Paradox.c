/*
 ============================================================================================================================================
 Name        : Birthday_Paradox.c
 Author      : The Shrewds (Group id: 41)
 Version     : 1
 Copyright   : Your copyright notice
 Description : Write a code that verifies - birthday paradox is indeed correct. 
 ============================================================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float TrialMethod(int PersonCount,int Trials)
{
	float Result;
	int i,j,Birthdays[365],temp,Flag=0,Count=0;

	 srand(time(NULL));

	for(i=0;i<Trials;i++)
	{
		Flag=0;
		for(j=0;j<365;j++)
		{
			Birthdays[j]=0;
		}

		for(j=0;j<PersonCount;j++)
		{
			temp=rand()%365;
			Birthdays[temp]+=1;
		}
		for(j=0;j<365;j++)
		{
			if(Birthdays[j]>1)
			{
				Flag=1;
				break;
			}
		}
		if(Flag==1)
			Count++;

	}
	Result=Count/(float)Trials;
	Result=Result*100;
	return Result;
}

float MathMethod(int PersonCount)
{
	float Result=1.0;
	int i;

	for(i=0;i<PersonCount;i++)
	{
		Result=Result*(365-i)/365;
	}

	return (1-Result)*100;
}

int main(void)
{
	float TrialProb[3], MathProb;
	int i;
	FILE *fp;

	fp=fopen("Data.csv","w");

	fprintf(fp,"No of Persons,25Trials,100Trials,1000Trials,Theoritical");

	for(i=1;i<=100;i++)
	{
		TrialProb[0]=TrialMethod(i,25);
		TrialProb[1]=TrialMethod(i,100);
		TrialProb[2]=TrialMethod(i,1000);
		MathProb=MathMethod(i);

		fprintf(fp,"\n%d,%f,%f,%f,%f",i,TrialProb[0],TrialProb[1],TrialProb[2],MathProb);
	}

	printf("\nCSV file generated...");
	return 0;
}
