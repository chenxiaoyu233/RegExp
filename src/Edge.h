#ifndef __EDGE_H__
#define __EDGE_H__

#include "common.h"
class State;

class Edge {
 public:
	string label;
	State *from, *to;

	Edge();
	Edge(string label, State *from, State *to);
};

#endif
