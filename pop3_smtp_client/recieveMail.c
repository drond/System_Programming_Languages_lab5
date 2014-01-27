
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>
#include "pop3_smtp.h"
#include "base64.h"

#define PORT 110

char* req;
char* buf;


char* getPop3Addr(char* server)
{
	if (strcmp(server,"yandex.ru")==0) //���� �����
		return "pop.yandex.ru";
	if (strcmp(server,"mail.ru")==0)
		return "pop.mail.ru";
}

int authenticationPop3(SOCKET s,char* mail,char* pass)
{
	char* login=getLogin(mail);
	req = (char*)malloc(sizeof(char)*strlen(login)+7);
	buf = (char*)malloc(sizeof(char)*255);
	printf("Trying to login...\t");
	sprintf(req,"%s%s%s","USER ",login,"\r\n");
	if (sendRequest(s,req,buf)==NULL)
	{
		printf("Failed!\n");
		return -1;
	}
	
	sprintf(req,"%s%s%s","PASS ",pass,"\r\n");
	buf = (char*)malloc(sizeof(char)*255);
	if (sendRequest(s,req,buf)==NULL)
	{
		printf("Failed!\n");
		return -1;
	}
	
		
	printf("Ok!\n");
	return getSize(buf);
}

char* getMessage(SOCKET s,char* buf)
{
	int i=0;
	recv(s,buf,strlen(buf),0);
	
	buf=strstr(buf,"\r\n\r\n");
	do{
		i++;
	}while(buf[strlen(buf)-i]!='.');
	buf[strlen(buf)-i]='\0';
	if (((buf[0]=='4')&&(buf[1]=='5'))||(buf[0]=='-'))
		return NULL;
	else
		return buf;
}


int getSize(char* message)
{
	int i=4;
	char mess[30];
	while(message[i]!=' ')
	{
		mess[i-4]=message[i];
		i++;
	}
	mess[i-4]='\0';
	return atoi(mess);
}

int recieveMail(char* login,char* pass)
{
	int size=0;
	int postBoxSize=0;
	WSADATA wsd;
	SOCKET s;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
	{
		return 1;
	}
	printf("Type 'man' to see help\n");
	s=conn(login,110);
	if (s==NULL)
		return 1;

	postBoxSize=authenticationPop3(s,login,pass);
		if (postBoxSize==-1)
			return 1;
	while(1)
	{
		req = (char*)malloc(sizeof(char)*15);
		fflush(stdin);
		fgets(req,15,stdin);
		req[strlen(req)-1]='\0';
		strcat(req,"\r\n");
		if ((strncmp(req,"LIST",4)==0)||((strncmp(req,"list",4)==0)))
		{
			if (strlen(req)<8)
			{
				buf = (char*)malloc(sizeof(char)*postBoxSize*50);
				sendRequest(s,req,buf);
				printf("%s\n",buf);
			}
			else
			{
				
				
				buf = (char*)malloc(sizeof(char)*50);
				sendRequest(s,req,buf);
				printf("%s\n",buf);
				req="";
			}
		
		}
		else if ((strncmp(req,"RETR",4)==0)||((strncmp(req,"retr",4)==0)))
		{
			buf = (char*)malloc(sizeof(char)*50);
			if (sendRequest(s,req,buf)==NULL)
				printf("%s\n",buf);
			else
			{
				buf  = (char*)malloc(sizeof(char)*getSize(buf)+100);
				printf("%s\n",getMessage(s,buf));
			}
		}
		else if ((strncmp(req,"QUIT",4)==0)||((strncmp(req,"quit",4)==0)))
		{
			return 0;
		}
		else if (strncmp(req,"man",3)==0)
		{
			printf("Available commands:\nlist [message num]\nstat\ndele <message num>\nretr <message num>\nquit\n");
		}
		else 
		{
			buf = (char*)malloc(sizeof(char)*50);
			sendRequest(s,req,buf);
			printf("%s\n",buf);
		}
			

	}
	return 0;
}