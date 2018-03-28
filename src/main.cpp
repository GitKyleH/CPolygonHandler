///Final Project, C++
/// Kyle Harfoot 8388047

/// A class hierarchy for creating and manipulating polygon shapes, storing the coordinates of the vertices.
//
/// Minimum specification :
///	Abstract base class for 2D polygons
///	Derived classes for : at least an isosceles triangle; rectangle; pentagon; hexagon
///	Pure virtual functions to translate each polygon in the x, y plane, to rescale its shape in x, y, and rotate it by a specified angle about its centre.
///	Polygon stores co - ordinates of each vertex in a suitable form
///	Rotation function implements matrix multiplication using a 2x2 rotation matrix
//
///	Minimum functionality :
/// User inputs details for polygons, creating library using STL container or array of base class pointers
/// Program demonstrates manipulation of polygons(translation, rescaling, rotation)
/// Program outputs details of each polygon
//
/// Extra functionality
/// Test for overlap between polygons;
/// Area of the overlap;
/// Manipulation of the overlap
/// Extensive outputs


#include <iostream>
#include <iomanip>
#include <memory>
#include <type_traits>
#include <string>

#include "polygonClass.h"
#include "coordClass.h"
#include "functions.h"

using namespace std;

int  main() {
	//Using STL <type_traits> library to check if base class is abstract
	cout <<boolalpha;
	std::cout << "Testing if base class is abstract :"<< is_abstract<polygon>::value << ".\n\n";
	system("COLOR 3F");
	cout << endl << "Testing coordinate class arithmetic" << endl;

	coord coord_test(10, 5);
	//Arithmetic testing in try/catch 
	try {
		//dividing by zero
		cout <<"Example coordinate: " << coord_test << endl;
		cout << "Multiplied by 5: " << coord_test * 5 << endl;
		cout << "Divided by 5: " << coord_test / 5 << endl;
		cout << "Magnitude: " << coord_test.magnitude() << endl;
		cout << "Normalised : " << coord_test.normalise() << endl;
		cout << "Divided by 0: ";
		cout << coord_test / 0 << endl;
	}
	//Catching any exceptions thrown within the try scope
	catch (overflow_error e) {
		cout << e.what() << endl;
	}
	//A function that requires the Enter input to proceed
	userContinue();

	{//creating a scope for smart pointer test
		
		vector<shared_ptr<polygon>> vec_general(4);
		//unique pointer, so exclusively each object has one pointer pointing to it.
		vec_general[0] = unique_ptr<general>(new general(7));
		vec_general[1] = unique_ptr<general>(new general(5));
		vec_general[2] = unique_ptr<general>(new general(4));
		vec_general[3] = unique_ptr<general>(new general(9));
		vec_general[0]->vertPlot(xyVec{coord(1, 20), coord(1, 1), coord(1, 10), coord(10, 20), coord(1, 10), coord(10, 1), coord(1, 10)});
		vec_general[1]->vertPlot(xyVec{coord(1, 20), coord(6, 14), coord(10, 20), coord(1, 1), coord(6, 14)});
		vec_general[2]->vertPlot(xyVec{coord(1, 1), coord(10, 1), coord(1, 1), coord(1, 20)});
		vec_general[3]->vertPlot(xyVec{coord(1, 1), coord(10, 1), coord(1, 1), coord(1, 10), coord(10, 10), coord(1, 10), coord(1, 20), coord(10, 20), coord(1, 20)});
		int trans_int{ 0 };
		//Rotating, translating and rescaling general polygon using iterator
		cout  << endl << "Demonstrating manipulation of general shapes" << endl;
		for (auto it = vec_general.begin(); it != vec_general.end(); it++) {
			(**it).rotate(0.1*trans_int, coord(1, 1));
			(**it).rescale(0.5,coord(1,1));
			(**it).translate(trans_int * 10,0);
			trans_int++;
		}
		polygon::bufferAll(vec_general);
		polygon::draw();
		polygon::clearbuffer();
		userContinue();
		//Testing move function
		cout << "Printing the info of the 'L' polygon being moved  : " << endl << endl;
		vec_general[2]->info();
		//adding a new polygon to back of the vector
		vec_general.push_back(move(vec_general[2]));
		cout << endl <<"The vector element should now be pointing at null" << endl;
		cout << "Value of the smart_ptr :" << vec_general[2] << endl;
		userContinue();

		//Proof move worked:
		cout << "Number of polygons before erasing: " << polygon::getPolyNum();

		//deleting that vector entry
		vec_general.erase(vec_general.begin() + 2);
		cout << "--> " << polygon::getPolyNum() << " after erasing the vector element!" << endl;

		userContinue();
		//Redrawing should have the 'wrong' polygon flipped
		vec_general[2]->rotate(3.14159);
		polygon::bufferAll(vec_general);
		polygon::draw();
		polygon::clearbuffer();
	}
	cout << "Number of polygons after the scope: " << polygon::getPolyNum() << endl;

	userContinue();

	//Creating a vector of polygons to preload into the user interface
	cout << "Starting Interface" << endl << endl << "Sample shapes should be loaded into the interface:";

	vector<shared_ptr<polygon>> preloadedVector;

	preloadedVector.push_back(shared_ptr<polygon>(new hexagon(5, coord(4, 4))));
	preloadedVector.push_back(shared_ptr<polygon>(new general(8)));

	xyVec weird{ coord(1, 1), coord(1, 10), coord(5, 10), coord(5, 5), coord(10, 5), coord(10, 10), coord(15, 10), coord(15, 1) };
	preloadedVector[1]->vertPlot(weird);

	//The main user interface is controlled by the following void function
	//Can take an argument, a pointer to a vector of polygons 'preloaded'
	userInterface(preloadedVector);
	return 0;
}
