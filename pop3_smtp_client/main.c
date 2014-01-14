#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#include "main.h"


#define HELLO "HELLO" //
#define FROM "MAIL FROM:" //---так короче ^^
#define RCPT "RCPT TO:" //



void main (int argc, char* argv[])
{
	struct sendMailInfo* info; //объявляем сруктуру
	info=getInfo(); //заполняем структуру

	//******************* Закоменченый код - тест работы с SMTP *************
	//info->server=getSmtpAddr(info->server);
	/*WSADATA wsd;
	SOCKET s;
	struct sockaddr_in addr;
	struct hostent* host;
	char buf[2048];
	int ret;
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
	{
		return 1;
	}
	s = socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(587);
	host = gethostbyname("smtp.yandex.ru");
	if (host==NULL)
	{
		printf("Error while getting host\n");
		getch();
		return 1;
	}
	CopyMemory(&addr.sin_addr, host->h_addr_list[0],host->h_length);
	if (connect(s,(struct sockaddr_in*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		printf("Error while conect");
		getch();
		return 1;
	}
	ret=recv(s,buf,2048,0);
	if (ret==SOCKET_ERROR)
	{
		printf("Error!");
		return 1;
	}
	else if ((buf[0]='2')&&(buf[1]='2')&&(buf[2]='0'))
	{
		free(buf);
		printf("Connection to smtp server <servername> sucessfull!\n");
	}
	ret=send(s,HELLO,strlen(HELLO),0);
	if (ret==SOCKET_ERROR)
	{
		printf("Error!");
		return 1;
	}

	getch();
	return 0;*/

}

struct sendMailInfo* getInfo()
{
	struct sendMailInfo* info;
	char tmp[255]; //буфер для чтения с консоли
	info = (struct sendMailInfo*)malloc(sizeof(struct sendMailInfo)); //выделяем память под структуру
	printf("Enter your login (format someuser@somenet.ex):\n"); 
	scanf("%s",tmp); //считываем с консоли имя пользователя в формате username@somenet.ex
	
	if ((tmp!="")&&(strchr(tmp,'@')!=NULL)) //если строка не пустая, и содержит символ @
	{
		info->server=getSmtpAddr(strchr(tmp,'@')+1); //оставляем только символы справа от @, и заменяем имя почтового сервера на адрес smtp сервера
		info->username=tmp; //username пишется полностью с @somenet.ex
		return info;
	}
	
}

char* getSmtpAddr(char* server)
{
	char* smtp="";
	if (strcmp(server,"yandex.ru")==0) //если равно
		smtp="smtp.yandex.ru"; //smtp сервер яндекса
	return smtp;
}