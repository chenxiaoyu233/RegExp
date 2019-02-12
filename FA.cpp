#include "FA.h"

FA::FA(FAType type, bool isFree): type(type), isFree(isFree) {
	states.clear(); accept.clear();
	start = NULL;
}

FA::FA(const FA &other, bool isFree): isFree(isFree) {
	states.clear(); accept.clear();
	type = other.type;
	start = other.start;

	// tmp variable to store the edge info
	map <State*, size_t> mp; mp.clear();
	vector<pair<string, pair<size_t, size_t> > > edges; edges.clear();

	// map the address to array index
	for (size_t i = 0; i < other.states.size(); i++) mp[other.states[i]] = i;

	// copy the state from original FA
	for (auto s: other.states){
		State *tmp = new State(*s);
		states.push_back(tmp);
		if (tmp -> type == StateType::accept) accept.push_back(tmp);
	}
	start = other.start;

	// using the array index to collect edge info
	for (auto s: other.states){
		for (auto e: s -> trans) {
			edges.push_back(make_pair(e.label, make_pair(mp[e.from], mp[e.to])));
		}
	}

	// rebuild all the edges
	for (auto e: edges) {
		State *from = states[e.second.first], *to = states[e.second.second];
		string label = states[e.first];
		(from -> trans).push_back(label, from, to);
	}
}

FA::~FA() {
	if (!isFree) return;
	for (auto s: states) {
		delete s;
	}
}

FA FA::operator - (const FA &a, const FA &b) {
	return Concat(a, b);
}

FA FA::operator + (const FA &a, const FA &b) {
	return Union(a, b);
}

FA FA::Concat(const FA &a, const FA &b) {
	assert(a.type == FAType::NFA && b.type == FAType::NFA);
	FA at(a, false), bt(b, false), c(NFA);

	// build new FA based on at and bt
	for (auto s: at.accept) {
		(s -> trans).push_back(Edge("", s, bt.start));
		s -> type = StateType::normal;
	}

	// copy states from at and bt
	for (auto s: at.states) c.states.push_back(s);
	for (auto s: bt.states) c.states.push_back(s);
	for (auto s: bt.accept) c.accept.push_back(s);
	c.start = a.start;

	return c;
}

FA FA::Union(const FA &a, const FA &b) {
	assert(a.type == FAType::NFA && b.type == FAType::NFA);
	FA at(a, false), bt(b, false), c(NFA);

	// build new FA based on at, bt and c
	State *st = new State(StateType::normal);
	c.states.push_back(st);
	(st -> trans).push_back(Edge("", st, at.start));
	(st -> trans).push_back(Edge("", st, bt.start));

	// copy states from at and bt
	for (auto s: at.states) c.states.push_back(s);
	for (auto s: bt.states) c.states.push_back(s);
	for (auto s: at.accept) c.accept.push_back(s);
	for (auto s: bt.accept) c.accept.push_back(s);

	return c;
}

FA FA::Closet(const FA &a) {
	assert(a.type == FAType::NFA);
	FA c(a, false);

	// build new FA based on c
	State *st = new State(StateType::normal);
	c.states.push_back(st);
	(st -> trans).push_back(Edge("", st, c.start));
	c.start -> type = StateType::accept;
	for (auto s: c.accept) {
		(s -> trans).push_back(Edge("", s, c.start));
	}
	c.accept.push_back(c.start);
	c.start = st;

	return c;
}

FA NtoD() {
}
