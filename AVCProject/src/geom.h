/*geometrical primitives*/

#include <iostream>
#include <algorithm>
#include <array>
#include <math.h>


struct Point2Di{
    int x;
    int y;
    Point2Di move(Point2Di d);
    Point2Di rotate(Point2Di cent, double angle);
    //void print(){std::cout<<" x="<<x<<" y="<<y<<std::endl;};
};

Point2Di Point2Di::move(Point2Di d){
	Point2Di pr{(int)std::round(x+d.x),(int)std::round(y+d.y)};
	return pr;
}

Point2Di Point2Di::rotate( Point2Di cent, double angle){
	double x1 = x - cent.x;
	double y1 = y - cent.y;
	double x2 = x1*cos(angle) + y1*sin(angle);
	double y2 = -x1*sin(angle) + y1*cos(angle);
	x2 = x2 + cent.x; 
	y2 = y2 + cent.y;
	Point2Di pr{(int)std::round(x2),(int)std::round(y2)};
	return pr;
}

// describes body of the robot mostly
struct Rect{
	std::array<Point2Di,4> points;
    Rect rotate_move(Point2Di cent, Point2Di d, double angle );
};


// the rotate around "cent" the move by dx,dy
Rect Rect::rotate_move( Point2Di cent, Point2Di d, double angle ){
	Rect r1;
    // rotate all
	std::transform(points.begin(),points.end(),r1.points.begin(),
                   [cent,angle](Point2Di p){ return p.rotate(cent,angle); } );	
	Rect r2;
	//shift all points by dx,dy
	std::transform(r1.points.begin(),r1.points.end(),r2.points.begin(),
                   [d](Point2Di p){ return p.move(d); } );	 
    return r2;               
}
