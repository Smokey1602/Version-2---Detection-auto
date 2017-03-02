// g++ -Wall -o test1 camera.c `pkg-config --cflags --libs opencv`

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>

using namespace cv;
using namespace std;


// -------PROTOTYPAGE ----------
void sauvegarde_graphique();
void calib_auto();
void sauvegarde_calib();
void calibration();
void get_time();
void window_create();
void passage(int i);
void sauvegarde_automatique();
void menu();
void initialisation();
void suppressbruit(Mat Pic);
void recall();
void sauvegarde_graphique();



//--------DECLARATION DES VARIABLES -------------------------------------------------------

char kalib=0; //variable flag pour la calibration
int X[20]={450,460,470,480,490,500,510,520,530,540,550,560,570,580,590,600,610,620,630,640};
int Y[2]={300,330};
int test=0,test1=0,test2=0;
int Q=0,C=0,video=1;
int quitterC=0;
int T_variable[100]={0},T_raw[24000]={0},T_data[7500]={0};

//---------Creation de toutes les images--------------------
Mat hsvcrop[10];
Mat source,hsvsource,masquesource,src;
Mat masquecrop[10];
Mat cropedImage[10];
Mat im_menu;
Mat Graph;


VideoCapture capture(0);

//////////////////////////////////////////////////////////////
//------------Creation de toutes les variables-----------------
int LowH =0,HighH =178,LowS =0,HighS=255,LowV=0,HighV=80; // permet la detection du noir
Mat Lignes = Mat::zeros( source.size(), CV_8UC3 ); //permet le dessin de lignes (pr le debug)
// ---------- declaration de toutes les variables de chaque image crées nous permet de creer les vecteurs de déplacement des abeilles --------
int LastX[10]={-1}, LastY[10]={-1}, posX[10],posY[10];

int flag[10]={0};
int numsauv=0;
/////////////////////////////////////////
int bisY[10]={0};
int deplacement[10]={0};
///////////////variables communes au programme///////////
int entree=0,sortie=0;
char name[30];

// --------- variables pour recupere l'heure permettant la sauvegarde -------
static int seconds_last = 99;
char DateString[20],Jour[20],Minute[20],HeureMinute[20],Time[20];
string oldday="\0",oldminute="\0";
FILE *file; //fichier de sortie des detections
FILE *file2; //fichier de sauvegarde de secours
FILE *variables; //fichier de sauvegarde des données de calibration
FILE *graph; //fichier pour creer le graphique
char nom[100];
char nom2[100];
char image[100];
char image2[100];
int quitterS=0,minuteS=1,compteurS=0;
int PassMinut=0; //flag permettant de signaler le changement de minute


float loop=0; //essai de fps

//-----------------FIN DE LA DECLARATION DES VARIABLES ---------------------------------------------------------


void calib_auto()

{
	int flag0=0,flag255=0,ecart=0,matj=0,nbporte=0,i=0;
	int calibauto[25]={0};

	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(0,100,100,0),Scalar(10,255,255,0),masquesource);
	suppressbruit(masquesource);
	//imshow("masksource",masquesource);
	
		for(matj=0;matj<masquesource.cols;matj++)
		{	
			switch(masquesource.at<uchar>(200,matj))
			{
				case 0:
				if(flag0==0 && ecart >10)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=1;flag255=0;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
				case 255:
				if(flag255==0 && ecart >10)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=0;flag255=1;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
			}
		ecart++;
			
		}
		for(i=0;i<20;i++)
		{
			X[i]=calibauto[i];
		}

	ecart=0;flag0=0;flag255=0;nbporte=0;

	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(90,100,100,0),Scalar(130,255,255,0),masquesource);
	suppressbruit(masquesource);
	//imshow("masksource",masquesource);
	printf("%d %d",masquesource.cols,masquesource.rows);
	
		for(matj=0;matj<masquesource.rows;matj++)
		{	
			switch(masquesource.at<uchar>(matj,250))
			{
				case 0:
				if(flag0==0 && ecart >10)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=1;flag255=0;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
				case 255:
				if(flag255==0 && ecart >10)
				{
					//printf("%d %d \n",matj,nbporte);
					flag0=0;flag255=1;ecart=0;
					calibauto[nbporte]=matj;nbporte++;
				}break;
				
			}
		ecart++;
			
		}
		Y[0]=calibauto[2];
		Y[1]=calibauto[3];
}
void sauvegarde_calib()
{
	int i=0;
	variables = fopen("sauvegarde.txt","w+");
	
	for(i=0;i<20;i++)
	{
		fprintf(variables,"%d ",X[i]);
	}
		fprintf(variables,"%d ",Y[0]);
		fprintf(variables,"%d ",Y[1]);
	fclose(variables);
}

void calibration(void)
{
	//VideoCapture capture("http://10.2.2.29/mjpg/video.mjpg");
	//VideoCapture capture(0);
	C=0;
	namedWindow("Calibration1", CV_WINDOW_AUTOSIZE);
	namedWindow("Calibration2", CV_WINDOW_AUTOSIZE);
	
	while(waitKey(30)!=1048675 )
	{
	capture >> source;	
		createTrackbar("ligne 1","Calibration1",&X[0], 640);
		createTrackbar("ligne 2","Calibration1",&X[1], 640);
		createTrackbar("ligne 3","Calibration1",&X[2], 640);
		createTrackbar("ligne 4","Calibration1",&X[3], 640);
		createTrackbar("ligne 5","Calibration1",&X[4], 640);
		createTrackbar("ligne 6","Calibration1",&X[5], 640);
		createTrackbar("ligne 7","Calibration1",&X[6], 640);
		createTrackbar("ligne 8","Calibration1",&X[7], 640);
		createTrackbar("ligne 9","Calibration1",&X[8], 640);
		createTrackbar("ligne 10","Calibration1",&X[9], 640);
		createTrackbar("ligne 11","Calibration1",&X[10], 640);
		createTrackbar("ligne 12","Calibration1",&X[11], 640);
		createTrackbar("ligne 13","Calibration2",&X[12], 640);
		createTrackbar("ligne 14","Calibration2",&X[13], 640);
		createTrackbar("ligne 15","Calibration2",&X[14], 640);
		createTrackbar("ligne 16","Calibration2",&X[15], 640);
		createTrackbar("ligne 17","Calibration2",&X[16], 640);
		createTrackbar("ligne 18","Calibration2",&X[17], 640);
		createTrackbar("ligne 19","Calibration2",&X[18], 640);
		createTrackbar("ligne 20","Calibration2",&X[19], 640);
		createTrackbar("colonne sup","Calibration2",&Y[0], 480);
		createTrackbar("colonne inf","Calibration2",&Y[1], 480);
		createTrackbar("Quitter","Calibration2",&quitterC, 1);
	

		line(source, Point(X[0], 0), Point(X[0], 640), Scalar(255,0,0), 2);
		line(source, Point(X[1], 0), Point(X[1], 640), Scalar(255,0,0), 2);
		line(source, Point(X[2], 0), Point(X[2], 640), Scalar(255,128,0), 2);
		line(source, Point(X[3], 0), Point(X[3], 640), Scalar(255,128,0), 2);
		line(source, Point(X[4], 0), Point(X[4], 640), Scalar(0,255,0), 2);
		line(source, Point(X[5], 0), Point(X[5], 640), Scalar(0,255,0), 2);
		line(source, Point(X[6], 0), Point(X[6], 640), Scalar(0,255,255), 2);
		line(source, Point(X[7], 0), Point(X[7], 640), Scalar(0,255,255), 2);
		line(source, Point(X[8], 0), Point(X[8], 640), Scalar(127,0,255), 2);
		line(source, Point(X[9], 0), Point(X[9], 640), Scalar(127,0,255), 2);
		line(source, Point(X[10], 0), Point(X[10], 640), Scalar(255,0,127), 2);
		line(source, Point(X[11], 0), Point(X[11], 640), Scalar(255,0,127), 2);
		line(source, Point(X[12], 0), Point(X[12], 640), Scalar(255,255,0), 2);
		line(source, Point(X[13], 0), Point(X[13], 640), Scalar(255,255,0), 2);
		line(source, Point(X[14], 0), Point(X[14], 640), Scalar(128,255,0), 2);
		line(source, Point(X[15], 0), Point(X[15], 640), Scalar(128,255,0), 2);
		line(source, Point(X[16], 0), Point(X[16], 640), Scalar(0,255,128), 2);
		line(source, Point(X[17], 0), Point(X[17], 640), Scalar(0,255,128), 2);
		line(source, Point(X[18], 0), Point(X[18], 640), Scalar(0,128,255), 2);
		line(source, Point(X[19], 0), Point(X[19], 640), Scalar(0,128,255), 2);
		line(source, Point(0, Y[0]), Point(720, Y[0]), Scalar(0,0,0), 2);
		line(source, Point(0, Y[1]), Point(720, Y[1]), Scalar(0,0,0), 2);
		imshow("flux_video", source); //show the original image
		if(quitterC==1)
		{
			sauvegarde_calib();
			quitterC=0;
			setTrackbarPos("Calibration","Menu", C);
			destroyWindow("Calibration1");
			destroyWindow("Calibration2");
			break;	
		}
	}
	destroyWindow("Calibration1");
	destroyWindow("Calibration2");
	setTrackbarPos("Calibration","Menu", C);
}
void get_time()//fonction nous permettant de recuperer la date et l heure de la raspberry
{
	timeval curTime;
	gettimeofday(&curTime, NULL);
	if (seconds_last == curTime.tv_sec)
	return;
	
	seconds_last = curTime.tv_sec;
	
	strftime(DateString, 80, "%d-%m-%Y", localtime(&curTime.tv_sec));
	strftime(Jour, 80, "%d", localtime(&curTime.tv_sec));
	strftime(Minute, 80, "%M:", localtime(&curTime.tv_sec));
	strftime(HeureMinute, 80, "%H:%M", localtime(&curTime.tv_sec));
	strftime(Time,20,"%X",localtime(&curTime.tv_sec));
}
void window_create(void)// fonction creant les fenetres principales
{
	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("Menu",CV_WINDOW_NORMAL);
	
}
void passage(int i)
{
	
	deplacement[i] = (bisY[i]-LastY[i]);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement[i]>0 && LastY[i]<Y[0]+30 && flag[i]==0)
		{
			sortie=sortie+1;
			flag[i]=1;			
		}
		if(deplacement[i]<0 && LastY[i]>Y[1]-30 && flag[i]==0)
		{	
			entree=entree+1;
			flag[i]=1;
		}
	if(LastY[i]>Y[0]+30 && LastY[i]<Y[1]-30)
	{
		flag[i]=0;
	}	
	bisY[i]=LastY[i];
}


void dessinligne(int i)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop[i]);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX[i] = (M10 / Area)+X[i*2];
   	posY[i] = (M01 / Area)+Y[0];        
        
		if (LastX[i] >= 0 && LastY[i] >= 0 && posX[i] >= 0 && posY[i] >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX[i], posY[i]), Point(LastX[i], LastY[i]), Scalar(0,0,255), 2);
   		}

    	LastX[i] = posX[i];
   	LastY[i] = posY[i];
  	}
  	//imshow("flux_video", source); //show the original image
	
}



void sauvegarde_automatique() //contient aussi la sauvegarde de secours
{
	
	if(oldday!=Jour)
	{
		
		snprintf(nom,sizeof(nom),"/var/www/onebee.fr/public_html/Données/Fichiers_csv/%s.csv",DateString);///var/www/html/
		snprintf(nom2,sizeof(nom2),"Sauvegarde/Fichiers_csv/%s.csv",DateString);//on enregistre avec le fichier
		file=fopen(nom,"a+");	
		file2=fopen(nom2,"a+");
		//fprintf(file,"heure,entree,sortie\n");
		oldday=Jour;
		calib_auto();
	}

	if(oldminute!=Minute || waitKey(30)==1048673)
	{
		compteurS++;
		oldminute=Minute;

	}
	if(minuteS!=0)
	{
		if(compteurS>=minuteS)
		{
			putText(source,"Sauvegarde", Point(250,240) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,255),2,false );
			
			file=fopen(nom,"a+");	
			file2=fopen(nom2,"a+");			
			fprintf(file,"%s;%d;%d\n",HeureMinute,entree,sortie);
			fprintf(file2,"%s;%d;%d\n",HeureMinute,entree,sortie);
			fclose(file);
			fclose(file2);
			
			entree=0;sortie=0;
			compteurS=0;
			//loop=loop/60;
			//printf("%.2f fps \n",loop);
			//loop=0;
			PassMinut=1;
		}
	}

}
void menu()
{
	im_menu = imread( "logo.jpg");	
	printf("#########################################\n");
	printf("#########################################\n");
	printf("##                MENU                 ##\n");
	printf("#########################################\n");
	printf("##Une fenetre avec des interrupteurs   ##\n");
	printf("##est a votre disposition modifiez son ##\n");
	printf("##etat pour entrer dans le menu desiré ##\n");
	printf("#########################################\n");
	printf("#########################################\n");

	createTrackbar("Calibration","Menu",&C, 1);
	createTrackbar("Video","Menu",&video, 3);
	createTrackbar("Minutes entre sauvegardes","Menu",&minuteS, 60);
	createTrackbar("Quitter","Menu",&Q, 1);
	imshow("Menu",im_menu);
	moveWindow("Menu",0,0);
	
}
void initialisation()
{
	printf("########################################\n");
	printf("##           Projet  OneBee           ##\n");
	printf("########################################\n");
	printf("##                MIND                ##\n");
	printf("########################################\n");
	printf("## crée   par: Jean-Camille Lebreton  ##\n");
	printf("##                                    ##\n");
	printf("########################################\n");
	printf("##          Annee  2015-2017          ##\n");
	printf("########################################\n");
	printf("########################################\n");
	printf("########################################\n");
	printf("######################  Version 2.2   ##\n");
	printf("########################################\n");
	printf("\n\n\n");
	menu();
}
		
void suppressbruit(Mat Pic)
{
	//---------------------------------------------------------------------------------------//
	// ce paquet permettent d'enlever tout les bruits résiduels qui pourrait etre interprétés comme une autre couleur que le noir
	//morphological opening (removes small objects from the foreground) 
  		erode(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  		dilate(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  	 //morphological closing (removes small holes from the foreground)
  		dilate(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  		erode(Pic, Pic, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	//---------------------------------------------------------------------------------------//
}
void recall()
{	
	int c=0,i=0,j=0,compteur_recall=0,test=0,indice=0;

	variables=fopen("sauvegarde.txt","r");
	if (variables == NULL)
	{
		printf("Chargement des données impossible, valeurs par defaut... recall\n");
		return;
	}
	do{
		c=(int)getc(variables);
		T_variable[i]=c-48;
		i++;
	}while (c!=EOF);
	for(j=0;j<i;j++)
	{
		
		if(T_variable[j]==-16)
		{
			if(compteur_recall == 2)
			{
				test = T_variable[j-compteur_recall]*10 + T_variable[j-compteur_recall+1];
				j++;
			}
			if(compteur_recall == 3)
			{
				test = T_variable[j-compteur_recall]*100 + T_variable[j-compteur_recall+1]*10 + T_variable[j-compteur_recall+2];
				j++;
			}
			compteur_recall=0;
			if(indice<20)
			{
				X[indice] = test;
			}
			else
			{
				Y[indice-20]= test;
			}
			indice++;
		}	
		
		compteur_recall++;
	}
	fclose(variables);
}

void sauvegarde_graphique()
{
	int cara=0,i=0,j=0,compteur_recall=-1,indice=0,test=0;
	char Heure[5];
	

	graph=fopen(nom2,"r");
	if (variables == NULL)
	{
		printf("Chargement des données impossible, valeurs par defaut... data\n");
		return;
	}
	
	if(PassMinut == 1)
	{
		Graph = imread("fond_graphique.jpg");

		PassMinut=0;
		do{
			cara = (int)getc(graph);
			T_raw[i] = cara - 48;		
			i++;	
		}while(cara!=EOF);
	
	
		for(j=0;j<i;j++)
		{
			//printf("%d = recall 	%d =j",compteur_recall,j);//debug
			//printf("T_raw[%d]=%d \n",j,T_raw[j]);//debug
			compteur_recall++;
			if((T_raw[j]==11) || (T_raw[j] == -38) || (T_raw[j] == -49))
			{	
			
				if(compteur_recall == 1)
				{
					test = T_raw[j-compteur_recall];
					//printf("recall = 1\n val=%d \n",test);//debug
				}
				if(compteur_recall == 2)
				{
					test = T_raw[j-compteur_recall]*10 + T_raw[j-compteur_recall+1];
					//printf("recall = 2 val=%d \n",test);//debug
				}
				if(compteur_recall == 3)
				{
					test = T_raw[j-compteur_recall]*100 + T_raw[j-compteur_recall+1]*10 + T_raw[j-compteur_recall+2];
					//printf("recall = 3 val=%d \n",test);//debug
				}
				if(compteur_recall == 5)
				{
					test = T_raw[j-compteur_recall]*600 + T_raw[j-compteur_recall+1]*60 + T_raw[j-compteur_recall+3]*10 + T_raw[j-compteur_recall+4];
					//printf("recall = 5 val=%d \n",test);//debug
				}
				compteur_recall=-1;
				T_data[indice] = test;
				indice++;
			}
			
		}
	fclose(graph);
	
		j=0;
		do{
			line(Graph,Point(T_data[j]+50,180),Point(T_data[j]+50,180-T_data[j+1]),Scalar(0,0,255),1);
			line(Graph,Point(T_data[j]+50,400),Point(T_data[j]+50,400-T_data[j+2]),Scalar(0,255,0),1);

			line(Graph,Point(50,180),Point(1490,180),Scalar(0,0,0),2);//horizontal entree
			line(Graph,Point(50,180),Point(50,25),Scalar(0,0,0),2);//vertical entree

			line(Graph,Point(50,400),Point(1490,400),Scalar(0,0,0),2);//horizontal sortie
			line(Graph,Point(50,400),Point(50,250),Scalar(0,0,0),2);//vertical sortie
			
			//les deux prochaines lignes sont pour l'affichage du texte sur le graphique
			putText(Graph,"entree", Point(5,15) , FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 1,false );
			putText(Graph,"sortie", Point(5,240) , FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 1,false );
			//les prochaines lignes sont la pour l'affichage de l'heure sur l axe des ordonnées
			for(i=0;i<24;i++)
			{	
				sprintf(Heure,"%d:00",i);
				putText(Graph,Heure, Point(50+60*i,190) , FONT_HERSHEY_SIMPLEX, 0.2, Scalar(0,0,0), 1,false );
			}
			for(i=0;i<24;i++)
			{	
				sprintf(Heure,"%d:00",i);
				putText(Graph,Heure, Point(50+60*i,410) , FONT_HERSHEY_SIMPLEX, 0.2, Scalar(0,0,0), 1,false );
			}
			//printf("T_data[j]= %d ;T_data[j+1]= %d ;T_data[j+2]= %d \n",T_data[j],T_data[j+1],T_data[j+2]);
			j=j+3;
		}while(j<4321);
		
		snprintf(image,sizeof(image),"Sauvegarde/Graphiques/Graphique_du_%s.jpeg",DateString);
		imwrite(image,Graph);
		snprintf(image2,sizeof(image2),"/var/www/onebee.fr/public_html/Données/Graphiques/Graphique_du_%s.jpeg",DateString);
		imwrite(image2,Graph);

	}
	
	//line(src,Point(10,10),Point(10,100),Scalar(0,0,255),1);
	


}

int main(int argc, char **argv)
{	
	int i=0;
	recall();


	//initialisation de la video pour la camera IP
	//VideoCapture capture("http://10.2.2.29/mjpg/video.mjpg");
	
	
	if(!capture.isOpened()){
	printf("impossible d'initialiser le flux video\n verifiez les branchements");
	return -1;
	}
	
	window_create();
	menu();
	while(1)
{	
	capture >> source ;
	get_time();
	//loop++;
	//on decoupe l'image principale pour ne garder que les parties qui nous interessent

	//calib_auto();

	for(i=0;i<10;i++)
	{
		cropedImage[i] = source(Rect(X[i*2],Y[0],X[i*2+1]-X[i*2],Y[1]-Y[0]));
			//on converti les images en HSV
		cvtColor(cropedImage[i],hsvcrop[i],CV_BGR2HSV);
			//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
		inRange(hsvcrop[i],Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop[i]);
			// NOTE : parametres du noir : (lowH=0 lowS=0 lowV=0, HighH=178 highS=255 highV=84 ! pour la detection d'abeille
			//on supprime le bruit de la video (fonction)
		suppressbruit(masquecrop[i]);
			//fonction de comptage d entrée sortie
		passage(i);
			//on trace les vecteurs
		dessinligne(i);
	}
	
	
	/*
	//on affiche ces images rognées (bebug)
	imshow("cropedImage1", masquecrop[0]);
	imshow("cropedImage2", masquecrop[1]);
	imshow("cropedImage3", masquecrop[2]);
	imshow("cropedImage4", masquecrop[3]);
	imshow("cropedImage5", masquecrop[4]);
	imshow("cropedImage6", masquecrop[5]);
	imshow("cropedImage7", masquecrop[6]);
	imshow("cropedImage8", masquecrop[7]);
	imshow("cropedImage9", masquecrop[8]);
	imshow("cropedImage10", masquecrop[9]);
	
	
	
	
	//affiche le masque de l'image d'origine
	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquesource);
	suppressbruit(masquesource);
	imshow("masque",masquesource);*/

	sauvegarde_automatique();
	sauvegarde_graphique();

	switch(video)
	{
	case 0:
		
		destroyWindow("flux_video");	
		break;
	case 1 :
			
		namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
		putText(source,Time, Point(10,440) , FONT_HERSHEY_SIMPLEX, .3, Scalar(0,255,255),1,false );
		sprintf(name,"Entrees :%d",entree);
    		putText(source,name, Point(10,130) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
		sprintf(name,"Sorties :%d",sortie);
    		putText(source,name, Point(10,190) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
		imshow("flux_video", source);
		break;
	case 2 :
		src = imread( "image1.jpg");
		namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
		sprintf(name,"Entrees :%d",entree);
    		putText(src,name, Point(20,200) , FONT_HERSHEY_SIMPLEX, 3, Scalar(255,0,0), 2,8,false );
		sprintf(name,"Sorties :%d",sortie);
    		putText(src,name, Point(20,380) , FONT_HERSHEY_SIMPLEX, 3, Scalar(255,0,0), 2,8,false );
		imshow("flux_video", src);
		break;
	case 3 :
		
		imshow("flux_video", Graph);
		break;	
	}
	
	if(C==1)
	{
		calibration();
	}
	if(Q==1)
	{	
	
		cout << "Quitter par demande de l'utilisateur" << endl;
    	    	destroyAllWindows();
	    	fclose(file);
	    	return 0;
	}
				
}
	

}