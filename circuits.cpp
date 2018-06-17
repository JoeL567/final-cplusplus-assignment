#include "stdafx.h"
#include "components.h"
#include "circuits.h"
#include "library.h"
#include <vector>
#include<utility>
#include<list>
#include<string>
#include<queue>
#include<set>
#include<map>
#include<deque>
#include<cmath>
#include<iostream>
#include<memory>

using namespace std;
using namespace comp_nsp;

const int logicflag{ -3 };
const int rangeflag{ -4 };
const int searchflag{ -1 };

//function to add two impedances in parallel
complex<double> parallel(complex<double> z1, complex<double> z2) {
	return pow(pow(z1, -1) + pow(z2, -1), -1);
}

//node member functions:
//copy constructors
node::node(const node &anode) {
	//cout << "called node copy constructor" << endl;
	label = anode.getlabel();
	connections = 0;
	if (anode.num_connections() > 0) {
		list<pair<comp_ptr, node*>> neighborsl = anode.getconnections();
		for (auto it = neighborsl.begin(); it != neighborsl.end(); ++it) {
			neighbors.push_back(*it);
			connections++;
		}
	}
}
node & node::operator=(const node &anode) {
	//cout << "called node assignment operator" << endl;
	label = anode.getlabel();
	if (&anode == this) return *this;
	if (connections > 0) {
		neighbors.clear();
	}
	neighbors.splice(neighbors.begin(), anode.getconnections());		//splices content of anode into this node
	connections = neighbors.size();
	return *this;
}

//move constructors
node::node(const node&& anode) {
	//cout << "node move constructor" << endl;
	label = anode.getlabel();
	connections = 0;
	if (anode.num_connections() > 0) {
		list<pair<comp_ptr, node*>> neighborsl = anode.getconnections();
		for (auto it = neighborsl.begin(); it != neighborsl.end(); ++it) {
			neighbors.push_back(*it);
			connections++;
		}
	}
}
node & node::operator=(node&& anode) {
	//cout << "node move assignment" << endl;
	swap(label, anode.label);
	swap(neighbors, anode.neighbors);
	swap(connections, anode.connections);
	return *this;
}



//member functions
int node::getlabel() const { return label; }

int node::num_connections() const { return connections; }

list<pair<comp_ptr, node*>> node::getconnections() const { return neighbors; }

void node::add_connection(pair<comp_ptr, node*> addition) { neighbors.push_back(addition); connections++; }

bool node::operator < (node& othernode) const {
	if (othernode.getlabel() < label) return true;
	else return false;
}

bool node::operator == (node& othernode) const {			//nodes are said to be equal if they have the same label (could this be dangerous??)
	if (othernode.getlabel() == label)return true;
	else return false;
}




//member function definitions for the circuit class
cgraph::cgraph() {
	node t1(0);
	node t2(-1);
	nodelist.push_back(t1);
	nodelist.push_back(t2);
	nodes = 2;
}


//copy constructors
cgraph::cgraph(const cgraph &oldgraph) {
	//add nodes to the new graph
	for (auto it = oldgraph.nodelist.begin(); it < oldgraph.nodelist.end(); ++it) {
		add_node(it->getlabel());
	}

	//add connections to the new graph
	for (auto it = oldgraph.nodelist.begin(); it < oldgraph.nodelist.end(); ++it) {		//iterate through the nodes, for each node iterate through its neighbors 
		list<pair<comp_ptr, node*>> neighbors = it->getconnections();
		for (auto it2 = neighbors.begin(); it2 != neighbors.end(); ++it2) {
			add_edge(it->getlabel(), it2->second->getlabel(), it2->first);
		}
	}
	nodes = oldgraph.node_number();
}

cgraph & cgraph::operator=(const cgraph &oldgraph) {
	if (&oldgraph == this) return *this;			//check for self assignment
													//delete existing data 
	nodelist.clear();

	//add nodes to the new graph
	for (auto it = oldgraph.nodelist.begin(); it < oldgraph.nodelist.end(); ++it) {
		add_node(it->getlabel());
	}

	//add connections to the new graph
	for (auto it = oldgraph.nodelist.begin(); it < oldgraph.nodelist.end(); ++it) {		//iterate through the nodes, for each node iterate through its neighbors 
		list<pair<comp_ptr, node*>> neighbors = it->getconnections();
		for (auto it2 = neighbors.begin(); it2 != neighbors.end(); ++it2) {
			add_edge(it->getlabel(), it2->second->getlabel(), it2->first);
		}
	}
	nodes = oldgraph.node_number();
	return *this;
}

//getters
int cgraph::node_number()const { return nodes; }
node cgraph::get_node_index(int i) const { return nodelist[i]; }		//search by index in vector

node cgraph::get_node(int lab) const {			//search by node label
	for (auto it = nodelist.begin(); it < nodelist.end(); ++it) {
		if (it->getlabel() == lab) {
			node answer = *it;
			return answer;
		}
	}
	throw searchflag;
	node fail;
	return fail;
}

//set the node number
void cgraph::set_node_number(const int num) {
	nodes = num;
}



//add a node to the graph (with no connections)
void cgraph::add_node(int label) {
	node addition(label);
	nodelist.push_back(addition);
	nodes++;
}

//add a connection to the graph
bool cgraph::add_edge(int head, int tail, comp_ptr weight) {
	for (auto iterate = nodelist.begin(); iterate != nodelist.end(); ++iterate) {		//go through node list, look for label corresponding to head
		if (iterate->getlabel() == head) {
			for (auto iterate2 = nodelist.begin(); iterate2 < nodelist.end(); ++iterate2) {		//if head is found, look for label corresponding to tail
				if (iterate2->getlabel() == tail) {
					node* tnode = &*iterate2;
					pair<comp_ptr, node*> item(weight, tnode);		//add head to tail connection and return true
					iterate->add_connection(item);
					return true;
				}
			}
		}
	}
	cerr << "Either the head or tail could not be found in the circuit, and the connection could not be made" << endl;
	return false;
}

//display the adjacency list for the graph
void cgraph::show() {
	cout << "Node   :  Connections" << endl;
	for (auto iterate = nodelist.begin(); iterate < nodelist.end(); ++iterate) {
		if (iterate->getlabel() == 0)cout << "Start" << "  :  ";
		else if (iterate->getlabel() == -1)cout << "End" << "    :  ";
		else cout << iterate->getlabel() << "      :  ";
		for (pair<comp_ptr, node*> n : iterate->getconnections()) {
			if (n.second->getlabel() == 0) cout << "(" << "Start" << ",";			/*Display "Start" and "End" rather than numeric node labels for these nodes*/
			else if (n.second->getlabel() == -1) cout << "(" << "End" << ",";
			else cout << "(" << n.second->getlabel() << ",";
			if (n.first->get_name() == "None") {
				cout << "component: Z = " << n.first->get_impedance() << ")";
			}
			else cout << n.first->get_name() << ") ";
		}
		cout << endl;
	}
	cout << endl;
}

//function to calculate the shortest distance between two nodes
int cgraph::distance(int initial, int goal) {			//this is an implementation of BFS algorithm

	map<int, int> visited;			//map of visited node linked to the distance integer
	deque<int> Q;

	//need to find the starting node in the node vector
	node current;
	node final;

	/*find starting and goal nodes*/
	for (auto it = nodelist.begin(); it < nodelist.end(); ++it) {
		if (it->getlabel() == initial) { current = *it; }		//set the current node to start node
		if (it->getlabel() == goal) {
			final = *it; //cout << "final: " << it->getlabel() << " neighbors: " << final.num_connections() << endl;
		}
	}

	visited[current.getlabel()] = 0;		//set start node distance to 0
	Q.push_back(current.getlabel());		//add current node to the queue
	while (Q.empty() == 0)		//repeat until the queue is empty and all nodes have been visited
	{
		Q.pop_front();		//remove first node from queue
							//cout << "node: " << current.getlabel() << " connections: " << current.num_connections() << endl;
		if (current.num_connections() > 0) {
			list<pair<comp_ptr, node*>> neighbors = current.getconnections();
			for (list<pair<comp_ptr, node*>>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {		//iterate through connections in current node
																													//cout << it->second->getlabel() << endl;
				if (visited.find(it->second->getlabel()) == visited.end()) {					//if connection has already been visited do nothing
					Q.push_back(it->second->getlabel());						//add the next node to the queue
					visited[it->second->getlabel()] = visited[current.getlabel()] + 1;	//mark the next node as visited with distance equal to current distance + 1
				}
			}
		}
		if (!Q.empty()) {
			node newnode = get_node(Q.front());	//set current to front of the queue
			current = newnode;
		}
	}

	//find the goal node in the visited map and return its value - if node cannot be found in visited map then no path exists; throw exception and return -1
	try { return visited.at(final.getlabel()); }
	catch (const std::out_of_range&) {
		cerr << "Error, node out of range. Make sure all nodes have at least two connections (except start and end)" << endl;
		return -1;
	}
}

cgraph cgraph::reduce() {
	cgraph newgraph;			//create a new graph
	node working;

	//iterate over the nodes, find the node which is the furthest distance away from both the start and the end node
	int currentdistance = 0;

	for (auto it = nodelist.begin(); it < nodelist.end(); ++it) {
		int d1 = distance(0, it->getlabel());
		int d2 = distance(-1, it->getlabel());
		if (d1 == -1 || d2 == -1) {		//catch the outorrange error from distance return
			throw rangeflag;
			cgraph fail;
			return fail;
		}

		int newdistance = d1 * d2;
		//cout << "label: " << it->getlabel() << endl;
		//cout << newdistance << endl;
		if (newdistance > currentdistance) {
			working = *it;
			currentdistance = newdistance;
		}
		else if (newdistance == currentdistance) {		//possiblity of there being two furthest nodes, in which case try to find one with only two connections
			map <int,int> names;		//create map to store the names of neighbors (using map for the count function)
			list<pair<comp_ptr, node*>> neighbors = it->getconnections();	//get the neighbors of the working node
			auto iter = neighbors.begin();								//iterate through the neighbors while the map is smaller than 3 in size
			while (iter != neighbors.end() && names.size() < 3) {		
				if (names.count(iter->second->getlabel()) > 0) { ; }	//if label is already in map do nothing
				else names[iter->second->getlabel()] = 1;		//otherwise add to the map
				iter++;
			}
			if (names.size() < 3) {		//only reset if the new furthest node has only two connecting nodes
				working = *it;
				currentdistance = newdistance;
			}
		}
	}

	//build a new graph with the same node labels except for the label of working
	for (auto it = nodelist.begin(); it < nodelist.end(); ++it) {
		if (it->getlabel() != working.getlabel() && it->getlabel() != 0 && it->getlabel() != -1) {			//build up new graph (except for start and end nodes as they already exist from constructor)
			newgraph.add_node(it->getlabel());
		}
	}

	//create two complex variables, imp_node1 and imp_node2
	bool assigned1 = false;		//we need to know if the nodes 1 and 2 have been initialized with a value
	bool assigned2 = false;
	node node1;
	node node2;
	complex<double> imp_node1, imp_node2, new_impedance;

	// add connections to each node in parallel
	list<pair<comp_ptr, node*>> neighbors = working.getconnections();
	for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {		//iterate through the neighbors of working node
		if (!assigned1) {
			//cout << "assigning 1" << endl;
			node1 = *it->second;
			imp_node1 = it->first->get_impedance();		//assign impedance of first node
			assigned1 = true;
		}
		else if (it->second->getlabel() != node1.getlabel() && !assigned2) {
			//cout << "assigning 2" << endl;
			node2 = *it->second;
			imp_node2 = it->first->get_impedance();		//assign impedance of second node
			assigned2 = true;
		}
		else if (it->second->getlabel() == node1.getlabel()) {	//connection to first node
			comp_ptr comp = it->first;
			imp_node1 = parallel(imp_node1, comp->get_impedance());		//add to imp_node1 in parallel
		}
		else if (it->second->getlabel() == node2.getlabel()) {	//connection to second node
			comp_ptr comp = it->first;
			imp_node2 = parallel(imp_node2, comp->get_impedance());		//add to imp_node2 in parallel
		}

		else {
			throw logicflag;	//if it finds a third node then the algorithm fails and the impedance can not be calculated
			cgraph fail;
			return fail;
		}
	}

	//add node 1 and node 2 impedances in series
	new_impedance = imp_node1 + imp_node2;


	//need to add connections to the new graph
	for (auto it = nodelist.begin(); it < nodelist.end(); ++it) {		//iterate through the nodes, for each node iterate through its neighbors 
		list<pair<comp_ptr, node*>> neighbors = it->getconnections();
		if (it->getlabel() != working.getlabel()) {			//only do this for the nodes not being deleted
			for (auto it2 = neighbors.begin(); it2 != neighbors.end(); ++it2) {
				if (it2->second->getlabel() != working.getlabel()) {	//if the neighbour connects to working do nothing, else copy the connection
					newgraph.add_edge(it->getlabel(), it2->second->getlabel(), it2->first);
				}
			}
		}
	}

	//add new connection from node1 to node2 in the new graph
	newgraph.add_edge(node2.getlabel(), node1.getlabel(), comp_ptr(new generic(new_impedance)));
	newgraph.add_edge(node1.getlabel(), node2.getlabel(), comp_ptr(new generic(new_impedance)));				//could be an issue here (assigned memory with no way to delete)

	newgraph.show();

	newgraph.set_node_number(nodelist.size() - 1);
	//cout << newgraph.node_number() << endl;

	return newgraph;		//return the reduced graph


}

//function to analyse the circuit and return a value for the complex impedance
complex<double> cgraph::impedance() {
	
	cgraph newgraph = *this;				//create a new, identical graph
	while (newgraph.node_number() > 2) {
		try {								//graph must be reducable
			newgraph = newgraph.reduce();
			newgraph.show();
		}
		catch (int errorFlag) {
			if (errorFlag == logicflag) {
				cerr << "Error: The complexity of the circuit is greater than this algorithm can solve" << endl;
				return complex<double>(-1, 0);
			}
			if (errorFlag == rangeflag) {
				return complex<double>(-1, 0);
			}
			else {
				cerr << "An unknown exception occured" << endl;
				exit(1);
			}
		}
	}
	complex<double> imp = (0.0, 0.0);	//only start and end nodes exist at this point
	list<pair<comp_ptr, node*>> neighbors = newgraph.get_node(0).getconnections();	//find connections from start node
	for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
		if (imp == (0.0, 0.0))imp = it->first->get_impedance();		//add first impedance to imp
		else imp = parallel(imp, it->first->get_impedance());		//add remaining impedances in parallel with imp
	}
	return imp;
}