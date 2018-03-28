//functions.cpp
//commonly used functions in main.cpp concerning user input and the associated safeguards

#include <sstream>
#include <fstream>
#include <algorithm>
#include "polygonClass.h"

const double M_PI = acos(-1);

using namespace std;




//Template function for efficiency considering the extensive user input required.
//Defined in the cpp file because template functions work at runtime
template <typename t> t number_question(string question, t maxval, t minval)
{
	cout << question;
	t output; //the variable to be returned by the function that the user inputs 
	bool flag{ true };
	while (flag) {
		cin >> output;
		if (cin.fail()) { //if what the user inputs isn't of the acceptable format (int), it will fail
			cin.clear(); //the failure flag in the stream is cleared
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignores up to the maximum size of the stream length, until \n new line.
			cout << "Please enter an appropriate value :";
		}
		else { //if user did input acceptable format, then it's compared against the limits
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (output < minval) cout << "Please enter value greater than or equal to" << minval << " :";
			else if (output > maxval) cout << "Please enter value less than or equal to" << maxval << " :";
			else flag = false; //breaks the while loop(success) if user input within limits
		}
	}
	cout << endl;
	return output;
};

//userContinue 
void userContinue(){
	cout << "\nPlease press enter to continue ";
	cin.ignore();
	cout << endl << string(160, '-') << endl;
}

//string question, the strict parameter negates the requirement for safeguards if false
string string_question(string question, string acceptable_letters, int length, bool strict = true)
{
	string output; //the variable to be returned by the function that the user inputs 
	bool flag{ true };
	while (flag) {
		cout << question;
		cin >> output;
		if (cin.fail()) { //if what the user inputs isn't of the acceptable format (int), it will fail
			cin.clear(); //the failure flag in the stream is cleared
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignores up to the maximum size of the stream length, until \n new line.
			cout << "Please enter a value :";
		}
		else { //if user did input acceptable format, then it's compared against the limits
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (!strict) flag = false;
			else if (string::npos == acceptable_letters.find(output) || string::npos != output.find(" ") || output.size() != length);
			else flag = false; //breaks the while loop(success) if user input within limits
		}
	}
	cout << endl;
	return output;
}


///userInterface allows user to output and store custom polygons, and also to manipulate them in 2D space.
//The user has access to info retrieval, area calculation, overlap calculation, Rotate/translate/rescale functions.
//userInterface also permits new overlapPolygons to be made/handled.
//predefinedList is an optional argument to load shapes into the interface ahead of time.
void userInterface(vector<shared_ptr<polygon>> predefinedList){
	//main container for object information: user_polygons
	pair <vector<string>, vector<shared_ptr<polygon>>> user_polygons;
	int numLoaded = predefinedList.size();
	//move predefined poylgon vector into userInterface
	if (predefinedList.size() != 0){
		int add{ 1 };
		for (auto it = predefinedList.begin(); it != predefinedList.end(); it++){
			user_polygons.second.push_back(*it);
			stringstream name;
			name << "Loaded Polygon " << add;
			user_polygons.first.push_back(name.str());
			add++;
		}
		cout << endl << numLoaded << " polygon(s) were loaded" << endl;
	}
	while ("y" == string_question("Do you wish to use the polygon manager? (y/n): ", "y n", 1)) {
		///using smart_ptrs, specifically shared_ptr, as a temporary buffer vector is used which points at the same coordinates/objects
		string tempstring;
		stringstream ss;
		bool main{ true };
		bool skip{ false }; //bool that allows user to skip shape creation.
		while (main) {
			//Doesn't let the user skip if there are no objects
			//uses coniditonal operator to display which question the user sees
			string choice_string = (user_polygons.second.size() > 0) ? string_question("Rectangle(r), Triangle(t), Hexagon(h), Pentagon(p), Misc(m)\nor skip to manipulation(s): ", "r t h p m s", 1)
				: string_question("Rectangle(r), Triangle(t), Hexagon(h), Pentagon(p), Misc(m): ", "r t h p m", 1);

			if (choice_string == "s") skip = true;
			if (choice_string == "r") {
				//rectangle
				double rWidth = number_question("Width: ", 100., 0.);
				double rHeight = number_question("Height: ", 100., 0.);
				coord rcoord;
				cout << "Now Enter coordinates in \"x y\": ";
				cin >> rcoord;
				user_polygons.second.push_back(shared_ptr<polygon>(new rectangle(rWidth, rHeight, rcoord)));
				user_polygons.first.push_back(string_question("Rectangle name: ", "", 0, false));
			}
			if (choice_string == "t") {
				//triangle
				double tbase = number_question("Base: ", 100., 0.);
				double tHeight = number_question("Height: ", 100., 0.);
				coord tcoord;
				cout << "Now Enter coordinates in \"x y\": ";
				cin >> tcoord;
				user_polygons.second.push_back(shared_ptr<polygon>(new isosTriangle(tbase, tHeight, tcoord)));
				user_polygons.first.push_back(string_question("Triangle name: ", "", 0, false));
			}
			if (choice_string == "h") {
				//hexagon
				double hSeg = number_question("Segment length: ", 100., 0.);
				coord hCoord;
				cout << "Now Enter coordinates in \"x y\": ";
				cin >> hCoord;
				user_polygons.second.push_back(shared_ptr<polygon>(new hexagon(hSeg, hCoord)));
				user_polygons.first.push_back(string_question("Hexagon name: ", "", 0, false));
			}
			if (choice_string == "p") {
				//pentagon
				double pSeg = number_question("Segment length: ", 100., 0.);
				coord pCoord;
				cout << "Now Enter coordinates in \"x y\": ";
				cin >> pCoord;
				user_polygons.second.push_back(shared_ptr<polygon>(new  pentagon(pSeg, pCoord)));
				user_polygons.first.push_back(string_question("Pentagon name: ", "", 0, false));
			}
			if (choice_string == "m") {
				//general polygon
				int numvertices = number_question<int>("Number of vertices: ", 20, 3);
				xyVec mcoord(numvertices);
				cout << "Now Enter coordinates, clockwise, in \"x y\": ";
				for (int x = 0; x < numvertices; x++) {
					//Counts how many left to go
					cout << endl << x + 1 << " of " << numvertices << ": ";
					cin >> mcoord[x];
				}
				//plot the user defined points on a new general polygon.
				shared_ptr<general> temp(new general(numvertices));
				(*temp).vertPlot(mcoord);
				user_polygons.second.push_back(temp);
				user_polygons.first.push_back(string_question("Shape name: ", "", 0, false));

			}
			if(!skip)choice_string = string_question("Add another object (y/n): ", "y n", 1);
			bool edit{ true };  //While true keeps the user editting the current selection of polygons
			if (skip == true || choice_string == "n") {
				while (edit) {
					bool reselect{ false }, discard{ false };
					int count = 1;
					cout << endl << "Here's a list of all your objects: " << endl;
					for (auto it = user_polygons.second.begin(); it != user_polygons.second.end(); it++) {
						cout << "\t" << count << ". " << user_polygons.first[count - 1] << endl;
						count++;
					}
					//centrecoord is the centre of all the selected polygons
					coord centrecoord, aboutcoord;
					//have a vector of pointers (shape_pointer) of what the user wants to edit;
					vector<shared_ptr<polygon>> shape_pointer;
					vector<int> chosenshapes;
					int shapenumber;
					//user can choose multiple shapes, pushed onto vector->chosenshapes.
					if ((int)user_polygons.first.size() != 1) {
						cout << "\t" << count << ". All Objects" << endl;
						shapenumber = number_question("Shape to manipulate: ", (int)user_polygons.first.size() + 1, 1) - 1;
					}
					else shapenumber = 0; //case if only one shape created.
					chosenshapes.push_back(shapenumber);
					//if all shapes are being editted (shapenumber is too big)
					if (shapenumber == (int)user_polygons.first.size()) {
						chosenshapes.clear();
						//Re assign shape numbers onto vector in order
						for (int x{ 0 }; x < (int)user_polygons.first.size(); x++){
							chosenshapes.push_back(x);
						}
						shape_pointer = user_polygons.second;
					}
					else {
						while (true) {
							//Will keep asking user to add more shapes until a no is supplied
							shape_pointer.push_back(user_polygons.second[shapenumber]);
							string multipleshapes;
							if (shape_pointer.size() + 1 < user_polygons.second.size()) {
								multipleshapes = string_question("Edit another shape at the same time (y/n): ", "y n", 1);
							}
							else if (shape_pointer.size() == user_polygons.second.size()) {
								//the size of the vector = the size of the polygon vector
								cout << endl << "You're editting all objects!" << endl;
								break;
							}
							else break;
							//exit if user says no
							if (multipleshapes == "n") break;
							shapenumber = number_question("Which shape: ", (int)user_polygons.first.size(), 1) - 1;
							while (true) {
								//Checks if user is trying to select the same object twice
								if (find(chosenshapes.begin(), chosenshapes.end(), shapenumber) != chosenshapes.end()) {
									shapenumber = number_question("Choose a different shape: ", (int)user_polygons.first.size(), 1) - 1;
								}
								else {
									chosenshapes.push_back(shapenumber);
									break;
								}
							}

						}
					}
					//calculat the centre of the selected shapes!
					for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
						centrecoord = centrecoord + (*it)->centre();
					}
					centrecoord = centrecoord * (1. / (double)shape_pointer.size());

					while (!reselect) {
						//Manipulation loop
						choice_string = string_question("\nAdd more(a) Modify(m) Print(p) Overlap(o) Info(i) Reselect(e) \nor Discard(d): ", "a m p i o e d", 1);
						if (choice_string == "a"){
							edit = false; //exit the edit section
							skip = false; //shape creation won't be skipped (because user needs it)
							break;
						}
						if (choice_string == "m"){
							//modify polygons

							choice_string = string_question("Rotate(r) Translate(t) Rescale(s) Back(b): ", "r t s b", 1);

							if (choice_string == "s") {
								//Rescale
								double rescaleamount = number_question<double>("By what magnitude: ", 100., -100);
								string rescalemanual = string_question("Rescale around shape centre (c), or manual coordinate (m): ", "c m", 1);
								if (rescalemanual == "c") aboutcoord = centrecoord;
								else{
									cout << "Around which coordinate: ";
									cin >> aboutcoord;
								}
								for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
									(*it)->rescale(rescaleamount, aboutcoord);
								}
							}

							if (choice_string == "t") {
								//Translate
								coord movecoord;
								cout << "Shift by \"x y\": ";
								cin >> movecoord;
								for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
									(*it)->translate(movecoord);
								}
							}

							if (choice_string == "r") {
								//Rotate
								double rotateamount = number_question<double>("How many degrees: ", 361., -360);
								string rotatemanual = string_question("Rotate around shape centre (c), or manually set (m): ", "c m", 1);
								if (rotatemanual == "m"){
									cout << "Around which coordinate: ";
									cin >> aboutcoord;
								}
								else aboutcoord = centrecoord;
								for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
									(*it)->rotate((M_PI / 180)*rotateamount, aboutcoord);
								}
							}

						}

						if (choice_string == "e"){
							//exit
							reselect = true; //will reslect polygons
							skip = false; //User won't be able to skip shape creation
						}

						if (choice_string == "d" && (string_question("Are you sure (y/n): ", "y n", 1) == "y")){
							//discard polygons, reset flags to get back to the appropriate place
							reselect = true; //reselection polygons will occur
							discard = true; 
							edit = false; //will exit the edit section
							main = false; //will exit the main of interface
							cout << polygon::getPolyNum() - numLoaded;
							break;
						}
						if (choice_string == "p") {
							//Print options
							if (shape_pointer.size() != user_polygons.second.size() && string_question("Print all(a) or just selected shapes(s): ", "a s", 1) == "a") {
								//buffer all shapes within interface
								polygon::bufferAll(user_polygons.second);
							}
							else {
								for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
									//buffer the selected shapes
									(*it)->buffer();
								}
							}
							choice_string = string_question("To console(c) textfile(f) matlab(m): ", "c f m", 1);
							if (choice_string == "c"){
								//console draw
								polygon::draw();
								polygon::clearbuffer();
							}
							else{
								//both option textfile and matlab require filename
								ofstream outfile;
								string filename = string_question("Enter filename: ", "", 0, false);
								//check to see if filename begins with a number, if so, add a letter
								//MATLAB has problems with filenames beginning with letters.
								if (isdigit(filename[0])) filename.insert(0, "m");
								if (choice_string == "f"){
									//writing a text file
									filename.append(".txt");
									outfile.open(filename);
									//checking if file opens properly
									if (outfile.is_open())
									{
										for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
											(*it)->info(outfile);
										}
										outfile.close();
									}
									else cout << "Unable to open file" << endl;
								}
								else{
									//The following code is writing matlab plotting code into the outfile
									filename.append(".m");
									outfile.open(filename);
									if (outfile.is_open())
									{   //Make each polygon a different colour
										outfile << "C = colormap(hsv(" << (int)shape_pointer.size() << "));\n";
										bool once{ true };
										int count = 1;
										for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
											outfile << "xy = [";
											for (auto itc = ((*it)->getVert()).begin(); itc != ((*it)->getVert()).end(); itc++){
												outfile << (*itc).getx() << "," << (*itc).gety() << ",";
											}
											outfile << "];\nx = xy(1:2:length(xy)); \ny = xy(2:2 : length(xy)); \nh = fill(x,y,C(" << count << ",:));\nset(h,'facealpha',.7);\n";
											count++;
											if (once){
												//only needs to be written once
												outfile << "grid on; \nhold on;\n";
												once = false;
											}
										}
										outfile.close();
									}
									else cout << "Unable to open file" << endl;
								}
							}
						}

						if (choice_string == "i") {
							//information output
							for (auto it = shape_pointer.begin(); it != shape_pointer.end(); it++) {
								(*it)->info();
								cout << endl << endl;
							}
						}

						if (choice_string == "o") {
							//overlap calculation
							int efficient_count = 0;
							bool found = false;
							for (size_t y = 0; y < shape_pointer.size() - 1; y++) {
								for (size_t x = 1 + (size_t)efficient_count; x < shape_pointer.size(); x++) {
									//loop through all polygons within the vector
									xyVec overlapCoords;
									double area = shape_pointer[y]->overlap_area(*shape_pointer[x], overlapCoords);
									if (area > 0) {
										cout << "Overlap area : " << area << endl;
										found = true;
										if (string_question("Add overlap shape to object list (y/n): ", "y n", 1) == "y"){
											//allow user to create an overlapPolygon if overlap found
											shared_ptr<polygon> overlap_ptr = shared_ptr<polygon>(new  overlapPolygon(overlapCoords.size()));
											overlap_ptr->vertPlot(overlapCoords);
											user_polygons.second.push_back(overlap_ptr);
											//create a name for the polygon
											ss << "Overlap " << overlapPolygon::sOverlapNum << " : " << user_polygons.first[chosenshapes[y]] << " and " << user_polygons.first[chosenshapes[x]] << endl;
											std::getline(ss, tempstring);
											user_polygons.first.push_back(tempstring);
											reselect = true;
										}
									}
								}
								efficient_count++;
							}
							//if overlap hasn't been 'found', ha.
							if (!found) { cout << "No Overlaps found!" << endl; }
						}
					}
					if (discard == true) {
						main = false;
						//User may discard an 'untidy' selection of polygons
						//careful deletion requires a descending sort (note use of rend rbegin)
						sort(chosenshapes.rbegin(), chosenshapes.rend());
						for (auto it = chosenshapes.begin(); it != chosenshapes.end(); it++){
							user_polygons.first.erase(user_polygons.first.begin() + *it);
							user_polygons.second.erase(user_polygons.second.begin() + *it);

						}
						break;
					}
				}
			}
		}
		//Printing over many polygons the user effectively discarded
		cout << " -> " << polygon::getPolyNum() - numLoaded << " polygons within interface." << endl;
		cout << string(160, '-') << endl;
	}
}
