//polygonClass.cpp 
//contains the abstract base class, polygon, and its derived classes
//rectangle, isosceles triangle, Pentagon, Hexagon, general, overlapShape


#include <sstream>
#include <algorithm>
#include <string>

#include "polygonClass.h"

const double M_PI = acos(-1);

using namespace std;


//declaring static variables (All prefixed with s)
int polygon::sPolyNum, rectangle::sRecNum, isosTriangle::sTriNum, hexagon::sHexNum, pentagon::sPentNum, general::sGenNum, overlapPolygon::sOverlapNum;
xyVec polygon::sDrawBuffer;

int polygon::getPolyNum() {
	return sPolyNum;
}

//use the shoelace method and iterators
double polygon::area() const {
	double area{ 0 };
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		//Iterator to the next vertex is required
		//so a conditional operator is used
		auto it2 = it + 1 == ptVert->end() ? ptVert->begin() : it + 1;
		area += (it->getx())*(it2->gety()) - it->gety()*it2->getx();
	}
	return abs(area) / 2.0;
}

coord polygon::centre() const {
	double count{ 0 }, tempx{ 0 }, tempy{ 0 };
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		tempx += it->getx();
		tempy += it->gety();
		count++;
	}
	return coord(tempx / count, tempy / count);
}

///Assigns a new vector of coordinates to the polygon
void polygon::vertPlot(const xyVec vec_cord) {
	//checks if the same size
	if (vertNum == (int)vec_cord.size()){
		*ptVert = vec_cord; ///Uses coord copy assignment
		centrePos = (*this).centre(); //update the centre position
	}
	else cerr << endl << "ERROR using vertPlot, please use correct vector size" << endl;
};
xyVec & polygon::getVert() const{
	return *ptVert;
};

//add a polygon to the draw buffer
//converts the vertex positions into lines of coordinates using makeVec
void polygon::buffer() {
	for (int l = 0; l < vertNum; l++)
	{
		int j = (l + 1) % (vertNum);
		int length = 2 +(int)((*(this->ptVert))[l] - (*(this->ptVert))[j]).magnitude();
		xyVec line = makeVec((*ptVert)[l], (*ptVert)[j], (int)length, true);
		sDrawBuffer.insert(sDrawBuffer.end(), line.begin(), line.end());
	}
}

//buffers all of the polygons held within polygon_array
void polygon::bufferAll(vector<polygon*> polygon_array) {
	for (auto it = polygon_array.begin(); it != polygon_array.end(); it++) {
		(*it)->buffer();
	}
}
void polygon::bufferAll(vector<shared_ptr<polygon>> polygon_array) {
	for (auto it = polygon_array.begin(); it != polygon_array.end(); it++) {
		(*it)->buffer();
	}
}
//clears the buffer vector
void polygon::clearbuffer() {
	sDrawBuffer.clear();
}
//draw() uses the buffer vector to output a console 'picture',
//the function will only draw the positive quadrant.
//The maximum size is the height/width of the default windows console.
void polygon::draw() {
	double maxyVec{ 15 }; //default minimum height of the draw plane
	for (auto it = sDrawBuffer.begin(); it != sDrawBuffer.end(); it++) {
		if (it->gety() > maxyVec) maxyVec = it->gety();
 	}
	//only draws the positive quadrant.
	for (int y = (int)ceil(maxyVec); y >= 0; y--) { //height of console window
		for (int x = 0; x < 78; x++) {//width of default console window
			if (find(sDrawBuffer.begin(), sDrawBuffer.end(), coord(x, y)) != sDrawBuffer.end()) {
				cout << "x";
			}
			else cout << " ";
		}
		cout << endl;
	}
}

//deep copy assignment
polygon & polygon::operator=(const polygon & p2) {
	if (&p2 == this) return *this;
	delete ptVert;
	vertNum = p2.vertNum;
	if (vertNum > 0) {
		ptVert = new xyVec(vertNum);
		for (int i = 0; i < vertNum; i++) {
			(*ptVert)[i] = (*p2.ptVert)[i];
		}
	}
	return *this;
}

void polygon::position(coord newposition) {
	coord trans = newposition - centrePos;
	(*this).translate(trans.getx(), trans.gety());
	//update centre position
	centrePos = newposition;
}

//Returns a true bool if polygon contains coordinate c1.
//Utilizes an efficient raycasting algorithm
bool polygon::contains(coord &c) const {
	int counter{ 0 };
	double xdist;
	//x1 current vertex, x2 next vertex clockwise.
	for (int x1 = 0; x1 < (int)ptVert->size(); x1++) {
		int x2 = (1 + x1) % (int)ptVert->size();

		//checks if a horizontal line from c1 intersects the x1->x2 vector
		if (((*ptVert)[x1].gety() >= c.gety()) != ((*ptVert)[x2].gety() >= c.gety())) {
			
			// x_distance of the 'intersection' from x1
			xdist = ((*ptVert)[x2].getx() - (*ptVert)[x1].getx()) * (c.gety() - (*ptVert)[x1].gety()) / ((*ptVert)[x2].gety() - (*ptVert)[x1].gety());
			// checking if  the coordinate is to the left of the x1-x2 vector that the ray intersects
			// most basic form  is (c_x <= x1_x) for adjacent lines
			if (c.getx() <= xdist + (*ptVert)[x1].getx()) {
				counter++;
			}
		}
	}
	//counter is the number of intersections a ray would make moving across the shape.
	//even signifies that it's outside the shape.
	if (counter % 2 != 0) return true;
	return false;
}


double polygon::overlap_area(const polygon &p2, xyVec &outputcoords) {
	///Returns the area of overlap between to polygons, changes 'outputcoords' to the overlapping/intersection coordinates
	pair<coord, bool> temp;
	int lastInside = 0;
	vector<int> furthestInside;
	bool once = false;
	//looping through p2 vertices
	for (int x = 0; x < (int)p2.ptVert->size(); x++) {
		//checking if p1 contains a p2 vertice
		if ((*this).contains((*p2.ptVert)[x])) { 
			outputcoords.push_back((*p2.ptVert)[x]);
		}
		//For when multiple points are inside the second polygon, intersection is unreliable,
		//unless the 'furthest' vertex is recorded ->lastInside 
		if(!once) for (int y = 0; y < (int)ptVert->size(); y++) {
			if (p2.contains((*ptVert)[y])) {
				lastInside = y;
				once = true;
			}
		}
		//Starts intersection loop from the vertex of p1 thats inside p2, lastinside.
		//nested looping through p1 vertices for intersection
		for (int j = 0; j < (int)ptVert->size(); j++) { 
			int y =  (j + lastInside) % (int)ptVert->size();
			//xx,yy select the next vertex
			int xx = (x + 1) % (int)p2.ptVert->size(); 
			int yy = (y + 1) % (int)ptVert->size();
			//intersection between p2 and p1 
			temp = intersection((*p2.ptVert)[x], (*p2.ptVert)[xx], (*ptVert)[y], (*ptVert)[yy]); 
			if (temp.second) {//if there is an intersection
				outputcoords.push_back(temp.first); //works ez ez
				if (p2.contains((*ptVert)[yy])) {
					outputcoords.push_back((*ptVert)[yy]);
					furthestInside.push_back(yy); //the last/furthest recorded position inside p2
				}
			}
		} //end of intersection loop
	}
	//start from that furthest positions
	for (int y = 0; y < (int)ptVert->size(); y++) {
		int yy;
		if((int)furthestInside.size() > 0)  yy = (y + furthestInside.back()) % (int)ptVert->size(); //start from the point furthest 'inside'
		else yy = y;
	if (find(furthestInside.begin(), furthestInside.end(), yy) == furthestInside.end()) { //if we haven't recorded the point 
			if (p2.contains((*ptVert)[yy])) {
				//ignore coordinates we already have.
					outputcoords.push_back((*ptVert)[yy]);
			}
		}
	}
	overlapPolygon overlapshape(outputcoords.size());
	overlapshape.vertPlot(outputcoords);
	return overlapshape.area();
}

//overloaded with no outputcoord parameter
double polygon::overlap_area(const polygon &p2) {
	xyVec outputcoords;
	return this->overlap_area(p2, outputcoords);
}



/*----------General Class Manipulations-------------*/
//All derived classes follow a similar format of standard manipulation.
//Calling functions from coord Class.
void general::rotate(double rad, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = it->rotate(rad, position);
	}
	//update centre position
	centrePos = (*this).centre();
}

void general::rotate(double angle_rad) {
	return rotate(angle_rad, centrePos);
}

void general::rescale(double mag, coord position) {
	//Iterate through and rescale each vertex wrt a position
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = (coord(*it - position)*mag) + position;
	}
}
//Overload allows user to rescale wrt centrePos position
void general::rescale(double mag) {
	rescale(mag, centrePos);
}

//Translate the polygon by x horizontally, y vertically
void general::translate(double xx, double yy) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += xx; it->gety() += yy;
	}
	//update centre position
	centrePos = (*this).centre();
}
//Overloaded to allow translation by a coordinate
void general::translate(coord amount) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += amount.getx(); it->gety() += amount.gety();
	}
	//update centre position
	centrePos = (*this).centre();
}
/*----------End Manipulations-------------*/

//Outputs information onto the ostream,
//Outputs shape identity and number, its vertex locations and its area.
//also outputs whether the shape has been 'deleted' 
void general::info(ostream& stream) const {
	if (this->ptVert == nullptr){
		stream << "Deleted General Polygon" << endl;
	}
	else{
		stream << "General Polygon " << genNum << " with " << vertNum << " vertices." << endl;
		stream << "Vertex coordinates: ";
		for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
			it->show(stream);
			if (it != ptVert->end() - 1) stream << ",";
		}
		stream << endl << "Area : " << this->area() << endl;
	}
};
//unparameterised constructor 
general::general() {
	sPolyNum++;
	sGenNum++;
	genNum = sGenNum;
	vertNum = 0;
	ptVert = new xyVec;
}
//paramtereised constructor
general::general(int vert) {
	sPolyNum++;
	sGenNum++;
	genNum = sGenNum;
	ptVert = new xyVec(vert);
	vertNum = vert;
}
//move constructor for general polygon
general::general(general &&movePolygon){
	//sPolyNum will not change
	genNum = sGenNum;
	ptVert = movePolygon.ptVert;
	vertNum = movePolygon.vertNum;
	movePolygon.ptVert = nullptr;
	movePolygon.vertNum = 0;
	centrePos = (*this).centre(); //update the centre position

}
//general destructor
general::~general() {
	sPolyNum--;
	sGenNum--;
	delete ptVert;
}
//overlapPolygon construct, utilizes the general constructor
overlapPolygon::overlapPolygon() : general() {
	sOverlapNum++;
	genNum = sGenNum;
}
//parameterised overlap constructor, again uses general param constructor
overlapPolygon::overlapPolygon(int vert) :general(vert) {
	sOverlapNum++;
	overlapNum = sOverlapNum;
}
//destructor
overlapPolygon::~overlapPolygon() {
	sOverlapNum--;
}


//rectangle constructor
rectangle::rectangle() {
	sPolyNum++;
	sRecNum++;
	recNum= sRecNum;
	vertNum = 4;
	centrePos = coord();
	ptVert = new xyVec;
}
rectangle::rectangle(double w, double h, coord centre) {
	sPolyNum++;
	sRecNum++;
	recNum = sRecNum;
	vertNum = 4; //rectangle
	ptVert = new xyVec(vertNum);
	(*ptVert) = { coord(0, 0), coord(0, h), coord(w, h), coord(w, 0) };
	//update centre position
	centrePos = this->centre();
	this->position(centre);
}

rectangle::~rectangle() {
	sPolyNum--;
	sRecNum--;
	delete ptVert;
}


/*----------Rectangle Manipulations-------------*/
void rectangle::rotate(double rad, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = it->rotate(rad, position);
	}
	//update centre position
	centrePos = (*this).centre();
}

void rectangle::rotate(double angle_rad) {
	return rotate(angle_rad, centrePos);
}

void rectangle::rescale(double mag, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = (coord(*it - position)*mag) + position;
	}
}

void rectangle::rescale(double mag) {
	rescale(mag, centrePos);
}

void rectangle::translate(double xx, double yy) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += xx; it->gety() += yy;
	}
	//update centre position
	centrePos = (*this).centre();
}

void rectangle::translate(coord amount) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += amount.getx(); it->gety() += amount.gety();
	}
	//update centre position
	centrePos = (*this).centre();
}

/*----------End Manipulations-------------*/

//Outputs information onto the ostream,
//Outputs shape identity and number, its vertex locations and its area.
void rectangle::info(ostream& stream) const {
	stream << "Rectangle " << recNum << endl;
	stream << "Vertex coordinates: ";
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->show(stream);
		if (it != ptVert->end() - 1) stream << ",";
	}
	stream << endl << "Area : " << this->area() << endl;
};

//Pentagon section
pentagon::pentagon() {
	sPolyNum++;
	sPentNum++;
	pentNum = sPentNum;
	vertNum = 5;
	ptVert = new xyVec;
}
//Parameterised constructor
pentagon::pentagon(double segmentlength, coord centre) {
	sPolyNum++;
	sPentNum++;
	pentNum = sPentNum;
	vertNum = 5; 
	ptVert = new xyVec(vertNum);
	//uses angles to position the coordinates
	(*ptVert)[0] = coord();
	(*ptVert)[1] = coord(segmentlength, 0).rotate(0.6*M_PI, coord());
	for (int x = 2; x <5; x++) {
		(*ptVert)[x] = (*ptVert)[x - 2].rotate(0.6*M_PI, (*ptVert)[x - 1]);
	}
	//update centre position
	centrePos = this->centre();
	(*this).position(centre);
}
pentagon::~pentagon() {
	sPolyNum--;
	sPentNum--;
	delete ptVert;
}

/*----------Pentagon Manipulations-------------*/
void pentagon::rotate(double rad, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = it->rotate(rad, position);
	}
	//update centre position
	centrePos = (*this).centre();
}

void pentagon::rotate(double angle_rad) {
	return rotate(angle_rad, centrePos);
}

void pentagon::rescale(double mag, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = (coord(*it - position)*mag) + position;
	}
}

void pentagon::rescale(double mag) {
	rescale(mag, centrePos);
}


void pentagon::translate(double xx, double yy) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += xx; it->gety() += yy;
	}
	//update centre position
	centrePos = (*this).centre();
}

void pentagon::translate(coord amount) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += amount.getx(); it->gety() += amount.gety();
	}
	//update centre position
	centrePos = (*this).centre();
}
/*----------End Manipulations-------------*/

//Outputs information onto the ostream,
//Outputs shape identity and number, its vertex locations and its area.
void pentagon::info(ostream& stream) const {
	stream << "Pentagon " << pentNum << endl;
	stream << "Vertex coordinates: ";
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->show(stream);
		if (it != ptVert->end() - 1) stream << ",";
	}
	stream << endl << "Area : " << this->area() << endl;
};



//hexagon constructor
hexagon::hexagon() {
	sPolyNum++;
	sHexNum++;
	hexNum = sHexNum;
	vertNum = 6;
	ptVert = new xyVec;
}

hexagon::hexagon(double segmentlength, coord centre) {
	sPolyNum++;
	sHexNum++;
	hexNum = sHexNum;
	vertNum = 6; //hexagon
	ptVert = new xyVec(vertNum);
	//uses angles to position the coordinates
	(*ptVert)[0] = coord();
	(*ptVert)[1] = coord(segmentlength, 0).rotate((2. / 3.)*M_PI, coord());
	for (int x = 2; x <6; x++) {
		(*ptVert)[x] = (*ptVert)[x - 2].rotate((2. / 3.)*M_PI, (*ptVert)[x - 1]);
	}
	//update centre position
	centrePos = this->centre();
	(*this).position(centre);
}
hexagon::~hexagon() {
	sHexNum--;
	sPolyNum--;
	delete ptVert;
}

/*----------Hexagon Manipulations-------------*/
void hexagon::rotate(double rad, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = it->rotate(rad, position);
	}
	//update centre position
	centrePos = (*this).centre();
}

void hexagon::rotate(double angle_rad) {
	return rotate(angle_rad, centrePos);
}

void hexagon::rescale(double mag, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = (coord(*it - position)*mag) + position;
	}
}

void hexagon::rescale(double mag) {
	rescale(mag, centrePos);
}

void hexagon::translate(double xx, double yy) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += xx; it->gety() += yy;
	}
	//update centre position
	centrePos = (*this).centre();
}

void hexagon::translate(coord amount) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += amount.getx(); it->gety() += amount.gety();
	}
	//update centre position
	centrePos = (*this).centre();
}
/*----------End Manipulations-------------*/

//Outputs information onto the ostream,
//Outputs shape identity and number, its vertex locations and its area.
void hexagon::info(ostream& stream) const {
	stream << "Hexagon " << hexNum << endl;
	stream << "Vertex coordinates: ";
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->show(stream);
		if (it != ptVert->end() - 1) stream << ",";
	}
	stream << endl << "Area : " << this->area() << endl;
};


//Isosceles Triangle constructor
isosTriangle::isosTriangle() {
	sPolyNum++;
	sTriNum++;
	triNum = sTriNum;
	vertNum = 3;
	ptVert = new xyVec;
}
isosTriangle::isosTriangle(double b, double h, coord centre) {
	sPolyNum++;
	sTriNum++;
	triNum = sTriNum;
	vertNum = 3; //isosTriangle
	ptVert = new xyVec(vertNum);
	(*ptVert) = { coord(0, 0), coord(b / 2, h), coord(b, 0) };
	//update centre position
	centrePos = this->centre();
	this->position(centre);
}

isosTriangle::~isosTriangle() {
	sPolyNum--;
	sTriNum--;
	delete ptVert;
}

/*----------Triangle Manipulations-------------*/

void isosTriangle::rotate(double rad, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = it->rotate(rad, position);
	}
	//update centre position
	centrePos = (*this).centre();
}

void isosTriangle::rotate(double angle_rad) {
	return rotate(angle_rad, centrePos);
}

void isosTriangle::rescale(double mag, coord position) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		*it = (coord(*it - position)*mag) + position;
	}
}
void isosTriangle::rescale(double mag) {
	rescale(mag, centrePos);
}
void isosTriangle::translate(double xx, double yy) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += xx; it->gety() += yy;
	}
	//update centre position
	centrePos = (*this).centre();
}

void isosTriangle::translate(coord amount) {
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->getx() += amount.getx(); it->gety() += amount.gety();
	}
	//update centre position
	centrePos = (*this).centre();
}
/*----------end Manipulations-------------*/

//Outputs information onto the ostream,
//Outputs shape identity and number, its vertex locations and its area.
void isosTriangle::info(ostream& stream) const {
	stream << "Isosceles Triangle " << triNum << endl;
	stream << "Vertex coordinates: ";
	for (auto it = ptVert->begin(); it != ptVert->end(); it++) {
		it->show(stream);
		if (it != ptVert->end() - 1) stream << ",";
	}
	stream << endl << "Area : " << this->area() << endl;
};


