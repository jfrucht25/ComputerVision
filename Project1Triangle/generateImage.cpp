#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>
#define SIZE 800
using namespace std;



void drawPixel(int (&pix)[800][800], double x, double y){
	if(x>=0 && x<SIZE && y>=0 && y<SIZE){
		int a = round(y);
		int b = round(x);
		pix[a][b] = 0;
	}
}
void drawCircle(int (&pix)[800][800], pair<double, double> center, double radius){
	double a = center.first;
	double b = center.second;
	double x = 0, y = radius; 
    double d = 3 - 2 * radius; 
    while (y >= x) 
    { 
        drawPixel(pix, x+a, y+b);
		drawPixel(pix, x+a, -y+b);
		drawPixel(pix, -x+a, y+b);
		drawPixel(pix, -x+a, -y+b);
		drawPixel(pix, y+a, x+b);
		drawPixel(pix, y+a, -x+b);
		drawPixel(pix, -y+a, x+b);
		drawPixel(pix, -y+a, -x+b);
        x++; 

        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        drawPixel(pix, x+a, y+b);
		drawPixel(pix, x+a, -y+b);
		drawPixel(pix, -x+a, y+b);
		drawPixel(pix, -x+a, -y+b);
		drawPixel(pix, y+a, x+b);
		drawPixel(pix, y+a, -x+b);
		drawPixel(pix, -y+a, x+b);
		drawPixel(pix, -y+a, -x+b);
    } 
}
void drawline(int (&pix)[800][800], pair<int, int> a, pair<int, int> b){
	bool ymajor = abs(b.second - a.second) > abs(b.first-a.first);
	if(ymajor){
		swap(a.first, a.second);
		swap(b.first, b.second);	
	}
	if(a.first > b.first){
		pair<int, int> t = a;
		a = make_pair(b.first, b.second);
		b = make_pair(t.first, t.second);
	
	}
	int dx = b.first - a.first;
	int dy = abs(a.second - b.second);
	
	double error = dx / (double)2;
	int ystep;
	if (b.second>a.second) ystep = 1;
	else ystep = -1;
	int j = a.second;
	for(int i=a.first; i<b.first; i++){
		if(ymajor) drawPixel(pix, j, i);
		else drawPixel(pix, i, j);
		error -= dy;
		if(error < 0){
			j+=ystep;
			error+=dx;
		}
	}
}
double magnitude(pair<int, int> a, pair<int, int> b){
	int dx = a.first - b.first;
	int dy = b.second - a.second;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}	
int main(void) {
	int pixels[SIZE][SIZE];
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			pixels[i][j]=1;
		}
	}
	srand(time(NULL));
	vector< pair <int, int> > points;
	for(int i=0; i<3; i++){
		points.push_back(make_pair(rand() % SIZE, rand() % SIZE));
	}
	sort(points.begin(), points.end());
	cout << "Vertices:\n";
	for(int i=0; i<3; i++){
		cout << "(" << points[i].first << "," << points[i].second << ")\n";
	}
	drawline(pixels, points[0], points[1]);
	drawline(pixels, points[0], points[2]);
	drawline(pixels, points[1], points[2]);
	
	
	double mags[] = {magnitude(points[1],points[2]),magnitude(points[0],points[2]),magnitude(points[0],points[1])};
	pair<double, double> incenter = make_pair(0,0);
	incenter.first = (mags[0] * points[0].first + mags[1] * points[1].first + mags[2] * points[2].first) / (mags[0]+mags[1]+mags[2]);	
	incenter.second = (mags[0] * points[0].second + mags[1] * points[1].second + mags[2] * points[2].second) / (mags[0]+mags[1]+mags[2]);
	cout << "The incenter is at ("<<incenter.first<<","<<incenter.second<<")\n";	
	double s = (mags[0]+mags[1]+mags[2])/2.0f;
	double rIn = sqrt((s-mags[0])*(s-mags[1])*(s-mags[2])/s);
	cout << "Incenter radius is " <<rIn<<endl;
	drawCircle(pixels, incenter, rIn);

	
	pair<double, double> circumcenter = make_pair(0,0);
	double d = 2*(points[0].first*(points[1].second-points[2].second)+points[1].first*(points[2].second-points[0].second)+points[2].first*(points[0].second-points[1].second));
	circumcenter.first = (double)((pow(points[0].first,2)+pow(points[0].second, 2))*(points[1].second-points[2].second)+(pow(points[1].first,2)+pow(points[1].second, 2))*(points[2].second-points[0].second)+(pow(points[2].first,2)+pow(points[2].second, 2))*(points[0].second-points[1].second))/d;	
	circumcenter.second = (double)((pow(points[0].first,2)+pow(points[0].second, 2))*(points[2].first-points[1].first)+(pow(points[1].first,2)+pow(points[1].second, 2))*(points[0].first-points[2].first)+(pow(points[2].first,2)+pow(points[2].second, 2))*(points[1].first-points[0].first))/d;
	double rCirc = magnitude(points[0], circumcenter);
	cout << "The circumcenter is at (" << circumcenter.first << "," << circumcenter.second <<")\n";
	cout << "Circumcenter radius is " << rCirc << endl;
	drawCircle(pixels, circumcenter, rCirc);

	pair<double, double> centroid = make_pair(0,0);
	centroid.first = (points[0].first+points[1].first+points[2].first)/3.0f;
	centroid.second = (points[0].second+points[1].second+points[2].second)/3.0f;
	cout << "The centroid is at (" << centroid.first << "," << centroid.second << ")\n";
	double m = (centroid.second - circumcenter.second) / (centroid.first - circumcenter.first);
	pair<double, double> p1 = make_pair(0, m*(0-centroid.first)+centroid.second);
	pair<double, double> p2 = make_pair(SIZE, m*(SIZE-centroid.first)+centroid.second);
	drawline(pixels, p1, p2);
	drawline(pixels, circumcenter, centroid);

	ofstream file;
	file.open("out.ppm");
	file << "P3  800  800  1" << endl;
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			for(int k=0; k<3; k++){
				file << pixels[i][j] << " ";
			}
		}
		file << endl;
	}
	file.close();
	return 0;
}


