#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "Edge.h"

enum StateType {accept, normal};

class State {
public:
	StateType type;
	vector<Edge> trans;
    void *info; // point to outer data structure
	State(StateType type);
	State(const State &other);
    void SetInfo(void *_info);
    void* GetInfo();
};

#endif
