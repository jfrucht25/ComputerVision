#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;


int main(){
	VideoCapture vid("chess.mov");
		
	int board_height = 7;
	int board_width = 7;
	Size patternsize(board_height, board_width);
	int board_n = board_height*board_width;
	int numFrames = vid.get(CV_CAP_PROP_FRAME_COUNT);
	vector<Point2f> corners;
	vector< vector<Point3f> > objectPoints;
	vector< vector<Point2f> > imagePoints;
	bool patternfound;
	Mat frame;
	Mat gray;
	
	
	Mat K;
	Mat D;
	vector< Mat > rvecs, tvecs;
	int flag = 0;
	flag |= CV_CALIB_FIX_K4;
	flag |= CV_CALIB_FIX_K5;
	int x=0;
	while(vid.read(frame) && waitKey(30)!=27 && x<29){
		flip(frame, frame, 0);
		flip(frame, frame, 1);
		cvtColor(frame, gray, CV_BGR2GRAY);
		vector< Point3f > obj;
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				obj.push_back(Point3f((float)i * 1, (float)j * 1, 0));
		patternfound = findChessboardCorners(gray, patternsize, corners);
	
		if(patternfound){
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(frame, patternsize, Mat(corners), patternfound);
			
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
			x++;
		}
		printf("frame %d/%d analyzed\n", x, numFrames);
	}
			
	calibrateCamera(objectPoints, imagePoints, Size((int)vid.get(CV_CAP_PROP_FRAME_HEIGHT),(int)vid.get(CV_CAP_PROP_FRAME_WIDTH)), K, D, rvecs, tvecs, flag);
	
	VideoCapture vid2("chess.mov");
	namedWindow("image",1);
	x=0;
	while(vid2.read(frame) && waitKey(30)!=27)
	{
		flip(frame, frame, 0);
		flip(frame, frame, 1);
		cvtColor(frame, gray, CV_BGR2GRAY);
		
		imagePoints.clear();
		
		patternfound = findChessboardCorners(gray, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE+ CALIB_CB_FAST_CHECK);
        
        if(patternfound){
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
			TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(frame, patternsize, Mat(corners), patternfound);
			
			vector< Point3f > obj;
			for (int i = 0; i < 7; i++)
				for (int j = 0; j < 7; j++)
					obj.push_back(Point3f((float)j * 1, (float)i * 1, 0));
			
			Mat rvec;
			Mat tvec;
			solvePnPRansac(obj, corners, K, D, rvec, tvec);
			
			
			vector< Point3f > upperLayer;
			vector< Point2f > projected;
			upperLayer.push_back(Point3f( (float) 2, (float) 2, -2));
			upperLayer.push_back(Point3f( (float) 2, (float) 4, -2));
			upperLayer.push_back(Point3f( (float) 4, (float) 2, -2));
			upperLayer.push_back(Point3f( (float) 4, (float) 4, -2));


			projectPoints(upperLayer, rvec, tvec, K, D, projected, noArray(), 0);

			line(frame, corners[16], corners[18],1, 10,8,0);
			line(frame, corners[16], corners[30],1, 10,8,0);
			line(frame, corners[32], corners[30],1, 10,8,0);
			line(frame, corners[32], corners[18],1, 10,8,0);
			
			line(frame, corners[16], projected[0],1, 10,8,0);
			line(frame, corners[18], projected[2],1, 10,8,0);
			line(frame, corners[30], projected[1],1, 10,8,0);
			line(frame, corners[32], projected[3],1, 10,8,0);
			
			line(frame, projected[0], projected[1],1, 10,8,0);
			line(frame, projected[0], projected[2],1, 10,8,0);
			line(frame, projected[1], projected[3],1, 10,8,0);
			line(frame, projected[2], projected[3],1, 10,8,0);
			x++;
		}
		
		
		imshow("image", frame);

	}

	return 0;
}
