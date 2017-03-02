#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <MYSQL/mysql.h>

int main(void)
{
	mySQL mysql;
	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_READ_DEAFAULT_GROUP;"option");

if(mysql_real_connect(&mysql,"localhost","root","","ma_base",0,NULL,0))
	{
		mysql_close(&mysql);
	}
	else
	{
		printf("Erreur");
	}
	return 0;
}