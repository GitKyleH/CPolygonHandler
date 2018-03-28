//functions.h Header for functions.cpp
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "polygonClass.h"
using namespace std;

///Function that deals with the user input, it also takes an argument of a vector of polygons
//userInterface allows user to store custom polygons, and manipulate them in 2D space
//also gives the user the ability to output the data to console, text file, MATLAB code.
void userInterface(vector<shared_ptr<polygon>> predfinedList = { 0 });

//Enter to continue function
void userContinue();

//string question, the strict parameter negates the requirement for safeguards if strict = false
string string_question(string question, string acceptable_letters, int length, bool strict = true);

#endif
