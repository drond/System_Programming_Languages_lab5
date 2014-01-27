#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include "recieveMail.h"
#include "sendMail.h"


int main (int argc, char* argv[])
{
	char mail[40];
	char pass[40];
	int key;
	
	printf("Enter email\n");
	scanf("%s",mail); //get mailaddress
	printf("Enter password\n");
	scanf("%s",pass); //get password
	

	
	while (1)
	{
		key = 0;
		printf("Select one of the menu items\n");
		printf("1. Send mail\n");
		printf("2. Recieve mail\n");
		printf("0. Exit\n");
		scanf("%i",&key);
		switch(key)
		{
		case 1:
			{
				if(sendMail(mail,pass)==0) //start sending mail
					printf("Message successfully sent!\n");
				
				break;
			}
		case 2:
			{
				recieveMail(mail,pass); //start receiving mail
				break;
			}
		case 0:
			return 0;
			break;
		}

	}

	

}

