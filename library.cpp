#include "stdafx.h"
#include "components.h"
#include"circuits.h"
#include "library.h"
#include <iostream>
#include <map>
#include <string>
#include<algorithm>
#include<memory>


using namespace std;
using namespace comp_nsp;

const int searchflag{ -1 };

library::~library() {
	complist.clear();
}

void library::show() {
	cout << "Component library:" << endl;
	for (auto iter = complist.begin(); iter != complist.end(); ++iter) {
		comp_ptr comp = iter->second;
		cout << iter->first << "  :  ";
		comp->info();
	}
	cout << endl;
	return;
}


void library::add(comp_ptr comp, string name) {
	bool exists = false;					/*need to check if name already exists in the library*/
	for (auto iter = complist.begin(); iter != complist.end(); ++iter) {
		if (iter->first == name) {
			cerr << "Name already exists in library!! The component could not be added." << endl;
			return;
		}
	}
	comp->set_name(name);			/*Name is set as component is added to the library (no reason to force components to have names
										if not added to the library*/
	complist[name] = comp;
}


comp_ptr library::get(string name) {
	if (complist.find(name) == complist.end()) {
		throw searchflag;
	}
	return complist[name];
}
