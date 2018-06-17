#include "stdafx.h"
#include "components.h"
#include<memory>
#include<math.h>


using namespace std;
using namespace comp_nsp;		//using custom namespace for components

								//member function definitions for resistor
resistor::resistor() {
	//cout << "Called default resistor constructor" << endl;			//default constructor
	frequency = 0;
	impedance = 0;
	resistance = 0;
	name = "None";
}
resistor::resistor(double freq, double res) {
	//cout << "Called parameterized resistor constructor" << endl;		//parametrised constructor
	frequency = freq;
	impedance.real(res);					//Z = R + 0i
	impedance.imag(0);
	resistance = res;
	name = "None";
}
double resistor::get_frequency() {
	return frequency;
}

double resistor::get_resistance() {
	return resistance;
}

void resistor::info() {
	cout << "resistor : R = " << get_resistance() <<" Ohms : |Z| = " << impedance_magnitude() <<" Ohms : phase = "<< get_phase_difference() <<" radians"<< endl;
}

void resistor::set_name(string nam) { name = nam; }
string resistor::get_name() { return name; }

void resistor::set_frequency(double value) {
	frequency = value;
}

complex<double> resistor::get_impedance() {
	return impedance;
}

double resistor::impedance_magnitude() {
	return abs(impedance);
}

double resistor::get_phase_difference() {
	return arg(impedance);
}

resistor::~resistor() { }//cout << "Deleting resistor" << endl; }

//member function definitions for capacitor
capacitor::capacitor() {
	//cout << "Called capacitor default constructor" << endl;
	frequency = 0;
	impedance = 0;
	capacitance = 0;
	name = "None";
}
capacitor::capacitor(double freq, double cap) {
	//cout << "Called capacitor parameterized constructor" << endl;
	frequency = freq;
	impedance.real(0);
	impedance.imag(-1 / (2*PI*frequency*cap));
	capacitance = cap;
	name = "None";
}
double capacitor::get_frequency() {
	return frequency;
}

double capacitor::get_capacitance() {
	return capacitance;
}

void capacitor::set_frequency(double value) {
	frequency = value;
}

void capacitor::info() {
	cout << "capacitor : C = " << get_capacitance() << "F : |Z| = " << impedance_magnitude() << " Ohms : phase = " << get_phase_difference() << " radians" << endl;
}

void capacitor::set_name(string nam) { name = nam; }
string capacitor::get_name() { return name; }

complex<double> capacitor::get_impedance() {
	return impedance;
}

double capacitor::impedance_magnitude() {
	return abs(impedance);
}

double capacitor::get_phase_difference() {
	return arg(impedance);
}

capacitor::~capacitor() { cout << "Deleting capacitor" << endl; }

//member function definitions for an inductor
inductor::inductor() {
	//cout << "Called inductor default constructor" << endl;
	frequency = 0;
	impedance = 0;
	inductance = 0;
	name = "None";
}
inductor::inductor(double freq, double ind) {
	//cout << "Called inductor parameterized constructor" << endl;
	frequency = freq;
	impedance.real(0);
	impedance.imag(2*PI*frequency*ind);
	inductance = ind;
	name = "None";
}
double inductor::get_frequency() {
	return frequency;
}

double inductor::get_inductance() {
	return inductance;
}

void inductor::info() {
	cout << "inductor : L = " << get_inductance() << "H : |Z| = " << impedance_magnitude() << " Ohms : phase = " << get_phase_difference() << " radians" << endl;
}

void inductor::set_name(string nam) { name = nam; }
string inductor::get_name() { return name; }

void inductor::set_frequency(double value) {
	frequency = value;
}

complex<double> inductor::get_impedance() {
	return impedance;
}

double inductor::impedance_magnitude() {
	return abs(impedance);
}

double inductor::get_phase_difference() {
	return arg(impedance);
}

inductor::~inductor() {} //cout << "Deleting inductor" << endl; }

//function definitions for the generic component class 
	generic::generic() {
		impedance = 0;
		name = "None";
	}

	generic::generic(complex<double> imp) {
		impedance = imp;
		name = "None";
	}

	double generic::get_frequency() { return frequency; }

	void generic::set_frequency(double value) { frequency = value; }

	void generic::info() { cout << "generic component : |Z| = " << impedance_magnitude() << endl; }

	void generic::set_name(string nam) { name = nam; }

	double generic::impedance_magnitude() { return abs(impedance); }

	string generic::get_name() { return name; }

	complex<double> generic::get_impedance() { return impedance; }

	double generic::get_phase_difference() { return arg(impedance); }

	generic::~generic() {// cout << "Deleting generic component" << endl; }
	}