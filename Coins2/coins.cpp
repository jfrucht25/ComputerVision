#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;

int main(){
	Mat src;
	Mat image;
	src = imread("coins.jpg", 1);
	cvtColor(src, image, CV_BGR2GRAY);
	GaussianBlur(image, image, Size(9,9), 2, 2);
	vector<Vec3f> circles;
	
	int p=0;
	int n=0;
	int q=0;
	HoughCircles(image, circles, CV_HOUGH_GRADIENT, 1, 20, 90, 90, 20, 450);
	int minRadius = INT_MAX;
	for(int i=0; i<circles.size(); i++){
		if(cvRound(circles[i][2]) < minRadius){
			minRadius = cvRound(circles[i][2]);
		}
	}
	for(int i=0; i<circles.size(); i++){
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius=cvRound(circles[i][2]);
		double ratio = (1.0*radius)/minRadius;
		if(ratio<1.13){
			p++;
			circle(src, center, radius, Scalar(0,0,255), 3, 8, 0);
		}
		else if(ratio<1.23){
			n++;
			circle(src, center, radius, Scalar(0,255,0), 3, 8, 0);
		}
		else{
			q++;
			circle(src, center, radius, Scalar(255,0,0), 3, 8, 0);
		}
		circle(src, center, 3, Scalar(0,255,0), -1, 8, 0);
	}
	cout << "There are " << p <<" pennies, " << n << " nickels, and " << q << "quarters" << endl;
	cout << "The total value is $" << 0.01*p+0.05*n+0.25*q << endl;
	imwrite("traced.jpg", src);
	return 0;
}
