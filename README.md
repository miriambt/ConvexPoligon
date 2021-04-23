#  ConvexPolygon

ConvexPolygon is a class that implements several functions that operate with sets of points. Within the package that you can download, besides the ConvexPolygon class, you will have access to a class of Points, which is needed for the ConvexPolygon implementation. Moreover, you will find the main program that works as a calculator of convex polygons, and some tests that prove its correctness. 

### Getting Started

These instructions will get you a copy of the project so that you can take advantage of the benefits of its features.

### Prerequisites

To install the package, you need the appropriate tar file. If it is not at your disposal, you can access it via GitHub: https://app.peergrade.io/student/courses/a3b1fc79-913b-4217-9c2d-ba008eebcdd4

### Calculator functions

#### "#" 

Lines starting with a hash sign "#" are comments. Their output is just a hash sign.

#### Polygon command

The polygon command associates an identifier with a convex polygon made by a set of zero or more points. If the polygon identifier is new, it will create it. If it already existed, it will overwrite the previous polygon. New polygons are black.

#### Print commmand

The print command prints the name and the vertices of a given polygon. The output contains just the vertices in the convex hull of the polygon, in clockwise order, starting from the vertex with lower X (and the vertex with lower Y in case of ties). They are printed in a single line, with one space separating each value.

#### 1. Area command

The area command prints the area of the given polygon.

#### 2. Perimeter command

The perimeter command prints the perimeter of the given polygon.

#### 3. Vertices command

The vertices command prints the number of vertices of the convex hull of the given polygon.

#### 4. Centroid command

The centroid command prints the centroid of the given polygon.

#### 5. List command

The list command lists all polygon identifiers, lexicographically sorted.

#### 6. Save command

The save command saves the given polygons in a file, overwriting it if it already existed. The contents of the file are the same as in the print command, with a polygon per line.

#### 7. Load command

The load command loads the polygons stored in a file, in the same way as polygon, but retrieving the vertices and identifiers from the file.

#### 8. Setcol command

The setcol command associates a color to the given polygon.

#### 9. Draw command

The draw command draws a list of polygons in a PNG file, each one with its associated color. The image is of 500x500 pixels, with white background and the coordinates of the vertices are scaled to fit in the 498x498 central part of the image while preserving the original aspect ratio.

#### 10. Intersection command

This command may receive two or three parameters:
	- When receiving two parameters p1 and p2, p1 is updated to the intersection of the original p1 and p2.
	- When receiving three parameters p1, p2 and p3, p1 is updated to the intersection of p2 and p3.

#### 11. Union command

Just as the intersection command, but with the convex union of polygons.

#### 12. Inside command

Given two polygons, the inside command prints yes or not to tell whether the first is inside the second or not.

#### 13. bbox command

The bbox command creates a new polygon with the four vertices corresponding to the bounding box of the given polygons.

#### 14. Commands without answer

Some commands do not produce an answer. "ok" is printed.

#### 15. Errors

If any command contains or produces an error, the error is printed in a line starting with error: and the command is completely ignored (as if it was not given). Possible errors include:
	- Invalid command
	- Command with wrong number or type of arguments
	- Undefined polygon identifier
	- Wrong format

## Running the tests

If you are looking forward to seeing an example of the implementation of the class, you have three tests available in the subdirectory named "input". Moreover, if you would like to check how the output of the run tests matches the expected output, you can write the following command line in the console: $ bash runTest.sh. Make sure you're in the directory /ConvexPolygon. This way, you will see a printed line saying the test succeeded in case the output of the input is as expected. On the contrary, you will see a line saying the test failed.
