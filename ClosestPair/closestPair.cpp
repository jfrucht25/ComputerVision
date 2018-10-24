#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Point{
	public:
	double x;
	double y;

	Point(double a, double b){
		x = a;
		y = b;
	}
};

ostream &operator<<(ostream &strm, const Point &p) {
    return strm << "(" << p.x << "," << p.y << ")";
}

vector<Point> generatePoints(int N){
	vector<Point> points;
        for (int i = 0; i < N; i++) {
                Point p((1.0*rand())/RAND_MAX,(1.0*rand())/RAND_MAX);
                points.push_back(p);
        }
	return points;
}

double dist(Point a, Point b){
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

pair<Point,Point> bruteForce(vector<Point> points){
	pair<Point, Point> min_dist(points[0], points[1]);
	for(int i=0; i<points.size()-1; i++){
		for(int j=i+1; j<points.size(); j++){
			if(i==j) continue;
			double d1=dist(points[i], points[j]);
			double d2=dist(min_dist.first, min_dist.second);
			if(d1<d2){
				min_dist = make_pair(points[i], points[j]);
			}
		}
	}
	return min_dist;
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	pair<Point, Point> closest = bruteForce(generatePoints(90));
	cout << closest.first << " " << closest.second << " " << dist(closest.first, closest.second) << endl;
	return 0;
}
