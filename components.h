//header guard
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <complex>				//include library for complex numbers
#include <iostream>
#include <string>
#include<memory>
//#include "stdafx.h"
using namespace std;
const double PI = atan(1.0) * 4;

namespace comp_nsp {
	class component {			//base class for a component - pure virtual class
	protected:
		double frequency;
		complex <double> impedance;
		string name;


	public:
		virtual ~component() {} //destructor
		virtual void info() {} //function to display info about the particle
		virtual void set_frequency(double value) {}
		virtual void set_name(string nam) = 0;
		virtual string get_name() = 0;
		virtual double get_frequency() = 0;
		virtual complex<double> get_impedance() = 0;
		virtual double impedance_magnitude() = 0;
		virtual double get_phase_difference() = 0;
	};


	class resistor : public component {
	private:
		double resistance;

	public:
		resistor();		//creates a default constructor 
		resistor(double freq, double resistance);		//prototype resistor functions
		double get_frequency();
		double get_resistance();
		void info();
		void set_name(string nam);
		string get_name();
		void set_frequency(double value);
		complex<double> get_impedance();
		double impedance_magnitude();
		double get_phase_difference();
		~resistor();
	};

	class capacitor : public component {		//class for a capacitor component
	private:
		double capacitance;

	public:
		capacitor();								//prototype all the capacitor functions
		capacitor(double freq, double capacitance);
		double get_frequency();
		double get_capacitance();
		void set_frequency(double value);
		void info();
		void set_name(string nam);
		string get_name();
		complex<double> get_impedance();
		double impedance_magnitude();
		double get_phase_difference();
		~capacitor();
	};

	class inductor : public component {			//class for an inductor component
	private:
		double inductance;

	public:
		inductor();									//prototype all inductor components
		inductor(double freq, double inductance);
		double get_frequency();
		double get_inductance();
		void set_frequency(double value);
		void info();
		void set_name(string nam);
		string get_name();
		complex<double> get_impedance();
		double impedance_magnitude();
		double get_phase_difference();
		~inductor();
	};

	class generic : public component{		//class for a generic component, takes only impedance in the constructor
	public:
		generic();
		generic(complex<double> imp);
		double get_frequency();
		void set_frequency(double value);
		void info();
		void set_name(string nam);		//potentially change to integer???
		string get_name();
		complex<double> get_impedance();
		double impedance_magnitude();
		double get_phase_difference();
		~generic();
	};




}
#endif