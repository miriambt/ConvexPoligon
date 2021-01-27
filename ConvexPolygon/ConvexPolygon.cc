#include "Point.hh"
#include "ConvexPolygon.hh"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <pngwriter.h>
#include <sstream>
#include <iostream>
using namespace std;

/** Implementation of the class ConvexPolygon */
ConvexPolygon::ConvexPolygon(vector<Point> v) {
	_points = convexHull(v);  	
}

/** Implementation of the class ConvexPolygon */
ConvexPolygon::ConvexPolygon() {}

/** Pre: the given vector must not be empty.
	Invariant: the point "minPoint" stores the point in [0 ... i] with the lowest X coordinate. In case of a tie, stores the one with the lowest Y coordinate. 
	The integer "minIndex" stores the index of the position of the "minPoint" in the vector. */
int ConvexPolygon::getLowestPoint(const vector<Point>& points) const {
	int size = points.size();
	Point minPoint = points[0];
	int minIndex = 0;
	for (int i = 1; i < size; ++i) {
		if (points[i].get_x() < minPoint.get_x()) {
			minPoint = points[i];
			minIndex = i;
		} else if (points[i].get_x() == minPoint.get_x()) {
			if (points[i].get_y() < minPoint.get_y()) {
				minPoint = points[i];
				minIndex = i;
			}
		}
	}

	return minIndex;
} 


/** Given two points, it returns "true" if they have the same X coordinate. Otherwise, it returns "false". */
bool linear (Point p0, Point p1) {
	return p0.get_x() == p1.get_x();
}

/** Given two points, it returns the slope between them. */
/** Pre: the two points must have a different X coordinate. */
double slope(Point p0, Point p1) {
	double dy = p1.get_y() - p0.get_y();
	double dx = p1.get_x() - p0.get_x();
	
	return dy/dx;
}

/** The struct "Comparator" stores the point p0 (first vertice of the ConvexPolygon), which will be used as a reference point
	to sort the other points in clockwise order. */
struct Comparator {
	Point p0; 
	Comparator(Point minPoint) { this -> p0 = minPoint;}

	bool operator () (Point p1, Point p2) {
	   if (linear(p0, p1) and linear(p0, p2)) return p1.get_y() < p2.get_y();      /** If two points are linear, the one with the lowest y coordinate will go first. */
	   else if (linear(p0, p1)) return p1.get_x() < p2.get_x();					   /** If one point is in the same vertical line as the point p0, it will go first than any other point. */
	   else if (slope(p0, p1) == slope(p0, p2)) return p1.get_x() < p2.get_x();	   /** If two point are collinear with p0, the one with lowest x coordinate will go first. */
	   else return slope(p0, p1) > slope(p0, p2);								   /** If none of the previous conditions occurs, the point with biggest slope will go first */
	}
};

/** Post: the first point in the vector must be the point with the lowest X coordinate and, in case of a tie, the lowest Y coordinate. 
	The function "getLowestPoint" returns its initial position in the vector.
	The others vertices must be correctly sorted in clockwise order. */
void ConvexPolygon::sortPoints(vector<Point>& points) {
	sort(points.begin(), points.end(), Comparator(points[getLowestPoint(points)]));
}

/** Given three points, it returns "0" if they are collinear, "1" if they make a left turn or "2" otherwise. */
int ConvexPolygon::orientation(Point p1, Point p2, Point p3) const {
	double orientation = (p2.get_x() - p1.get_x())*(p3.get_y() - p1.get_y()) - (p2.get_y() - p1.get_y())*(p3.get_x() - p1.get_x());
	if (orientation == 0) return 0;       /** The three points are collinear */
	return (orientation > 0) ? 1 : 2;     /** The points are clockwise or counter-clockwise */
}

/** Given a vector of points, it sorts them in the correct order with the call of the function "sortPoints" and returns 
	a vector that will contain just the points that are the vertices of a ConvexPolygon. 
	We make a distinction between three cases:
		- If the size of the sorted vector is =  "1", we return it. (The ConvexPolygon is a point).  
		- If the size of the sorted vector is =  "2", we check if its two points are the same. In this case, we remove the last one. (The ConvexPolygon is a segment).
		- If the size of the sorted vector is >= "3", we create a new vector named "finalPolygon", initialized with the first two vertices of the sorted vector.  
		  Afterwards, we compare the points of the sorted vector one by one with the last two points of the vector "finalPolygon".
		  	 - If they are counter-clockwise, we push the point to the "finalPolygon".
			 - Otherwise, we remove the last points of the "finalPolygon" (it formed a >180 degrees inside angle or it was collinear with the other two points.
			   Then, we push the point and we check whether the three last points of the "finalPolygon" are collinear. */
vector<Point> ConvexPolygon::convexHull(vector<Point>& points) {
    sortPoints(points);
    if (points.size() == 1) return points;
    else if (points.size() == 2 and points[0] == points[1]) {
    	points.pop_back(); 
    	return points;
    } 
    else {
		vector<Point> finalPolygon = {points[0], points[1]};
		int size = 2;  
		for (int i = 2; i < points.size(); ++i) {
			 if (orientation(finalPolygon[size-2], finalPolygon[size-1], points[i]) == 2) {
				finalPolygon.push_back(points[i]);
				++size;
			} else if (orientation(finalPolygon[size-2], finalPolygon[size-1], points[i]) != 2) {
				finalPolygon.pop_back();
				finalPolygon.push_back(points[i]);
				if (size > 2 and orientation(finalPolygon[size-3], finalPolygon[size-2], finalPolygon[size-1]) == 0) {
					Point p = finalPolygon[size-1];
					finalPolygon.pop_back();
					finalPolygon.pop_back();
					finalPolygon.push_back(p);
					--size;
				}		
			}
		} 

		return finalPolygon;
	}
}

void ConvexPolygon::printVertices() const {
	for (int i = 0; i < _points.size(); ++i) { 
		cout << " ";
		_points[i].print();
	} 
	cout << endl;
}

double ConvexPolygon::getArea() const {
	double area = 0;
	int j = _points.size() - 1;    	 /** This index allows us to count the first vertice twice. */
	for (int i = 0; i < _points.size(); ++i) {
		area += ((_points[j].get_x() + _points[i].get_x()) * (_points[j].get_y() - _points[i].get_y()));
		j = i;
	}

	return abs(area/2);
}

/** Adds all distances up, from one vertice to the next one, from first to last. */
double ConvexPolygon::getPerimeter() const {
	double perimeter = 0;
	int j = _points.size() - 1;   /** This index allows us to count the first vertice twice. */
	for (int i = 0; i < _points.size(); ++i) {
		double dx = _points[j].get_x() - _points[i].get_x();
		double dy = _points[j].get_y() - _points[i].get_y();
		perimeter += sqrt(dx*dx + dy*dy);
		j = i;
	} 

	return perimeter;
}

/** The number of vertices of a ConvexPolygon is equal to the size of its vector of points that defines the polygon. */
int ConvexPolygon::getVertices() const {
	return _points.size();
}

/** The number of edges of a ConvexPolygon is equal to the size of the vector of points that defines the polygon. */
double ConvexPolygon::getEdges() const {
	return _points.size();
}

/** The X coordinate of the baricenter of a ConvexPolygon can be calculated as 1/6 of the area * the sumatorium, from i = 0,...,i = n - 1,
	of (X_i + X_i+1)*(X_i*Y_i+1 - X_i+1*Y_i). The same with the Y coordinate but replacing the first two X for two Y. */
Point ConvexPolygon::getCentroid() const {
	double cX = 0;
	double cY = 0;

	int j = _points.size() - 1;   /** This index allows us to count the first vertice twice. */
	for (int i = 0; i < _points.size(); ++i) {
		cX += (_points[i].get_x() + _points[j].get_x()) * (_points[i].get_x()*_points[j].get_y() - _points[j].get_x()*_points[i].get_y());
		cY += (_points[i].get_y() + _points[j].get_y()) * (_points[i].get_x()*_points[j].get_y() - _points[j].get_x()*_points[i].get_y());
		j = i;
	} 
	cX /= (getArea()*6);
	cY /= (getArea()*6);

	return {cX, cY};
}

/** If the X coordinate of a given point p is bigger than the given "xmax" passed by reference, xmax = X coordinate of p.
	The same with the lowest X coordinate and with the Y coordinate. */
void ConvexPolygon::lowestAndBiggest(double& xmax, double& xmin, double& ymax, double& ymin, Point p) const {
	xmax = max(xmax, p.get_x());
	xmin = min(xmin, p.get_x());
	ymax = max(ymax, p.get_y());
	ymin = min(ymin, p.get_y());
}

/** First, we calculate the maximum and minimum X and Y coordinates of the first point of the first given ConvexPolyon.
	Then, we compare these values with all the points of all the ConvexPolygons in the map whose name is stored in the given vector "insiders", 
	and store the maximum and minimum X and Y coordinates in "xmax", "xmin", "ymax" and "ymin". 
	Finally, we return the ConvexPolygon that has for vertices the points: (xmin, ymin), (xmax, ymin), (xmax, ymax) and (xmin, ymax). */
ConvexPolygon ConvexPolygon::boundingBox(map<string, ConvexPolygon>& polygons, const vector<string>& insiders) const {
	double xmax, xmin, ymax, ymin;
	xmax = xmin = polygons[insiders[0]]._points[0].get_x();
	ymax = ymin = polygons[insiders[0]]._points[0].get_y();

	for (int i = 0; i < insiders.size(); ++i) {
		string name = insiders[i];		
 		for (int j = 0; j < polygons[name]._points.size(); ++j) {
			lowestAndBiggest(xmax, xmin, ymax, ymin, polygons[name]._points[j]);
		}
	} 
	ConvexPolygon squarePolygon({{xmin, ymin}, {xmax, ymin}, {xmax, ymax}, {xmin, ymax}});
	
	return squarePolygon;
}

/** If the X coordinate of the given point "p" is bigger than its Y coordinate, it means that the boundingBox that contains 
	all ConvexPolygons is a rectangle with larger base rather than height. We must zoom in so that the large of the base becomes 498. 
	Othrwise, we must zoom in so that the large of the height becomes 498. */
double ConvexPolygon::getScaleFactor(Point p, Point& translator) const {
	double scaleFactor;
	
	if (p.get_x() >= p.get_y()) {
		scaleFactor = 498 / p.get_x();
		double y = (498 - p.get_y()*scaleFactor)/2 + 1;
		translator = {1, y};
	} else {
		scaleFactor = 498 / p.get_y();
		double x = (498 - p.get_x()*scaleFactor)/2 + 1;
		translator = {x, 1};
	}

	return scaleFactor;
}

/** We modify the X coordinate of the given point "p" by adding the corresponding value stored in "dx" to its X coordinate.
	Then, we muliply it by the given "scaleFactor" and finally we add the X coordinate of the given points "translator".
	The same with the Y coordinate. */
Point ConvexPolygon::scaleAndTranslate(Point& p, double dx, double dy, double scaleFactor, Point& translator) const {
	double x = (p.get_x() + dx)*scaleFactor + translator.get_x();
	double y = (p.get_y() + dy)*scaleFactor + translator.get_y();
	Point q = {x, y};

	return q;
} 

/** The procedure used to scale and translate the set of ConvexPolygons is the following:
	First, we calculate the distance between the lowest and leftest point of the given boundingBox that contains all the ConvexPolygons to the point (0,0). 
	From the reference values above, we translate all the vertices of the ConvexPolygons, so that the lowest and leftest point of the new boundingBox containing them
	would be the point (0,0).
	Next, we zoom in or zoom out so that the ConvexPolygons fit perfectly in the 498x498 square, and finally translate again so that they are centered in the middle of the image. */
void ConvexPolygon::drawPolygon(map<string, ConvexPolygon>& polygons, map<string, vector<double>>& colors, vector<string> insiders, ConvexPolygon& boundingBox, string nameFile) const {
	double dx = 0 - boundingBox._points[0].get_x();
	double dy = 0 - boundingBox._points[0].get_y();
	
	Point p = boundingBox._points[2];  
	p += {dx, dy};			
	
	Point translator;
	double scaleFactor = getScaleFactor(p, translator);
	const int size = 500;	
	pngwriter png(size, size, 1.0, nameFile.c_str());
	for (int k = 0; k < insiders.size(); ++k) {
		ConvexPolygon currentPolygon = polygons[insiders[k]];
		int j = currentPolygon._points.size() - 1;
		Point lastPoint = scaleAndTranslate(currentPolygon._points[j], dx, dy, scaleFactor, translator);	
		for (int i = 0; i < currentPolygon._points.size(); ++i) {
			Point currentPoint = scaleAndTranslate(currentPolygon._points[i], dx, dy, scaleFactor, translator);
			png.line(currentPoint.get_x(), currentPoint.get_y(), lastPoint.get_x(), lastPoint.get_y(), colors[insiders[k]][0], colors[insiders[k]][1], colors[insiders[k]][2]);
			j = i;
			lastPoint = currentPoint;
		}
	}

	png.close();

}

/** The point "p1" lies on segment "p0p2" if its X coordinate is smaller than the maximum X coordinate between "p0" and "p2", 
	but bigger than the minimum X coordinate between "p0" and "p2". Moreover, the Y coordinate of "p1" must be smaller
	than the maximum Y coordinate between "p0" and "p2", but bigger than the minimum Y coordinate between "p0" and "p2".*/
bool ConvexPolygon::onSegment(Point p0, Point p1, Point p2) const {
	return p1.get_x() <= max(p0.get_x(), p2.get_x()) and p1.get_x() >= min(p0.get_x(), p2.get_x()) and
		   p1.get_y() <= max(p0.get_y(), p2.get_y()) and p1.get_y() >= min(p0.get_y(), p2.get_y());
}

/** We must distinguish between 5 cases: */
bool ConvexPolygon::doIntersect(Point p1, Point q1, Point p2, Point q2, Point& intersection) const {
	int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
    double x;
    double y;
  
    /** (1) General case */
    if (o1 != o2 and o3 != o4) {
    	double n1, n2;
    	if ((p1.get_x() - q1.get_x()) == 0)	{                  /** 'p1q1' is vertical. */
    		if ((p2.get_x() - q2.get_x()) == 0) return false;  /** 'p2q2' is also vertical -> don't intersect. */
    		else {
    			x = p1.get_x();   							   /** The x coordinate of the intersection is the x coordinate of the vertical line that contains 'p1q1'. */
    			n2 = p2.get_y() - slope(p2, q2)*p2.get_x();	   /** n2 is the ordinate of the line that contains the segment 'p2q2'. */
    			y = slope(p2, q2)*x + n2;					   /** We calculate the y coordinate of the intersection point with the equation of the line. */
    		}
    	} 
    	else if ((p2.get_x() - q2.get_x()) == 0) { 		  	   /** 'p2q2' is vertical but 'p1q1' is not. */
    		x = p2.get_x();									   /** The x coordinate of the intersection is the x coordinate of the vertical line that contains 'p2q2'. */
    		n1 = p1.get_y() - slope(p1, q1)*p1.get_x();		   /** n1 is the ordinate of the line that contains the segment 'p1q1'. */
    		y = slope(p1, q1)*x + n1;						   /** We calculate the y coordinate of the intersection point with the equation of the line. */
    	} 
    	else {
    		n1 = p1.get_y() - slope(p1, q1)*p1.get_x();  	   /** If none of the lines is vertical, we calculate the equation of the two lines and solve the system -> find the intersection */
    		n2 = p2.get_y() - slope(p2, q2)*p2.get_x();
    		double dm = slope(p1, q1) - slope(p2, q2);
    		double dn = n2 - n1;
    		if (dm != 0) {
    			x = dn / dm;
    			y = slope(p1, q1)*x + n1;
    		}						          
    		else return false; /** In case that the two lines had the same slope, they'd either be the same line or be parellel. We will treat this exception as there was no intersection. */
    	}
    	intersection = {x, y};
  		return true;
    }

    /** (2) Special case: p1,q1 and p2 are collinear and p2 lies on segment 'p1q1'. */
    else if (o1 == 0 and onSegment(p1, p2, q1)) {
    	intersection = p2;	return true;
    }
 
    /** (3) Special case: p1, q1 and p2 are collinear and q2 lies on segment 'p1q1'. */ 
    else if (o2 == 0 and onSegment(p1, q2, q1)) {
    	intersection = q2;	return true; 
  	}
    /** (4) Special case: p2, q2 and p1 are collinear and p1 lies on segment 'p2q2'. */ 
   else if (o3 == 0 and onSegment(p2, p1, q2)) {
    	intersection = p1;	return true; 
    }
  
    /** (5) Special case: p2, q2 and q1 are collinear and q1 lies on segment 'p2q2'. */ 
    else if (o4 == 0 and onSegment(p2, q1, q2)) {
    	intersection = q1;	return true;
    } 

    return false;
}

/** We create a point that has the same Y coordinate as the point p, and with infinite X. This points acts as an infinite line. 
	To know wheter "p" is inside the polygon or not, we check how many edges intersects the infinite line. 
	The function returns "true" if count of intersections is odd ("p" is inside the ConvexPolygon), and "false" otherwise. */
bool ConvexPolygon::PointInsidePolygon(Point p) const {
	#define INF 10000          				/** We define Infinite (Using INT_MAX causes overflow problems). */
	Point infinite = {INF, p.get_y()};      /** We create a point for line segment from "p" to infinite. */
	int count = 0; 							
	Point intersection;					
	
	/** If "p" is collinear with line segment 'i-j', we check whether it lies on segment or not. If it does, we've finished.
		Otherwise, if the infinite line intersects the segment 'i-j', we increase the counter. */
    int j = _points.size() - 1; 
	bool foundOnSegment = false;
	for (int i = 0; i < _points.size() and not foundOnSegment; ++i) {
		if (doIntersect(_points[i], _points[j], p, infinite, intersection)) {
			if (orientation(_points[i], _points[j], p) == 0) {
				if (not onSegment(_points[i], p, _points[j])) return false;
				else foundOnSegment = true;
			} 
			++count;
		}		
		j = i;
	} 

	return (count%2) == 1;  
}

/** First, we check wheter the points of the first polygon (p1) are inside the second one (p2). In this case, we push them into the new vector "points", that will define the intersection Polygon. 
	Next, we calculate the intersection points between the edges of the first and the second polygon and push them into the vector "points".
	Finally, we check wheter the points of the second polygon are inside the first one. In this case, we push them into the vector "points". */
ConvexPolygon ConvexPolygon::getIntersection(ConvexPolygon& p1, ConvexPolygon& p2) const{
	vector<Point> points;
	Point intersection;

	int j = p1._points.size() - 1;
	int l = p2._points.size() - 1;

	for (int i = 0; i < p1._points.size(); ++i) {
		if (p2.PointInsidePolygon(p1._points[i]) == true) points.push_back(p1._points[i]);
		for (int k = 0; k < p2._points.size(); ++k) {
			if (doIntersect(p1._points[i], p1._points[j], p2._points[l], p2._points[k], intersection)) points.push_back(intersection);
			l = k; 
		}

		j = i;
	}

	j = p2._points.size() - 1;
	l = p1._points.size() - 1;

	for (int i = 0; i < p2._points.size(); ++i) {
		if (p1.PointInsidePolygon(p2._points[i]) == true) points.push_back(p2._points[i]);
		j = i;
	} 

	ConvexPolygon intersectionPolygon(points);
	return intersectionPolygon;
}

/** We make a new vector "points" that contains both the points in the first given ConvexPolygon (p1) and the points in the second given ConvexPolygon (p2).
	Finally, we create a new polygon "unionPolygon", that will be defined by the ConvexHull of the vector "points". */
ConvexPolygon ConvexPolygon::getUnion(ConvexPolygon& p1, ConvexPolygon& p2) const{
	vector<Point> points = p1._points;
	for (int i = 0; i < p2._points.size(); ++i) {
		points.push_back(p2._points[i]);
	}
	ConvexPolygon unionPolygon(points);
	
	return unionPolygon;
}

/** We check wheter all the vertices of the given ConvexPolygon are inside the own ConvexPolyogn
	If we find a point which is not inside it, we return "false". 
	Otherwise, we return "true". */
bool ConvexPolygon::inside(ConvexPolygon secondPolygon) const {
	for (int i = 0; i < _points.size(); ++i) {
		if (not secondPolygon.PointInsidePolygon(_points[i])) {
			return false;
		} 		
	}  return true;
}

/** we calculate the distance between the given point "p" and the point (250, 250).
	If it is bigger thasn "maxDistance", then we've found a point that is farther: "fartesPoint" becomes the point "p", +
	and "maxDistance" will store the value of the recently distance calculated. */
void ConvexPolygon::getFarthestPoint(Point p, Point& farthestPoint, double& maxDistance) const {
	double dx = p.get_x() - 250;
	double dy = p.get_y() - 250;
	double distance = dx*dx + dy*dy;	
	
	if (distance > maxDistance) {
		maxDistance = distance;
		farthestPoint = p;
	}
}

/** We first check whether the "farthesPoint" is inside the framework or not: 
	In case it is inside, we check which of the edges will intersect the segment that goes from "farthestPoint to the center (250, 250), when scaled,
	and according to that, we find the "intersection" point. 
	In case it is outside, we go through all the edges of the framework until we find the one that intersects the segment that goes from "farthestPoint to the center (250, 250).
	Whhen found, we store in the variable "intersection" the intersection point. */
double ConvexPolygon::getCenteredScaleFactor(Point farthestPoint, ConvexPolygon framework, Point center) const{
	Point intersection;																		  																					
    if (framework.PointInsidePolygon(farthestPoint)) {	
    	double x = farthestPoint.get_x();
    	double y = farthestPoint.get_y();						
		double m = slope(farthestPoint, center);												 
		double n = y - m*x;
		if (x > 250 and y <= x and y >= -x + 500) {  
			double point_y = m*499 + n;     
			intersection = {499, point_y};    /** It intersects the edge that goes from (499, 1) to (499, 499) */
		} 
		else if (y > 250 and x >= 500 - y and x <= y) {  
			double point_x = (499 - n)/m;  
			intersection = {point_x, 499};    /** It intersects the edge that goes from (1, 499) to (499, 499) */
		} 
		else if (x < 250 and y <= (-x + 500) and y >= x) { 
			double point_y = m + n;  
			intersection = {1, point_y};      /** It intersects the edge that goes from (1, 1) to (1, 499) */
		} 
		else if (y < 250 and x >= y and x <= 500 - y) { 
			double point_x = (1 - n)/m;
			intersection = {point_x, 1};      /** It intersects the edge that goes from (1, 1) to (499, 1) */
		}

	} else {
		int j = framework._points.size() - 1;
		bool found = false;
		for (int i = 0; i < framework._points.size() and not found; ++i) {
			if (doIntersect(framework._points[i], framework._points[j], farthestPoint, center, intersection)) found = true;
			j = i; 
		}
	}

	double scaleFactor = (intersection.get_x() - farthestPoint.get_x()) / (farthestPoint.get_x() - 250);
	return scaleFactor;
}

/** First, we go through all the points of the given vector with the objective of finding which point is farther from the center (250, 250), 
	and store it in the point "farthestPoint". Next, we calculate the "scaleFactor". Finally, we apply the scaleFactor to all the points in the vector, 
	so that we obtain a new reduced or grown ConvexPolygon, which fits in a 498x498 square. */
void ConvexPolygon::scaleCenteredPolygon(vector<Point>& newpoints) {
	Point centroid = getCentroid();
	double dx = 250 - centroid.get_x();    
	double dy = 250 - centroid.get_y();    
	double maxDistance = 0; 	
	Point farthestPoint = newpoints[0];
	
	for (int i = 0; i < newpoints.size(); ++i) {
		newpoints[i] += {dx, dy};
		getFarthestPoint(newpoints[i], farthestPoint, maxDistance);
	}

	ConvexPolygon framework({{1,1}, {1,499}, {499,499}, {499,1}});
	Point center = {250,250};
	double scaleFactor = getCenteredScaleFactor(farthestPoint, framework, center);

	bool found = false;
	for (int i = 0; i < newpoints.size(); ++i) {
		double x = scaleFactor*(newpoints[i].get_x() - 250);
		double y = scaleFactor*(newpoints[i].get_y() - 250);
		newpoints[i] += {x, y};
		if (not framework.PointInsidePolygon(newpoints[i])) {
			found = true;
			farthestPoint = newpoints[i];
		}
	} if (found) {
		scaleFactor = getCenteredScaleFactor(farthestPoint, framework, center);
		for (int i = 0; i < newpoints.size(); ++i) {
			double x = scaleFactor*(newpoints[i].get_x() - 250);
			double y = scaleFactor*(newpoints[i].get_y() - 250);
			newpoints[i] += {x, y};
		}
	}
}

/** We create a copy of the vector of points.
    We scale the points of the new vector and then draw edge by edge in a PNG file, with its associated color. */
void ConvexPolygon::drawCenteredPolygon(vector<double> colors, pngwriter& png) {
	vector<Point> newpoints = _points;
	scaleCenteredPolygon(newpoints);
	int j = newpoints.size() - 1;
	for (int i = 0; i < newpoints.size(); ++i) {
		png.line(newpoints[i].get_x(), newpoints[i].get_y(), newpoints[j].get_x(), newpoints[j].get_y(), colors[0], colors[1], colors[2]);
		j = i;
	}
}