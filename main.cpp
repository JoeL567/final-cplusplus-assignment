//main.cpp  -  Joe Longman
/*Project allows the user to imput an arrangement of any number of resistors, capacitors and inductors in series and in parallel.
Outputs a visual representation of the circuit in the console and returns the impedance of the circuit*/

//PLEASE NOTE: Throughout this assignment variables containing the word "impedance" have been consistently misspelt as "impedance"


#include "stdafx.h"
#include "components.h"
#include"circuits.h"
#include "library.h"
#include <iostream>
#include <map>
#include <string>
#include<algorithm>
#include <sstream>
#include<memory>

using namespace std;
using namespace comp_nsp;

const int searchflag{ -1 };
const int convertflag{ -2 };
string Ohm = "Ohm";

string lower(string &data) {					//converts string to lower case
	transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

double getnumber() {		//function requests a number and checks the response is a number
	string response;
	double result;
	getline(cin, response);
	for (auto it = response.begin(); it != response.end(); ++it) {		//check the response for non-numerical characters (will there be an issue with start and end??)
		char check{ '.' };
		if (!isdigit(*it) && *it != check) {
			throw convertflag;		//throw a convert flag if there are any problems here
			return -1;
		}
	}
	stringstream convert(response);		//conversion from string to double achieved using a string stream
	if (!(convert >> result)) {
		result = -1;
		throw convertflag;
	}
	return result;
}

int getint() {
	string response;
	int result;
	getline(cin, response);
	if (lower(response) == "start") return 0;			//need exceptions for start and end which must be converted into their corresponding integer values
	if (lower(response) == "end") return -1;
	for (auto it = response.begin(); it != response.end(); ++it) {		//check the response for non-numerical characters
		if (!isdigit(*it)) {
			throw convertflag;		//throw a convert flag if there are any problems here
			return -1;
		}
	}
	stringstream convert(response);		//conversion from string to double achieved using a string stream
	if (!(convert >> result)) {
		result = -1;
		throw convertflag;
	}
	return result;
}

void addrequest(const double &frequency, library &lib) {			//requests the type, R/L/C, and name of the component to add to the library
	cout << "Enter type of component (resistor (R), capacitor (C), inductor(I/L)):" << endl;
	string answer; getline(cin, answer);

	if (lower(answer) == "r" || lower(answer) == "resistor") {
		cout << "Enter resistance ("<< Ohm << ")" << endl;
		double resistance;
		try { resistance = getnumber(); }
		catch (int errorFlag) {
			if (errorFlag == convertflag) {
				cerr << "Error: Something went wrong, did you enter a numerical value? Component could not be added to library." << endl;
				return;
			}
		}
		cout << "Enter name for component (e.g. \'R1\')" << endl;
		string name; getline(cin, name);
		//resistor res(frequency, resistance);
		//cout << resistance << endl;
		comp_ptr resistor_ptr{ new resistor(frequency, resistance) };
		lib.add(resistor_ptr, name);
		return;
	}
	//answer for capacitor
	else if (lower(answer) == "c" || lower(answer) == "capacitor") {
		cout << "Enter capacitance (F):" << endl;
		double capacitance;
		try { capacitance = getnumber(); }
		catch (int errorFlag) {
			if (errorFlag == convertflag) {
				cerr << "Error: Something went wrong, did you enter a numerical value? Component could not be added to library." << endl;
				return;
			}
		}
		cout << "Enter name for component (e.g. \'C1\')" << endl;
		string name; getline(cin, name);
		lib.add(comp_ptr(new capacitor(frequency, capacitance)), name);
		return;
	}
	//answer for inductor
	else if (lower(answer) == "i" || lower(answer) == "inductor" || lower(answer) == "l") {
		cout << "Enter inductance (H):" << endl;
		double inductance;
		try { inductance = getnumber(); }
		catch (int errorFlag) {
			if (errorFlag == convertflag) {
				cerr << "Error: Something went wrong, did you enter a numerical value? Component could not be added to library." << endl;
				return;
			}
		}
		cout << "Enter name for component (e.g. \'L1\')" << endl;
		string name; getline(cin, name);
		lib.add(comp_ptr(new inductor(frequency, inductance)), name);
		return;
	}

	else { return; }

}

void connectrequest(cgraph &circuit, library &components) {			//function to handle user input and error checking for node connections
	int first;		//start and end nodes
	int second;
	cout << "Enter first connecting node:" << endl;
	try { first = getint(); }					//all the nodes are labelled by numbers 
	catch (int errorFlag) {
		if (errorFlag == convertflag) {
			cerr << "Error: Something went wrong, did you enter an integer value (or start/end)?" << endl;
			return;
		}
	}
	cout << "Enter second connecting node:" << endl;
	try { second = getint(); }
	catch (int errorFlag) {
		if (errorFlag == convertflag) {
			cerr << "Error: Something went wrong, did you enter an integer value (or start/end)?" << endl;
			return;
		}
	}

	cout << "Enter component from library to connect the nodes:" << endl;
	string name;
	getline(cin, name);
	try {
		circuit.add_edge(first, second, components.get(name));
		circuit.add_edge(second, first, components.get(name));
		cout << "Connection added!" << endl << endl;
	}
	catch (int errorFlag) {
		if (errorFlag == searchflag) {
			cerr << "Error: search value could not be found in the library" << endl;
			return;
		}
	}
}


int main() {

	library components;		//instantiate the component library and circuit
	cgraph circuit;
	int currentnode = 1;

	cout << "Welcome to the RLC circuit builder, this program allows you to build circuits of resistors, capacitors and inductors and calculate the impedance." << endl;
	cout << endl;

	//first need to get the frequency of the circuit
	cout << "Enter the circuit frequency" << endl;
	double frequency;
	bool done = false;
	while (!done) {				//keep trying until a valid input is found
		try { frequency = getnumber();	done = true; }
		catch (int errorFlag) {
			if (errorFlag == convertflag) cerr << "Error: could not convert input to number: Did you enter an numerical value?" << endl;
			else { cerr << "Error: There was an unidentifed problem" << endl; exit(1); }
		}
	}

	comp_ptr res1(new resistor(frequency, 60));
	components.add(res1, "R1");


	cout << "frequency = " << frequency << endl;

	done = false;
	while (!done) {
		cout << "\nSelect an option..." << endl;
		cout << "1: View component library\n2: Add component to library\n3: View circuit\n4: Add node to circuit\n5: Add component to circuit\n6: Calculate impedance of circuit\n7: exit\n" << endl;

		string answer;
		getline(cin, answer);

		if (answer == "1") {		//view library
			components.show();
		}

		else if (answer == "2") {	//add component to library
			addrequest(frequency, components);
		}

		else if (answer == "3") {			//view circuit
			circuit.show();
		}

		else if (answer == "4") {			//add node
			circuit.add_node(currentnode);
			cout << "Node " << currentnode << " has been added!" << endl << endl;
			currentnode++;
		}

		else if (answer == "5") {			//add connection
			connectrequest(circuit, components);
		}

		else if (answer == "6") {
			circuit.show();
			complex<double> imp = circuit.impedance();
			cout << "\nimpedance:  Z = " << imp.real() << " + " << imp.imag() << "i "<<Ohm << endl;
			cout << "magnitude: " << abs(imp) << " "<<Ohm << endl;
			cout << "phase: " << arg(imp) <<" radians = "<< (180* arg(imp))/PI << " degrees" << endl;
			cout << endl;
		}

		else if (answer == "7") {
			done = true;
		}
	}
	return 0;
}