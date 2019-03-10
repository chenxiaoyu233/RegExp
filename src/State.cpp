#include "State.h"

State::State(StateType type) {
	this -> type = type;
	trans.clear();
    info.clear();
}

State::State(const State &other) {
	type = other.type;
	info = other.info;
	trans.clear();
}

void State::SetInfo(vector<void*> _info) {
    info = _info;
}

vector<void*> State::GetInfo() {
    return info;
}

void State::InfoPushBack(void *infoItem) {
    info.push_back(infoItem);
}
