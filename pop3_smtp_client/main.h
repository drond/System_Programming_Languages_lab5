struct sendMailInfo //структура с необходимыми данными для отправки сообщений
{
	char* username;
	char* server;
	char* message;
	char* to;
	char* from;
	char* subject;
};
struct sendMailInfo* getInfo(); //заполняем структуру
char* getSmtpAddr(char*); //парсим правую часть логина (после @) и выбираем соотвествующий SMTP сервер