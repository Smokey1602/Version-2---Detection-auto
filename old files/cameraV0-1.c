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

Mat hsv,source,masque;
int LowH =0,HighH =179,LowS =0,HighS=255,LowV=0,HighV=255;
 
static void onMouse( int event, int x, int y, int f, void* ){
 Vec3b pix=hsv.at<Vec3b>(y,x);
 LowH=pix.val[0];
 LowS=pix.val[1];
 LowV=pix.val[2];
}
void window_create(void)
{
	namedWindow("flux_video", CV_WINDOW_AUTOSIZE);
	namedWindow("image_hsv", CV_WINDOW_AUTOSIZE);
	namedWindow("image_masque", CV_WINDOW_AUTOSIZE);
}
void window_control(void)//cette fonction nous permettra de choisir precisement la couleur que nous voulons detecter

{
	namedWindow("Control",CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("LowH","Control",&LowH,179);
	cvCreateTrackbar("HighH","Control",&HighH,179);
	cvCreateTrackbar("LowS","Control",&LowS,255);
	cvCreateTrackbar("HighS","Control",&HighS,255);
	cvCreateTrackbar("LowV","Control",&LowV,255);
	cvCreateTrackbar("HighV","Control",&HighV,255);
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
	window_control();

	//on recupere la couleur que nous voulons detecter
	/*while(H==0||V==0||S==0){
	
	// on recupere la premiere image de la camera
	capture >> source;
	//on affiche cette image
	imshow("flux_video", source);	
	//on converti en hsv
	cvtColor(source,hsv,CV_BGR2HSV);
	imshow("image_hsv",hsv);
	setMouseCallback("image_hsv", onMouse, 0 );*/
	
	
	while(1)
{	
	//on recupere les image au fur et a mesure
	capture >> source;

	//on filtre le bruit
	//fastNlMeansDenoising(source, source);

	//on affiche cette image
	imshow("flux_video", source);		

	//on converti en hsv et on l affiche
	cvtColor(source,hsv,CV_BGR2HSV);
	imshow("image_hsv",hsv);

	//on recupere la couleur que nous voulons detecter
	setMouseCallback("image_hsv", onMouse, 0 );

	//on supprime les couleurs que nous voulons pas et affichons celle que l'on veut en blanc
	inRange(hsv,Scalar(LowH,LowS,LowV),Scalar(HighH,HighS,HighV),masque);
	
	//on affiche l image finale
	imshow("image_masque",masque);


	waitKey(50);//attends la pression sur une touche(prendra aussi en compte une nouvelle image
}
	

}