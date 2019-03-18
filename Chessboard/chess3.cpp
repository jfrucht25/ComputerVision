#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;


int main(){
	VideoCapture vid("chess.mov");
		
	namedWindow("image",1);
	int board_height = 7;
	int board_width = 7;
	Size patternsize(board_height, board_width);
	int board_n = board_height*board_width;
	vector<Point2f> corners;
	vector< vector<Point3f> > objectPoints;
	vector< vector<Point2f> > imagePoints;
	bool patternfound;
	Mat frame;
	Mat gray;
	while(vid.read(frame) && waitKey(30)!=27)
	{
		flip(frame, frame, 0);
		flip(frame, frame, 1);
		cvtColor(frame, gray, CV_BGR2GRAY);
		
		objectPoints.clear();
		imagePoints.clear();
		
		patternfound = findChessboardCorners(gray, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE+ CALIB_CB_FAST_CHECK);
        
        if(patternfound){
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(frame, patternsize, Mat(corners), patternfound);
			
			vector< Point3f > obj;
			for(int i=0; i<board_height; i++){
				for(int j=0; j<board_width; j++){
					obj.push_back(Point3f((float)j,(float)i, 0));
				}
			}
			
			line(frame, corners[16], corners[18],1, 10,8,0);
			line(frame, corners[16], corners[30],1, 10,8,0);
			line(frame, corners[32], corners[30],1, 10,8,0);
			line(frame, corners[32], corners[18],1, 10,8,0);
			
			double res1 = norm(corners[18]-corners[16]);
			double res2 = norm(corners[18]-corners[32]);
			double res3 = norm(corners[30]-corners[32]);
			double res4 = norm(corners[16]-corners[30]);

			double res = (res1+res2+res3+res4)/4;
			
			Point2f a = corners[16] - Point2f(0,res);
			Point2f b = corners[18] - Point2f(0,res);
			Point2f c = corners[30] - Point2f(0,res);
			Point2f d = corners[32] - Point2f(0,res);
			
			line(frame, corners[16], a,1, 10,8,0);
			line(frame, corners[18], b,1, 10,8,0);
			line(frame, corners[30], c,1, 10,8,0);
			line(frame, corners[32], d,1, 10,8,0);
			
			line(frame, a, b,1, 10,8,0);
			line(frame, a, c,1, 10,8,0);
			line(frame, b, d,1, 10,8,0);
			line(frame, c, d,1, 10,8,0);
		}
		
		
		
		imshow("image", frame);

	}

	return 0;
}
