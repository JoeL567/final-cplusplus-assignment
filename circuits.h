//circuits.h
/*in order to calculate simple arrangements of components, the circuit will need to be described by a graph structure
Each instance of a component connects two nodes*/

//header guard
#ifndef CIRCUITS_H
#define CIRCUITS_H

#include <vector>
#include "components.h"
#include "library.h"
#include<utility>
#include<list>
#include <string>
#include<memory>

using namespace std;
using namespace comp_nsp;

class node {
private:
	list <pair<comp_ptr, node*>> neighbors;
	int label;
	int connections;
public:
	node() { label = -2; connections = 0; }			//default node has label = -2
	node(int lab) { label = lab; connections = 0; }
	~node() {};

	//copy constructors
	node(const node &anode);
	node & operator=(const node &anode);

	//move constructors
	node(const node&& anode);
	node & operator=(node&& anode);


	//member functions
	int num_connections() const;
	int getlabel() const;
	list<pair<comp_ptr, node*>> getconnections() const;
	void add_connection(pair<comp_ptr, node*> addition);

	//operators
	bool operator < (node& othernode) const;			//need to define this to use the node struct as a key in a map
	bool operator == (node& othernode) const;
};

class cgraph {		//graph structure for circuit
					/*The graph is represented using an adjacency list, contains a vector of nodes*/
private:
	vector<node> nodelist;		//vector to store list of nodes
	int nodes;
public:

	cgraph();
	~cgraph() {};

	//copy constructor
	cgraph(const cgraph &oldgraph);
	cgraph & operator=(const cgraph &oldgraph);


	//member functions
	int node_number() const;
	void set_node_number(const int num);
	node get_node(int label) const;
	node get_node_index(int i)const;


	void add_node(int label);
	bool add_edge(int head, int tail, comp_ptr weight);

	void show();

	int distance(int head, int tail);		//function to find the shortest distance between two nodes

	cgraph reduce();


	complex<double> impedance();
};







#endif