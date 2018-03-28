//coordClass.cpp 
//contains the class used to store the vertex information
//including functions requried for arthimetic, memory allocation, copy/move, 
//and also functions required for function overlap

#include <sstream>
#include <algorithm>
#include <string>

#include "polygonClass.h"

using namespace std;


///uses pythagoras theorem to determine is a coordinate is between 2 others.
bool coord::between(coord c1, coord c2) {
	double d1 = (*this - c1).magnitude();
	double d2 = (*this - c2).magnitude();
	double d3 = (c1 - c2).magnitude();
	double error = d1 / 1e5;
	//due to the nature of computational precision, this value will almost never be exactly zero
	//so an error/precision variable is required.
	if (-error < (d1 + d2 - d3) && (d1 + d2 - d3) < error) return true;
	return false;
}

///Returns the intersection coord between 2 vectors using 4 coordinates as arguments
//pair.first is the coordinate of intersection
//pair.second is checks if interesection valid(true).
pair<coord, bool> intersection(coord start1, coord end1, coord start2, coord end2) {
	
	double x, y, c1, c2, f1, f2;
	//vector definitions
	coord v1 = (end1 - start1);
	coord v2 = (end2 - start2);
	bool check = false;

	//Parallel checks
	if ((v2.getx() == v1.getx()) || (v2.gety() == v1.gety())) {
		//returning a false result
		return make_pair(coord(), false);
	}
	//90 degrees vector check
	if (v1.getx() == 0 && v2.gety() == 0) {
		check = true;
		x = start1.getx();
		y = start2.gety();
	}
	//90 degrees vector check
	if (v1.gety() == 0 && v2.getx() == 0) {
		check = true;
		x = start2.getx();
		y = start1.gety();
	}
	//If the vectors are not the simple cases stated above
	if (!check) {
		if (v2.gety() != 0 && v2.getx() != 0) { 
			f2 = v2.gety() / v2.getx();
			c2 = end2.gety() - end2.getx()*f2;
			if (v1.gety() != 0 && v1.getx() != 0) { 
				f1 = v1.gety() / v1.getx();
				c1 = end1.gety() - end1.getx()*f1;
				x = (c2 - c1) / (f1 - f2);
				y = f1*x + c1;
			}
			else if (v1.gety() == 0) {
				y = end1.gety();
				x = (y - c2) / f2;
			}
			else {
				x = end1.getx();
				y = f2*x + c2;
			}
		}
		else if (v2.gety() == 0) {
			y = end2.gety();
			f1 = v1.gety() / v1.getx();
			c1 = end1.gety() - end1.getx()*f1;
			x = (y - c1) / f1; 
		}
		else {
			x = end2.getx();
			f1 = (v1.gety() / v1.getx());
			c1 = end1.gety() - end1.getx()*f1;
			y = f1*x + c1;
		}
	}
	//All non-parallel vectors will eventually intersect
	//So the intersection point needs to be tested as to whether it's within limits
	if (coord(x, y).between(start1, end1) && coord(x, y).between(start2, end2))
	{
		return make_pair(coord(x, y), true);
	}
	//returning a false bool.
	return make_pair(coord(), false);
}


//Rotate coordinate in radians around a given position
coord coord::rotate(double rad, coord pos) {
	double xx, yy;
	// x' = ( cos() - sin() ) * x
	// y' = ( sin() + cos() ) * y
	// set pos as the origin point
	xx = (x - pos.getx())*cos(rad) - (y - pos.gety())*sin(rad);
	yy = (x - pos.getx())*sin(rad) + (y - pos.gety())*cos(rad);
	// shift coordinates back to pos 
	return coord(xx + pos.getx(), yy + pos.gety());
}
///Normalise, returns coord of the normalised vector
coord coord::normalise() const {
	double magnitude = this->magnitude();
	//Need to manually define zero result, otherwise divide by zero error with occur.
	if (x == 0 && y == 0) return coord(0, 0);
	return coord(x / magnitude, y / magnitude);
}

//Magnitude, returns double magnitude
double coord::magnitude() const {
	return sqrt(pow(x, 2) + pow(y, 2));
}


//returns a vector of coordinates between two coordinates, numpoints specifies number of points in the vector
//roundflag->true causes output to be rounded to int . 
xyVec makeVec(coord c1, coord c2, int numpoints, bool roundflag) {
	xyVec output;
	double xinc = (c2.getx() - c1.getx()) / (numpoints - 1);
	double yinc = (c2.gety() - c1.gety()) / (numpoints - 1);
	for (int x = 0; x < numpoints; x++) {
		if (roundflag) output.push_back(coord(round(c1.getx() + (x*xinc)), round(c1.gety() + (x*yinc))));
		else output.push_back(coord(c1.getx() + (x*xinc), c1.gety() + (x*yinc)));

	}
	return output;
}


//Get functions that return member data
double & coord::getx() { return x; } double & coord::gety() { return y; }
double coord::getx() const { return x; } double coord::gety() const { return y; }

//Returns double of the dot product
double coord::dot(coord c2) const {
	return x*c2.getx() + c2.gety()*y;
}
void coord::show(ostream &stream){ stream << fixed << setprecision(2) << "(" << x << "," << y << ")"; };


//Operator overloading
//Inequality
bool coord::operator<(const coord &c) {
	return (sqrt(pow(x, 2) + pow(y, 2)) < sqrt(pow(c.getx(), 2) + pow(c.gety(), 2)));
}
bool coord::operator>(const coord &c) {
	return (sqrt(pow(x, 2) + pow(y, 2)) > sqrt(pow(c.getx(), 2) + pow(c.gety(), 2)));
}

//Insertion 
ostream &operator<< (ostream& cstream, const coord& coordinates) {
	cstream << "(" << coordinates.getx() << "," << abs(coordinates.gety()) << ")";
	return cstream;
}
//extraction operator
istream &operator>> (istream& cstream, coord& coordinates) {
	double FirstInt, SecondInt;
	string cstr;
	while (true) {
		getline(cin, cstr);
		//There must be a space in the string, and it must end with a digit, and a digit before the first space
		if (cstr.find(" ") != string::npos && cstr[0] != ' '
			&& isdigit(cstr.back()) && isdigit(cstr[cstr.find(" ") - 1])){
			//testing if 2 digits are in the stream
			stringstream ss(cstr);
			if (ss >> FirstInt >> SecondInt)break;
		}

		cout << "Please enter in the Format \"x y\" :";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear(); //resets stream flag
	}

	coordinates.getx() = FirstInt;
	coordinates.gety() = SecondInt;
	//ss.ignore(256, '\n');
	return cstream;
}


//equality check
bool coord::operator== (const coord &c2)
{
	return ((*this).getx() == c2.getx() && (*this).gety() == c2.gety());
}

//addition
coord coord::operator+ (const coord &c2)
{
	return coord((*this).getx() + c2.getx(), (*this).gety() + c2.gety());
}
//subtraction
coord coord::operator-(const coord &c2)
{
	return coord((*this).getx() - c2.getx(), (*this).gety() - c2.gety());
}
//division, with error throw for zero exception
coord coord::operator/ (const double &a){
	//throws an exception
	if (a == 0) throw overflow_error("ERROR: Divide by zero exception");
	return coord(x / a, y / a);
}
//multiplication
coord coord::operator*(const double &a) {
	return coord(x*a, y*a);
}

//Friend function, overloading multiplication of double*const
coord operator*(const double a, const coord & c) {
	return coord(c.getx()*a, c.gety()*a);
}
//default constructor
coord::coord() { 
	x = 0; y = 0;
};
coord::coord(double xx, double yy){
	x = xx; y = yy; 
}
//deep copy assignment
coord & coord::operator=(const coord &  c1) {
	if (&c1 == this) return *this;
	x = c1.getx();
	y = c1.gety();
	return *this;
}
//copy constructor
coord::coord(const coord &c1) {
	x = c1.getx();
	y = c1.gety();
}
//move constructor
coord::coord(coord &&c1) {
	x = c1.x;
	y = c1.y;
	c1.x = 0;
	c1.y = 0;
}
//move assignment
coord & coord::operator=(coord &&c1) {
	swap(x, c1.x);
	swap(y, c1.y);
	return *this;
}
