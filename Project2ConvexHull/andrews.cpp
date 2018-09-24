//
// Created by james on 9/23/18.
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <ctime>

#define SIZE 100
using namespace std;

class Point {
public:
    int x;
    int y;

    Point(int a, int b) {
        x = a;
        y = b;
    }

    Point() = default;

    bool operator<(Point other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};


int cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.y);
}


void drawPixel(int (&pix)[SIZE][SIZE], double x, double y) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        int a = round(y);
        int b = round(x);
        pix[a][b] = 0;
    }
}

void drawline(int (&pix)[SIZE][SIZE], Point a, Point b) {
    bool ymajor = abs(b.y - a.y) > abs(b.x - a.x);
    if (ymajor) {
        swap(a.x, a.y);
        swap(b.x, b.y);
    }
    if (a.x > b.x) {
        Point t = a;
        a = Point(b.x, b.y);
        b = Point(t.x, t.y);

    }
    int dx = b.x - a.x;
    int dy = abs(a.y - b.y);

    double error = dx / (double) 2;
    int ystep;
    if (b.y > a.y) ystep = 1;
    else ystep = -1;
    int j = a.y;
    for (int i = a.x; i < b.x; i++) {
        if (ymajor) drawPixel(pix, j, i);
        else drawPixel(pix, i, j);
        error -= dy;
        if (error < 0) {
            j += ystep;
            error += dx;
        }
    }
}

int main(int argc, char **argv) {
    int N = atoi(argv[1]);
    cout << "N is " << N << endl;
    int pixels[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            pixels[i][j] = 1;
        }
    }
    srand(time(NULL));
    vector<Point> points;
    cout << "The points are:" << endl;
    for (int i = 0; i < N; i++) {
        Point p;
        p.x = rand() % SIZE;
        p.y = rand() % SIZE;
        points.push_back(p);
        p = points[i];
        cout << "(" << p.x << "," << p.y << ")" << endl;
        drawPixel(pixels, p.x, p.y);
    }

    sort(points.begin(), points.end());

    vector<Point> lower;

    for (int i = 0; i < N; ++i) {
        while (lower.size() >= 2 && (cross(lower[lower.size() - 2], lower[lower.size() - 1], points[i]) <= 0))
            lower.pop_back();
        lower.push_back(points[i]);
    }
    vector<Point> upper;

    for (int i = N-1; i > 0; i--) {
        while (upper.size() >= 2 && (cross(upper[upper.size() - 2], upper[upper.size() - 1], points[i]) <= 0))
            upper.pop_back();
        upper.push_back(points[i]);
    }
    lower.pop_back();
    vector<Point> hull;
    hull.insert(hull.end(), lower.begin(), lower.end());
    hull.insert(hull.end(), upper.begin(), upper.end());

    cout << "The hull is: " << endl;
    cout << "(" << hull[0].x << "," << hull[0].y << ")" << endl;
    for (int i = 1; i < hull.size(); i++) {
        drawline(pixels, hull[i - 1], hull[i]);
        cout << "(" << hull[i].x << "," << hull[i].y << ")" << endl;
    }
    ofstream file;
    file.open("out.ppm");
    file << "P3  "<<SIZE<<" "<<SIZE<<" 1" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < 3; k++) {
                file << pixels[i][j] << " ";
            }
        }
        file << endl;
    }
    file.close();
    return 0;
}


