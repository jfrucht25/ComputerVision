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

#define SIZE 2000

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

int compX(const void* a, const void* b){
	Point* p1 = (Point *)a;
	Point* p2 = (Point *)b;
	return (p1->x - p2->x);
}
int compY(const void* a, const void* b){
	return (((Point*)a)->y-((Point*)b)->y);
}

ostream &operator<<(ostream &strm, const Point &p) {
    return strm << "(" << p.x << "," << p.y << ")";
}


double dist(Point a, Point b){
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

double bruteForce(Point points[], int size){
	double min_dist = DBL_MAX;
	for(int i=0; i<size-1; i++){
		for(int j=i+1; j<size; j++){
			min_dist= min(dist(points[i], points[j]), min_dist);
		}
	}
	return min_dist;
}

double closestStrip (Point strip[], int size, double d){
	double minPair = d;
	sort(strip, strip+size, [](Point const & a, Point const & b) -> bool { return a.y < b.y; } );
	for (int i = 0; i < size-1; ++i){
        	for (int j = i+1; j < size && abs(strip[i].y-strip[j].y)<minPair; ++j){
            		minPair = min(minPair, dist(strip[i], strip[j]));
		}
  	}
	return minPair; 
}
double recursiveHelper(Point points[], int size){
	if(size <= 3){
		return bruteForce(points, size);
	}	
	int midIndex = (size)/2;
	Point midPoint = points[midIndex];
	double dl = recursiveHelper(points, midIndex);
	double dr = recursiveHelper(points+midIndex, size-midIndex);
	
        double d = min(dl, dr);

	Point strip[size];
	int j=0;
        for(int i=0; i<size; i++){
                if(abs(points[i].x-midPoint.x) < d){
                        strip[j] = points[i];
			j++;
                }
        }
        double stripD = closestStrip(strip, j, d);
	return min(stripD, d);
}
double recursive(Point points[], int size){
	sort(points, points+size, [](Point const & a, Point const & b) -> bool { return a.x < b.x; } );
	return recursiveHelper(points, size);
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	Point points[100];
	for(int i=0; i<100; i++){
		points[i] = Point(1.0*rand()/RAND_MAX, 1.0*rand()/RAND_MAX);
	}
	cout << bruteForce(points, 100) << endl;
	cout << recursive(points, 100) << endl;
	cout << "N\tBruteForce\tRecursive\n";
	clock_t t1, t2;
	float diff;
	for(int i=10000; i<=1000000; i+=10000){
		cout << i << "\t";
		Point points[i];
		for(int j=0; j<i; j++){
			points[j] = Point(1.0*rand()/RAND_MAX, 1.0*rand()/RAND_MAX);
		}
		t1=clock();
		//double b =bruteForce(points, i);
		t2=clock();
		diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
		cout << diff << "\t";
		t1=clock();
                double r = recursive(points, i);
                t2=clock();
                diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
                cout << diff << "\n";
	}
	return 0;
}
