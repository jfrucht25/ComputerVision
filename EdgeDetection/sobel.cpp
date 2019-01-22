#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>

#define THRESHOLD 128

using namespace std;

int WIDTH;
int HEIGHT;
int pixels[5000][5000];
int sobel[5000][5000];

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

int convolution(int a[3][3], int b[3][3]){
	return a[2][2]*b[0][0] + a[2][1]*b[0][1] + a[2][0]*b[0][2] +
		a[1][2]*b[1][0] + a[1][1]*b[1][1] + a[1][0]*b[1][2] +
		a[0][2]*b[2][0] + a[0][1]*b[2][1] + a[0][0]*b[2][2];
}

void fillSobel(){
	int x[3][3] = {{1,0,-1}, {2, 0, -2}, {1,0,-1}};
	int y[3][3] = {{1,2,1}, {0, 0, 0}, {-1,-2,-1}};
	int g;
	for(int i=1; i<HEIGHT-1; i++){
		for(int j=1; j<WIDTH-1; j++){
			int conv[3][3] = {{pixels[i-1][j-1],pixels[i-1][j],pixels[i-1][j+1]},
					{pixels[i][j-1],pixels[i][j],pixels[i][j+1]},
					{pixels[i+1][j-1],pixels[i+1][j],pixels[i+1][j+1]}};
			int gx = convolution(x, conv);
			int gy = convolution(y, conv);
			g = sqrt(gx*gx + gy*gy);
			sobel[i][j] = g;
		}
	}
}


int main(int argc, char* argv[]){
	getGrayscale("image.ppm");
	fillSobel();
	
	ofstream file;
	file.open("out.ppm");
	file << "P3 " <<WIDTH <<" " << HEIGHT << " 1" << endl;
	for(int i=0; i<HEIGHT; i++){
		for(int j=0; j<WIDTH; j++){
			for(int k=0; k<3; k++){
				if (sobel[i][j] > THRESHOLD){	
					file << 0 << " ";
				}
				else{
					file << 1 << " ";
				}
			}
		}
		file << endl;
	}
	file.close();

	return 0;
}
