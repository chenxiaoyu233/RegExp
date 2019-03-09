#include "State.h"

State::State(StateType type) {
	this -> type = type;
	trans.clear();
}

State::State(const State &other) {
	type = other.type;
	info = other.info;
	trans.clear();
}

void State::SetInfo(void *_info) {
    info = _info;
}

void* State::GetInfo() {
    return info;
}
