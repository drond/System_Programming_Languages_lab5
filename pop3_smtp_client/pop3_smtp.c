#include "pop3_smtp.h"
#include <stdio.h>
#include "base64.h"
#include "sendMail.h"
#include "recieveMail.h"



char* sendRequest(SOCKET s,char* req,char* buf) //send request and get answer from server
{
	int ret=0;
	send(s,req,strlen(req),0);
	ret=recv(s,buf,strlen(buf),0);
	buf[ret]='\0'; //cut rubbish
	if ((buf[0]>'3')||(buf[0]=='-')) //if first symbol > 3 or == - then something wrong
		return NULL;
	else
		return buf;
}


SOCKET conn(char* mail, int port) //connect to server
{
	char* buf=(char*)malloc(sizeof(char)*255);
	struct sockaddr_in addr; //sockaddr struct
	struct hostent* host; //host to connect
	char* server="";
	SOCKET s;
	if (port==587) //if port =587 then connect to SMTP
		server= getSmtpAddr(strchr(mail,'@')+1);
	else //else connect to POP3
		server = getPop3Addr(strchr(mail,'@')+1); 
	addr.sin_family=AF_INET; //intrnet address family
	addr.sin_port = htons(port); //get network byte order for port
	host = gethostbyname(server); //get real address by hostname
	s = socket(AF_INET,SOCK_STREAM,0); //declare socket in inet family, stream type, without flags
	CopyMemory(&addr.sin_addr, host->h_addr_list[0],host->h_length); //copy hostname to addr struct

	printf("Trying connect to %s...\t",server);

	if (connect(s,(struct sockaddr_in*)&addr,sizeof(addr))==SOCKET_ERROR) //try connect to server
	{
		printf("failed\n");
		printf("Cant connect to %s\n",server);
		return NULL;
	}
	else if (port==587) //if port = 587 send EHLO command to smtp server
	{
		buf = (char*)malloc(sizeof(char)*255);
		recv(s,buf,1024,0);
		
		if (sendRequest(s,"EHLO me\r\n",buf)==NULL)
		{
			printf("Failed!\n");
			return NULL;
		}
		printf("ok!\n");
		return s;
	}
	else if (port==110) //if port=110 recieve greeting from server
	{
		recv(s,buf,strlen(buf),0);
		if (buf[0]=='+')
		printf("ok!\n");
		else
		{
			printf("Failed!\n");
			return NULL;
		}
		return s;
	}
	return s;
}

char* getLogin(char* addr) //get login from mail
{
	char* login;
	int i=0;
	while((addr[i]!='@')&&(addr[i]!='\0'))
	{
		i++;
	}
	if(addr[i]=='\0')
	{
		printf("Invalid address\n");
	}
	else
	{
		login = (char*)malloc(sizeof(char)*i);
		strncpy(login,addr,i);
		login[i]='\0';
	}
	return login;
}




