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

//--------DECLARATION DES VARIABLES -------------------------------------------------------

char kalib=0; //variable flag pour la calibration
int X1=1,X2=20,X3=30,X4=40,X5=50,X6=60,X7=70,X8=80,X9=90,X10=100,X11=110,X12=120,X13=130,X14=140,X15=150,X16=160,X17=170,X18=180,X19=190,X20=200;
int Y1=10,Y2=20;
int test=0,test1=0,test2=0;
int Q=0,S=0,C=0,video=1;
int quitterC=0;

//---------Creation de toutes les images--------------------
Mat hsvcrop1,hsvcrop2,hsvcrop3,hsvcrop4,hsvcrop5,hsvcrop6,hsvcrop7,hsvcrop8,hsvcrop9,hsvcrop10;
Mat source,hsvsource,masquesource,src;
Mat masquecrop1,masquecrop2,masquecrop3,masquecrop4,masquecrop5,masquecrop6,masquecrop7,masquecrop8,masquecrop9,masquecrop10;
Mat cropedImage1,cropedImage2,cropedImage3,cropedImage4,cropedImage5,cropedImage6,cropedImage7,cropedImage8,cropedImage9,cropedImage10;

//////////////////////////////////////////////////////////////:
//------------Creation de toutes les variables-----------------
int LowH =0,HighH =178,LowS =0,HighS=255,LowV=0,HighV=80; // permet la detection du noir
Mat Lignes = Mat::zeros( source.size(), CV_8UC3 ); //permet le dessin de lignes (pr le debug)
// ---------- declaration de toutes les variables de chaque image crées --------
int LastX1=-1,LastY1 = -1,posX1,posY1;
int LastX2=-1,LastY2 = -1,posX2,posY2;
int LastX3=-1,LastY3 = -1,posX3,posY3;
int LastX4=-1,LastY4 = -1,posX4,posY4;
int LastX5=-1,LastY5 = -1,posX5,posY5;
int LastX6=-1,LastY6 = -1,posX6,posY6;
int LastX7=-1,LastY7 = -1,posX7,posY7;
int LastX8=-1,LastY8 = -1,posX8,posY8;
int LastX9=-1,LastY9 = -1,posX9,posY9;
int LastX10=-1,LastY10 = -1,posX10,posY10;
int flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0,flag9=0,flag10=0;
int numsauv=0;
/////////////////////////////////////////
int bisY1=0,bisY2=0,bisY3=0,bisY4=0,bisY5=0,bisY6=0,bisY7=0,bisY8=0,bisY9=0,bisY10=0;
///////////////variables communes au programme///////////
int entree=0,sortie=0;
char name[30];
VideoCapture capture(0);

// --------- variables pour recupere l'heure permettant la sauvegarde -------
static int seconds_last = 99;
char DateString[20],Jour[20],Minute[20],HeureMinute[20];
string oldday="\0",oldminute="\0";
FILE *file; //fichier de sortie des detections
char nom[20];
int quitterS=0,minuteS=1,compteurS=0;


//-----------------FIN DE LA DECLARATION DES VARIABLES ---------------------------------------------------------


void calibration(void)
{	
	C=0;
	namedWindow("Calibration1", CV_WINDOW_AUTOSIZE);
	namedWindow("Calibration2", CV_WINDOW_AUTOSIZE);
	
	while(waitKey(30)!=1048675)
	{
	capture >> source;
		sprintf(name,"appuyez sur 'c' pour quitter");
    		putText(source,name, Point(10,130) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,255), 2,8,false );
		

		createTrackbar("ligne 1","Calibration1",&X1, 640);
		createTrackbar("ligne 2","Calibration1",&X2, 640);
		createTrackbar("ligne 3","Calibration1",&X3, 640);
		createTrackbar("ligne 4","Calibration1",&X4, 640);
		createTrackbar("ligne 5","Calibration1",&X5, 640);
		createTrackbar("ligne 6","Calibration1",&X6, 640);
		createTrackbar("ligne 7","Calibration1",&X7, 640);
		createTrackbar("ligne 8","Calibration1",&X8, 640);
		createTrackbar("ligne 9","Calibration1",&X9, 640);
		createTrackbar("ligne 10","Calibration1",&X10, 640);
		createTrackbar("ligne 11","Calibration1",&X11, 640);
		createTrackbar("ligne 12","Calibration1",&X12, 640);
		createTrackbar("ligne 13","Calibration2",&X13, 640);
		createTrackbar("ligne 14","Calibration2",&X14, 640);
		createTrackbar("ligne 15","Calibration2",&X15, 640);
		createTrackbar("ligne 16","Calibration2",&X16, 640);
		createTrackbar("ligne 17","Calibration2",&X17, 640);
		createTrackbar("ligne 18","Calibration2",&X18, 640);
		createTrackbar("ligne 19","Calibration2",&X19, 640);
		createTrackbar("ligne 20","Calibration2",&X20, 640);
		createTrackbar("colonne sup","Calibration2",&Y1, 480);
		createTrackbar("colonne inf","Calibration2",&Y2, 480);
		createTrackbar("Quitter","Calibration2",&quitterS, 1);
	

		line(source, Point(X1, 0), Point(X1, 640), Scalar(255,0,0), 2);
		line(source, Point(X2, 0), Point(X2, 640), Scalar(255,128,0), 2);
		line(source, Point(X3, 0), Point(X3, 640), Scalar(255,255,0), 2);
		line(source, Point(X4, 0), Point(X4, 640), Scalar(128,255,0), 2);
		line(source, Point(X5, 0), Point(X5, 640), Scalar(0,255,0), 2);
		line(source, Point(X6, 0), Point(X6, 640), Scalar(0,255,128), 2);
		line(source, Point(X7, 0), Point(X7, 640), Scalar(0,255,255), 2);
		line(source, Point(X8, 0), Point(X8, 640), Scalar(0,128,255), 2);
		line(source, Point(X9, 0), Point(X9, 640), Scalar(127,0,255), 2);
		line(source, Point(X10, 0), Point(X10, 640), Scalar(255,0,255), 2);
		line(source, Point(X11, 0), Point(X11, 640), Scalar(255,0,127), 2);
		line(source, Point(X12, 0), Point(X12, 640), Scalar(255,0,0), 2);
		line(source, Point(X13, 0), Point(X13, 640), Scalar(255,128,0), 2);
		line(source, Point(X14, 0), Point(X14, 640), Scalar(255,255,0), 2);
		line(source, Point(X15, 0), Point(X15, 640), Scalar(128,255,0), 2);
		line(source, Point(X16, 0), Point(X16, 640), Scalar(0,255,0), 2);
		line(source, Point(X17, 0), Point(X17, 640), Scalar(0,255,128), 2);
		line(source, Point(X18, 0), Point(X18, 640), Scalar(0,255,255), 2);
		line(source, Point(X19, 0), Point(X19, 640), Scalar(0,128,255), 2);
		line(source, Point(X20, 0), Point(X20, 640), Scalar(127,0,255), 2);
		line(source, Point(0, Y1), Point(720, Y1), Scalar(255,0,255), 2);
		line(source, Point(0, Y2), Point(720, Y2), Scalar(255,0,127), 2);
		imshow("flux_video", source); //show the original image
		if(quitterC==1)
		{
			quitterC=0;
			break;
			destroyWindow("Calibration1");
			destroyWindow("Calibration2");
			setTrackbarPos("Calibration","Menu", C);
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
	
	strftime(DateString, 80, "%Y-%m-%d", localtime(&curTime.tv_sec));
	strftime(Jour, 80, "%d", localtime(&curTime.tv_sec));
	strftime(Minute, 80, "%M:", localtime(&curTime.tv_sec));
	strftime(HeureMinute, 80, "%H:%M", localtime(&curTime.tv_sec));
}
void window_create(void)// fonction creant les fenetres
{
	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("Menu", CV_WINDOW_AUTOSIZE);
}
void passage1(void)
{
	int deplacement1=0;
	
	deplacement1 = (bisY1-LastY1);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement1>0 && LastY1<Y1+18 && flag1==0)
		{
			sortie=sortie+1;
			flag1=1;			
		}
		if(deplacement1<0 && LastY1>Y2-18 && flag1==0)
		{	
			entree=entree+1;
			flag1=1;
		}
	if(LastY1>Y1+18 && LastY1<Y2-18)
	{
		flag1=0;
	}	
	bisY1=LastY1;
}
void passage2(void)
{
	int deplacement2=0;
	
	deplacement2 = (bisY2-LastY2);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement2>0 && LastY2<Y1+18 && flag2==0)
		{
			sortie=sortie+1;
			flag2=1;			
		}
		if(deplacement2<0 && LastY2>Y2-18 && flag2==0)
		{	
			entree=entree+1;
			flag2=1;
		}
	if(LastY2>Y1+18 && LastY2<Y2-18)
	{
		flag2=0;
	}	
	bisY2=LastY2;
}
void passage3(void)
{
	int deplacement3=0;
	
	deplacement3 = (bisY3-LastY3);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement3>0 && LastY3<Y1+18 && flag3==0)
		{
			sortie=sortie+1;
			flag3=1;			
		}
		if(deplacement3<0 && LastY3>Y2-18 && flag3==0)
		{	
			entree=entree+1;
			flag3=1;
		}
	if(LastY3>Y1+18 && LastY3<Y2-18)
	{
		flag3=0;
	}	
	bisY3=LastY3;
}
void passage4(void)
{
	int deplacement4=0;
	
	deplacement4 = (bisY4-LastY4);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement4>0 && LastY4<Y1+18 && flag4==0)
		{
			sortie=sortie+1;
			flag4=1;			
		}
		if(deplacement4<0 && LastY4>Y2-18 && flag4==0)
		{	
			entree=entree+1;
			flag4=1;
		}
	if(LastY4>Y1+18 && LastY4<Y2-18)
	{
		flag4=0;
	}	
	bisY4=LastY4;
}
void passage5(void)
{
	int deplacement5=0;
	
	deplacement5 = (bisY5-LastY5);
	//printf("deplacement5 :%d; bisY5:%d; LastX5:%d;\n",deplacement5,bisY5,LastX5);

		if(deplacement5>0 && LastY5<Y1+30 && flag5==0)
		{
			sortie=sortie+1;
			flag5=1;			
		}
		if(deplacement5<0 && LastY5>Y2-18 && flag5==0)
		{	
			entree=entree+1;
			flag5=1;
		}
	if(LastY5>Y1+18 && LastY5<Y2-18)
	{
		flag5=0;
	}	
	bisY5=LastY5;
}
void passage6(void)
{
	int deplacement6=0;
	
	deplacement6 = (bisY6-LastY6);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement6>0 && LastY6<Y1+18 && flag6==0)
		{
			sortie=sortie+1;
			flag6=1;			
		}
		if(deplacement6<0 && LastY6>Y2-18 && flag6==0)
		{	
			entree=entree+1;
			flag6=1;
		}
	if(LastY6>Y1+18 && LastY6<Y2-18)
	{
		flag6=0;
	}	
	bisY6=LastY6;
}
void passage7(void)
{
	int deplacement7=0;
	
	deplacement7 = (bisY7-LastY7);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement7>0 && LastY7<Y1+18 && flag7==0)
		{
			sortie=sortie+1;
			flag7=1;			
		}
		if(deplacement7<0 && LastY7>Y2-18 && flag7==0)
		{	
			entree=entree+1;
			flag7=1;
		}
	if(LastY7>Y1+18 && LastY7<Y2-18)
	{
		flag7=0;
	}	
	bisY7=LastY7;
}
void passage8(void)
{
	int deplacement8=0;
	
	deplacement8 = (bisY8-LastY8);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement8>0 && LastY8<Y1+18 && flag8==0)
		{
			sortie=sortie+1;
			flag8=1;			
		}
		if(deplacement8<0 && LastY8>Y2-18 && flag8==0)
		{	
			entree=entree+1;
			flag8=1;
		}
	if(LastY8>Y1+18 && LastY8<Y2-18)
	{
		flag8=0;
	}	
	bisY8=LastY8;
}
void passage9(void)
{
	int deplacement9=0;
	
	deplacement9 = (bisY9-LastY9);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement9>0 && LastY9<Y1+18 && flag9==0)
		{
			sortie=sortie+1;
			flag9=1;			
		}
		if(deplacement9<0 && LastY9>Y2-18 && flag9==0)
		{	
			entree=entree+1;
			flag9=1;
		}
	if(LastY9>Y1+18 && LastY9<Y2-18)
	{
		flag9=0;
	}	
	bisY9=LastY9;
}
void passage10(void)
{
	int deplacement10=0;
	
	deplacement10 = (bisY10-LastY10);
	//printf("deplacement :%d; bisY:%d; LastX:%d;\n",deplacement,bisY,LastX);

		if(deplacement10>0 && LastY10<Y1+18 && flag10==0)
		{
			sortie=sortie+1;
			flag10=1;			
		}
		if(deplacement10<0 && LastY10>Y2-18 && flag10==0)
		{	
			entree=entree+1;
			flag10=1;
		}
	if(LastY10>Y1+18 && LastY10<Y2-18)
	{
		flag10=0;
	}	
	bisY10=LastY10;
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
   	posX1 = M10 / Area+X1;
   	posY1 = (M01 / Area)+Y1;        
        
		if (LastX1 >= 0 && LastY1 >= 0 && posX1 >= 0 && posY1 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX1, posY1), Point(LastX1, LastY1), Scalar(0,0,255), 2);
   		}

    	LastX1 = posX1;
   	LastY1 = posY1;
  	}
  	//imshow("flux_video", source); //show the original image
	
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
   	posX2 = (M10 / Area) +X3;
   	posY2 = (M01 / Area)+Y1;        
        
		if (LastX2 >= 0 && LastY2 >= 0 && posX2 >= 0 && posY2 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX2, posY2), Point(LastX2, LastY2), Scalar(0,0,255), 2);
   		}

    	LastX2 = posX2;
   	LastY2 = posY2;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne3(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop3);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX3 = (M10 / Area) +X5;
   	posY3 = (M01 / Area)+Y1;        
        
		if (LastX3 >= 0 && LastY3 >= 0 && posX3 >= 0 && posY3 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX3, posY3), Point(LastX3, LastY3), Scalar(0,0,255), 2);
   		}

    	LastX3 = posX3;
   	LastY3 = posY3;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne4(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop4);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX4 = (M10 / Area) +X7;
   	posY4 = (M01 / Area)+Y1;        
        
		if (LastX4 >= 0 && LastY4 >= 0 && posX4 >= 0 && posY4 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX4, posY4), Point(LastX4, LastY4), Scalar(0,0,255), 2);
   		}

    	LastX4 = posX4;
   	LastY4 = posY4;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne5(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop5);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX5 = (M10 / Area) +X9;
   	posY5 = (M01 / Area)+Y1;        
        
		if (LastX5 >= 0 && LastY5 >= 0 && posX5 >= 0 && posY5 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX5, posY5), Point(LastX5, LastY5), Scalar(0,0,255), 2);
   		}

    	LastX5 = posX5;
   	LastY5 = posY5;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne6(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop6);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX6 = (M10 / Area) +X11;
   	posY6 = (M01 / Area)+Y1;        
        
		if (LastX6 >= 0 && LastY6 >= 0 && posX6 >= 0 && posY6 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX6, posY6), Point(LastX6, LastY6), Scalar(0,0,255), 2);
   		}

    	LastX6 = posX6;
   	LastY6 = posY6;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne7(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop7);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX7 = (M10 / Area) +X13;
   	posY7 = (M01 / Area)+Y1;        
        
		if (LastX7 >= 0 && LastY7 >= 0 && posX7 >= 0 && posY7 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX7, posY7), Point(LastX7, LastY7), Scalar(0,0,255), 2);
   		}

    	LastX7 = posX7;
   	LastY7 = posY7;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne8(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop8);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX8 = (M10 / Area) +X15;
   	posY8 = (M01 / Area)+Y1;        
        
		if (LastX8 >= 0 && LastY8 >= 0 && posX8 >= 0 && posY8 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX8, posY8), Point(LastX8, LastY8), Scalar(0,0,255), 2);
   		}

    	LastX8 = posX8;
   	LastY8 = posY8;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne9(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop9);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX9 = (M10 / Area) +X17;
   	posY9 = (M01 / Area)+Y1;        
        
		if (LastX9 >= 0 && LastY9 >= 0 && posX9 >= 0 && posY9 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX9, posY9), Point(LastX9, LastY9), Scalar(0,0,255), 2);
   		}

    	LastX9 = posX9;
   	LastY9 = posY9;
  	}
  	//imshow("flux_video", source); //show the original image
	
}
void dessinligne10(void)//dessine les lignes pour suivi d objet
{
	Moments Moments = moments(masquecrop10);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX10 = (M10 / Area) +X19;
   	posY10 = (M01 / Area)+Y1;        
        
		if (LastX10 >= 0 && LastY10 >= 0 && posX10 >= 0 && posY10 >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX10, posY10), Point(LastX10, LastY10), Scalar(0,0,255), 2);
   		}

    	LastX10 = posX10;
   	LastY10 = posY10;
  	}
  	//imshow("flux_video", source); //show the original image
	
}


void sauvegarde_automatique()
{
	
	if(oldday!=Jour)
	{
		
		snprintf(nom,sizeof(nom),"%s.txt",DateString);
		file=fopen(nom,"w+");
		fprintf(file,"heure	entree	sortie \n");
		oldday=Jour;
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
			printf("sauvegarde ... \n");
			file=fopen(nom,"a");
			fprintf(file,"%s	%d	%d \n",HeureMinute,entree,sortie);
			entree=0;sortie=0;
			fclose(file);
			compteurS=0;	
		}
	}

}
void menu()
{
	printf("#########################################\n");
	printf("##                MENU                 ##\n");
	printf("#########################################\n");
	printf("##Une fenetre avec des interrepteurs   ##\n");
	printf("##est a votre disposition modifiez son ##\n");
	printf("##etat pour entrer dans le menu desiré ##\n");
	printf("#########################################\n");
	printf("#########################################\n");

	createTrackbar("Calibration","Menu",&C, 1);
	createTrackbar("Quitter","Menu",&Q, 1);
	createTrackbar("Sauvegarde","Menu",&S, 1);
	createTrackbar("Video","Menu",&video, 2);
	createTrackbar("Minute entre sauvegardes","Menu",&minuteS, 60);

	
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
	printf("######################  Version 1.1   ##\n");
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
	
	if(!capture.isOpened()){
	printf("impossible d'initialiser le flux video\n verifiez les branchements");
	return -1;
	}
	
	window_create();
	initialisation();
	while(1)
{	
	capture >> source;
	switch(video)
	{
		case 0:
		
			destroyWindow("flux_video");	
		break;
		case 1 :
		
			
			namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
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
    			putText(src,name, Point(20,200) , FONT_HERSHEY_SIMPLEX, 4, Scalar(255,0,0), 2,8,false );
			sprintf(name,"Sorties :%d",sortie);
    			putText(src,name, Point(20,380) , FONT_HERSHEY_SIMPLEX, 4, Scalar(255,0,0), 2,8,false );
			imshow("flux_video", src);
		break;
		
	}
	get_time();
	//on recupere les images au fur et a mesure
	
	
	//image rognée nous permettant pour le moment de couper l image generale en 2
	cropedImage1 = source(Rect(X1,Y1,X2-X1,Y2-Y1));
	cropedImage2 = source(Rect(X3,Y1,X4-X3,Y2-Y1));
	cropedImage3 = source(Rect(X5,Y1,X6-X5,Y2-Y1));
	cropedImage4 = source(Rect(X7,Y1,X8-X7,Y2-Y1));
	cropedImage5 = source(Rect(X9,Y1,X10-X9,Y2-Y1));
	cropedImage6 = source(Rect(X11,Y1,X12-X11,Y2-Y1));
	cropedImage7 = source(Rect(X13,Y1,X14-X13,Y2-Y1));
	cropedImage8 = source(Rect(X15,Y1,X16-X15,Y2-Y1));
	cropedImage9 = source(Rect(X17,Y1,X18-X17,Y2-Y1));
	cropedImage10 = source(Rect(X19,Y1,X20-X19,Y2-Y1));

	//on affiche ces images rognées (bebug)

	/*imshow("croped1", cropedImage1);
	imshow("croped2", cropedImage2);
	imshow("croped3", cropedImage3);
	imshow("croped4", cropedImage4);
	imshow("croped5", cropedImage5);
	imshow("croped6", cropedImage6);
	imshow("croped7", cropedImage7);
	imshow("croped8", cropedImage8);
	imshow("croped9", cropedImage9);
	imshow("croped10", cropedImage10);*/		

	//on converti en hsv
	cvtColor(cropedImage1,hsvcrop1,CV_BGR2HSV);
	cvtColor(cropedImage2,hsvcrop2,CV_BGR2HSV);
	cvtColor(cropedImage3,hsvcrop3,CV_BGR2HSV);
	cvtColor(cropedImage4,hsvcrop4,CV_BGR2HSV);
	cvtColor(cropedImage5,hsvcrop5,CV_BGR2HSV);
	cvtColor(cropedImage6,hsvcrop6,CV_BGR2HSV);
	cvtColor(cropedImage7,hsvcrop7,CV_BGR2HSV);
	cvtColor(cropedImage8,hsvcrop8,CV_BGR2HSV);
	cvtColor(cropedImage9,hsvcrop9,CV_BGR2HSV);
	cvtColor(cropedImage10,hsvcrop10,CV_BGR2HSV);

	//on convertit l'image source en hsv (debug)
	cvtColor(source,hsvsource,CV_BGR2HSV);
	inRange(hsvsource,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquesource);
	//imshow("hsv source",masquesource);


	//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
	inRange(hsvcrop1,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop1);
	inRange(hsvcrop2,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop2);
	inRange(hsvcrop3,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop3);
	inRange(hsvcrop4,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop4);
	inRange(hsvcrop5,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop5);
	inRange(hsvcrop6,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop6);
	inRange(hsvcrop7,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop7);
	inRange(hsvcrop8,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop8);
	inRange(hsvcrop9,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop9);
	inRange(hsvcrop10,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masquecrop10);

	// NOTE : couleurs du noir : (lowH=0 lowS=0 lowV=0, HighH=178 highS=255 highV=84 ! pas pour la detection d'abeille
	
	suppressbruit(masquecrop1);
	suppressbruit(masquecrop2);
	suppressbruit(masquecrop3);
	suppressbruit(masquecrop4);
	suppressbruit(masquecrop5);
	suppressbruit(masquecrop6);
	suppressbruit(masquecrop7);
	suppressbruit(masquecrop8);
	suppressbruit(masquecrop9);
	suppressbruit(masquecrop10);

	//on affiche l image finale
	//imshow("maskcroped1",masquecrop1);
	//imshow("maskcroped2",masquecrop2);

	
	dessinligne1();
	dessinligne2();
	dessinligne3();
	dessinligne4();
	dessinligne5();
	dessinligne6();
	dessinligne7();
	dessinligne8();
	dessinligne9();
	dessinligne10();



	passage1(); //fonction de comptage des entrées sorties
	passage2();
	passage3();
	passage4();
	passage5();
	passage6();
	passage7();
	passage8();
	passage9();
	passage10();


	//printf("entree:%d; sortie:%d \n",entree,sortie);
	//printf("%d \n",waitKey(30));

	
    	
	
	
	sauvegarde_automatique();
		
	if(C==1)
	{
		calibration();
	}
	if(Q==1)
	{
		cout << "Quitter par demande de l'utilisateur" << endl;
    	    	destroyAllWindows();
	    	fclose(file);
	    	break;
	}
				
}
	

}