// g++ -Wall -o test1 camera.c `pkg-config --cflags --libs opencv`

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;
//--------DECLARATION DES VARIABLES --------------------------
Mat hsv,source,masque;
int LowH =0,HighH =178,LowS =0,HighS=255,LowV=0,HighV=80;
vector<Vec3f> cercles;
Mat Lignes = Mat::zeros( source.size(), CV_8UC3 );
int LastX=-1,LastY = -1,posX,posY;
int entree=0,sortie=0;
int bisY=0;
int tempo=0;
//------------------------------------------------------------

void window_create(void)
{
	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("image_masque", CV_WINDOW_AUTOSIZE);
}
void passage(void)
{
	int detect=0;
	
	detect  = (bisY-LastY);
	printf("detection :%d; bisY:%d; LastY:%d;\n",detect,bisY,LastY);
	if(detect>0 && LastY<30)
	{
		entree=entree+1;
	}
	if(detect<0&& LastY>350)
	{	
		sortie=sortie+1;
	}
	bisY=LastY;
}
void dessinligne(void)
{
	Moments Moments = moments(masque);

  	double M01 = Moments.m01;
 	double M10 = Moments.m10;
 	double Area = Moments.m00;

   // si area <= 10000, cela signifie que l'objet est trop petit pour etre detecté 
	if (Area > 400)
 	{
	//calculate the position of the ball
   	posX = M10 / Area;
   	posY = M01 / Area;        
        
		if (LastX >= 0 && LastY >= 0 && posX >= 0 && posY >= 0)
   		{
    		//Draw a red line from the previous point to the current point
    		line(source, Point(posX, posY), Point(LastX, LastY), Scalar(0,0,255), 2);
   		}

    	LastX = posX;
   	LastY = posY;
  	}
  	imshow("flux_video", source); //show the original image
	if(tempo>=7)
	{
		passage();
	}tempo++;
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
	
	while(1)
{	
	//on recupere les images au fur et a mesure
	capture >> source;
	

	//on affiche cette image
	//imshow("flux_video", source);		

	//on converti en hsv
	cvtColor(source,hsv,CV_BGR2HSV);

	//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
	inRange(hsv,Scalar(LowH,LowS,LowV,0),Scalar(HighH,HighS,HighV,0),masque);
	// NOTE : couleurs du noir : (lowH=0 lowS=0 lowV=0, HighH=178 highS=255 highV=84 ! pas pour la detection d'abeille
	
	//---------------------------------------------------------------------------------------//
	// ce paquet permettent d'enlever tout les bruits résiduels qui pourrait etre interprétés comme une autre couleur que le noir
	//morphological opening (removes small objects from the foreground) 
  		erode(masque, masque, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  		dilate(masque, masque, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  	 //morphological closing (removes small holes from the foreground)
  		dilate(masque, masque, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  		erode(masque, masque, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	//---------------------------------------------------------------------------------------//
	//on affiche l image finale
	imshow("image_masque",masque);
	
	dessinligne();
	
	printf("entree:%d; sortie:%d \n",entree,sortie);

	//waitKey(50);//attends 50ms
	 if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
    	    destroyAllWindows();
	    return -1; 
	    
       }
}
	

}