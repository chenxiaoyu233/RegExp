#ifndef __EDGE_H__
#define __EDGE_H__

#include "common.h"
class State;

class Edge {
private:
	string label;
	State *from, *to;

public:
	Edge();
	Edge(string label, State *from, State *to);
};

#endif
