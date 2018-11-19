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

bool compX(Point a, Point b){
	return (a.x<b.x);
}
bool compY(Point a, Point b){
	return (a.y<b.y);
}

ostream &operator<<(ostream &strm, const Point &p) {
    return strm << "(" << p.x << "," << p.y << ")";
}

vector<Point> generatePoints(int N){
	vector<Point> points;
        for (int i = 0; i < N; i++) {
                points.push_back(Point((1.0*rand())/RAND_MAX,(1.0*rand())/RAND_MAX));
        }
	return points;
}

double dist(Point a, Point b){
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

double bruteForce(vector<Point> points, int a, int b){
	double min_dist = DBL_MAX;
	for(int i=a; i<b; i++){
		for(int j=i+1; j<b; j++){
			min_dist= min(dist(points[i], points[j]), min_dist);
		}
	}
	return min_dist;
}

double closestStrip (vector<Point> strip, double d){
	double minPair = d;
	sort(strip.begin(), strip.end(), compY);
	for (int i = 0; i < (strip.size()-1); ++i){
        	for (int j = i+1; j < strip.size() &&(strip[j].y-strip[i].y<minPair); ++j){
            			minPair = min(minPair, dist(strip[i], strip[j]));
		}
  	}
	return minPair; 
}
double recursiveHelper(vector<Point> points, int a, int b){
	if(b-a <= 3){
		return bruteForce(points, a, b);
	}	
	int midIndex = (b+a)/2;
	Point midPoint = points[midIndex];
		
	double dl = recursiveHelper(points, a, midIndex+1);
	double dr = recursiveHelper(points, midIndex, b);
	
        double d = min(dl, dr);
	
        vector<Point> strip;
        for(int i=0; i<points.size(); i++){
                if(abs(points[i].x-midPoint.x) < d){
                        strip.push_back(points[i]);
                }
        }
        double stripD = closestStrip(strip, d);
	return min(stripD, d);
}
double recursive(vector<Point> points){
	sort(points.begin(), points.end(), compX);
	return recursiveHelper(points, 0, points.size());
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	cout << "N\tBruteForce\tRecursive\n";
	clock_t t1, t2;
	float diff;
	vector<Point> points;
	//points = generatePoints(100);
	//cout << bruteForce(points, 0, 100) << endl;
	//cout << recursive(points) << endl;
	for(int i=10; i<1000; i+=10){
		cout << i << "\t";
		points = generatePoints(i);
		t1=clock();
		double b = bruteForce(points, 0, i);
		t2=clock();
		diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
		cout << diff << "\t";
		t1=clock();
                double r = recursive(points);
                t2=clock();
                diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
                cout << diff << "\n";
	}
	return 0;
}
