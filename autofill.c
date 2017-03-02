#include <stdlib.h>
#include <stdio.h>

int main (void)
{
	FILE *variables;
	int minute=0,heure=0,entree=0,sortie=0;
	variables = fopen("essai.txt","w+");
	
	while((heure<24) || (minute<60) )
	{
		for(minute=0;minute<60;minute++)
		{
			entree = rand()%150;
			sortie = rand()%120;
		
			if(heure<10 && minute<10)
			{
				fprintf(variables,"0%d:0%d;%d;%d\n",heure,minute,entree,sortie);
				
			}
			else
			{
				if(heure<10)
				{
					fprintf(variables,"0%d:%d;%d;%d\n",heure,minute,entree,sortie);
				}
				if(minute<10)
				{
					fprintf(variables,"%d:0%d;%d;%d\n",heure,minute,entree,sortie);
				}
				if(minute>=10 && heure>=10)
				{
					fprintf(variables,"%d:%d;%d;%d\n",heure,minute,entree,sortie);
				}
				
			}
		}
		heure++;
		
	}
	fclose(variables);
}