struct sendMailInfo //��������� � ������������ ������� ��� �������� ���������
{
	char* username;
	char* server;
	char* message;
	char* to;
	char* from;
	char* subject;
};
struct sendMailInfo* getInfo(); //��������� ���������
char* getSmtpAddr(char*); //������ ������ ����� ������ (����� @) � �������� �������������� SMTP ������