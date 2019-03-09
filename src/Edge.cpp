#include "Edge.h"

Edge::Edge() { }

Edge::Edge(string label, State *from, State *to) {
	this -> label = label;
	this -> from = from;
	this -> to = to;
}
