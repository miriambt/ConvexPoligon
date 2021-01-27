#ifndef ConvexPolygon_hh
#define ConvexPolygon_hh
#include <vector>
#include <iostream>
#include <pngwriter.h>
#include <sstream>
#include <map>
#include "Point.hh"
using namespace std;

/* The ConvexPolygon class stores a vector of two dimensional points in the plane and provides some usefull operations. 
Invariant: all the points of a ConvexPolygon form a Convex Hull. */

class ConvexPolygon {

public:
	/** Constructor. */
	ConvexPolygon(std::vector<Point> v);
	
	/** Constructor. */
	ConvexPolygon();  
	
	/** Given a vector of points, returns the position of the point with lowest X coordinate. 
		In case of a tie, the one with lowest Y coordinate. */
	int getLowestPoint(const vector<Point>& points) const;
	
	/** Given a vector of points, returns a new vector of points with the points
		that are the vertices of a ConvexPolygon, so that all inside angles are <180 degrees. */
	vector<Point> convexHull(vector<Point>& points); 
	
	/** Prints the X and Y coordinates of the vertices of a ConvexPolygon. */	
	void printVertices() const;
	
	/** Returns the area of a ConvexPolygon. */
	double getArea() const;
	
	/** Returns the perimeter of a ConvexPolygon. */
	double getPerimeter() const;
	
	/** Returns the number of vertices of a ConvexPolygon. */
	int getVertices() const;
	
	/** Returns the number of edges of a ConvexPolygon. */
	double getEdges() const;
	
	/** Returns the centroid of a ConvexPolygon. */
	Point getCentroid() const;
	
	/** Given a set of polygons, returns the smallest bounding box that contains all of them.*/
	ConvexPolygon boundingBox(map<string, ConvexPolygon>& polygons, const vector<string>& insiders) const; 

	/** Given a set of ConvexPolygons, draws them in a PNG file with the name of the string "nameFile", each one with its associated color, with white background and 
		the coordinates of the vertices scaled to fit in the 498x498 central part of the image, while preserving the original aspect ratio.*/
	void drawPolygon(map<string, ConvexPolygon>& polygons, map<string, vector<double>>& colors, vector<string> insiders, ConvexPolygon& boundingBox, string nameFile) const;

	/** Returns the intersection ConvexPolygon between two given ConvexPolygons. */
	ConvexPolygon getIntersection(ConvexPolygon& p1, ConvexPolygon& p2) const;
	
	/** Returns the union ConvexPolygon between two given ConvexPolygons. */
	ConvexPolygon getUnion(ConvexPolygon& p1, ConvexPolygon& p2) const;

	/** Returns "true" if the given ConvexPolygon is inside the own ConvexPolygon. */
	bool inside(ConvexPolygon cp) const;
	
	/** Draws a ConvexPolygon in a given PNG file, with its associated color and white background, so that its centroid becomes the point (250, 250)
		and at least one of its vertices remains in one of the edges of the 498x498 square, while preserving the original aspect ratio. */
	void drawCenteredPolygon(vector<double> colors, pngwriter& png);

	std::vector<Point> _points;

private:

	/** Given a vector of points, sorts them so that the first point in the vector is the one with lowest X coordinate and, in case of a tie, the one with lowest Y coordinate. 
		The other points are sorted in clockwise order. */
	void sortPoints(vector<Point>& points);

	/** Given three points, returns "0" if they are collinear, returns "1" if they make a left turn or returns "2" otherwise. */
	int orientation(Point p1, Point p2, Point p3) const;

	/** Given a point "p", stores in "xmax" the maximum between its initial value and the X coordinate of "p". 
		"xmin" stores the minimum between its initial value and the X coordinate of p. 
		The same with ymax and ymin. */
	void lowestAndBiggest(double& xmax, double& xmin, double& ymax, double& ymin, Point p) const;

	/** Given two points, returns the sclaeFactor that multiplied by "p" moves this point to one of the edges of a 498x498 square.
		and stores in "translator" the value needed to tranlate a scaled polygon to the middle of the 498x498 square. */
	double getScaleFactor(Point p, Point& translator) const;

	/** Modifies the X and Y coordinate of a given point p by applying some different operations. */
	Point scaleAndTranslate(Point& p, double dx, double dy, double scaleFacto, Point& translator) const;

	/** Given three collinear points "p0", "p1" and "p2", returns "true" if "p1" lies on the segment 'p0p2'. 
		Otherwise, returns "false". */
	bool onSegment(Point p0, Point p1, Point p2) const;
	
	/** Given fours points ("p0", "q1", "p2" and "q2"), returns "true" if line segment 'p1q1' and 'p2q2' intersect, 
		and stores the intersection point in the variable "intersection". */
	bool doIntersect(Point p1, Point q1, Point p2, Point q2, Point& intersection) const;

	/** Given a point "p", returns "true" if it is inside the ConvexPolygon. */
	bool PointInsidePolygon(Point p) const;

	/** Given a point "p", calculates its distance to the point (250, 250) and if its bigger than the "maxDistance", 
		"maxDistance" becomes the values calculated and the point "farthestPoint" stores the point "p". */
	void getFarthestPoint(Point p, Point& farthestPoint, double& maxDistance) const;

	/** Returns the scaleFactor need to scale a centered Polygon so that at least one of its vertices intersects one of the edges 
		of a given framework. */
	double getCenteredScaleFactor(Point p, ConvexPolygon framework, Point center) const;

	/** Modifies a given vector of points so that they fit in the center of a 489x489 square, preserving the original aspect ratio. */
	void scaleCenteredPolygon(vector<Point>& points);
};

#endif