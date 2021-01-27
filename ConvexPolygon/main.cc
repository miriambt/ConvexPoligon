#include "ConvexPolygon.hh"
#include "Point.hh"
#include <map>
#include <iostream>
#include <pngwriter.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


/** If the given name does not match any of the ConvexPolygon's name in all the set of polygons, writes an error line and returns "true" 
	since there's been an error.
	Otherwise, returns "false". */
bool undefinedIdentifier(map<string, ConvexPolygon>& polygons, string name) {
	auto it = polygons.find(name);
	if (it == polygons.end()) {
		cout << "error: undefined identifier" << endl;
		return true;
	}
	return false;
}

/** If the first letter of the given name is a digit, writes an error line and returns "false", there's an error on the type of the argument. 
	Otherwise, returns "true". */
bool isString(string name) {
	if (isdigit(name[0])) {
		cout << "error: wrong type argument" << endl;
		return false;
	}
	return true;
}

/** Associates an identifier with a convex polygon made by a set of zero or more points. 
	If the polygon identifier is new, it will create it. 
	If it already existed, it will overwrite the previous polygon. 
	New polygons are black. */
void createNewPolygon(map<string, ConvexPolygon>& polygons, map<string, vector<double>>& colors, istringstream& iss) {
	string name;
	iss >> name;
	if (not isString(name)) return;
	
	vector<Point> points;
	double x, y;
	while (iss >> x >> y) {
		Point p = {x, y};
		points.push_back(p);
	} 
	
	ConvexPolygon newPolygon(points);
    polygons[name] = newPolygon;
    colors[name] = {0, 0, 0};
}

/** Prints the name and the vertices of a given ConvexPolygon. */
void printVertices(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name;
	if (undefinedIdentifier(polygons, name)) return;
	cout << name;
	polygons[name].printVertices();
}

/** Prints the area of the given ConvexPolygon. */
void getArea(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name;
	
	if (undefinedIdentifier(polygons, name)) return;
	cout << polygons[name].getArea() << endl;
}

/** Prints the perimeter of the given ConvexPolygon. */
void getPerimeter(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name;
	if (undefinedIdentifier(polygons, name)) return;
	cout << polygons[name].getPerimeter() << endl;
}

/** Prints the number of vertices of the convex hull of the given polygon. */
void getVertices(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name;
	if (undefinedIdentifier(polygons, name)) return;
	cout << polygons[name].getVertices() << endl;
}

/** Prints the centroid of the given ConvexPolygon. */
void getCentroid(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name;
	
	if (undefinedIdentifier(polygons, name)) return;
	cout << polygons[name].getCentroid().get_x() << " " << polygons[name].getCentroid().get_y() << endl;
}

/** lists all polygon identifiers, lexycographically sorted. */
void getList(map<string, ConvexPolygon>& polygons) {
	bool first = true;
	for (auto e : polygons) {
		if (first) {
			cout << e.first;
			first = false;
		} else cout << " " << e.first;
	} cout << endl;
	if (first) cout << "error: void list" << endl;
}

/** Saves the given polygons in a file, overwriting it if it already existed. 
	The contents of the file are the same as in the print command, with a polygon per line. */
void saveFile(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string outFile;
	iss >> outFile;
	ofstream out(outFile);
	string name;
	
	while (iss >> name) {
		if (undefinedIdentifier(polygons, name)) return;
		out << name;
		for (int i = 0; i < polygons[name]._points.size(); ++i) out << " " << polygons[name]._points[i].get_x() << " " << polygons[name]._points[i].get_y();
		out << endl;
	} 
	
	out.close();
	cout << "ok" << endl;
}

/** Loads the polygons stored in a file, in the same way as polygon, but retrieving the vertices and identifiers from the file. */
void loadFile(map<string, ConvexPolygon>& polygons, map<string, vector<double>>& colors, istringstream& iss) {
	string nameFile;
	iss >> nameFile; 
	if (not isString(nameFile)) return;
	ifstream inFile(nameFile);
	string line;
	while (getline(inFile, line)) {
		istringstream iss(line);
		createNewPolygon(polygons, colors, iss);
	}
	inFile.close();
	cout << "ok" << endl;
}

/** Associates a color to the given polygon. */
void setCol(map<string, ConvexPolygon>& polygons, map<string, vector<double>>& colors, istringstream& iss) {
	string name;
	iss >> name;
	if (undefinedIdentifier(polygons, name)) return;
	vector<double> setColor;
	double color;
	while (iss >> color) {
		setColor.push_back(color);
		colors[name] = setColor;
	}	
	cout << "ok" << endl;
}

/** Draws a list of polygons in a PNG file, each one with its associated color. 
	The image is of 500x500 pixels, with white background and the coordinates of the vertices are scaled
	to fit in the 498x498 central part of the image, while preserving the original aspect ratio. */
void drawPolygon(map<string, ConvexPolygon>& polygons, map<string, vector<double>>& colors, istringstream& iss) {
	string nameFile;  
	iss >> nameFile;
	string namePolygon;
	vector<string> insiders;
	while (iss >> namePolygon) {
		if (undefinedIdentifier(polygons, namePolygon)) return;
		else insiders.push_back(namePolygon);
	}
	ConvexPolygon boundingBox;
	boundingBox = polygons[insiders[0]].boundingBox(polygons, insiders);
	boundingBox.drawPolygon(polygons, colors, insiders, boundingBox, nameFile);
	cout << "ok" << endl;
}

/** When receiving two parameters ("p1" and "p2"), "p1" is updated to the intersection of the original "p1" and "p2".
	When receiving three parameters ("p1", "p2" and "p3"), "p1" is updated to the intersection of p2 and p3. */
void getIntersection(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name; 
	string p1 = name;
	iss >> name;
	string p2 = name;
	if (iss >> name) {
		string p3 = name;
		if (undefinedIdentifier(polygons, p2) or undefinedIdentifier(polygons, p3)) return;
		polygons[p1] = polygons[p1].getIntersection(polygons[p2], polygons[p3]);
	} else {
		if (undefinedIdentifier(polygons, p1) or undefinedIdentifier(polygons, p2)) return;
		polygons[p1] = polygons[p1].getIntersection(polygons[p1], polygons[p2]);
	}  
	cout << "ok" << endl;
} 

/** Just as the intersection command, but with the convex union of polygons. */
void getUnion(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string name;
	iss >> name;
	string p1 = name;
	iss >> name;
	string p2 = name;
	if (iss >> name) {
		string p3 = name;
		if (undefinedIdentifier(polygons, p2) or undefinedIdentifier(polygons, p3)) return;
		polygons[p1] = polygons[p1].getUnion(polygons[p2], polygons[p3]);
	} else {
		if (undefinedIdentifier(polygons, p1) or undefinedIdentifier(polygons, p2)) return;
		polygons[p1] = polygons[p1].getUnion(polygons[p1], polygons[p2]);
	}  
	cout << "ok" << endl;
} 

/** Given two polygons, prints "yes" or "not" to tell whether the first is inside the second or not. */
void inside(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string firstPolygon, secondPolygon;
	iss >> firstPolygon >> secondPolygon;
	if (undefinedIdentifier(polygons, firstPolygon) or undefinedIdentifier(polygons, secondPolygon)) return;
	cout << (polygons[firstPolygon].inside(polygons[secondPolygon]) ? "yes" : "no") << endl;
}

/** Creates a new polygon with the four vertices corresponding to the bounding box of the given polygons. */
void boundingBox(map<string, ConvexPolygon>& polygons, istringstream& iss) {
	string nameBox;
	iss >> nameBox;
	if (not isString(nameBox)) return;
	vector<string> insiders;
	string namePolygon;
	while (iss >> namePolygon) {
		if (undefinedIdentifier(polygons, namePolygon)) return;
		insiders.push_back(namePolygon);
	}
	polygons[nameBox] = polygons[nameBox].boundingBox(polygons, insiders);
	cout << "ok" << endl;
}

int main() {
	cout.setf(ios::fixed);	
	cout.precision(3);
	string line;
	map<string, ConvexPolygon> polygons;
	map<string, vector<double>> colors;
	while (getline(cin, line)) {
		istringstream iss(line);
        string command;
        iss >> command;
        if (command == "#") cout << "#" << endl;  /** For comments */ 
        else if (command == "polygon") {
        	createNewPolygon(polygons, colors, iss);
  			cout << "ok" << endl;
		}
		else if (command == "print") printVertices(polygons, iss);		
		else if (command == "area") getArea(polygons, iss);
		else if (command == "perimeter") getPerimeter(polygons, iss);	
		else if (command == "vertices") getVertices(polygons, iss);
		else if (command == "centroid") getCentroid(polygons, iss);
		else if (command == "list") getList(polygons);
		else if (command == "save") saveFile(polygons, iss);
		else if (command == "load") loadFile(polygons, colors, iss);
		else if (command == "setcol") setCol(polygons, colors, iss);		
		else if (command == "draw") drawPolygon(polygons, colors, iss);		
		else if (command == "intersection") getIntersection(polygons, iss);
		else if (command == "union") getUnion(polygons, iss);
		else if (command == "inside") inside(polygons, iss);	
		else if (command == "bbox") boundingBox(polygons, iss);
		else cout << "error: unrecognized command" << endl;
 	}
}

