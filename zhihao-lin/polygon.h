#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <string>
using namespace std;

class Polygon{
public:
    Polygon(int x1, int y1, int x2, int y2, int i){
        id = i;
        left_x = x1; bottom_y = y1;
        right_x = x2; top_y = y2;
    }
    int get_left ()const { return left_x; }
    int get_right()const { return right_x; }
    int get_bottom() const{ return bottom_y; }
    int get_top() const { return top_y; }
    int get_distance()const { return left_x+bottom_y; } // idstance from the bottom left corner of layer
    double area() const { return (right_x - left_x)*(top_y - bottom_y); }
    virtual string get_type()const = 0;

private:
    int id;
    int left_x, right_x, bottom_y, top_y;
};

class Critical: public Polygon{
public:
    Critical(int x1,int y1, int x2, int y2, int i):Polygon(x1, y1, x2, y2, i){}
    virtual string get_type()const {return "critical";}
private:
};

class NonCritic: public Polygon{
public:
    NonCritic( int x1,int y1, int x2, int y2, int i):Polygon(x1, y1, x2, y2, i){}
    virtual string get_type()const {return "non-critical";}
private:
};

class PowerGround: public Polygon{
public:
    PowerGround(int x1,int y1, int x2, int y2, int i):Polygon(x1, y1, x2, y2, i){}
    virtual string get_type()const {return "ground"; }
private:
};

#endif