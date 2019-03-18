#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

#define THRESHOLD 128
#define LOWER 100
#define UPPER 200

using namespace std;

int WIDTH;
int HEIGHT;

int pixels[5000][5000];
double gradMag[5000][5000];
double gradAngle[5000][5000];
double gaussian[5000][5000];
double suppressed[5000][5000];
int threshold[5000][5000];
int edges[5000][5000];

int colorToGrayscale(int r, int g, int b){
	return (0.3*r+0.59*g+0.11*b);
}

void getGrayscale(string filename){	
	ifstream infile;
	infile.open(filename);
	string line;
	getline(infile, line);
	istringstream iss(line);
	vector<string> tokens;
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter(tokens));
	int width = stoi(tokens[1]);
	int height = stoi(tokens[2]);
	WIDTH = width;
	HEIGHT = height;
	for(int i=0; i<height; i++){
		getline(infile, line);
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter(tokens));
		for(int j=0; j<width; j++){
			int r=stoi(tokens[3*j]);
			int g=stoi(tokens[3*j+1]);
			int b=stoi(tokens[3*j+2]);
			pixels[i][j] = colorToGrayscale(r,g,b);
		}
	}
}

double convolution(double a[3][3], double b[3][3]){
	return (a[2][2]*b[0][0] + a[2][1]*b[0][1] + a[2][0]*b[0][2] +
		a[1][2]*b[1][0] + a[1][1]*b[1][1] + a[1][0]*b[1][2] +
		a[0][2]*b[2][0] + a[0][1]*b[2][1] + a[0][0]*b[2][2]);
}

void fillSobel(){
	double x[3][3] = {{1,0,-1}, {2, 0, -2}, {1,0,-1}};
	double y[3][3] = {{1,2,1}, {0, 0, 0}, {-1,-2,-1}};
	double g;
	double theta;
	for(int i=1; i<HEIGHT-1; i++){
		for(int j=1; j<WIDTH-1; j++){
			double conv[3][3] = {{pixels[i-1][j-1],pixels[i-1][j],pixels[i-1][j+1]},
					{pixels[i][j-1],pixels[i][j],pixels[i][j+1]},
					{pixels[i+1][j-1],pixels[i+1][j],pixels[i+1][j+1]}};
			int gx = convolution(x, conv);
			int gy = convolution(y, conv);
			g = sqrt(gx*gx + gy*gy);
			theta = atan2(gy, gx);
			gradMag[i][j] = g;
			gradAngle[i][j] = theta;
		}
	}
}

void generateFilter(double GKernel[][5]) 
{ 
    double sigma = 1.0; 
    double r, s = 2.0 * sigma * sigma; 
  
    double sum = 0.0; 
  
    for (int x = -2; x <= 2; x++) { 
        for (int y = -2; y <= 2; y++) { 
            r = sqrt(x * x + y * y); 
            GKernel[x + 2][y + 2] = (exp(-(r * r) / s)) / (M_PI * s); 
            sum += GKernel[x + 2][y + 2]; 
        } 
    } 
  
    for (int i = 0; i < 5; ++i) 
        for (int j = 0; j < 5; ++j) 
            GKernel[i][j] /= sum; 
} 

void applyFilter(){
	double filter[5][5];
	generateFilter(filter);
	for(int i=0; i<HEIGHT-4; i++){
		for(int j=0; j<WIDTH-4; j++){
			double sum = 0.0;
			for(int x=0; x<5; x++){
				for(int y=0; y<5; y++){
					sum+=pixels[i+x][j+y]*filter[x][y];
				}
			}
			gaussian[i+2][j+2] = sum;;
		}
	}
}

void suppressEdges(){
	for(int i=1; i<HEIGHT-1; i++){
		for(int j=1; j<WIDTH-1; j++){
			int dx = int(abs(cos(gradAngle[i][j])) > 0.5);
			int dy = int(abs(sin(gradAngle[i][j])) > 0.5);
			double a = gradMag[i+dx][j+dy];
			double b = gradMag[i-dx][j-dy];
			if(gradMag[i][j] < a || gradMag[i][j] < b ){
				suppressed[i][j] = 0.0;	
			}
			else{
				suppressed[i][j] = gradMag[i][j];
			}
		}
	}
}

void applyThreshold(){
	for(int i=0; i<HEIGHT-1; i++){
                for(int j=0; j<WIDTH-1; j++){
			if(suppressed[i][j] > UPPER){
				threshold[i][j] = 2;
			}
			else if(suppressed[i][j] > LOWER){
				threshold[i][j] = 1;\
			}
			else {
				threshold[i][j] = 0;
			}
		}
	}
}

bool isConnected(int i,int j,int previ, int prevj){
	if(i<3||j<3||i>=HEIGHT-3||j>=WIDTH-3){
		return false;
	}
	if(threshold[i][j] == 2){
		return true;
	}
	else if(threshold[i][j]==1){
		bool connected = false;
		for(int x = -1; x<2; x++){
			for(int y=-1; y<2; y++){
				if((x==previ && y==prevj)||(x==0&&y==0)){
					continue;
				}
				if(isConnected(i+x, y+j, i,j)){
					connected = true;
					threshold[i][j] = true;
				}
			}
		}
		return connected;
	}
	return false;
}

void hysteresis(){
	for(int i=0; i<HEIGHT-1; i++){
                for(int j=0; j<WIDTH-1; j++){
                        if(threshold[i][j] == 2){
				edges[i][j] = 1;
			}
                        else if(threshold[i][j] == 1){
				if(true/*isConnected(i,j,-1,-1)*/){
					edges[i][j]=1;
				}
				else{
					edges[i][j]=0;
				}
			}
                        else{
				edges[i][j] = 0;
			}
                }
        }
}


int main(int argc, char* argv[]){
	getGrayscale("image.ppm");
	applyFilter();
	fillSobel();
	suppressEdges();
	applyThreshold();
	hysteresis();	

	ofstream file;
	file.open("out.ppm");
	file << "P3 " <<WIDTH <<" " << HEIGHT << " 255" << endl;
	for(int i=0; i<HEIGHT; i++){
		for(int j=0; j<WIDTH; j++){
			for(int k=0; k<3; k++){
				file<<int(edges[i][j])*255<<" ";
				/*if (pixels[i][j] < THRESHOLD){	
					file << 0 << " ";
				}
				else{
					file << 1 << " ";
				}*/
			}
		}
		file << endl;
	}
	file.close();

	return 0;
}
