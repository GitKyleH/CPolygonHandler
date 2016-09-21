//coordClass.h header file for the coord class
//contains the class used to store the vertex information
//including functions requried for arthimetic, memory allocation, copy/move, 
//and also non-member functions required for function overlap.
#ifndef COORD_CLASS_H
#define COORD_CLASS_H

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

//coord class handles and stores coordinate information in 2D space.
class coord {
private:
	double x, y;;
public:
	//default constructor
	coord();
	//parameterised constructor
	coord(double xx, double yy);
	//copy constructor
	coord(const coord &c2);
	//move constructor
	coord(coord &&vec);

	//deep copy assignment
	coord & operator=(const coord & c2);
	//move assignment
	coord & operator=(coord && c2);

	//opeerator overloading
	bool operator== (const coord &n2);
	coord operator+ (const coord &n2);
	coord operator- (const coord &n2);
	coord operator*(const double &a);
	coord operator/(const double &a);
	bool operator<(const coord &c);
	bool operator>(const coord &c);

	//friend function declarations
	friend coord operator*(const double a, const coord &coord);
	friend ostream &operator<<(ostream& cstream, const coord& coordinates);
	friend istream &operator>> (istream& cstream, coord& coordinates);

	//member functions
	//dot product
	double dot(coord c2) const;
	//Rotate current coordinate(this) with respect to a coordinate(pos)
	coord rotate(double rad, coord pos);
	//returns magnitude value
	double magnitude() const;
	//returns normalised coordinate
	coord normalise() const;
	//Checks if (this) coord between c1 or c2
	bool between(coord c1, coord c2) ;
	//Member data retrieval, x and y
	double getx() const, gety() const, &getx(), &gety();
	//Outputs coorduinate information to an ostream, defaults to std::cout
	void show(ostream &stream = cout);
	
};
//For readability sake, xyVec is shortened
typedef  vector<coord> xyVec;

//declaration of non member functions

//returns a vector of coordinates between two coordinates c1 and c2
//numPoints defines the length of the returned vector
//roundflag(true) rounds the coordinate data to nearest int.
xyVec makeVec(coord c1, coord c2, int numPoints, bool roundflag = false);

///Returns pair, the intersection (coord) between 2 vectors (start1->end1) and (start2->end2).
// and bool(false) if intersection doesn't occur
pair<coord, bool> intersection(coord start1, coord end1, coord start2, coord end2);

#endif