#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <ctime>
#define SIZE 800
using namespace std;

class Point {
public:
    int x{};
    int y{};

    Point(int a, int b) {
        x = a;
        y = b;
    }

    Point() = default;
};

Point pivot;

double magnitude(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

int ccw(Point a, Point b, Point c) {
    int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.y);
    if (area > 0)
        return -1;
    if (area == 0)
        return 0;
    return 1;
}

int compAngle(const void *p1, const void *p2) {
    auto *a = (Point *)p1;
    auto *b = (Point *)p2;
    int mode = ccw(pivot, *a, *b);
    if (mode == 0) {
        //just take the closest one
        double dista = magnitude(pivot, *a);
        double distb = magnitude(pivot, *b);
        return dista < distb;
    }
    return (mode == -1);

}

void drawPixel(int (&pix)[800][800], double x, double y) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        int a = round(y);
        int b = round(x);
        pix[a][b] = 0;
    }
}

void drawline(int (&pix)[800][800], Point a, Point b) {
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
    int N;
    cin >> N;
    int pixels[SIZE][SIZE];
    for (auto &pixel : pixels) {
        for (int &j : pixel) {
            j = 1;
        }
    }
    srand(time(nullptr));
    Point points[N];
    for (int i = 0; i < N; i++) {
        Point p(rand() % SIZE, rand() % SIZE);
        points[i] = p;
        drawPixel(pixels, p.x, p.y);
    }

    int min = 0;
    for (int i = 0; i < N; i++) {
        if ((points[i].y < points[min].y) || (points[i].y < points[min].y && points[i].x < points[min].x)) {
            min = i;
        }
    }
    swap(points[0], points[min]);
    pivot = points[0];
    qsort(&points[1], static_cast<size_t>(N - 1), sizeof(Point), compAngle);
    stack <Point> s;
    s.push(points[0]);
    s.push(points[1]);
    s.push(points[2]);
    for (int i = 3; i < N; i++) {
        Point top = s.top();
        s.pop();
        Point second = s.top();
        s.push(top);
        cout << "i is " << i << endl;
        while (ccw(top, second, points[i]) >= 0) {
            cout << s.size() << endl;
            s.pop();
        }
        s.push(points[i]);
    }
    Point first = s.top();
    Point prev = s.top();
    while (!s.empty()) {
        Point t = s.top();
        s.pop();
        drawline(pixels, t, prev);
        prev = t;
    }
    drawline(pixels, first, prev);

    ofstream file;
    file.open("out.ppm");
    file << "P3  800  800  1" << endl;
    for (int i = 0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            for (int k = 0; k < 3; k++) {
                file << pixels[i][j] << " ";
            }
        }
        file << endl;
    }
    file.close();
    return 0;
}


