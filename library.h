//header guard
#ifndef LIBRARY_H
#define LIBRARY_H

#include "components.h"
#include "stdafx.h"

#include"circuits.h"
#include <iostream>
#include <map>
#include <string>
#include<algorithm>
#include<memory>

typedef std::shared_ptr<comp_nsp::component> comp_ptr;		//defines a type of the shared pointer to simplify code

class library {
private:
	map<string, comp_ptr> complist;	//stl map connects the component smart pointer to the name variable

public:
	library() {};
	~library();

	void show();
	void add(comp_ptr comp, string name);		//add component to the library
												//add requires a name to join to component (for access)

	comp_ptr get(string name);

};
#endif
