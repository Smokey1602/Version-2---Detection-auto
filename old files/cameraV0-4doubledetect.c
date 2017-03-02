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
//--------DECLARATION DES VARIABLES --------------------------
Mat hsvcrop1,hsvcrop2,source,masquecrop1,masquecrop2,cropedImage1,cropedImage2;
int LowH =0,HighH =178,LowS =0,HighS=255,LowV=0,HighV=80;
vector<Vec3f> cercles;
Mat Lignes = Mat::zeros( source.size(), CV_8UC3 );
int LastX1=-1,LastY1 = -1,posX1,posY1;
int LastX2=-1,LastY2 = -1,posX2,posY2;
int entree=0,sortie=0;
int bisY1=0,bisY2=0;
int tempo=0;
int flag=0;
int input=0;
static int seconds_last = 99;
char DateString[20],HeureString[20];
char sauvegarde=1;
char heure=0,oldheure=0;
FILE *file; //fichier de sortie des detections
//------------------------------------------------------------
void get_time()
{
	timeval curTime;
	gettimeofday(&curTime, NULL);
	if (seconds_last == curTime.tv_sec)
		return;
	
	seconds_last = curTime.tv_sec;
	
	strftime(DateString, 80, "%Y-%m-%d", localtime(&curTime.tv_sec));
	strftime(HeureString, 80, "%H:%M:%S", localtime(&curTime.tv_sec));
	heure=10*HeureString[0]+HeureString[1];
}
void window_create(void)
{
	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("maskcroped1", CV_WINDOW_AUTOSIZE);
	namedWindow("maskcroped2", CV_WINDOW_AUTOSIZE);
}
void passage1(void)
{
	int deplacement1=0;
	
	deplacement1 = (bisY1-LastY1);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement1>0 && LastY1<50 && flag==0)
		{
			entree=entree+1;
			flag=1;			
		}
		if(deplacement1<0 && LastY1>430&&flag==0)
		{	
			sortie=sortie+1;
			flag=1;
		}
	if(LastY1>50 && LastY1<430)
	{
		flag=0;
	}	
	bisY1=LastY1;
}
void passage2(void)
{
	int deplacement2=0;
	
	deplacement2 = (bisY2-LastY2);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement2>0 && LastY2<50 && flag==0)
		{
			entree=entree+1;
			flag=1;			
		}
		if(deplacement2<0 && LastY2>430&&flag==0)
		{	
			sortie=sortie+1;
			flag=1;
		}
	if(LastY2>50 && LastY2<430)
	{
		flag=0;
	}	
	bisY2=LastY2;
}
void dessinligne1(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop1);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX1 = M10 / Area;
   	posY1 = M01 / Area;        
        
		if (LastX1 >= 0 && LastY1 >= 0 && posX1 >= 0 && posY1 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX1, posY1), Point(LastX1, LastY1), Scalar(0,0,255), 2);
   		}

    	LastX1 = posX1;
   	LastY1 = posY1;
  	}
  	imshow("flux_video", source); //show the original image
	
}
void dessinligne2(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop2);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX2 = (M10 / Area) +320;
   	posY2 = M01 / Area;        
        
		if (LastX2 >= 0 && LastY2 >= 0 && posX2 >= 0 && posY2 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX2, posY2), Point(LastX2, LastY2), Scalar(0,0,255), 2);
   		}

    	LastX2 = posX2;
   	LastY2 = posY2;
  	}
  	imshow("flux_video", source); //show the original image
	
}
void sauvegarde_manuelle()
{
	char nom[20];
	snprintf(nom,sizeof(nom),"%s.txt",DateString);

	printf("sauvegarde ... \n");
	
	file=fopen(nom,"w+");
	fprintf(file,"le %s\n entrées:%d \n sorties:%d\n",HeureString,entree,sortie);
	entree=0;sortie=0;
	printf("sauvegarde reussie");
	

}
void sauvegarde_automatique()
{
	if(oldheure!=heure)
	{
		oldheure=heure;	

		char nom[20];
		snprintf(nom,sizeof(nom),"%s.txt",DateString);
		printf("sauvegarde ... \n");
		
		file=fopen(nom,"w+");
		fprintf(file,"le %s\n entrées:%d \n sorties:%d\n",HeureString,entree,sortie);
		entree=0;sortie=0;
		waitKey(100);
		printf("sauvegarde reussie");
	}

}
void menu()
{
	printf("#########################################\n");
	printf("##                MENU                 ##\n");
	printf("#########################################\n");
	printf("##Le chiffre correspond a la touche a  ##\n");
	printf("##appuyer pour changer la configuration##\n");
	printf("#########################################\n");
	printf("##a: sauvegarde manuelle               ##\n");
	printf("##z: sauvegarde automatique            ##\n");
	printf("#########################################\n");
	printf("##c: reprendre le programme            ##\n");
	printf("#########################################\n");
	printf("#########################################\n");
	printf("##echap pour quitter                   ##\n");
	printf("#########################################\n");
	while(waitKey(30)!=1048675)
	{
		if(waitKey(30)==1048673)
		{
			sauvegarde=1;
			printf("mode manuel\n tapez c pour continuer \n");
		}
		if(waitKey(30)==1048698)
		{
			sauvegarde=0;
			printf("mode automatique\n tapez c pour continuer \n");
		}
	}
	
}
void initialisation()
{
	printf("########################################\n");
	printf("##           Projet  OneBee           ##\n");
	printf("##    Universite Savoie Mont Blanc    ##\n");
	printf("########################################\n");
	printf("##         Proposé par Mind           ##\n");
	printf("########################################\n");
	printf("## Realisé par: Jean-Camille Lebreton ##\n");
	printf("##                                    ##\n");	
	printf("########################################\n");
	printf("##          Groupe OneBee  2          ##\n");
	printf("########################################\n");
	printf("##          Annee  2015-2016          ##\n");
	printf("########################################\n");
	printf("########################################\n");
	printf("########################################\n");
	printf("######################  Version 0.4   ##\n");
	printf("########################################\n");
	printf("\n\n\n");
	
	printf("Touches utiles :\n");
	printf("echap pour quitter le programme \n ");
	printf("appuyez a tout moment sur 'm' pour le menu\n");
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
int main(int argc, char **argv)
{	
	//initialisation de la video
	VideoCapture capture(0);
	if(!capture.isOpened()){
	printf("impossible d'initialiser le flux video\n verifiez les branchements");
	return -1;
	}

	window_create();	
	initialisation();
	while(1)
{	
	get_time();
	//on recupere les images au fur et a mesure
	capture >> source;
	
	//image rognée nous permettant pour le moment de couper l image generale en 2
	cropedImage1 = source(Rect(0,0,320,460));
	cropedImage2 = source(Rect(320,0,320,460));

	//on affiche ces images rognées
	//imshow("croped1", cropedImage1);
	//imshow("croped2", cropedImage2);		

	//on converti en hsv
	cvtColor(cropedImage1,hsvcrop1,CV_BGR2HSV);
	cvtColor(cropedImage2,hsvcrop2,CV_BGR2HSV);

	//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
	inRange(hsvcrop1,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop1);
	inRange(hsvcrop2,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop2);

	// NOTE : couleurs du noir : (lowH=0 lowS=0 lowV=0, HighH=178 highS=255 highV=84 ! pas pour la detection d'abeille
	
	suppressbruit(masquecrop1);
	suppressbruit(masquecrop2);

	//on affiche l image finale
	imshow("maskcroped2",masquecrop2);
	imshow("maskcroped1",masquecrop1);
	
	dessinligne1();
	dessinligne2();


	passage1(); //fonction de comptage des entrées sorties
	passage2();

	//printf("entree:%d; sortie:%d \n",entree,sortie);
	//printf("%d",waitKey(30));
	if((waitKey(30)==1048691 || waitKey(30)==1048659) && sauvegarde==1)
	{
		sauvegarde_manuelle();

	}
	if(sauvegarde ==0)
	{
		sauvegarde_automatique();
	}
	 if (waitKey(30) == 1048603) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "Quitter par demande de l'utilisateur" << endl;
    	    destroyAllWindows();
			fclose(file);
	    break;
	    
       }
	if (waitKey(30) == 1048685)
	{
		menu();
	}
}
	

}