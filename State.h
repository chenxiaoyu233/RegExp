#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "Edge.h"

enum StateType {accept, normal};

class State {
public:
	StateType type;
	vector<Edge> trans;
	State(StateType type);
	State(const State &other);
};

#endif
