#include "sendMail.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "base64.h"
#include "pop3_smtp.h"

#define PORT 587

char* buf; //for reciece answer from server
char* req; //send request to server

int sendMail(char* login,char* pass)
{
	WSADATA wsd; //wsdata for socket
	SOCKET s; //socket
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0) //starting socket
	{
		return 1; 
	}
	s=conn(login,PORT); //try connect to server, conn(char*,int) - pop3_smtp.c
	if (s==NULL) //if can't connect
		return 1;
	
	if (authenticationSmtp(s,login,pass)==1) //authentication on smtp server 
		return 1;
	if (mailFrom(s)==1) //entering MAIL FROM: command
		return 1;
	if (recipients(s)==1)//enter recipients of message, end entering - empty string
		return 1;
	if (data(s)==1)//entering message, if fields from:,to: and subject: are empty your message can get in spam folder
		return 1;
	free(buf);
	return 0;
}

int authenticationSmtp(SOCKET s,char* mail,char* pass)
{
	char* login=getLogin(mail); //cut login from mail address
	buf = (char*)malloc(sizeof(char)*255); //recieve buffer
	printf("Trying to login...\t"); 
	
	
	if (sendRequest(s,"AUTH LOGIN\r\n",buf)==NULL) //send login request to server
	{
		printf("Failed!\n");
		return 1;
	}
	buf = (char*)malloc(sizeof(char)*255); 
	req = (char*)malloc((sizeof(login)*4)/3); //allocate memory for encode login
	base64_encode((unsigned char*)login,strlen(login),req,strlen(req)); //encoding login to base64
	strcat(req,"\r\n"); //add <crlf>

	
	if (sendRequest(s,req,buf)==NULL) //send login
		return 1;
	req = (char*)malloc((sizeof(pass)*4)/3); //allocate memory for encode password
	buf = (char*)malloc(sizeof(char)*255);
	base64_encode((unsigned char*)pass,strlen(pass),req,strlen(req)); //encode password
	strcat(req,"\r\n");
	if (sendRequest(s,req,buf)==NULL) //send password
		return 1;
	printf("Ok!\n");
	return 0;
}

char* getSmtpAddr(char* server) //getting smtp server from mail
{
	if (strcmp(server,"yandex.ru")==0) 
		return "smtp.yandex.ru";
	if (strcmp(server,"mail.ru")==0)
		return "smtp.mail.ru";
}

int recipients(SOCKET s) //enter recipients
{
	char recipient[40];
	int i=0;
	int ret = 0;
	buf = (char*)malloc(sizeof(char)*255); 
	printf("Enter recipients\n");
	while(i<10) // 10 recipients max
	{
		fflush(stdin); //clear the stdin, if don't clear it fgets can get rubbish from stdin
		fgets(recipient,40,stdin); //get recipient from stdin
		req = (char*)malloc(sizeof(char)*strlen(recipient)+9); //allocate memory for recipient. +9 needed for RCPT TO: and <crlf>
		if(recipient[0]=='\n') //if empty string - end entering recipients
		{
			break;
		}
		sprintf(req,"%s%s","RCPT TO: ",recipient); //concatenate RCPT TO: and recipient into req
		if (sendRequest(s,req,buf)==NULL) //send recipient to server
		{
			printf("%s",buf); //if recipient is wrong - print buf
		}
		i++;
	}
	return 0;
}

int mailFrom(SOCKET s) //MAIL FROM: command
{
	char from[40];
	buf=(char*)malloc(sizeof(char)*255);
	printf("Mail from: ");
	scanf("%s",from);
	req = (char*)malloc(sizeof(char)*strlen(from)+sizeof(char)*12);
	sprintf(req,"%s%s%s","MAIL FROM:",from,"\r\n");
	if (sendRequest(s,req,buf)==NULL) 
	{
		printf("%s",buf);
		return 1;
	}
	return 0;
}

int data(SOCKET s) //enter message
{
	char message[255];
	buf=(char*)malloc(sizeof(char)*255);
	if(sendRequest(s,"DATA\r\n",buf)==NULL) //send command DATA to server for start entering message
		return 1;
	
	fflush(stdin); //clear stdin
	printf("from: ");
	fgets(message,255,stdin); //get filed 'from:'
	req=(char*)malloc(sizeof(char)*strlen(message)+5);
	sprintf(req,"%s%s","from:",message); 
	send(s,req,strlen(req),0);
	printf("to: ");
	fgets(message,255,stdin); //get filed 'to:'
	req=(char*)malloc(sizeof(char)*strlen(message)+3);
	sprintf(req,"%s%s","to: ",message);
	send(s,req,strlen(req),0);
	printf("subject: ");
	fgets(message,255,stdin); //get field 'subject:'
	req=(char*)malloc(sizeof(char)*strlen(message)+9);
	sprintf(req,"%s%s","subject: ",message); 
	send(s,req,strlen(req),0);
	send(s,"\r\n",strlen("\r\n"),0);
	while(1)
	{
		fflush(stdin); 
		req=(char*)malloc(sizeof(char)*255);
		fgets(req,255,stdin); //get line from stdin
		
		if((req[0]=='.')&&(req[1]=='\n')) //if first sybmol '.' and second '\n' send req and get answer
		{
			if (sendRequest(s,req,buf)==NULL) 
				return 1;
			return 0;
		}
		
		send(s,req,strlen(req),0); //if first sybmol not '.' or second not '\n' send message without answer
		
	}
	return 0;
}

