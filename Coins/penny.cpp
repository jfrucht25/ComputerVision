#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	Mat image = imread("~/ComputerVision/EdgeDetection/coins.jpg");
	Mat contours;
	Canny(image,contours,10,350);
	imshow("Contours",contours);
	waitKey(0);
	return 0;
}
