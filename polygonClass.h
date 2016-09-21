// polygonClass.h header file for the polygon class
//contains the abstract base class, polygon, and its derived classes
//rectangle, isosceles triangle, Pentagon, Hexagon, general, overlapPolygon

#ifndef POLYGON_CLASS_H
#define POLYGON_CLASS_H

#include <memory>
#include "coordClass.h"

using namespace std;

//Abstract base class polygon
//Defines the basic structure of the derived polygons (rectangle, isosceles triangle, Pentagon, Hexagon, general, overlapPolygon)
//through the use of virtual member data and functions
//(xyVec = xyVec)
class polygon
{
private:
	//The static vector of vertices that are used for the visualisation/drawing of polygons
	static xyVec sDrawBuffer;
protected:
	//Static number of polygons
	static int sPolyNum;
	//ptrVertices is a pointer to the vector of the polygon's vertices
	xyVec* ptVert;
	//coordinate of the centre of the polygon
	coord centrePos;
	//number of vertices
	int vertNum;
public:
	//virtual destructor
	virtual ~polygon() {}

	//pure virtual const function
	//outputs the information of a polygon to an ostream
	virtual void info(ostream& stream = cout) const = 0;

	//pure virtual functions that alter the class data;
	//these are overridden in derived classes.
	//rescale and rotate overloads, such that if point not given, defaults to centrePos
	virtual void rescale(double mag) = 0;
	virtual void rescale(double mag, coord point) = 0;
	virtual void rotate(double rad, coord point) = 0;
	virtual void rotate(double rad) = 0;
	virtual void translate(double x, double y) = 0;
	virtual void translate(coord amount) = 0;

	//friend overload
	friend ostream &operator<<(ostream& cstream, const polygon& polygon);

	//deep copy assignment
	polygon &operator=(const polygon &p2);
	
	//non virtual member functions

	//adds polygon to draw buffer
	void buffer(); 
	//moves the polygon such that its new centre is newCentre
	void position(coord newCentre);
	//Re-assign the vertices of the polygon
	void vertPlot(const  xyVec);
	//returns true if a coordinate c1 is within the bounds of the polygon
	bool contains(coord &c1) const;
	//Returns the centre of the polygon 
	coord centre() const;
	//returns a reference to the polygon's vertex coordinates
	xyVec &getVert() const;
	//calculates and returns the polygon area.
	double area() const;
	//calculates and returns the area of any overlap with polygon p2
	double overlap_area(const polygon &p2);
	//overloaded such that the coordinates of overlap are moved onto outputCoord
	double overlap_area(const polygon &p2, xyVec &outputCoord);

	
	//static functions

	//returns the global number of polygons created
	static int getPolyNum();
	//adds all polygons on the polygonArray to the global draw buffer
	static void bufferAll(vector<polygon*>polygonArray);
	static void bufferAll(vector<shared_ptr<polygon>> polygonArray);
	//Clears the global draw buffer vector
	static void clearbuffer();
	// draws everything in polygon buffer
	static void draw(); 


};


//General class, allows nonregular, nonconvec polygons to be made
class general : public polygon
{
protected:
	int genNum;
public:
	//constructor / destructors 
	general();
	//move constructor
	general(general &&movePolygon);
	general(int vertnumber);
	~general();

	//overrided functions 
	void info(ostream& stream = cout) const;
	static int sGenNum;
	void rotate(double rad, coord position);
	void rotate(double angle_rad);
	void rescale(double magnitude, coord position);	
	void rescale(double magnitude);
	void translate(double x, double y);
	void translate(coord amount);
};

//Rectangle class
class rectangle : virtual public polygon
{
protected:
	//The specific labelled number of the rectangle
	int recNum;
public:
	rectangle();
	rectangle(double width, double height, coord centre = coord());
	~rectangle();
	//The static global number of rectangle polygons
	static int sRecNum;
	void info(ostream& stream = cout) const;
	void rotate(double rad, coord position);
	void rotate(double angle_rad);
	void rescale(double magnitude);
	void rescale(double magnitude, coord position);
	void translate(double x, double y);
	void translate(coord amount);
};
//IsosTriangle class, for isosceles triangles
class isosTriangle : virtual public polygon
{
protected:
	int triNum;
public:
	static int sTriNum;
	//constructor 
	isosTriangle();
	isosTriangle(double baselength, double height, coord centre = coord());
	~isosTriangle();
	void info(ostream& stream = cout) const;
	void rotate(double rad, coord position);
	void rotate(double angle_rad);
	void rescale(double magnitude);
	void rescale(double magnitude, coord position);
	void translate(double x, double y);
	void translate(coord amount);
};
//Pentagon class, shapes of regular pentagons
class pentagon : virtual public polygon
{
protected:
	int pentNum;
public:
	//constructor 
	pentagon();
	pentagon(double segmentlength, coord centre = coord(0, 0));
	~pentagon();
	static int sPentNum;
	void info(ostream& stream = cout) const;
	void rotate(double rad, coord position);
	void rotate(double angle_rad);
	void rescale(double magnitude, coord position);
	void rescale(double magnitude);
	void translate(double x, double y);
	void translate(coord amount);
};
//Hexagon class, shapes of regular hexagons
class hexagon : virtual public polygon
{
protected:
	int hexNum;
public:
	//constructor 
	hexagon();
	hexagon(double segmentlength, coord centre = coord(0, 0));
	~hexagon();
	static int sHexNum;
	void info(ostream& stream = cout) const;
	void rotate(double rad, coord position);
	void rotate(double angle_rad);
	void rescale(double magnitude, coord position);
	void rescale(double magnitude);
	void translate(double x, double y);
	void translate(coord amount);
};



//specialised class for polygons created from the overlap of polygons.
class overlapPolygon : public general
{
protected:
	int overlapNum;
public:
	//constructor / destructors 
	overlapPolygon();
	overlapPolygon(int vertnumber);
	~overlapPolygon();
	static int sOverlapNum;
};

#endif