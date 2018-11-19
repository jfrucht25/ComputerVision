#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <fstream>

#define N 30
#define SIZE 800

using namespace std;

class Point{
	public:
	double x;
	double y;

	Point(double a, double b){
		x = a;
		y = b;
	}
	Point(){
		x=0;
		y=0;
	}
};

void drawPixel(int (&pix)[800][800], double x, double y){
	if(x>=0 && x<SIZE && y>=0 && y<SIZE){
		int a = round(y);
		int b = round(x);
		pix[a][b] = 0;
	}
}

void draw(int (&pix)[SIZE][SIZE], int x, int y, int width) {
        for (int i = x - width; i < x + width; i++) {
            for (int j = y - width; j < y + width; j++) {
                drawPixel(pix, i, j);
            }
        }
    }

bool compX(Point a, Point b){
	return (a.x<b.x);
}
bool compY(Point a, Point b){
	return (a.y<b.y);
}
ostream &operator<<(ostream &strm, const Point &p) {
    return strm << "(" << p.x << "," << p.y << ")";
}


double dist(Point a, Point b){
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

pair<Point, Point> bruteForce(Point points[], int size){
	double min_dist = DBL_MAX;
	pair<Point, Point> minPair = make_pair(points[0], points[1]);
	for(int i=0; i<size-1; i++){
		for(int j=i+1; j<size; j++){
			if(dist(points[i], points[j]) < min_dist){
					min_dist = dist(points[i], points[j]);
					minPair = make_pair(points[i], points[j]);
			}
		}
	}
	return minPair;
}
Point temp[SIZE];
pair<Point, Point> closestStrip (Point strip[], int size, double d){
	double min_dist = d;
	pair<Point, Point> minPair = make_pair(strip[0], strip[1]);
	sort(strip, strip+size, compY);
	for (int i = 0; i < size-1; ++i){
        	for (int j = i+1; j < size && abs(strip[i].y-strip[j].y)<min_dist; ++j){
            		if(dist(strip[i], strip[j]) < min_dist){
						min_dist = dist(strip[i], strip[j]);
						minPair = make_pair(strip[i], strip[j]);
				}
			}
		
  	}
	return minPair; 
}
pair<Point, Point> recursiveHelper(Point points[], int size){
	if(size <= 3){
		return bruteForce(points, size);
	}	
	int midIndex = (size)/2;
	Point midPoint = points[midIndex];
	pair<Point, Point> dl = recursiveHelper(points, midIndex);
	pair<Point, Point> dr = recursiveHelper(points+midIndex, size-midIndex);
	
    pair<Point, Point> d;
    if(dist(dl.first, dl.second) < dist(dr.first, dr.second)){
		d=dl;
	}
	else{
		d=dr;
	}
	int j=0;
        for(int i=0; i<size; i++){
                if(abs(points[i].x-midPoint.x) < dist(d.first, d.second)){
                        temp[j] = points[i];
			j++;
                }
        }
        pair<Point, Point> stripD = closestStrip(temp, j, dist(d.first, d.second));
	if(dist(stripD.first, stripD.second), dist(d.first, d.second)){
		return stripD;
		}
	return d;
}
pair<Point, Point> recursive(Point points[], int size){
	sort(points, points+size, compX);
	return recursiveHelper(points, size);
}
Point points[N];

int main(){
	srand(time(NULL));
	
	int pixels[SIZE][SIZE];
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			pixels[i][j]=1;
		}
	}
	
	for(int i=0; i<N; i++){
		points[i] = Point(1.0*rand()/RAND_MAX, 1.0*rand()/RAND_MAX);
	}
	pair<Point, Point> closest = recursive(points, N);
	
	for(int i=0; i<N; i++){
		draw(pixels, (int)(points[i].x*800),(int)(points[i].y*800), 1);
	}
	
	draw(pixels, (int)(closest.first.x*800),(int)(closest.first.y*800), 5);
	draw(pixels, (int)(closest.second.x*800),(int)(closest.second.y*800), 5);
	
	cout << closest.first << closest.second << endl;
	
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
