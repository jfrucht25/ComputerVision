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

#define SIZE 800
#define WIDTH 4
using namespace std;

void drawPixel(int (&pix)[SIZE][SIZE], double x, double y) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        int a = round(y);
        int b = round(x);
        pix[a][b] = 0;
    }
}

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

    void draw(int (&pix)[SIZE][SIZE]) {
        for (int i = x - WIDTH; i < x + WIDTH; i++) {
            for (int j = y - WIDTH; j < y + WIDTH; j++) {
                drawPixel(pix, i, j);
            }
        }
    }
};

ostream &operator<<(ostream &strm, const Point &p) {
    return strm << "(" << p.x << "," << p.y << ")";
}


long cross(Point a, Point b, Point c) {
    double c1 = (b.x - a.x) * (a.y-c.y);
    double c2 = (c.x - a.x) * (a.y-b.y);
    double t = c1 - c2;
    return t;
}


void drawline(int (&pix)[SIZE][SIZE], Point p1, Point p2) {
    pair<int, int> a = make_pair(p1.x, p1.y);
    pair<int, int> b = make_pair(p2.x, p2.y);
    bool ymajor = abs(b.second - a.second) > abs(b.first - a.first);
    if (ymajor) {
        swap(a.first, a.second);
        swap(b.first, b.second);
    }
    if (a.first > b.first) {
        pair<int, int> t = a;
        a = make_pair(b.first, b.second);
        b = make_pair(t.first, t.second);

    }
    int dx = b.first - a.first;
    int dy = abs(a.second - b.second);

    double error = dx / (double) 2;
    int ystep;
    if (b.second > a.second) ystep = 1;
    else ystep = -1;
    int j = a.second;
    for (int i = a.first; i < b.first; i++) {
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
    int N;
    if (argc == 2) N = atoi(argv[1]);
    else cin >> N;
    cout << "N is " << N << endl;
    int pixels[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            pixels[i][j] = 1;
        }
    }
    srand(time(NULL));
    vector<Point> points;

    for (int i = 0; i < N; i++) {
        Point p;
        p.x = rand() % SIZE;
        p.y = rand() % SIZE;
        points.push_back(p);
        p.draw(pixels);
    }
    sort(points.begin(), points.end());
    cout << "The points are:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "(" << points[i].x << "," << points[i].y << ")" << endl;
    }
    vector<Point> lower;
    for (int i = 0; i < N; ++i) {
        while (lower.size() >= 2 && (cross(lower[lower.size() - 2], lower[lower.size() - 1], points[i]) <= 0)) {
            lower.pop_back();
        }
        lower.push_back(points[i]);
    }
    vector<Point> upper;

    for (int i = N - 1; i >= 0; i--) {
        while (upper.size() >= 2 && (cross(upper[upper.size() - 2], upper[upper.size() - 1], points[i]) <= 0)) {
            upper.pop_back();
        }
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
    drawline(pixels, hull[0], hull[hull.size() - 1]);
    ofstream file;
    file.open("out.ppm");
    file << "P3  " << SIZE << " " << SIZE << " 1" << endl;
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


